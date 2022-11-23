/****************************************************************************************************************************
  PingRespPacket.cpp

  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support

  Based on and modified from :

  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)

  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#include "PingRespPacket.hpp"

using AsyncMqttClientInternals::PingRespPacket;

PingRespPacket::PingRespPacket(ParsingInformation* parsingInformation, OnPingRespInternalCallback callback)
  : _parsingInformation(parsingInformation)
  , _callback(callback)
{
}

PingRespPacket::~PingRespPacket()
{
}

void PingRespPacket::parseVariableHeader(char* data, size_t len, size_t* currentBytePosition)
{
  (void)data;
  (void)len;
  (void)currentBytePosition;
}

void PingRespPacket::parsePayload(char* data, size_t len, size_t* currentBytePosition)
{
  (void)data;
  (void)len;
  (void)currentBytePosition;
}
