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

#include <stdexcept>
#include <msclr/auto_gcroot.h>
#include <msclr/marshal_cppstd.h>
#include <MaximInterfaceCore/ChangeSizeType.hpp>
#include <MaximInterfaceCore/Error.hpp>
#include "OneWireLinkLayerMaster.hpp"

namespace MaximInterfaceDotnet {

using DalSemi::OneWire::Adapter::AdapterException;
using namespace MaximInterfaceCore;
using msclr::interop::marshal_as;
using std::string;
using System::IntPtr;
using System::String;
using System::Runtime::InteropServices::Marshal;

struct OneWireLinkLayerMaster::Data {
  msclr::auto_gcroot<DalSemi::OneWire::Adapter::PortAdapter ^> adapter;
  bool connected = false;
};

OneWireLinkLayerMaster::OneWireLinkLayerMaster(const string & adapterName)
    : data(std::make_unique<Data>()) {
  try {
    data->adapter = DalSemi::OneWire::AccessProvider::GetAdapter(
        marshal_as<String ^>(adapterName));
  } catch (AdapterException ^ e) {
    throw std::invalid_argument(marshal_as<string>(e->Message));
  }
}

OneWireLinkLayerMaster::~OneWireLinkLayerMaster() = default;

OneWireLinkLayerMaster::OneWireLinkLayerMaster(
    OneWireLinkLayerMaster && rhs) noexcept = default;

OneWireLinkLayerMaster & OneWireLinkLayerMaster::
operator=(OneWireLinkLayerMaster && rhs) noexcept = default;

Result<void> OneWireLinkLayerMaster::connect(const string & portName) {
  if (connected()) {
    return AlreadyConnectedError;
  }

  try {
    auto adapterResult =
        data->adapter->OpenPort(marshal_as<String ^>(portName));
    if (!adapterResult) {
      return OpenPortError;
    }
    adapterResult = data->adapter->BeginExclusive(true);
    if (!adapterResult) {
      data->adapter->FreePort();
      return OpenPortError;
    }
    data->connected = true;
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return none;
}

void OneWireLinkLayerMaster::disconnect() {
  data->adapter->EndExclusive();
  data->adapter->FreePort();
  data->connected = false;
}

bool OneWireLinkLayerMaster::connected() const { return data->connected; }

string OneWireLinkLayerMaster::adapterName() const {
  return marshal_as<string>(data->adapter->AdapterName);
}

string OneWireLinkLayerMaster::portName() const {
  return connected() ? marshal_as<string>(data->adapter->PortName) : "";
}

Result<void> OneWireLinkLayerMaster::reset() {
  using DalSemi::OneWire::Adapter::OWResetResult;

  try {
    auto resetResult = data->adapter->Reset();
    switch (resetResult) {
    case OWResetResult::RESET_SHORT:
      return ShortDetectedError;

    case OWResetResult::RESET_NOPRESENCE:
      return NoSlaveError;
    }
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return none;
}

Result<bool> OneWireLinkLayerMaster::touchBitSetLevel(bool sendBit,
                                                      Level afterLevel) {
  try {
    switch (afterLevel) {
    case StrongLevel: {
      auto adapterResult = data->adapter->StartPowerDelivery(
          DalSemi::OneWire::Adapter::OWPowerStart::CONDITION_AFTER_BIT);
      if (!adapterResult) {
        return PowerDeliveryError;
      }
    } break;

    case NormalLevel:
      break;

    default:
      return InvalidLevelError;
    }
    if (sendBit) {
      sendBit = data->adapter->GetBit();
    } else {
      data->adapter->PutBit(0);
    }
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return sendBit;
}

Result<void> OneWireLinkLayerMaster::writeByteSetLevel(uint_least8_t sendByte,
                                                       Level afterLevel) {
  try {
    switch (afterLevel) {
    case StrongLevel: {
      auto adapterResult = data->adapter->StartPowerDelivery(
          DalSemi::OneWire::Adapter::OWPowerStart::CONDITION_AFTER_BYTE);
      if (!adapterResult) {
        return PowerDeliveryError;
      }
    } break;

    case NormalLevel:
      break;

    default:
      return InvalidLevelError;
    }
    data->adapter->PutByte(sendByte);
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return none;
}

Result<uint_least8_t>
OneWireLinkLayerMaster::readByteSetLevel(Level afterLevel) {
  uint_least8_t recvByte;
  try {
    switch (afterLevel) {
    case StrongLevel: {
      auto adapterResult = data->adapter->StartPowerDelivery(
          DalSemi::OneWire::Adapter::OWPowerStart::CONDITION_AFTER_BYTE);
      if (!adapterResult) {
        return PowerDeliveryError;
      }
    } break;

    case NormalLevel:
      break;

    default:
      return InvalidLevelError;
    }
    recvByte = data->adapter->GetByte();
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return recvByte;
}

Result<void>
OneWireLinkLayerMaster::writeBlock(span<const uint_least8_t> sendBuf) {
  return changeSizeType<int>(
      [this](const uint_least8_t * data, int dataSize) -> Result<void> {
        try {
          auto dataManaged = gcnew cli::array<uint_least8_t>(dataSize);
          Marshal::Copy(static_cast<IntPtr>(const_cast<uint_least8_t *>(data)),
                        dataManaged, 0, dataSize);
          this->data->adapter->DataBlock(dataManaged, 0, dataSize);
        } catch (AdapterException ^) {
          return CommunicationError;
        }
        return none;
      },
      sendBuf);
}

Result<void> OneWireLinkLayerMaster::readBlock(span<uint_least8_t> recvBuf) {
  return changeSizeType<int>(
      [this](uint_least8_t * data, int dataSize) -> Result<void> {
        try {
          auto dataManaged = gcnew cli::array<uint_least8_t>(dataSize);
          this->data->adapter->GetBlock(dataManaged, dataSize);
          Marshal::Copy(dataManaged, 0, static_cast<IntPtr>(data), dataSize);
        } catch (AdapterException ^) {
          return CommunicationError;
        }
        return none;
      },
      recvBuf);
}

Result<void> OneWireLinkLayerMaster::setSpeed(Speed newSpeed) {
  using DalSemi::OneWire::Adapter::OWSpeed;

  try {
    switch (newSpeed) {
    case OverdriveSpeed:
      data->adapter->Speed = OWSpeed::SPEED_OVERDRIVE;
      break;

    case StandardSpeed:
      data->adapter->Speed = OWSpeed::SPEED_REGULAR;
      break;

    default:
      return InvalidSpeedError;
    }
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return none;
}

Result<void> OneWireLinkLayerMaster::setLevel(Level newLevel) {
  try {
    switch (newLevel) {
    case StrongLevel: {
      auto setResult = data->adapter->StartPowerDelivery(
          DalSemi::OneWire::Adapter::OWPowerStart::CONDITION_NOW);
      if (!setResult) {
        return PowerDeliveryError;
      }
    } break;

    case NormalLevel:
      data->adapter->SetPowerNormal();
      break;

    default:
      return InvalidLevelError;
    }
  } catch (AdapterException ^) {
    return CommunicationError;
  }
  return none;
}

const error_category & OneWireLinkLayerMaster::errorCategory() {
  static class : public error_category {
  public:
    virtual const char * name() const {
      return "MaximInterfaceDotnet.OneWireLinkLayerMaster";
    }

    virtual string message(int condition) const {
      switch (condition) {
      case CommunicationError:
        return "Communication Error";

      case OpenPortError:
        return "Open Port Error";

      case PowerDeliveryError:
        return "Power Delivery Error";

      case AlreadyConnectedError:
        return "Already Connected Error";
      }
      return defaultErrorMessage(condition);
    }
  } instance;
  return instance;
}

} // namespace MaximInterfaceDotnet