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

#include "Test.hpp"

extern "C" {

void MaximInterfaceTest_report_info(const char * message, const char * file,
                                    int line);

void MaximInterfaceTest_report_warning(const char * message, const char * file,
                                       int line);

void MaximInterfaceTest_report_error(const char * message, const char * file,
                                     int line);

void MaximInterfaceTest_report_fatal(const char * message, const char * file,
                                     int line);

} // extern "C"

namespace MaximInterfaceTest {

void info(const char * message, const char * file, int line) {
  MaximInterfaceTest_report_info(message, file, line);
}

void warning(const char * message, const char * file, int line) {
  MaximInterfaceTest_report_warning(message, file, line);
}

void error(const char * message, const char * file, int line) {
  MaximInterfaceTest_report_error(message, file, line);
}

void fatal(const char * message, const char * file, int line) {
  MaximInterfaceTest_report_fatal(message, file, line);
}

namespace detail {

void reportMessage(Severity & severity, const char * message, const char * file,
                   int line) {
  severity(message, file, line);
}

void reportMessage(Severity & severity, const std::string & message,
                   const char * file, int line) {
  reportMessage(severity, message.c_str(), file, line);
}

void insertAssertionFailedPrefix(std::ostream & stream, const char * expected) {
  stream << "Assertion '" << expected << "' failed.\nActual result was '"
         << std::boolalpha;
}

void insertAssertionFailedSuffix(std::ostream & stream) { stream << "'."; }

} // namespace detail
} // namespace MaximInterfaceTest
