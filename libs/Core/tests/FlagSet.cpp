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

#include <sstream>
#include <string>
#include <MaximInterfaceCore/FlagSet.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

namespace {

enum Bits { Bit0 = 1, Bit1 = 2, Bit2 = 4, Bit3 = 8 };

} // namespace

void runTest() {
  using MaximInterfaceTest::error;
  using std::string;

  typedef MaximInterfaceCore::FlagSet<Bits, 4> BitSet;

  // Test default constructor.
  BitSet a;
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_FALSE(error, a.any());
  ASSERT_TRUE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 0u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 0u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0000");

  // Test unsigned long constructor and copy assignment.
  a = 10;
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 10u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1010");

  // Test set function.
  a.set();
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 4u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 15u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1111");

  // Test reset function.
  a.reset();
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_FALSE(error, a.any());
  ASSERT_TRUE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 0u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 0u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0000");

  // Test flip function.
  a.flip();
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 4u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 15u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1111");

  // Test operator~ function.
  a = ~a;
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_FALSE(error, a.any());
  ASSERT_TRUE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 0u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 0u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0000");

  // Test flip bit function.
  a.flip(Bit0);
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 1u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 1u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0001");

  // Test set bit function.
  a.set(Bit2);
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 5u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0101");

  // Test bit reference flip function.
  a[Bit3].flip();
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 3u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 13u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1101");

  // Test reset bit function.
  a.reset(Bit3);
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 5u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0101");

  // Test bit reference assignment.
  a[Bit1] = a[Bit2];
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 3u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 7u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0111");

  // Test bit reference operator~ function.
  a.set(Bit1, ~a[Bit2]);
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_FALSE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_FALSE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 5u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0101");

  // Test operator!= function.
  BitSet b;
  ASSERT_NOT_EQUAL(error, a, b);

  // Test operator|= function.
  b |= a;
  ASSERT_TRUE(error, b.test(Bit0));
  ASSERT_FALSE(error, b.test(Bit1));
  ASSERT_TRUE(error, b.test(Bit2));
  ASSERT_FALSE(error, b.test(Bit3));
  ASSERT_TRUE(error, b.any());
  ASSERT_FALSE(error, b.none());
  ASSERT_EQUAL(error, b.count(), 2u);
  ASSERT_EQUAL(error, b.size(), 4u);
  ASSERT_EQUAL(error, b.to_ulong(), 5u);
  ASSERT_EQUAL(error,
               (b.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0101");

  // Test operator== function.
  ASSERT_EQUAL(error, a, b);

  // Test operator^= function.
  b = 10;
  a ^= b;
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 4u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 15u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1111");

  // Test operator&= function.
  a &= b;
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 10u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1010");

  // Try to set bits that are out of range.
  a = 0xFF;
  ASSERT_TRUE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_TRUE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 4u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 15u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1111");

  // Test operator<< in conjunction with the string constructor.
  std::stringstream stream;
  stream << BitSet(10);

  // Test string constructor.
  a = BitSet(stream.str());
  ASSERT_FALSE(error, a.test(Bit0));
  ASSERT_TRUE(error, a.test(Bit1));
  ASSERT_FALSE(error, a.test(Bit2));
  ASSERT_TRUE(error, a.test(Bit3));
  ASSERT_TRUE(error, a.any());
  ASSERT_FALSE(error, a.none());
  ASSERT_EQUAL(error, a.count(), 2u);
  ASSERT_EQUAL(error, a.size(), 4u);
  ASSERT_EQUAL(error, a.to_ulong(), 10u);
  ASSERT_EQUAL(error,
               (a.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1010");

  // Test operator>> function.
  stream >> b;
  ASSERT_FALSE(error, b.test(Bit0));
  ASSERT_TRUE(error, b.test(Bit1));
  ASSERT_FALSE(error, b.test(Bit2));
  ASSERT_TRUE(error, b.test(Bit3));
  ASSERT_TRUE(error, b.any());
  ASSERT_FALSE(error, b.none());
  ASSERT_EQUAL(error, b.count(), 2u);
  ASSERT_EQUAL(error, b.size(), 4u);
  ASSERT_EQUAL(error, b.to_ulong(), 10u);
  ASSERT_EQUAL(error,
               (b.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1010");

  a = 14;
  b = 7;
  BitSet c;

  // Test operator| function.
  c = a | b;
  ASSERT_TRUE(error, c.test(Bit0));
  ASSERT_TRUE(error, c.test(Bit1));
  ASSERT_TRUE(error, c.test(Bit2));
  ASSERT_TRUE(error, c.test(Bit3));
  ASSERT_TRUE(error, c.any());
  ASSERT_FALSE(error, c.none());
  ASSERT_EQUAL(error, c.count(), 4u);
  ASSERT_EQUAL(error, c.size(), 4u);
  ASSERT_EQUAL(error, c.to_ulong(), 15u);
  ASSERT_EQUAL(error,
               (c.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1111");

  // Test operator& function.
  c = a & b;
  ASSERT_FALSE(error, c.test(Bit0));
  ASSERT_TRUE(error, c.test(Bit1));
  ASSERT_TRUE(error, c.test(Bit2));
  ASSERT_FALSE(error, c.test(Bit3));
  ASSERT_TRUE(error, c.any());
  ASSERT_FALSE(error, c.none());
  ASSERT_EQUAL(error, c.count(), 2u);
  ASSERT_EQUAL(error, c.size(), 4u);
  ASSERT_EQUAL(error, c.to_ulong(), 6u);
  ASSERT_EQUAL(error,
               (c.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "0110");

  // Test operator^ function.
  c = a ^ b;
  ASSERT_TRUE(error, c.test(Bit0));
  ASSERT_FALSE(error, c.test(Bit1));
  ASSERT_FALSE(error, c.test(Bit2));
  ASSERT_TRUE(error, c.test(Bit3));
  ASSERT_TRUE(error, c.any());
  ASSERT_FALSE(error, c.none());
  ASSERT_EQUAL(error, c.count(), 2u);
  ASSERT_EQUAL(error, c.size(), 4u);
  ASSERT_EQUAL(error, c.to_ulong(), 9u);
  ASSERT_EQUAL(error,
               (c.to_string<string::value_type, string::traits_type,
                            string::allocator_type>()),
               "1001");
}
