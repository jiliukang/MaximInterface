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

#include <mxc_device.h>
#include "Error.hpp"
#include "I2CMaster.hpp"

#define MXC_I2CM_TX_TIMEOUT 0x5000
#define MXC_I2CM_RX_TIMEOUT 0x5000

namespace MaximInterfaceMAX32625 {

using MaximInterfaceCore::Result;

static int I2CM_WriteTxFifo(mxc_i2cm_regs_t * i2cm, mxc_i2cm_fifo_regs_t * fifo,
                            const uint16_t data) {
  int32_t timeout = MXC_I2CM_TX_TIMEOUT;

  // Read the TX FIFO to determine if it's full
  do {

    // Wait for the TX FIFO to have room and check for errors
    if (i2cm->intfl &
        (MXC_F_I2CM_INTFL_TX_NACKED | MXC_F_I2CM_INTFL_TX_LOST_ARBITR)) {

      return E_COMM_ERR;
    }

    if ((i2cm->intfl & MXC_F_I2CM_INTFL_TX_TIMEOUT) || !timeout--) {
      return E_TIME_OUT;
    }

  } while (fifo->tx);

  fifo->tx = data;

  return E_NO_ERROR;
}

static void I2CM_Recover(mxc_i2cm_regs_t * i2cm) {
  // Disable and clear interrupts
  i2cm->inten = 0;
  i2cm->intfl = i2cm->intfl;
  i2cm->ctrl = MXC_F_I2CM_CTRL_MSTR_RESET_EN;
  i2cm->ctrl = MXC_F_I2CM_CTRL_TX_FIFO_EN | MXC_F_I2CM_CTRL_RX_FIFO_EN;
}

static int I2CM_TxInProgress(mxc_i2cm_regs_t * i2cm) {
  int32_t timeout = MXC_I2CM_TX_TIMEOUT;

  while ((i2cm->trans & MXC_F_I2CM_TRANS_TX_IN_PROGRESS) && --timeout);

  if (i2cm->intfl &
      (MXC_F_I2CM_INTFL_TX_NACKED | MXC_F_I2CM_INTFL_TX_LOST_ARBITR)) {
    I2CM_Recover(i2cm);
    return E_COMM_ERR;
  }

  if ((i2cm->intfl & MXC_F_I2CM_INTFL_TX_TIMEOUT) && !timeout--) {
    I2CM_Recover(i2cm);
    return E_TIME_OUT;
  }

  return E_NO_ERROR;
}

static void verifyTransactionStarted(mxc_i2cm_regs_t & i2cm) {
  if (!(i2cm.trans & MXC_F_I2CM_TRANS_TX_IN_PROGRESS)) {
    i2cm.trans |= MXC_F_I2CM_TRANS_TX_START;
  }
}

Result<void> I2CMaster::initialize(const sys_cfg_i2cm_t & sysCfg,
                                   i2cm_speed_t speed) {
  return makeResult(I2CM_Init(i2cm, &sysCfg, speed));
}

Result<void> I2CMaster::shutdown() { return makeResult(I2CM_Shutdown(i2cm)); }

Result<void> I2CMaster::writeByte(uint_least8_t data, bool start) {
  const uint16_t fifoData =
      static_cast<uint16_t>(data & 0xFF) |
      (start ? MXC_S_I2CM_TRANS_TAG_START : MXC_S_I2CM_TRANS_TAG_TXDATA_ACK);
  Result<void> result = makeResult(I2CM_WriteTxFifo(i2cm, i2cmFifo, fifoData));
  if (!result) {
    I2CM_Recover(i2cm);
    return result;
  }
  verifyTransactionStarted(*i2cm);
  result = makeResult(I2CM_TxInProgress(i2cm));
  return result;
}

Result<void> I2CMaster::start(uint_least8_t address) {
  return writeByte(address, true);
}

Result<void> I2CMaster::stop() {
  Result<void> result =
      makeResult(I2CM_WriteTxFifo(i2cm, i2cmFifo, MXC_S_I2CM_TRANS_TAG_STOP));
  if (result) {
    result = makeResult(I2CM_TxInProgress(i2cm));
  }
  return result;
}

Result<void> I2CMaster::writeByte(uint_least8_t data) {
  return writeByte(data, false);
}

Result<uint_least8_t> I2CMaster::readByte(DoAck doAck) {
  uint16_t fifoData = (doAck == Nack) ? MXC_S_I2CM_TRANS_TAG_RXDATA_NACK
                                      : MXC_S_I2CM_TRANS_TAG_RXDATA_COUNT;
  if (const MaximInterfaceCore::error_code error =
          makeErrorCode(I2CM_WriteTxFifo(i2cm, i2cmFifo, fifoData))) {
    I2CM_Recover(i2cm);
    return error;
  }
  verifyTransactionStarted(*i2cm);
  do {
    int timeout = MXC_I2CM_RX_TIMEOUT;
    while ((i2cm->bb & MXC_F_I2CM_BB_RX_FIFO_CNT) == 0) {
      if ((--timeout <= 0) || (i2cm->trans & MXC_F_I2CM_TRANS_TX_TIMEOUT)) {
        I2CM_Recover(i2cm);
        return makeErrorCode(E_TIME_OUT);
      }
      if (i2cm->trans &
          (MXC_F_I2CM_TRANS_TX_LOST_ARBITR | MXC_F_I2CM_TRANS_TX_NACKED)) {
        I2CM_Recover(i2cm);
        return NackError;
      }
    }
    fifoData = i2cmFifo->rx;
  } while (fifoData & MXC_S_I2CM_RSTLS_TAG_EMPTY);
  return fifoData & 0xFF;
}

} // namespace MaximInterfaceMAX32625
