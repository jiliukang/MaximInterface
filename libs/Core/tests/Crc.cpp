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

#include <MaximInterfaceCore/Crc.hpp>
#include <MaximInterfaceTest/TestShortMacros.hpp>

using MaximInterfaceTest::error;

static void testCalculateCrc8() {
  using MaximInterfaceCore::calculateCrc8;

  const uint_least8_t data[] = {0x02, 0x1C, 0xB8, 0x01, 0x00, 0x00, 0x00};
  const unsigned int crc = 0xA2;
  ASSERT_EQUAL(error, static_cast<unsigned int>(calculateCrc8(data)), crc);
  ASSERT_EQUAL(error, static_cast<unsigned int>(calculateCrc8(crc, crc)), 0u);
}

static void testCalculateCrc16() {
  using MaximInterfaceCore::calculateCrc16;

  const uint_least8_t data[] = {0x05, 0xAA, 0xB2, 0x4C, 0x4F, 0x59, 0xF5, 0x17};
  ASSERT_EQUAL(error, calculateCrc16(data), 0xB001u);
  uint_fast16_t crc = (static_cast<uint_fast16_t>(data[1]) << 8) | data[0];
  crc = calculateCrc16(crc, data[0]);
  crc = calculateCrc16(crc, data[1]);
  ASSERT_EQUAL(error, crc, 0u);
}

void runTest() {
  testCalculateCrc8();
  testCalculateCrc16();
}
