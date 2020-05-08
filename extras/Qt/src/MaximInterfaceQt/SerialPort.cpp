/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/

#include <QSerialPortInfo>
#include <MaximInterfaceCore/ChangeSizeType.hpp>
#include <MaximInterfaceCore/Error.hpp>
#include "SerialPort.hpp"
#include "Sleep.hpp"

namespace MaximInterfaceQt {

using namespace MaximInterfaceCore;
using std::string;

static_assert(QSerialPort::NoError == 0, "Remapping required for error_code.");

static constexpr int timeout = 1000;

template <typename Func>
Result<void> SerialPort::executeIfConnected(Func operation) {
  return connected() ? operation() : make_error_code(QSerialPort::NotOpenError);
}

template <typename Func, typename... Args>
Result<void> SerialPort::executePortOperation(Func operation, Args &&... args) {
  return (port.*operation)(std::forward<Args>(args)...)
             ? makeResult(none)
             : make_error_code(port.error());
}

Result<void> SerialPort::connect(const string & portName) {
  port.setPort(QSerialPortInfo(QString::fromStdString(portName)));
  auto result =
      executePortOperation(&QSerialPort::open, QSerialPort::ReadWrite);
  if (!result) {
    return result;
  }
  result = executePortOperation(&QSerialPort::setDataTerminalReady, true);
  if (!result) {
    port.close();
  }
  return result;
}

Result<void> SerialPort::disconnect() {
  port.close();
  return none;
}

bool SerialPort::connected() const { return port.isOpen(); }

string SerialPort::portName() const { return port.portName().toStdString(); }

Result<void> SerialPort::setBaudRate(int_least32_t baudRate) {
  return executePortOperation(&QSerialPort::setBaudRate, baudRate,
                              QSerialPort::AllDirections);
}

Result<void> SerialPort::sendBreak() {
  return executeIfConnected([this] {
    auto result = executePortOperation(&QSerialPort::setBreakEnabled, true);
    if (result) {
      sleep(1);
      result = executePortOperation(&QSerialPort::setBreakEnabled, false);
    }
    return result;
  });
}

Result<void> SerialPort::clearReadBuffer() {
  return executeIfConnected([this] {
    return executePortOperation(&QSerialPort::clear, QSerialPort::Input);
  });
}

Result<void> SerialPort::writeByte(uint_least8_t data) {
  return executeIfConnected([this, data] {
    auto result = executePortOperation(&QSerialPort::putChar, data);
    if (result) {
      result = executePortOperation(&QSerialPort::waitForBytesWritten, timeout);
    }
    return result;
  });
}

Result<void> SerialPort::writeBlock(span<const uint_least8_t> data) {
  return executeIfConnected([this, data] {
    return changeSizeType<qint64>(
        [this](const char * data, qint64 dataLen) -> Result<void> {
          while (dataLen > 0) {
            const auto dataWritten = port.write(data, dataLen);
            if (dataWritten == -1) {
              return make_error_code(port.error());
            }
            data += dataWritten;
            dataLen -= dataWritten;
            MaximInterfaceCore_TRY(executePortOperation(
                &QSerialPort::waitForBytesWritten, timeout));
          }
          return none;
        },
        make_span(reinterpret_cast<const char *>(data.data()), data.size()));
  });
}

Result<uint_least8_t> SerialPort::readByte() {
  uint_least8_t data;
  MaximInterfaceCore_TRY(executeIfConnected([this, &data] {
    if (port.atEnd()) {
      const auto result =
          executePortOperation(&QSerialPort::waitForReadyRead, timeout);
      if (!result) {
        return result;
      }
    }
    return executePortOperation(&QSerialPort::getChar,
                                reinterpret_cast<char *>(&data));
  }));
  return data;
}

Result<void> SerialPort::readBlock(span<uint_least8_t> data) {
  return executeIfConnected([this, data] {
    return changeSizeType<qint64>(
        [this](char * data, qint64 dataLen) -> Result<void> {
          while (dataLen > 0) {
            if (port.atEnd()) {
              MaximInterfaceCore_TRY(executePortOperation(
                  &QSerialPort::waitForReadyRead, timeout));
            }
            const auto dataRead = port.read(data, dataLen);
            if (dataRead == -1) {
              return make_error_code(port.error());
            }
            data += dataRead;
            dataLen -= dataRead;
          }
          return none;
        },
        make_span(reinterpret_cast<char *>(data.data()), data.size()));
  });
}

const error_category & SerialPort::errorCategory() {
  static class : public error_category {
  public:
    virtual const char * name() const override {
      return "MaximInterfaceQt.SerialPort";
    }

    virtual string message(int condition) const override {
      switch (condition) {
      case QSerialPort::DeviceNotFoundError:
        return "Device Not Found";

      case QSerialPort::PermissionError:
        return "Permission Error";

      case QSerialPort::OpenError:
        return "Open Error";

      case QSerialPort::NotOpenError:
        return "Not Open Error";

      case QSerialPort::WriteError:
        return "Write Error";

      case QSerialPort::ReadError:
        return "Read Error";

      case QSerialPort::ResourceError:
        return "Resource Error";

      case QSerialPort::UnsupportedOperationError:
        return "Unsupported Operation Error";

      case QSerialPort::TimeoutError:
        return "Timeout Error";
      }
      return defaultErrorMessage(condition);
    }

    // Make QSerialPort::TimeoutError equivalent to Uart::TimeoutError.
    virtual error_condition default_error_condition(int code) const override {
      return (code == QSerialPort::TimeoutError)
                 ? make_error_condition(Uart::TimeoutError)
                 : error_category::default_error_condition(code);
    }
  } instance;
  return instance;
}

} // namespace MaximInterfaceQt