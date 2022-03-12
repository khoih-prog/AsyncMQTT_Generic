/****************************************************************************************************************************
  SubAckPacket.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.0.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
 *****************************************************************************************************************************/
 
#include "SubAckPacket.hpp"

using AsyncMqttClientInternals::SubAckPacket;

SubAckPacket::SubAckPacket(ParsingInformation* parsingInformation, OnSubAckInternalCallback callback)
: _parsingInformation(parsingInformation)
, _callback(callback)
, _bytePosition(0)
, _packetIdMsb(0)
, _packetId(0) {
}

SubAckPacket::~SubAckPacket() {
}

void SubAckPacket::parseVariableHeader(char* data, size_t len, size_t* currentBytePosition) {
  char currentByte = data[(*currentBytePosition)++];
  if (_bytePosition++ == 0) {
    _packetIdMsb = currentByte;
  } else {
    _packetId = currentByte | _packetIdMsb << 8;
    _parsingInformation->bufferState = BufferState::PAYLOAD;
  }
}

void SubAckPacket::parsePayload(char* data, size_t len, size_t* currentBytePosition) {
  char status = data[(*currentBytePosition)++];

  /* switch (status) {
    case 0:
      Serial.println("Success QoS 0");
      break;
    case 1:
      Serial.println("Success QoS 1");
      break;
    case 2:
      Serial.println("Success QoS 2");
      break;
    case 0x80:
      Serial.println("Failure");
      break;
  } */

  _parsingInformation->bufferState = BufferState::NONE;
  _callback(_packetId, status);
}
