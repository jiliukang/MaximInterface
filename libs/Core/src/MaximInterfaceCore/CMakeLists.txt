#[[*****************************************************************************
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
******************************************************************************]]

add_library(MaximInterfaceCore
            "Algorithm.hpp"
            "array.hpp"
            "array_span.hpp"
            "ChangeSizeType.hpp"
            "Config.hpp"
            "Crc.cpp"
            "Crc.hpp"
            "Ecc256.cpp"
            "Ecc256.hpp"
            "Error.cpp"
            "Error.hpp"
            "FlagSet.hpp"
            "Function.hpp"
            "HexString.cpp"
            "HexString.hpp"
            "I2CMaster.cpp"
            "I2CMaster.hpp"
            "I2CMasterDecorator.cpp"
            "I2CMasterDecorator.hpp"
            "LoggingI2CMaster.cpp"
            "LoggingI2CMaster.hpp"
            "LoggingOneWireMaster.cpp"
            "LoggingOneWireMaster.hpp"
            "LoggingSleep.cpp"
            "LoggingSleep.hpp"
            "ManId.hpp"
            "None.hpp"
            "OneWireMaster.cpp"
            "OneWireMaster.hpp"
            "OneWireMasterDecorator.cpp"
            "OneWireMasterDecorator.hpp"
            "Optional.hpp"
            "Result.hpp"
            "RomCommands.cpp"
            "RomCommands.hpp"
            "RomId.hpp"
            "RunCommand.cpp"
            "RunCommand.hpp"
            "SafeBool.hpp"
            "SelectRom.cpp"
            "SelectRom.hpp"
            "Sleep.hpp"
            "SleepDecorator.cpp"
            "SleepDecorator.hpp"
            "span.hpp"
            "system_error.cpp"
            "system_error.hpp"
            "type_traits.hpp"
            "Uart.cpp"
            "Uart.hpp"
            "Unconstructible.hpp"
            "Uncopyable.hpp"
            "WriteMessage.hpp")
MaximInterface_configureLibrary(MaximInterfaceCore)

# Disable extraneous warnings from MSVC.
if(MSVC)
  target_compile_definitions(MaximInterfaceCore
                             PUBLIC "_SCL_SECURE_NO_WARNINGS"
                             PRIVATE "_CRT_SECURE_NO_WARNINGS")
endif()