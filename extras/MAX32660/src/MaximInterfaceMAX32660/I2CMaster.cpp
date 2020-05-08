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

#include <stddef.h>
#include <mxc_device.h>
#include "Error.hpp"
#include "I2CMaster.hpp"

namespace MaximInterfaceMAX32660 {

using MaximInterfaceCore::Result;

static Result<void> checkError(const mxc_i2c_regs_t & i2c) {
  const uint32_t i2cError =
      MXC_F_I2C_INT_FL0_ARB_ER | MXC_F_I2C_INT_FL0_TO_ER |
      MXC_F_I2C_INT_FL0_ADDR_NACK_ER | MXC_F_I2C_INT_FL0_DATA_ER |
      MXC_F_I2C_INT_FL0_DO_NOT_RESP_ER | MXC_F_I2C_INT_FL0_START_ER |
      MXC_F_I2C_INT_FL0_STOP_ER;
  return (i2c.int_fl0 & i2cError) ? makeErrorCode(E_COMM_ERR)
                                  : makeResult(MaximInterfaceCore::none);
}

Result<void> I2CMaster::initialize(i2c_speed_t speed) {
  return makeResult(I2C_Init(i2c, speed, NULL));
}

Result<void> I2CMaster::shutdown() { return makeResult(I2C_Shutdown(i2c)); }

Result<void> I2CMaster::writeByte(uint_least8_t data, bool start) {
  if (start) {
    // Clear the lock out bit (W1C) in case it is set.
    i2c->int_fl0 = MXC_F_I2C_INT_FL0_TX_LOCK_OUT;

    // Enable master mode and interactive receive mode.
    i2c->ctrl |= (MXC_F_I2C_CTRL_MST | MXC_F_I2C_CTRL_RX_MODE);

    // Send start condition.
    i2c->master_ctrl |= MXC_F_I2C_MASTER_CTRL_START;
  }

  while (i2c->status & MXC_F_I2C_STATUS_TX_FULL) {
    const Result<void> result = checkError(*i2c);
    if (!result) {
      return result;
    }
  }
  i2c->fifo = data;
  while (!(i2c->status & MXC_F_I2C_STATUS_TX_EMPTY)) {
    const Result<void> result = checkError(*i2c);
    if (!result) {
      return result;
    }
  }

  return checkError(*i2c);
}

Result<void> I2CMaster::start(uint_least8_t address) {
  return writeByte(address, true);
}

Result<void> I2CMaster::stop() {
  // Send stop condition.
  i2c->master_ctrl |= MXC_F_I2C_MASTER_CTRL_STOP;

  // Wait for Done flag.
  while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_DONE)) {
    const Result<void> result = checkError(*i2c);
    if (!result) {
      return result;
    }
  }
  i2c->int_fl0 = MXC_F_I2C_INT_FL0_DONE;

  // Wait for Stop flag.
  while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_STOP)) {
    const Result<void> result = checkError(*i2c);
    if (!result) {
      return result;
    }
  }
  i2c->int_fl0 = MXC_F_I2C_INT_FL0_STOP;

  return checkError(*i2c);
}

Result<void> I2CMaster::writeByte(uint_least8_t data) {
  return writeByte(data, false);
}

Result<uint_least8_t> I2CMaster::readByte(DoAck doAck) {
  // Set receive count.
  i2c->rx_ctrl1 = 1;

  // Wait for received data and send ACK or NACK response.
  while (!(i2c->int_fl0 & MXC_F_I2C_INT_FL0_RX_MODE)) {
    MaximInterfaceCore_TRY(checkError(*i2c));
  }
  if (doAck == Nack) {
    i2c->ctrl |= MXC_F_I2C_CTRL_RX_MODE_ACK;
  } else {
    i2c->ctrl &= ~MXC_F_I2C_CTRL_RX_MODE_ACK;
  }
  i2c->int_fl0 = MXC_F_I2C_INT_FL0_RX_MODE;

  const uint_least8_t data = i2c->fifo;
  MaximInterfaceCore_TRY(checkError(*i2c));
  return data;
}

} // namespace MaximInterfaceMAX32660
