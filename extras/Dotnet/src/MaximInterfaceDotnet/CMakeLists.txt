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

if(NOT MSVC)
  message(SEND_ERROR "MSVC must be used to build MaximInterfaceDotnet.")
endif()

string(REGEX REPLACE "(^| +)[/-]RTC[1csu]( +|$)"
                     " "
                     CMAKE_CXX_FLAGS_DEBUG
                     "${CMAKE_CXX_FLAGS_DEBUG}")
string(STRIP "${CMAKE_CXX_FLAGS_DEBUG}" CMAKE_CXX_FLAGS_DEBUG)

string(REGEX REPLACE "(^| +)[/-]EH[sacr-]+( +|$)"
                     " "
                     CMAKE_CXX_FLAGS
                     "${CMAKE_CXX_FLAGS}")
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
string(APPEND CMAKE_CXX_FLAGS " /EHa")

add_library(MaximInterfaceDotnet
            SHARED
            "Config.hpp"
            "OneWireLinkLayerMaster.cpp"
            "OneWireLinkLayerMaster.hpp"
            "SerialPort.cpp"
            "SerialPort.hpp"
            "Sleep.cpp"
            "Sleep.hpp")
MaximInterface_configureLibrary(MaximInterfaceDotnet)
target_compile_definitions(MaximInterfaceDotnet PRIVATE "NOMINMAX")
target_link_libraries(MaximInterfaceDotnet PUBLIC MaximInterface::Core)

set(dotnetFrameworkVersion "v4.5")
set(
  dotnetFrameworkDir
  "C:/Program Files (x86)/Reference Assemblies/Microsoft/Framework/.NETFramework/${dotnetFrameworkVersion}"
  )
if(NOT EXISTS ${dotnetFrameworkDir})
  string(REPLACE " (x86)" "" dotnetFrameworkDir ${dotnetFrameworkDir})
  if(NOT EXISTS ${dotnetFrameworkDir})
    message(SEND_ERROR ".NET Framework directory does not exist.")
  endif()
endif()
set(dotnetReferences
    "${dotnetFrameworkDir}/System.dll"
    "${CMAKE_CURRENT_SOURCE_DIR}/OneWireLinkLayer.dll"
    "${dotnetFrameworkDir}/mscorlib.dll")

if(CMAKE_GENERATOR MATCHES "^Visual Studio ([0-9]+)")
  if(NOT CMAKE_MATCH_1 GREATER_EQUAL 10)
    message(
      SEND_ERROR
        "Visual Studio 2010 or later is required to build MaximInterfaceDotnet."
      )
  endif()

  set_property(TARGET MaximInterfaceDotnet PROPERTY VS_GLOBAL_CLRSupport "true")
  set_property(TARGET MaximInterfaceDotnet
               PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION
                        ${dotnetFrameworkVersion})
  set_property(TARGET MaximInterfaceDotnet
               PROPERTY VS_DOTNET_REFERENCES ${dotnetReferences})
else()
  list(TRANSFORM dotnetReferences PREPEND "/FU")
  target_compile_options(MaximInterfaceDotnet
                         PRIVATE "/AI${dotnetFrameworkDir}"
                                 "/clr"
                                 ${dotnetReferences}
                                 "/clr:nostdlib")
endif()

install(FILES "OneWireLinkLayer.dll" DESTINATION ${CMAKE_INSTALL_LIBDIR})
