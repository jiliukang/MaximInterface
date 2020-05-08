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

cmake_minimum_required(VERSION 3.13)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

project(MaximInterface LANGUAGES CXX VERSION 2.1)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(EXPORT_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/MaximInterface")
option(BUILD_SHARED_LIBS "Build supported targets as a shared library." OFF)
option(MaximInterface_configPostfix
       "Append build configuration to library output names." OFF)

# Enable testing if this is a native build or a target emulator is available.
if(NOT CMAKE_CROSSCOMPILING OR CMAKE_CROSSCOMPILING_EMULATOR)
  enable_testing()
endif()

# Configure submodule library target.
function(MaximInterface_configureLibrary target)
  if(NOT ARGC EQUAL 1)
    message(SEND_ERROR "Unexpected function argument.")
  endif()
  
  get_property(sourceDir TARGET ${target} PROPERTY SOURCE_DIR)
  
  # Search in parent directory for includes.
  get_filename_component(buildIncludeDir ${sourceDir} DIRECTORY)
  target_include_directories(${target}
                             PUBLIC $<BUILD_INTERFACE:${buildIncludeDir}>)
  
  # Export DLL symbols.
  if(MSVC)
    get_property(type TARGET ${target} PROPERTY TYPE)
    if(type STREQUAL "SHARED_LIBRARY")
      target_compile_definitions(
        ${target}
        PRIVATE "${target}_EXPORT=__declspec(dllexport)"
        INTERFACE "${target}_EXPORT=__declspec(dllimport)")
    endif()
  endif()
  
  if(MaximInterface_configPostfix)
    set_property(
      TARGET ${target}
      PROPERTY OUTPUT_NAME
               "${target}-$<IF:$<CONFIG:>,noconfig,$<LOWER_CASE:$<CONFIG>>>")
  endif()
  
  # Determine the component and export name.
  string(REGEX REPLACE "^MaximInterface" "" component ${target})
  set(exportName "MaximInterface::${component}")
  set_property(TARGET ${target} PROPERTY EXPORT_NAME ${exportName})
  
  # Add an alias from the export name to the target.
  add_library(${exportName} ALIAS ${target})
  
  # Set up target install and export.
  install(DIRECTORY ${sourceDir}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          FILES_MATCHING
          PATTERN "*.hpp"
          PATTERN ".svn" EXCLUDE)
  install(TARGETS ${target}
          EXPORT ${component}
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
          INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(EXPORT ${component} DESTINATION "${EXPORT_INSTALL_DIR}/components")
  export(TARGETS ${target}
         FILE "${PROJECT_BINARY_DIR}/components/${component}.cmake")
endfunction(MaximInterface_configureLibrary)

# Set up package export.
configure_file("MaximInterfaceConfig.cmake" "MaximInterfaceConfig.cmake"
               COPYONLY)
write_basic_package_version_file("MaximInterfaceConfigVersion.cmake"
                                 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/MaximInterfaceConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/MaximInterfaceConfigVersion.cmake"
        DESTINATION ${EXPORT_INSTALL_DIR})

# Generate Doxygen configuration.
configure_file("Doxyfile.in" "Doxyfile")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/DoxygenFooter.tex" "\\end{document}")

add_subdirectory("extras")
add_subdirectory("libs")