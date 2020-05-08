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

#include <MaximInterfaceCore/I2CMaster.hpp>
#include <MaximInterfaceCore/Result.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

namespace {

using namespace MaximInterfaceCore;
using MaximInterfaceTest::error;
using std::ostream;

template <typename T>
ostream & operator<<(ostream & os, const Result<T> & result) {
  if (result) {
    os << "value: " << result.value();
  } else {
    os << "error: " << result.error();
  }
  return os;
}

template <> ostream & operator<<(ostream & os, const Result<void> & result) {
  if (result) {
    os << "value: (none)";
  } else {
    os << "error: " << result.error();
  }
  return os;
}

ostream & operator<<(ostream & os, None) {
  os << "none";
  return os;
}

void testIntResult() {
  Result<int> result;
  ASSERT_FALSE(error, result.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.value(), int());
  ASSERT_EQUAL(error, result.error(), error_code());
  ASSERT_EQUAL(error, result, error_code());
  ASSERT_NOT_EQUAL(error, result, int());

  const int value = 3;
  result = value;
  ASSERT_TRUE(error, result.hasValue());
  ASSERT_TRUE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.value(), value);
  ASSERT_EQUAL(error, result.error(), error_code());
  ASSERT_EQUAL(error, result, value);
  ASSERT_NOT_EQUAL(error, result, error_code());

  result = I2CMaster::NackError;
  ASSERT_FALSE(error, result.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.value(), int());
  ASSERT_EQUAL(error, result.error(), I2CMaster::NackError);
  ASSERT_EQUAL(error, result, I2CMaster::NackError);
  ASSERT_NOT_EQUAL(error, result, int());
}

void testVoidResult() {
  Result<void> result;
  ASSERT_FALSE(error, result.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.error(), error_code());
  ASSERT_EQUAL(error, result, error_code());
  ASSERT_NOT_EQUAL(error, result, none);

  result = none;
  ASSERT_TRUE(error, result.hasValue());
  ASSERT_TRUE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.error(), error_code());
  ASSERT_EQUAL(error, result, none);
  ASSERT_NOT_EQUAL(error, result, error_code());

  result = I2CMaster::NackError;
  ASSERT_FALSE(error, result.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(result));
  ASSERT_EQUAL(error, result.error(), I2CMaster::NackError);
  ASSERT_EQUAL(error, result, I2CMaster::NackError);
  ASSERT_NOT_EQUAL(error, result, none);
}

} // namespace

void runTest() {
  testIntResult();
  testVoidResult();
}
