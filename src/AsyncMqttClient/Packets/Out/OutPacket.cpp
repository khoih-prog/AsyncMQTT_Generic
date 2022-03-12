/****************************************************************************************************************************
  OutPacket.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.1.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
 *****************************************************************************************************************************/

#include "OutPacket.hpp"

using AsyncMqttClientInternals::OutPacket;

OutPacket::OutPacket()
  : next(nullptr)
  , timeout(0)
  , noTries(0)
  , _released(true)
  , _packetId(0) {}

OutPacket::~OutPacket() {}

bool OutPacket::released() const 
{
  return _released;
}

uint8_t OutPacket::packetType() const 
{
  return data(0)[0] >> 4;
}

uint16_t OutPacket::packetId() const 
{
  return _packetId;
}

uint8_t OutPacket::qos() const 
{
  if (packetType() == AsyncMqttClientInternals::PacketType.PUBLISH) 
  {
    return (data()[1] & 0x06) >> 1;
  }
  
  return 0;
}

void OutPacket::release() 
{
  _released = true;
}

uint16_t OutPacket::_nextPacketId = 0;

uint16_t OutPacket::_getNextPacketId() 
{
  if (++_nextPacketId == 0) 
  {
    ++_nextPacketId;
  }
  
  return _nextPacketId;
}
