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
#include <MaximInterfaceCore/Ecc256.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using namespace MaximInterfaceCore::Ecc256;
using MaximInterfaceTest::error;

template <typename T> static std::string toString(const T & value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

static void testPoint() {
  // Initialize array.
  Point::array data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    data1.x[i] = static_cast<uint_least8_t>(i);
    data1.y[i] = static_cast<uint_least8_t>(i + Scalar::size);
  }

  // Test array to const_span conversion.
  Point::const_span a = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.x[i]), i) {
      ERROR("a.x[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.y[i]), i + Scalar::size) {
      ERROR("a.y[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test array to span conversion.
  Point::span b = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.x[i]), i) {
      ERROR("b.x[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.y[i]), i + Scalar::size) {
      ERROR("b.y[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test span to const_span conversion.
  a = b;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.x[i]), i) {
      ERROR("a.x[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.y[i]), i + Scalar::size) {
      ERROR("a.y[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in true case.
  ASSERT_TRUE(error, equal(a, b));

  // Test assignment.
  Point::array data2 = {{0}, {0}};
  b = data2;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<int>(b.x[i]), 0) {
      ERROR("b.x[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<int>(b.y[i]), 0) {
      ERROR("b.y[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in false case.
  ASSERT_FALSE(error, equal(a, b));

  // Test copy function.
  copy(a, b);
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.x[i]), i) {
      ERROR("b.x[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.y[i]), i + Scalar::size) {
      ERROR("b.y[" + toString(i) + "] has unexpected value.");
    }
  }
}

static void testKeyPair() {
  // Initialize array.
  KeyPair::array data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    data1.privateKey[i] = static_cast<uint_least8_t>(i);
    data1.publicKey.x[i] = data1.publicKey.y[i] =
        static_cast<uint_least8_t>(i + Scalar::size);
  }

  // Test array to const_span conversion.
  KeyPair::const_span a = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.privateKey[i]), i) {
      ERROR("a.privateKey[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.publicKey.x[i]), i + Scalar::size) {
      ERROR("a.publicKey.x[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test array to span conversion.
  KeyPair::span b = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.privateKey[i]), i) {
      ERROR("b.privateKey[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.publicKey.x[i]), i + Scalar::size) {
      ERROR("b.publicKey.x[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test span to const_span conversion.
  a = b;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.privateKey[i]), i) {
      ERROR("a.privateKey[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.publicKey.x[i]), i + Scalar::size) {
      ERROR("a.publicKey.x[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in true case.
  ASSERT_TRUE(error, equal(a, b));

  // Test assignment.
  KeyPair::array data2 = {{0}, {{0}, {0}}};
  b = data2;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<int>(b.privateKey[i]), 0) {
      ERROR("b.privateKey[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<int>(b.publicKey.x[i]), 0) {
      ERROR("b.publicKey.x[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in false case.
  ASSERT_FALSE(error, equal(a, b));

  // Test copy function.
  copy(a, b);
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.privateKey[i]), i) {
      ERROR("b.privateKey[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.publicKey.x[i]), i + Scalar::size) {
      ERROR("b.publicKey.x[" + toString(i) + "] has unexpected value.");
    }
  }
}

static void testSignature() {
  // Initialize array.
  Signature::array data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    data1.r[i] = static_cast<uint_least8_t>(i);
    data1.s[i] = static_cast<uint_least8_t>(i + Scalar::size);
  }

  // Test array to const_span conversion.
  Signature::const_span a = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.r[i]), i) {
      ERROR("a.r[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.s[i]), i + Scalar::size) {
      ERROR("a.s[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test array to span conversion.
  Signature::span b = data1;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.r[i]), i) {
      ERROR("b.r[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.s[i]), i + Scalar::size) {
      ERROR("b.s[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test span to const_span conversion.
  a = b;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.r[i]), i) {
      ERROR("a.r[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(a.s[i]), i + Scalar::size) {
      ERROR("a.s[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in true case.
  ASSERT_TRUE(error, equal(a, b));

  // Test assignment.
  Signature::array data2 = {{0}, {0}};
  b = data2;
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<int>(b.r[i]), 0) {
      ERROR("b.r[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<int>(b.s[i]), 0) {
      ERROR("b.s[" + toString(i) + "] has unexpected value.");
    }
  }

  // Test equal function in false case.
  ASSERT_FALSE(error, equal(a, b));

  // Test copy function.
  copy(a, b);
  for (size_t i = 0; i < Scalar::size; ++i) {
    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.r[i]), i) {
      ERROR("b.r[" + toString(i) + "] has unexpected value.");
    }

    ASSERT_EQUAL_ELSE(static_cast<size_t>(b.s[i]), i + Scalar::size) {
      ERROR("b.s[" + toString(i) + "] has unexpected value.");
    }
  }
}

void runTest() {
  testPoint();
  testKeyPair();
  testSignature();
}
