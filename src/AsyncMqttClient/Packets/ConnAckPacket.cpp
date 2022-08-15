/****************************************************************************************************************************
  ConnAckPacket.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/
 
#include "ConnAckPacket.hpp"

using AsyncMqttClientInternals::ConnAckPacket;

ConnAckPacket::ConnAckPacket(ParsingInformation* parsingInformation, OnConnAckInternalCallback callback)
: _parsingInformation(parsingInformation)
, _callback(callback)
, _bytePosition(0)
, _sessionPresent(false)
, _connectReturnCode(0) 
{
}

ConnAckPacket::~ConnAckPacket() 
{
}

void ConnAckPacket::parseVariableHeader(char* data, size_t len, size_t* currentBytePosition) 
{
  (void)len;
  
  char currentByte = data[(*currentBytePosition)++];
  
  if (_bytePosition++ == 0) 
  {
    _sessionPresent = (currentByte << 7) >> 7;
  } 
  else 
  {
    _connectReturnCode = currentByte;
    _parsingInformation->bufferState = BufferState::NONE;
    _callback(_sessionPresent, _connectReturnCode);
  }
}

void ConnAckPacket::parsePayload(char* data, size_t len, size_t* currentBytePosition) 
{
  (void)data;
  (void)len;
  (void)currentBytePosition;
}
