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

#include <MaximInterfaceCore/Optional.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using namespace MaximInterfaceCore;
using MaximInterfaceTest::error;
using std::ostream;

template <typename T>
static ostream & operator<<(ostream & os, const Optional<T> & optional) {
  os << "value: ";
  if (optional) {
    os << *optional;
  } else {
    os << "(none)";
  }
  return os;
}

static ostream & operator<<(ostream & os, None) {
  os << "none";
  return os;
}

void runTest() {
  Optional<int> optional;
  ASSERT_FALSE(error, optional.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(optional));
  ASSERT_EQUAL(error, optional.value(), int());
  ASSERT_EQUAL(error, *optional, int());
  ASSERT_EQUAL(error, optional, none);
  ASSERT_NOT_EQUAL(error, optional, int());

  const int value = 3;
  optional = value;
  ASSERT_TRUE(error, optional.hasValue());
  ASSERT_TRUE(error, static_cast<bool>(optional));
  ASSERT_EQUAL(error, optional.value(), value);
  ASSERT_EQUAL(error, *optional, value);
  ASSERT_EQUAL(error, optional, value);
  ASSERT_NOT_EQUAL(error, optional, none);

  optional = none;
  ASSERT_FALSE(error, optional.hasValue());
  ASSERT_FALSE(error, static_cast<bool>(optional));
  ASSERT_EQUAL(error, optional.value(), int());
  ASSERT_EQUAL(error, *optional, int());
  ASSERT_EQUAL(error, optional, none);
  ASSERT_NOT_EQUAL(error, optional, int());
}
