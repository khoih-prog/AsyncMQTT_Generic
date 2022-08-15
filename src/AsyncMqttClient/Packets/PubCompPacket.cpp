/****************************************************************************************************************************
  PubCompPacket.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/
 
#include "PubCompPacket.hpp"

using AsyncMqttClientInternals::PubCompPacket;

PubCompPacket::PubCompPacket(ParsingInformation* parsingInformation, OnPubCompInternalCallback callback)
: _parsingInformation(parsingInformation)
, _callback(callback)
, _bytePosition(0)
, _packetIdMsb(0)
, _packetId(0) 
{
}

PubCompPacket::~PubCompPacket() 
{
}

void PubCompPacket::parseVariableHeader(char* data, size_t len, size_t* currentBytePosition) 
{
  (void)len;
  
  char currentByte = data[(*currentBytePosition)++];
  
  if (_bytePosition++ == 0) 
  {
    _packetIdMsb = currentByte;
  } 
  else 
  {
    _packetId = currentByte | _packetIdMsb << 8;
    _parsingInformation->bufferState = BufferState::NONE;
    _callback(_packetId);
  }
}

void PubCompPacket::parsePayload(char* data, size_t len, size_t* currentBytePosition) 
{
  (void)data;
  (void)len;
  (void)currentBytePosition;
}
