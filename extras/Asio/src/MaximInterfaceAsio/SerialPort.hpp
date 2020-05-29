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

#ifndef MaximInterfaceAsio_SerialPort_hpp
#define MaximInterfaceAsio_SerialPort_hpp

#include <string>
#include <asio.hpp>
#include <MaximInterfaceCore/SerialPort.hpp>
#include "Config.hpp"

namespace MaximInterfaceAsio {

class SerialPort : public MaximInterfaceCore::SerialPort {
public:
  MaximInterfaceAsio_EXPORT SerialPort();

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  connect(const std::string & portName) override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  disconnect() override;

  /// @brief Check if currently connected to a port.
  /// @returns True if connected.
  MaximInterfaceAsio_EXPORT bool connected() const;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  setBaudRate(int_least32_t baudRate) override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  sendBreak() override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  clearReadBuffer() override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  writeByte(uint_least8_t data) override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  writeBlock(MaximInterfaceCore::span<const uint_least8_t> data) override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<uint_least8_t>
  readByte() override;

  MaximInterfaceAsio_EXPORT virtual MaximInterfaceCore::Result<void>
  readBlock(MaximInterfaceCore::span<uint_least8_t> data) override;

private:
  asio::io_context context;
  asio::serial_port port;
};

} // namespace MaximInterfaceAsio

#endif
