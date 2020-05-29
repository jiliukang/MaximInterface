Maxim Interface
===============

Maxim Interface is a library framework focused on providing flexible and expressive hardware interfaces. Both communication interfaces such as I2C and 1-Wire and device interfaces such as DS18B20 are supported. Modern C++ concepts are used extensively while keeping compatibility with C++98/C++03 and requiring no external dependencies. The embedded-friendly design does not depend on exceptions or RTTI.

A version of the project compatible with Arm Mbed is hosted on the Mbed website:  
<https://os.mbed.com/teams/Maxim-Integrated/code/MaximInterface>

Conventions
-----------

Maxim Interface uses a modular organization. Main submodules under the *libs* directory provide platform-independent access to the hardware. The Core submodule provides generic types and communication interfaces used by all other submodules. The Devices submodule provides support for various [Maxim Integrated](https://maximintegrated.com) parts. Additional submodules under the *extras* directory provide platform bindings that are typically consumed by the main submodules. Bindings for new platforms can be added by simply implementing the required interface.

Prerequisites
-------------

Building the project only requires a C++ compiler, [CMake](https://cmake.org) version 3.13 or later, and a CMake-compatible build system such as [Ninja](https://ninja-build.org). Most submodules are compatible with C++98/C++03. Asio and Qt bindings require a compiler with C++14 support. Dotnet bindings require the Microsoft Visual C++ (MSVC) compiler. Generating the API documentation requires [Doxygen](http://doxygen.nl) and the *dot* tool from the [Graphviz](https://www.graphviz.org) package.

The project has been tested with the following compilers:
* LLVM (clang) 8.0.0
* Visual Studio 2015
* GCC 4.8.3
* Arm Compiler 5 (armcc) 5.06
* IAR Embedded Workbench for ARM (EWARM) 7.50
* GNU Arm Embedded Toolchain 6.3.1

Quick start
-----------

CMake provides a unified configuration system across many toolchains, build systems, and operating systems. It does not build the project directly but instead generates input files for a build system such as Ninja, Visual Studio, or Make. This example uses the graphical CMake interface.

1. Launch the cmake-gui application.

2. Enter the location of the project source directory in the *Where is the source code* field.

3. Enter the location of the build directory in the *Where to build the binaries* field. This is typically a new directory that is either under or outside the source directory.

4. Click on the Configure button.

5. In the wizard that pops up, specify the generator for the project according to your build system. Examples of common generators are Ninja, Visual Studio 14 2015, and Unix Makefiles.

6. If cross-compiling for a different platform, select the *Specify toolchain file for cross-compiling* option, click on the Next button, and enter the path to the toolchain file. Example toolchain files can be found in the *tools* source subdirectory.

7. Click on the Finish button to exit the wizard.

8. In the list of CMake variables that is now populated, make selections to enable or disable submodules or otherwise customize the generated build system.

9. Click on the Generate button to create the build system in the build directory.

10. Invoke the build system normally using the input files in the build directory. Some generators support using the Open Project button as a shortcut.

11. Optionally, generate the API documentation by running Doxygen from the build directory. The output will be placed in the *docs* build subdirectory.

CMake variables
---------------

**BUILD_SHARED_LIBS** - Build supported targets as a shared library.

**CMAKE_BUILD_TYPE** - Specifies the build type, such as Debug or Release, on single-configuration generators.

**CMAKE_INSTALL_PREFIX** - Path where files will be installed by the build system.

**MaximInterface_configPostfix** - Append build configuration to library output names. Enables installing multiple configurations.

**MaximInterface_extras_Asio** - Build Asio platform support. If Asio cannot be found automatically, set the Asio_INCLUDE_DIR advanced variable.

**MaximInterface_extras_Asio_examples** - Build examples for MaximInterfaceAsio.

**MaximInterface_extras_Boost** - Build Boost platform support. See the [FindBoost](https://cmake.org/cmake/help/v3.13/module/FindBoost.html) documentation for variables that control finding Boost libraries.

**MaximInterface_extras_Boost_examples** - Build examples for MaximInterfaceBoost.

**MaximInterface_extras_Dotnet** - Build .NET platform support.

**MaximInterface_extras_Dotnet_examples** - Build examples for MaximInterfaceDotnet.

**MaximInterface_extras_MAX32625** - Build MAX32625 platform support. Requires that the MAX32625_Libraries_DIR variable is set to the directory containing MAX32625PeriphDriver and CMSIS.

**MaximInterface_extras_MAX32625_examples** - Build examples for MaximInterfaceMAX32625.

**MaximInterface_extras_MAX32660** - Build MAX32660 platform support. Requires that the MAX32660_Libraries_DIR variable is set to the directory containing MAX32660PeriphDriver and CMSIS.

**MaximInterface_extras_MAX32660_examples** - Build examples for MaximInterfaceMAX32660.

**MaximInterface_extras_Qt** - Build Qt platform support. If Qt cannot be found automatically, set the Qt5_DIR variable to the directory containing the package configuration file (Qt5Config.cmake) in the Qt installation.

**MaximInterface_extras_Qt_examples** - Build examples for MaximInterfaceQt.

**MaximInterface_libs_Core** - Build core functionality.

**MaximInterface_libs_Core_tests** - Build tests for MaximInterfaceCore.

**MaximInterface_libs_Devices** - Build various device support.

**MaximInterface_libs_Test** - Build testing support.

**MaximInterface_libs_Test_examples** - Build examples for MaximInterfaceTest.

License
-------

Maxim Interface is licensed under the MIT License. See the [license file](LICENSE.txt) for more information.
