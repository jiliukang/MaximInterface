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

add_library(MaximInterfaceDevices
            "Config.hpp"
            "DS18B20.cpp"
            "DS18B20.hpp"
            "DS28C16.cpp"
            "DS28C16.hpp"
            "DS28C36_DS2476.cpp"
            "DS28C36_DS2476.hpp"
            "DS28C39.cpp"
            "DS28C39.hpp"
            "DS28C40.cpp"
            "DS28C40.hpp"
            "DS28E15_22_25.cpp"
            "DS28E15_22_25.hpp"
            "DS28E16.cpp"
            "DS28E16.hpp"
            "DS28E17.cpp"
            "DS28E17.hpp"
            "DS28E38.cpp"
            "DS28E38.hpp"
            "DS28E39.cpp"
            "DS28E39.hpp"
            "DS28E83_DS28E84.cpp"
            "DS28E83_DS28E84.hpp"
            "DS1920.cpp"
            "DS1920.hpp"
            "DS2413.cpp"
            "DS2413.hpp"
            "DS2431.cpp"
            "DS2431.hpp"
            "DS2465.cpp"
            "DS2465.hpp"
            "DS2480B.cpp"
            "DS2480B.hpp"
            "DS2482_DS2484.cpp"
            "DS2482_DS2484.hpp"
            "DS9400.cpp"
            "DS9400.hpp"
            "DS9481P_300.cpp"
            "DS9481P_300.hpp")
MaximInterface_configureLibrary(MaximInterfaceDevices)
target_link_libraries(MaximInterfaceDevices PUBLIC MaximInterface::Core)
