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

#include <MaximInterfaceCore/ChangeSizeType.hpp>
#include <MaximInterfaceCore/Error.hpp>
#include <msclr/auto_gcroot.h>
#include <msclr/marshal_cppstd.h>
#include "SerialPort.hpp"
#include "Sleep.hpp"

namespace MaximInterfaceDotnet {

using namespace MaximInterfaceCore;
using msclr::interop::marshal_as;
using System::Runtime::InteropServices::Marshal;

template <typename Type, typename Input> static bool isType(Input in) {
  return dynamic_cast<Type>(in) != nullptr;
}

template <typename Func>
static Result<void> executeTryCatchOperation(Func tryOperation) {
  return executeTryCatchOperation(tryOperation, [] {});
}

template <typename TryFunc, typename CatchFunc>
static Result<void> executeTryCatchOperation(TryFunc tryOperation,
                                             CatchFunc catchOperation) {
  try {
    tryOperation();
  } catch (System::Exception ^ e) {
    catchOperation();
    if (isType<System::ArgumentException ^>(e)) {
      return SerialPort::ArgumentError;
    }
    if (isType<System::InvalidOperationException ^>(e)) {
      return SerialPort::InvalidOperationError;
    }
    if (isType<System::IO::IOException ^>(e)) {
      return SerialPort::IOError;
    }
    if (isType<System::UnauthorizedAccessException ^>(e)) {
      return SerialPort::UnauthorizedAccessError;
    }
    if (isType<System::TimeoutException ^>(e)) {
      return SerialPort::TimeoutError;
    }
    throw;
  }
  return none;
}

template <typename Func>
Result<void> SerialPort::executeIfConnected(Func operation) {
  return connected() ? operation() : SerialPort::NotConnectedError;
}

struct SerialPort::Data {
  msclr::auto_gcroot<System::IO::Ports::SerialPort ^> port;
};

SerialPort::SerialPort() : data(std::make_unique<Data>()) {}

SerialPort::~SerialPort() = default;

SerialPort::SerialPort(SerialPort &&) noexcept = default;

SerialPort & SerialPort::operator=(SerialPort &&) noexcept = default;

Result<void> SerialPort::connect(const std::string & portName) {
  data->port = gcnew System::IO::Ports::SerialPort;
  return executeTryCatchOperation(
      [this, &portName] {
        data->port->PortName = marshal_as<System::String ^>(portName);
        data->port->DtrEnable = true;
        data->port->ReadTimeout = 1000;
        data->port->WriteTimeout = 1000;
        data->port->Open();
      },
      [this] { data->port.reset(); });
}

Result<void> SerialPort::disconnect() {
  data->port.reset();
  return none;
}

bool SerialPort::connected() const { return data->port.get() != nullptr; }

std::string SerialPort::portName() const {
  return connected() ? marshal_as<std::string>(data->port->PortName) : "";
}

Result<void> SerialPort::setBaudRate(int_least32_t baudRate) {
  return executeIfConnected([this, baudRate] {
    return executeTryCatchOperation(
        [this, baudRate] { data->port->BaudRate = baudRate; });
  });
}

Result<void> SerialPort::sendBreak() {
  return executeIfConnected([this] {
    return executeTryCatchOperation([this] {
      data->port->BreakState = true;
      sleep(1);
      data->port->BreakState = false;
    });
  });
}

Result<void> SerialPort::clearReadBuffer() {
  return executeIfConnected([this] {
    return executeTryCatchOperation([this] { data->port->ReadExisting(); });
  });
}

Result<void> SerialPort::writeByte(uint_least8_t data) {
  return writeBlock(make_span(&data, 1));
}

Result<void> SerialPort::writeBlock(span<const uint_least8_t> data) {
  return executeIfConnected([this, data] {
    return changeSizeType<int>(
        [this](const uint_least8_t * dataChunk, int dataChunkSize) {
          return executeTryCatchOperation([this, dataChunk, dataChunkSize] {
            auto dataManaged = gcnew cli::array<uint_least8_t>(dataChunkSize);
            Marshal::Copy(static_cast<System::IntPtr>(
                              const_cast<uint_least8_t *>(dataChunk)),
                          dataManaged, 0, dataChunkSize);
            this->data->port->Write(dataManaged, 0, dataChunkSize);
          });
        },
        data);
  });
}

Result<uint_least8_t> SerialPort::readByte() {
  uint_least8_t data;
  MaximInterfaceCore_TRY(executeIfConnected([this, &data] {
    return executeTryCatchOperation(
        [this, &data] { data = this->data->port->ReadByte(); });
  }));
  return data;
}

Result<void> SerialPort::readBlock(span<uint_least8_t> data) {
  return executeIfConnected([this, data] {
    return changeSizeType<int>(
        [this](uint_least8_t * dataChunk, int dataChunkSize) {
          return executeTryCatchOperation([this, dataChunk, dataChunkSize] {
            auto dataManaged = gcnew cli::array<uint_least8_t>(dataChunkSize);
            int bytesRead = 0;
            do {
              bytesRead += this->data->port->Read(dataManaged, bytesRead,
                                                  dataChunkSize - bytesRead);
            } while (bytesRead < dataChunkSize);
            Marshal::Copy(dataManaged, 0,
                          static_cast<System::IntPtr>(dataChunk),
                          dataChunkSize);
          });
        },
        data);
  });
}

const error_category & SerialPort::errorCategory() {
  static class : public error_category {
  public:
    virtual const char * name() const override {
      return "MaximInterfaceDotnet.SerialPort";
    }

    virtual std::string message(int condition) const override {
      switch (condition) {
      case NotConnectedError:
        return "Not Connected Error";

      case ArgumentError:
        return "Argument Error";

      case InvalidOperationError:
        return "Invalid Operation Error";

      case IOError:
        return "IO Error";

      case UnauthorizedAccessError:
        return "Unauthorized Access Error";
      }
      return defaultErrorMessage(condition);
    }
  } instance;
  return instance;
}

} // namespace MaximInterfaceDotnet
