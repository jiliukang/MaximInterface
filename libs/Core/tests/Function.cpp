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

#include <functional>
#include <MaximInterfaceCore/Function.hpp>
#include <MaximInterfaceCore/SelectRom.hpp>
#include <MaximInterfaceCore/WriteMessage.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

namespace {

using namespace MaximInterfaceCore;
using MaximInterfaceTest::error;

const int a = 3;
const int b = 4;
const int c = 5;

int sum(int a, int b, int c) { return a + b + c; }

class DelayedSum {
public:
  DelayedSum(int a, int b, int c) : a(a), b(b), c(c) {}

  int operator()() const { return a + b + c; }

private:
  int a;
  int b;
  int c;
};

void testZeroArgumentFunction() {
  Function<int()> function;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(), int());
  function = DelayedSum(a, b, c);
  ASSERT_TRUE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(), a + b + c);
  function = none;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(), int());
}

void testOneArgumentFunction() {
  Function<int(int)> function;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a), int());
  function = std::negate<int>();
  ASSERT_TRUE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a), -a);
  function = none;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a), int());
}

void testTwoArgumentFunction() {
  Function<int(int, int)> function;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b), int());
  function = std::minus<int>();
  ASSERT_TRUE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b), a - b);
  function = none;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b), int());
}

void testThreeArgumentFunction() {
  Function<int(int, int, int)> function;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b, c), int());
  function = sum;
  ASSERT_TRUE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b, c), a + b + c);
  function = none;
  ASSERT_FALSE(error, static_cast<bool>(function));
  ASSERT_EQUAL(error, function(a, b, c), int());
}

void testSize() {
  const size_t expectedSize = 32;
  ASSERT_EQUAL(error, expectedSize, sizeof(SelectRom));
  ASSERT_EQUAL(error, expectedSize, sizeof(WriteMessage));
}

void testAlignment() {
  typedef detail::TypeStorage<char, 1> BasicTypeStorage;

  typedef WriteMessage::result_type (*WriteMessageFuncPtr)(
      WriteMessage::argument_type);
  ASSERT_LESS_EQUAL(error, alignment_of<WriteMessageFuncPtr>::value,
                    alignment_of<WriteMessage>::value);
  ASSERT_LESS_EQUAL(error, alignment_of<WriteMessageFuncPtr>::value,
                    alignment_of<BasicTypeStorage>::value);

  typedef SelectRom::result_type (*SelectRomFuncPtr)(SelectRom::argument_type);
  ASSERT_LESS_EQUAL(error, alignment_of<SelectRomFuncPtr>::value,
                    alignment_of<SelectRom>::value);
  ASSERT_LESS_EQUAL(error, alignment_of<SelectRomFuncPtr>::value,
                    alignment_of<BasicTypeStorage>::value);

  ASSERT_LESS_EQUAL(error, alignment_of<SelectMatchRom>::value,
                    alignment_of<SelectRom>::value);
  ASSERT_LESS_EQUAL(error, alignment_of<SelectMatchRom>::value,
                    alignment_of<BasicTypeStorage>::value);
}

} // namespace

void runTest() {
  testZeroArgumentFunction();
  testOneArgumentFunction();
  testTwoArgumentFunction();
  testThreeArgumentFunction();
  testSize();
  testAlignment();
}
