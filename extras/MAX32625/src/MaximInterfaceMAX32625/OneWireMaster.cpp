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

#include "Error.hpp"
#include "OneWireMaster.hpp"

namespace MaximInterfaceMAX32625 {

using MaximInterfaceCore::none;
using MaximInterfaceCore::Result;

Result<void> OneWireMaster::initialize(const owm_cfg_t & owmCfg,
                                       const sys_cfg_owm_t & sysCfgOwm) {
  extStrongPup = (owmCfg.ext_pu_mode != OWM_EXT_PU_UNUSED);
  return makeResult(OWM_Init(owm, &owmCfg, &sysCfgOwm));
}

Result<void> OneWireMaster::shutdown() {
  return makeResult(OWM_Shutdown(owm));
}

Result<void> OneWireMaster::reset() {
  return OWM_Reset(owm) ? makeResult(none) : NoSlaveError;
}

Result<bool> OneWireMaster::touchBitSetLevel(bool sendBit, Level afterLevel) {
  const bool recvBit = OWM_TouchBit(owm, sendBit);
  MaximInterfaceCore_TRY(setLevel(afterLevel));
  return recvBit;
}

Result<void> OneWireMaster::writeByteSetLevel(uint_least8_t sendByte,
                                              Level afterLevel) {
  Result<void> result = makeResult(OWM_WriteByte(owm, sendByte));
  if (result) {
    result = setLevel(afterLevel);
  }
  return result;
}

Result<uint_least8_t> OneWireMaster::readByteSetLevel(Level afterLevel) {
  const uint_least8_t recvByte = OWM_ReadByte(owm);
  MaximInterfaceCore_TRY(setLevel(afterLevel));
  return recvByte;
}

Result<void> OneWireMaster::setSpeed(Speed newSpeed) {
  if (newSpeed == OverdriveSpeed) {
    OWM_SetOverdrive(owm, 1);
  } else if (newSpeed == StandardSpeed) {
    OWM_SetOverdrive(owm, 0);
  } else {
    return InvalidSpeedError;
  }
  return none;
}

Result<void> OneWireMaster::setLevel(Level newLevel) {
  if (extStrongPup) {
    if (newLevel == StrongLevel) {
      OWM_SetExtPullup(owm, 1);
    } else if (newLevel == NormalLevel) {
      OWM_SetExtPullup(owm, 0);
    } else {
      return InvalidLevelError;
    }
  } else if (newLevel != NormalLevel) {
    return InvalidLevelError;
  }
  return none;
}

} // namespace MaximInterfaceMAX32625
