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

#ifndef MaximInterfaceQt_SerialPort_hpp
#define MaximInterfaceQt_SerialPort_hpp

#include <QSerialPort>
#include <MaximInterfaceCore/SerialPort.hpp>
#include "Config.hpp"

namespace MaximInterfaceQt {

class SerialPort : public MaximInterfaceCore::SerialPort {
public:
  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  connect(const std::string & portName) override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  disconnect() override;

  /// @brief Check if currently connected to a port.
  /// @returns True if connected.
  MaximInterfaceQt_EXPORT bool connected() const;

  /// Get the currently connected port name.
  MaximInterfaceQt_EXPORT std::string portName() const;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  setBaudRate(int_least32_t baudRate) override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  sendBreak() override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  clearReadBuffer() override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  writeByte(uint_least8_t data) override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  writeBlock(MaximInterfaceCore::span<const uint_least8_t> data) override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<uint_least8_t>
  readByte() override;

  MaximInterfaceQt_EXPORT virtual MaximInterfaceCore::Result<void>
  readBlock(MaximInterfaceCore::span<uint_least8_t> data) override;

  MaximInterfaceQt_EXPORT static const MaximInterfaceCore::error_category &
  errorCategory();

private:
  template <typename Func>
  MaximInterfaceCore::Result<void> executeIfConnected(Func operation);

  template <typename Func, typename... Args>
  MaximInterfaceCore::Result<void> executePortOperation(Func operation,
                                                        Args &&... args);

  QSerialPort port;
};

inline MaximInterfaceCore::error_code
make_error_code(QSerialPort::SerialPortError e) {
  return MaximInterfaceCore::error_code(e, SerialPort::errorCategory());
}

} // namespace MaximInterfaceQt

#endif