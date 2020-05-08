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

include(FindPackageHandleStandardArgs)

function(_reportError)
  set(errorMessage "Could NOT find MAX32660PeriphDriver")
  if(NOT ARGV STREQUAL "")
    string(APPEND errorMessage " (missing:")
    foreach(item IN LISTS ARGV)
      string(APPEND errorMessage " ${item}")
    endforeach()
    string(APPEND errorMessage ")")
  endif()
  
  if(MAX32660PeriphDriver_FIND_REQUIRED)
    message(FATAL_ERROR ${errorMessage})
  endif()
  
  if(NOT MAX32660PeriphDriver_FIND_QUIETLY)
    message(STATUS ${errorMessage})
  endif()
  
  set(MAX32660PeriphDriver_FOUND FALSE)
endfunction(_reportError)

if(NOT DEFINED MAX32660_Libraries_DIR)
  set(MAX32660_Libraries_DIR ""
      CACHE PATH "MAX32660 firmware libraries directory.")
endif()

if(NOT IS_ABSOLUTE ${MAX32660_Libraries_DIR})
  _reportError("MAX32660_Libraries_DIR")
  return()
endif()

set(MAX32660PeriphDriver_LIBRARIES
    "${MAX32660_Libraries_DIR}/MAX32660PeriphDriver/Build/PeriphDriver.a")

if(NOT EXISTS ${MAX32660PeriphDriver_LIBRARIES})
  _reportError(${MAX32660PeriphDriver_LIBRARIES})
  unset(MAX32660PeriphDriver_LIBRARIES)
  return()
endif()

find_package_handle_standard_args(MAX32660PeriphDriver HANDLE_COMPONENTS
                                  REQUIRED_VARS MAX32660PeriphDriver_LIBRARIES)

if(MAX32660PeriphDriver_FOUND)
  set(MAX32660PeriphDriver_INCLUDE_DIRS
      "${MAX32660_Libraries_DIR}/MAX32660PeriphDriver/Include"
      "${MAX32660_Libraries_DIR}/CMSIS/Include"
      "${MAX32660_Libraries_DIR}/CMSIS/Device/Maxim/MAX32660/Include")
  set(MAX32660PeriphDriver_DEFINITIONS "TARGET=MAX32660" "TARGET_REV=0x4131")
  
  if(NOT TARGET MAX32660PeriphDriver::MAX32660PeriphDriver)
    add_library(MAX32660PeriphDriver::MAX32660PeriphDriver STATIC IMPORTED)
    set_property(TARGET MAX32660PeriphDriver::MAX32660PeriphDriver
                 PROPERTY IMPORTED_LOCATION ${MAX32660PeriphDriver_LIBRARIES})
    set_property(TARGET MAX32660PeriphDriver::MAX32660PeriphDriver
                 PROPERTY INTERFACE_COMPILE_DEFINITIONS
                          ${MAX32660PeriphDriver_DEFINITIONS})
    set_property(TARGET MAX32660PeriphDriver::MAX32660PeriphDriver
                 PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                          ${MAX32660PeriphDriver_INCLUDE_DIRS})
  endif()
else()
  unset(MAX32660PeriphDriver_LIBRARIES)
endif()