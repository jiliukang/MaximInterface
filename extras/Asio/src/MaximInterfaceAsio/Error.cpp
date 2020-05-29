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

#include <asio/error.hpp>
#include <MaximInterfaceCore/Error.hpp>
#include "Error.hpp"

namespace MaximInterfaceAsio {

using namespace MaximInterfaceCore;

const error_category & systemCategory() {
  static class : public error_category {
  public:
    virtual const char * name() const { return "MaximInterfaceAsio.system"; }

    virtual std::string message(int condition) const {
      return asio::error::system_category.message(condition);
    }
  } instance;
  return instance;
}

const error_category & unknownCategory() {
  static class : public error_category {
  public:
    virtual const char * name() const { return "MaximInterfaceAsio.unknown"; }

    virtual std::string message(int condition) const {
      return defaultErrorMessage(condition);
    }
  } instance;
  return instance;
}

error_code convertErrorCode(const std::error_code & ec) {
  return error_code(ec.value(), ec.category() == asio::error::system_category
                                    ? systemCategory()
                                    : unknownCategory());
}

} // namespace MaximInterfaceAsio
