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

#ifndef MaximInterfaceDotnet_SerialPort_hpp
#define MaximInterfaceDotnet_SerialPort_hpp

#include <memory>
#include <string>
#include <MaximInterfaceCore/SerialPort.hpp>
#include "Config.hpp"

namespace MaximInterfaceDotnet {

/// Serial port interface using .NET type System::IO::Ports::SerialPort.
class SerialPort : public MaximInterfaceCore::SerialPort {
public:
  enum ErrorValue {
    NotConnectedError = 1,
    ArgumentError,
    InvalidOperationError,
    IOError,
    UnauthorizedAccessError
  };

  MaximInterfaceDotnet_EXPORT SerialPort();

  MaximInterfaceDotnet_EXPORT ~SerialPort();

  MaximInterfaceDotnet_EXPORT SerialPort(SerialPort &&) noexcept;

  MaximInterfaceDotnet_EXPORT SerialPort & operator=(SerialPort &&) noexcept;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  connect(const std::string & portName) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  disconnect() override;

  /// @brief Check if currently connected to a port.
  /// @returns True if connected.
  MaximInterfaceDotnet_EXPORT bool connected() const;

  /// Get the currently connected port name.
  MaximInterfaceDotnet_EXPORT std::string portName() const;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  setBaudRate(int_least32_t baudRate) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  sendBreak() override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  clearReadBuffer() override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  writeByte(uint_least8_t data) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  writeBlock(MaximInterfaceCore::span<const uint_least8_t> data) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<uint_least8_t>
  readByte() override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  readBlock(MaximInterfaceCore::span<uint_least8_t> data) override;

  MaximInterfaceDotnet_EXPORT static const MaximInterfaceCore::error_category &
  errorCategory();

private:
  template <typename Func>
  MaximInterfaceCore::Result<void> executeIfConnected(Func operation);

  struct Data;

  std::unique_ptr<Data> data;
};

} // namespace MaximInterfaceDotnet
namespace MaximInterfaceCore {

template <>
struct is_error_code_enum<MaximInterfaceDotnet::SerialPort::ErrorValue>
    : true_type {};

} // namespace MaximInterfaceCore
namespace MaximInterfaceDotnet {

inline MaximInterfaceCore::error_code
make_error_code(SerialPort::ErrorValue e) {
  return MaximInterfaceCore::error_code(e, SerialPort::errorCategory());
}

} // namespace MaximInterfaceDotnet

#endif
