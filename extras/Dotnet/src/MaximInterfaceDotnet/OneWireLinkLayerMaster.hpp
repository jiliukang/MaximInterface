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

#ifndef MaximInterfaceDotnet_OneWireLinkLayerMaster_hpp
#define MaximInterfaceDotnet_OneWireLinkLayerMaster_hpp

#include <memory>
#include <string>
#include <MaximInterfaceCore/OneWireMaster.hpp>
#include "Config.hpp"

namespace MaximInterfaceDotnet {

/// 1-Wire interface using the OneWireLinkLayer DLL from the Compact.NET API.
class OneWireLinkLayerMaster : public MaximInterfaceCore::OneWireMaster {
public:
  enum ErrorValue {
    CommunicationError = 1,
    OpenPortError,
    PowerDeliveryError,
    AlreadyConnectedError
  };

  /// @param adapterName Adapter type name recognized by OneWireLinkLayer.
  MaximInterfaceDotnet_EXPORT explicit OneWireLinkLayerMaster(
      const std::string & adapterName);

  MaximInterfaceDotnet_EXPORT ~OneWireLinkLayerMaster();

  MaximInterfaceDotnet_EXPORT
  OneWireLinkLayerMaster(OneWireLinkLayerMaster &&) noexcept;

  MaximInterfaceDotnet_EXPORT OneWireLinkLayerMaster &
  operator=(OneWireLinkLayerMaster &&) noexcept;

  /// Connect to an adapter on the specified COM port.
  MaximInterfaceDotnet_EXPORT MaximInterfaceCore::Result<void>
  connect(const std::string & portName);

  /// Disconnect from the adapter on the current COM port.
  MaximInterfaceDotnet_EXPORT void disconnect();

  /// @brief Check if currently connected to an adapter.
  /// @returns True if connected.
  MaximInterfaceDotnet_EXPORT bool connected() const;

  /// Get the adapter type name.
  MaximInterfaceDotnet_EXPORT std::string adapterName() const;

  /// Get the currently connected COM port name.
  MaximInterfaceDotnet_EXPORT std::string portName() const;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  reset() override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<bool>
  touchBitSetLevel(bool sendBit, Level afterLevel) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  writeByteSetLevel(uint_least8_t sendByte, Level afterLevel) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<uint_least8_t>
  readByteSetLevel(Level afterLevel) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  writeBlock(MaximInterfaceCore::span<const uint_least8_t> sendBuf) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  readBlock(MaximInterfaceCore::span<uint_least8_t> recvBuf) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  setSpeed(Speed newSpeed) override;

  MaximInterfaceDotnet_EXPORT virtual MaximInterfaceCore::Result<void>
  setLevel(Level newLevel) override;

  MaximInterfaceDotnet_EXPORT static const MaximInterfaceCore::error_category &
  errorCategory();

private:
  struct Data;

  std::unique_ptr<Data> data;
};

} // namespace MaximInterfaceDotnet
namespace MaximInterfaceCore {

template <>
struct is_error_code_enum<
    MaximInterfaceDotnet::OneWireLinkLayerMaster::ErrorValue> : true_type {};

} // namespace MaximInterfaceCore
namespace MaximInterfaceDotnet {

inline MaximInterfaceCore::error_code
make_error_code(OneWireLinkLayerMaster::ErrorValue e) {
  return MaximInterfaceCore::error_code(
      e, OneWireLinkLayerMaster::errorCategory());
}

} // namespace MaximInterfaceDotnet

#endif
