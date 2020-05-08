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
#include <MaximInterfaceCore/Algorithm.hpp>
#include <MaximInterfaceCore/span.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using MaximInterfaceCore::span;
using MaximInterfaceTest::error;

static const int size = 10;

template <typename T> static std::string toString(const T & value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

template <typename T>
static std::ostream & operator<<(std::ostream & os, span<T> value) {
  for (typename span<T>::index_type i = 0; i < value.size(); ++i) {
    if (i > 0) {
      os << ',';
    }
    os << value[i];
  }
  return os;
}

static void testStaticSpan() {
  // Test capacity functions.
  ASSERT_FALSE(error, (span<int, size>::empty()));
  ASSERT_EQUAL(error, (span<int, size>::size()),
               (static_cast<span<int, size>::index_type>(size)));
  ASSERT_EQUAL(error, (span<int, size>::size_bytes()), size * sizeof(int));

  // Initialize data array;
  int data1[size];
  for (int i = 0; i < size; ++i) {
    data1[i] = i + 1;
  }

  // Test data function.
  span<int, size> a = data1;
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a.data()[i], data1[i]) {
      ERROR("a.data()[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test operator[] function.
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a[i], data1[i]) {
      ERROR("a[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test copy initialization.
  span<int, size> b = a;
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function and forward iterators in true case.
  ASSERT_TRUE(error, equal(a, b));

  // Test assignment.
  int data2[size] = {0};
  b = data2;
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], data2[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function and forward iterators in false case.
  ASSERT_FALSE(error, equal(a, b));

  // Test reverse iterators.
  std::copy(a.rbegin(), a.rend(), b.begin());
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[size - 1 - i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test front and back functions.
  ASSERT_EQUAL(error, b.front(), size);
  ASSERT_EQUAL(error, a.back(), size);

  // Test copy function.
  copy(a, b);
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }
}

static void testDynamicSpan() {
  using MaximInterfaceTest::fatal;

  // Test capacity functions in empty case.
  span<int> a;
  ASSERT_TRUE(error, a.empty());
  ASSERT_EQUAL(error, a.size(), 0u);
  ASSERT_EQUAL(error, a.size_bytes(), 0u);

  // Initialize data vector.
  int data1[size];
  for (int i = 0; i < size; ++i) {
    data1[i] = i + 1;
  }

  // Test cacapcity functions in non-empty case.
  a = data1;
  ASSERT_FALSE(error, a.empty());
  ASSERT_EQUAL(error, a.size(), (static_cast<span<int>::index_type>(size)));
  ASSERT_EQUAL(error, a.size_bytes(), size * sizeof(int));

  // Test data function.
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a.data()[i], data1[i]) {
      ERROR("a.data()[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test operator[] function.
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(a[i], data1[i]) {
      ERROR("a[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test copy initialization.
  span<int> b = a;
  // Fatal since further checks depend on equal size.
  ASSERT_EQUAL(fatal, b.size(), a.size());
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test forward iterators;
  ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));

  // Test assignment.
  int data2[size] = {0};
  b = data2;
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], data2[i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test reverse iterators.
  std::copy(a.rbegin(), a.rend(), b.begin());
  for (int i = 0; i < size; ++i) {
    ASSERT_EQUAL_ELSE(b[i], a[size - 1 - i]) {
      ERROR("b[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test front and back functions.
  ASSERT_EQUAL(error, b.front(), size);
  ASSERT_EQUAL(error, a.back(), size);

  std::copy(data1, data1 + size, data2);

  // Test first function.
  a = a.first(size / 2);
  b = b.first(size / 2);
  ASSERT_EQUAL(error, a.data(), data1);
  ASSERT_EQUAL(error, b.data(), data2);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }

  a = data1;
  b = data2;

  // Test first function template.
  a = a.first<size / 2>();
  b = b.first<size / 2>();
  ASSERT_EQUAL(error, a.data(), data1);
  ASSERT_EQUAL(error, b.data(), data2);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }

  a = data1;
  b = data2;
  
  // Test last function.
  a = a.last(size / 2);
  b = b.last(size / 2);
  ASSERT_EQUAL(error, a.data(), data1 + size / 2);
  ASSERT_EQUAL(error, b.data(), data2 + size / 2);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }

  a = data1;
  b = data2;

  // Test last function template.
  a = a.last<size / 2>();
  b = b.last<size / 2>();
  ASSERT_EQUAL(error, a.data(), data1 + size / 2);
  ASSERT_EQUAL(error, b.data(), data2 + size / 2);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }

  a = data1;
  b = data2;

  // Test subspan function.
  a = a.subspan(size / 4, size / 2);
  b = b.subspan(size / 4, size / 2);
  ASSERT_EQUAL(error, a.data(), data1 + size / 4);
  ASSERT_EQUAL(error, b.data(), data2 + size / 4);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }

  a = data1;
  b = data2;

  // Test subspan function template.
  a = a.subspan<size / 4, size / 2>();
  b = b.subspan<size / 4, size / 2>();
  ASSERT_EQUAL(error, a.data(), data1 + size / 4);
  ASSERT_EQUAL(error, b.data(), data2 + size / 4);
  ASSERT_EQUAL(error, a.size(), static_cast<span<int>::index_type>(size / 2));
  if (ASSERT_EQUAL(error, b.size(), a.size())) {
    ASSERT_TRUE(error, std::equal(a.begin(), a.end(), b.begin()));
  }
}

void runTest() {
  testStaticSpan();
  testDynamicSpan();
}
