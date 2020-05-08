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

#include <boost/config.hpp>
#include <boost/system/system_error.hpp>
#include "Error.hpp"
#include "SerialPort.hpp"
#include "Sleep.hpp"

namespace MaximInterfaceBoost {

using namespace MaximInterfaceCore;
namespace asio = boost::asio;
namespace system = boost::system;

static const int timeout = 1000;

static void asyncCallback(const system::error_code & ec, size_t) {
  if (ec && ec != asio::error::operation_aborted) {
    throw system::system_error(ec);
  }
}

SerialPort::SerialPort() : context(), port(context) {}

template <typename Arg>
Result<void>
SerialPort::executeTryCatchOperation(void (SerialPort::*operation)(Arg),
                                     Arg arg) {
  try {
    (this->*operation)(arg);
  } catch (const system::system_error & e) {
    return convertErrorCode(e.code());
  } catch (const system_error & e) {
    return e.code();
  }
  return none;
}

Result<void> SerialPort::connect(const std::string & portName) {
  return executeTryCatchOperation<const std::string &>(&SerialPort::doConnect,
                                                       portName);
}

void SerialPort::doConnect(const std::string & portName) {
  typedef asio::serial_port::flow_control flow_control;

  port.open(portName);
  port.set_option(flow_control(flow_control::hardware));
}

Result<void> SerialPort::disconnect() {
  return executeTryCatchOperation(&SerialPort::doDisconnect, none);
}

void SerialPort::doDisconnect(None) { port.close(); }

bool SerialPort::connected() const { return port.is_open(); }

Result<void> SerialPort::setBaudRate(int_least32_t baudRate) {
  return executeTryCatchOperation(&SerialPort::doSetBaudRate, baudRate);
}

void SerialPort::doSetBaudRate(int_least32_t baudRate) {
  port.set_option(asio::serial_port::baud_rate(baudRate));
}

#ifdef BOOST_WINDOWS

Result<void> SerialPort::sendBreak() {
  // GetLastError can be used with boost::system::system_category according to
  // "boost/system/windows_error.hpp".

  BOOL result = SetCommBreak(port.native_handle());
  if (!result) {
    return error_code(GetLastError(), systemCategory());
  }
  sleep(1);
  result = ClearCommBreak(port.native_handle());
  if (!result) {
    return error_code(GetLastError(), systemCategory());
  }
  return none;
}

#else

Result<void> SerialPort::sendBreak() {
  return executeTryCatchOperation(&SerialPort::doSendBreak, none);
}

void SerialPort::doSendBreak(None) { port.send_break(); }

#endif // BOOST_WINDOWS

Result<void> SerialPort::clearReadBuffer() {
  return executeTryCatchOperation(&SerialPort::doClearReadBuffer, none);
}

void SerialPort::doClearReadBuffer(None) {
  do {
    uint_least8_t data[10];
    async_read(port, asio::buffer(data), asyncCallback);
    context.restart();
    context.run_for(asio::chrono::milliseconds(10));
  } while (context.stopped());
  port.cancel();
  context.run();
}

Result<void> SerialPort::writeByte(uint_least8_t data) {
  return writeBlock(make_span(&data, 1));
}

Result<void> SerialPort::writeBlock(span<const uint_least8_t> data) {
  return executeTryCatchOperation(&SerialPort::doWriteBlock, data);
}

void SerialPort::doWriteBlock(span<const uint_least8_t> data) {
  async_write(port, asio::buffer(data.data(), data.size()), asyncCallback);
  context.restart();
  context.run_for(asio::chrono::milliseconds(timeout));
  if (!context.stopped()) {
    port.cancel();
    context.run();
    throw system_error(TimeoutError);
  }
}

Result<uint_least8_t> SerialPort::readByte() {
  uint_least8_t data;
  MaximInterfaceCore_TRY(readBlock(make_span(&data, 1)));
  return data;
}

Result<void> SerialPort::readBlock(span<uint_least8_t> data) {
  return executeTryCatchOperation(&SerialPort::doReadBlock, data);
}

void SerialPort::doReadBlock(span<uint_least8_t> data) {
  async_read(port, asio::buffer(data.data(), data.size()), asyncCallback);
  context.restart();
  context.run_for(asio::chrono::milliseconds(timeout));
  if (!context.stopped()) {
    port.cancel();
    context.run();
    throw system_error(TimeoutError);
  }
}

} // namespace MaximInterfaceBoost