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

#include <MaximInterfaceCore/OneWireMaster.hpp>
#include <MaximInterfaceCore/Uart.hpp>
#include <MaximInterfaceCore/system_error.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using namespace MaximInterfaceCore;
using MaximInterfaceTest::error;
using std::ostream;

enum ErrorConditionEnum { ErrorConditionEnumValue = 5 };

namespace MaximInterfaceCore {

template <> struct is_error_condition_enum<ErrorConditionEnum> : true_type {};

} // namespace MaximInterfaceCore

static error_condition make_error_condition(ErrorConditionEnum e) {
  return error_condition(e, system_category());
}

static ostream & operator<<(ostream & os, const error_category & ec) {
  os << &ec;
  return os;
}

static ostream & operator<<(ostream & os, const error_condition & ec) {
  os << ec.category().name() << ':' << ec.value();
  return os;
}

static void test_error_condition() {
  // Test default constructor.
  error_condition a;
  ASSERT_EQUAL(error, a.value(), 0);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_FALSE(error, a);

  // Test operator== function.
  error_condition b;
  ASSERT_EQUAL(error, a, b);

  // Test value and category constructor.
  a = error_condition(OneWireMaster::NoSlaveError,
                      OneWireMaster::errorCategory());
  ASSERT_EQUAL(error, a.value(), OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, a.category(), OneWireMaster::errorCategory());
  ASSERT_TRUE(error, a);

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test error condition enum constructor.
  a = error_condition(ErrorConditionEnumValue);
  ASSERT_EQUAL(error, a.value(), ErrorConditionEnumValue);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_TRUE(error, a);

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test assign function.
  a.assign(Uart::OverrunError, Uart::errorCategory());
  ASSERT_EQUAL(error, a.value(), Uart::OverrunError);
  ASSERT_EQUAL(error, a.category(), Uart::errorCategory());
  ASSERT_TRUE(error, a);

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test clear function.
  a.clear();
  ASSERT_EQUAL(error, a.value(), 0);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_FALSE(error, a);

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test assignment from error code enum.
  a = ErrorConditionEnumValue;
  ASSERT_EQUAL(error, a.value(), ErrorConditionEnumValue);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_TRUE(error, a);

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test operator< function.
  b.assign(ErrorConditionEnumValue + 1, system_category());
  ASSERT_LESS(error, a, b);
}

