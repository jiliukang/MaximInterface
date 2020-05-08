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
#include <QCoreApplication>
#include <MaximInterfaceCore/HexString.hpp>
#include <MaximInterfaceCore/RomCommands.hpp>
#include <MaximInterfaceDevices/DS9481P_300.hpp>
#include <MaximInterfaceQt/SerialPort.hpp>
#include <MaximInterfaceQt/Sleep.hpp>

static MaximInterfaceQt::Sleep sleep;
static MaximInterfaceQt::SerialPort port;
static MaximInterfaceDevices::DS9481P_300 adapter(sleep, port);

int main(int argc, char * argv[]) {
  namespace Core = MaximInterfaceCore;

  QCoreApplication app(argc, argv);
  if (argc != 2) {
    std::cout << "Usage: " << (argv[0] ? argv[0] : "ProgramName")
              << " AdapterPort" << std::endl;
    return EXIT_FAILURE;
  }

  {
    const Core::Result<void> result = adapter.connect(argv[1]);
    if (!result) {
      std::cout << "Failed to open DS9481P-300: " << result.error().message()
                << std::endl;
      return EXIT_FAILURE;
    }
  }

  const Core::Result<Core::RomId::array> romId =
      readRom(adapter.oneWireMaster());
  if (!romId) {
    std::cout << "Failed to read ROM ID: " << romId.error().message()
              << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Read ROM ID: " << toHexString(romId.value()) << std::endl;
  return EXIT_SUCCESS;
}