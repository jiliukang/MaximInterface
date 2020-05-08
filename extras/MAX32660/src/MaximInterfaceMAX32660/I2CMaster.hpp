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

#ifndef MaximInterfaceMAX32660_I2CMaster_hpp
#define MaximInterfaceMAX32660_I2CMaster_hpp

#include <MaximInterfaceCore/I2CMaster.hpp>
#include <i2c.h>

namespace MaximInterfaceMAX32660 {

class I2CMaster : public MaximInterfaceCore::I2CMaster {
public:
  explicit I2CMaster(mxc_i2c_regs_t & i2c) : i2c(&i2c) {}

  void set_mxc_i2c(mxc_i2c_regs_t & i2c) { this->i2c = &i2c; }

  /// Initialize the hardware interface for use.
  MaximInterfaceCore::Result<void> initialize(i2c_speed_t speed);

  /// Shutdown the hardware interface after use.
  MaximInterfaceCore::Result<void> shutdown();

  virtual MaximInterfaceCore::Result<void> start(uint_least8_t address);

  virtual MaximInterfaceCore::Result<void> stop();

  virtual MaximInterfaceCore::Result<void> writeByte(uint_least8_t data);

  virtual MaximInterfaceCore::Result<uint_least8_t> readByte(DoAck doAck);

private:
  MaximInterfaceCore::Result<void> writeByte(uint_least8_t data, bool start);

  mxc_i2c_regs_t * i2c;
};

} // namespace MaximInterfaceMAX32660

#endif
