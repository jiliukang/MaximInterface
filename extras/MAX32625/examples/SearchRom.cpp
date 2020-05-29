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

#include <cstdlib>
#include <iostream>
#include <MaximInterfaceCore/HexString.hpp>
#include <MaximInterfaceCore/RomCommands.hpp>
#include <MaximInterfaceMAX32625/OneWireMaster.hpp>

static MaximInterfaceMAX32625::OneWireMaster oneWireMaster(*MXC_OWM);

static const owm_cfg_t owmCfg = {
    /* int_pu_en */ true,
    /* ext_pu_mode */ OWM_EXT_PU_UNUSED,
    /* long_line_mode */ false,
    /* overdrive_spec */ OWM_OVERDRIVE_UNUSED};

static const sys_cfg_owm_t sysCfgOwm = {
    /* clk_scale */ CLKMAN_SCALE_DIV_1,
    /* io_cfg */ {/* req_reg */ &MXC_IOMAN->owm_req,
                  /* ack_reg */ &MXC_IOMAN->owm_ack,
                  /* req_val */ {/* value */ 0x10}}};

int main() {
  MaximInterfaceCore::Result<void> result =
      oneWireMaster.initialize(owmCfg, sysCfgOwm);
  if (!result) {
    std::cout << "Failed to initialize 1-Wire master: "
              << result.error().message() << std::endl;
    return EXIT_FAILURE;
  }

  MaximInterfaceCore::SearchRomState searchState;
  do {
    result = searchRom(oneWireMaster, searchState);
    if (!result) {
      std::cout << "Failed to find device: " << result.error().message()
                << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << "Found ROM ID: " << toHexString(searchState.romId)
              << std::endl;
  } while (!searchState.lastDevice);

  return EXIT_SUCCESS;
}
