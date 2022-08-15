/****************************************************************************************************************************
  PubAck.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#include "PubAck.hpp"

using AsyncMqttClientInternals::PubAckOutPacket;

PubAckOutPacket::PubAckOutPacket(PendingAck pendingAck) 
{
  _data[0] = pendingAck.packetType;
  _data[0] = _data[0] << 4;
  _data[0] = _data[0] | pendingAck.headerFlag;
  _data[1] = 2;
  _packetId = pendingAck.packetId;
  _data[2] = pendingAck.packetId >> 8;
  _data[3] = pendingAck.packetId & 0xFF;
  
  if (packetType() == AsyncMqttClientInternals::PacketType.PUBREL ||
      packetType() == AsyncMqttClientInternals::PacketType.PUBREC) 
  {
    _released = false;
  }
}

const uint8_t* PubAckOutPacket::data(size_t index) const 
{
  return &_data[index];
}

size_t PubAckOutPacket::size() const 
{
  return 4;
}
