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

#include <algorithm>
#include <sstream>
#include <string>
#include <MaximInterfaceCore/array.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using MaximInterfaceCore::array;
using MaximInterfaceTest::error;

template <typename T> static std::string toString(const T & value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

template <typename T, size_t N>
static std::ostream & operator<<(std::ostream & os, const array<T, N> & value) {
  for (typename array<T, N>::size_type i = 0; i < value.size(); ++i) {
    if (i > 0) {
      os << ',';
    }
    os << value[i];
  }
  return os;
}

static void testNonEmptyArray() {
  const int size = 10;

  // Test capacity functions.
  ASSERT_FALSE(error, (array<int, size>::empty()));
  ASSERT_EQUAL(error, (array<int, size>::size()),
               (static_cast<array<int, size>::size_type>(size)));
  ASSERT_EQUAL(error, (array<int, size>::max_size()),
               (static_cast<array<int, size>::size_type>(size)));

  // Initialize private data array.
  array<int, size> a;
  for (int i = 0; i < size; ++i) {
    a.data_[i] = i + 1;
  }

  // Test data function.
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a.data()[i], a.data_[i]) {
      ERROR("a.data()[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test operator[] function.
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a[i], a.data()[i]) {
      ERROR("a[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test copy initialization.
  array<int, size> b = a;
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test operator== function and forward iterators.
  ASSERT_EQUAL(error, a, b);

  // Test fill function.
  b.fill(0);
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], 0) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test operator!= function.
  ASSERT_NOT_EQUAL(error, a, b);

  // Test swap function.
  swap(a, b);
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a[i], 0) {
      ERROR("a[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test reverse iterators.
  std::copy(b.rbegin(), b.rend(), a.begin());
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a[i], b[size - 1 - i]) {
      ERROR("a[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test front and back functions.
  ASSERT_EQUAL(error, a.front(), size);
  ASSERT_EQUAL(error, b.back(), size);

  // Test relational operators (b < a).
  ASSERT_LESS(error, b, a);
  ASSERT_LESS_EQUAL(error, b, a);
  ASSERT_GREATER(error, a, b);
  ASSERT_GREATER_EQUAL(error, a, b);
}

static void testEmptyArray() {
  // Test capacity functions.
  ASSERT_TRUE(error, (array<int, 0>::empty()));
  ASSERT_EQUAL(error, (array<int, 0>::size()),
               (static_cast<array<int, 0>::size_type>(0)));
  ASSERT_EQUAL(error, (array<int, 0>::max_size()),
               (static_cast<array<int, 0>::size_type>(0)));

  // Test iterators.
  array<int, 0> a;
  ASSERT_EQUAL(error, a.begin(), a.end());
  ASSERT_TRUE(error, a.rbegin() == a.rend());
  ASSERT_TRUE(error, a.crbegin() == a.crend());

  // Test equality and relational operators.
  array<int, 0> b;
  ASSERT_TRUE(error, a == b);
  ASSERT_FALSE(error, a != b);
  ASSERT_FALSE(error, a < b);
  ASSERT_FALSE(error, a > b);
  ASSERT_TRUE(error, a <= b);
  ASSERT_TRUE(error, a >= b);
}

void runTest() {
  testNonEmptyArray();
  testEmptyArray();
}