static void test_error_code() {
  // Test default constructor.
  error_code a;
  ASSERT_EQUAL(error, a.value(), 0);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_FALSE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(), 0);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               system_category());
  ASSERT_FALSE(error, a.default_error_condition());
  ASSERT_EQUAL(error, a, a.default_error_condition());

  // Test operator== function.
  error_code b;
  ASSERT_EQUAL(error, a, b);

  // Test value and category constructor.
  a = error_code(OneWireMaster::NoSlaveError, OneWireMaster::errorCategory());
  ASSERT_EQUAL(error, a.value(), OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, a.category(), OneWireMaster::errorCategory());
  ASSERT_TRUE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(),
               OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               OneWireMaster::errorCategory());
  ASSERT_TRUE(error, a.default_error_condition());

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test inequivalence between error_code and error_condition through
  // equivalent and operator!= functions.
  ASSERT_FALSE(error,
               a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_FALSE(error, b.category().equivalent(a, b.value()));
  ASSERT_NOT_EQUAL(error, a, b.default_error_condition());

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test equivalence between error_code and error_condition through equivalent
  // and operator== functions.
  ASSERT_TRUE(error,
              a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_TRUE(error, b.category().equivalent(a, b.value()));
  ASSERT_EQUAL(error, a, b.default_error_condition());

  // Test error code enum constructor.
  a = error_code(Uart::TimeoutError);
  ASSERT_EQUAL(error, a.value(), Uart::TimeoutError);
  ASSERT_EQUAL(error, a.category(), Uart::errorCategory());
  ASSERT_TRUE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(), Uart::TimeoutError);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               Uart::errorCategory());
  ASSERT_TRUE(error, a.default_error_condition());

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test inequivalence between error_code and error_condition through
  // equivalent and operator!= functions.
  ASSERT_FALSE(error,
               a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_FALSE(error, b.category().equivalent(a, b.value()));
  ASSERT_NOT_EQUAL(error, b.default_error_condition(), a);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test equivalence between error_code and error_condition through equivalent
  // and operator== functions.
  ASSERT_TRUE(error,
              a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_TRUE(error, b.category().equivalent(a, b.value()));
  ASSERT_EQUAL(error, b.default_error_condition(), a);

  // Test assignment from error code enum.
  a = OneWireMaster::ShortDetectedError;
  ASSERT_EQUAL(error, a.value(), OneWireMaster::ShortDetectedError);
  ASSERT_EQUAL(error, a.category(), OneWireMaster::errorCategory());
  ASSERT_TRUE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(),
               OneWireMaster::ShortDetectedError);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               OneWireMaster::errorCategory());
  ASSERT_TRUE(error, a.default_error_condition());

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test inequivalence between error_code and error_condition through
  // equivalent and operator!= functions.
  ASSERT_FALSE(error,
               a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_FALSE(error, b.category().equivalent(a, b.value()));
  ASSERT_NOT_EQUAL(error, a, b.default_error_condition());

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test equivalence between error_code and error_condition through equivalent
  // and operator== functions.
  ASSERT_TRUE(error,
              a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_TRUE(error, b.category().equivalent(a, b.value()));
  ASSERT_EQUAL(error, a, b.default_error_condition());

  // Test assign function.
  a.assign(Uart::OverrunError, Uart::errorCategory());
  ASSERT_EQUAL(error, a.value(), Uart::OverrunError);
  ASSERT_EQUAL(error, a.category(), Uart::errorCategory());
  ASSERT_TRUE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(), Uart::OverrunError);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               Uart::errorCategory());
  ASSERT_TRUE(error, a.default_error_condition());

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test inequivalence between error_code and error_condition through
  // equivalent and operator!= functions.
  ASSERT_FALSE(error,
               a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_FALSE(error, b.category().equivalent(a, b.value()));
  ASSERT_NOT_EQUAL(error, b.default_error_condition(), a);

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test equivalence between error_code and error_condition through equivalent
  // and operator== functions.
  ASSERT_TRUE(error,
              a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_TRUE(error, b.category().equivalent(a, b.value()));
  ASSERT_EQUAL(error, b.default_error_condition(), a);

  // Test clear function.
  a.clear();
  ASSERT_EQUAL(error, a.value(), 0);
  ASSERT_EQUAL(error, a.category(), system_category());
  ASSERT_FALSE(error, a);

  // Test default_error_condition function.
  ASSERT_EQUAL(error, a.default_error_condition().value(), 0);
  ASSERT_EQUAL(error, a.default_error_condition().category(),
               system_category());
  ASSERT_FALSE(error, a.default_error_condition());

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test inequivalence between error_code and error_condition through
  // equivalent and operator!= functions.
  ASSERT_FALSE(error,
               a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_FALSE(error, b.category().equivalent(a, b.value()));
  ASSERT_NOT_EQUAL(error, a, b.default_error_condition());

  // Test operator== function.
  b = a;
  ASSERT_EQUAL(error, a, b);

  // Test equivalence between error_code and error_condition through equivalent
  // and operator== functions.
  ASSERT_TRUE(error,
              a.category().equivalent(a.value(), b.default_error_condition()));
  ASSERT_TRUE(error, a.category().equivalent(a, b.value()));
  ASSERT_EQUAL(error, a, b.default_error_condition());

  // Test operator< function.
  b.assign(1, system_category());
  ASSERT_LESS(error, a, b);
}

static void test_system_error() {
  system_error e = error_code(OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, e.code().value(), OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, e.code().category(), OneWireMaster::errorCategory());

  e = system_error(error_code(Uart::TimeoutError),
                   std::string("Timeout Error"));
  ASSERT_EQUAL(error, e.code().value(), Uart::TimeoutError);
  ASSERT_EQUAL(error, e.code().category(), Uart::errorCategory());

  e = system_error(error_code(OneWireMaster::ShortDetectedError),
                   "Short Detected Error");
  ASSERT_EQUAL(error, e.code().value(), OneWireMaster::ShortDetectedError);
  ASSERT_EQUAL(error, e.code().category(), OneWireMaster::errorCategory());

  e = system_error(Uart::OverrunError, Uart::errorCategory());
  ASSERT_EQUAL(error, e.code().value(), Uart::OverrunError);
  ASSERT_EQUAL(error, e.code().category(), Uart::errorCategory());

  e = system_error(OneWireMaster::NoSlaveError, OneWireMaster::errorCategory(),
                   std::string("No Slave Error"));
  ASSERT_EQUAL(error, e.code().value(), OneWireMaster::NoSlaveError);
  ASSERT_EQUAL(error, e.code().category(), OneWireMaster::errorCategory());

  e = system_error(Uart::TimeoutError, Uart::errorCategory(), "Timeout Error");
  ASSERT_EQUAL(error, e.code().value(), Uart::TimeoutError);
  ASSERT_EQUAL(error, e.code().category(), Uart::errorCategory());
}

void runTest() {
  test_error_condition();
  test_error_code();
  test_system_error();
}
