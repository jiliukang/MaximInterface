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

#include <MaximInterfaceCore/HexString.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

void runTest() {
  using namespace MaximInterfaceCore;
  using namespace MaximInterfaceTest;

  // Test conversion from hex string to byte array.
  Optional<std::vector<uint_least8_t> > byteArray =
      fromHexString("01 23 45 67 89 ab cd ef\t");
  ASSERT_TRUE(fatal, byteArray.hasValue());
  if (ASSERT_EQUAL(error, byteArray->size(), 8u)) {
    ASSERT_EQUAL(error, byteArray.value()[0], 0x01);
    ASSERT_EQUAL(error, byteArray.value()[1], 0x23);
    ASSERT_EQUAL(error, byteArray.value()[2], 0x45);
    ASSERT_EQUAL(error, byteArray.value()[3], 0x67);
    ASSERT_EQUAL(error, byteArray.value()[4], 0x89);
    ASSERT_EQUAL(error, byteArray.value()[5], 0xAB);
    ASSERT_EQUAL(error, byteArray.value()[6], 0xCD);
    ASSERT_EQUAL(error, byteArray.value()[7], 0xEF);
  }

  // Test conversion from byte array to hex string.
  std::string hexString = toHexString(byteArray.value());
  ASSERT_EQUAL(error, hexString, "0123456789ABCDEF");

  // Test odd string length.
  hexString += 'F';
  byteArray = fromHexString(hexString);
  ASSERT_FALSE(error, byteArray.hasValue());

  // Test invalid character.
  hexString += 'G';
  byteArray = fromHexString(hexString);
  ASSERT_FALSE(error, byteArray.hasValue());
}
