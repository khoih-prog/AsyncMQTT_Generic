/****************************************************************************************************************************
  PublishPacket.hpp
  
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
 
#pragma once

#ifndef PUBLISH_PACKET_HPP
#define PUBLISH_PACKET_HPP

#include "Arduino.h"
#include "Packet.hpp"
#include "../Flags.hpp"
#include "../ParsingInformation.hpp"
#include "../Callbacks.hpp"

namespace AsyncMqttClientInternals 
{
	class PublishPacket : public Packet 
	{
	 public:
		explicit PublishPacket(ParsingInformation* parsingInformation, OnMessageInternalCallback dataCallback, OnPublishInternalCallback completeCallback);
		~PublishPacket();

		void parseVariableHeader(char* data, size_t len, size_t* currentBytePosition);
		void parsePayload(char* data, size_t len, size_t* currentBytePosition);

	 private:
		ParsingInformation* _parsingInformation;
		OnMessageInternalCallback _dataCallback;
		OnPublishInternalCallback _completeCallback;

		void _preparePayloadHandling(uint32_t payloadLength);

		bool _dup;
		uint8_t _qos;
		bool _retain;

		uint8_t _bytePosition;
		char _topicLengthMsb;
		uint16_t _topicLength;
		bool _ignore;
		char _packetIdMsb;
		uint16_t _packetId;
		uint32_t _payloadLength;
		uint32_t _payloadBytesRead;
	};
}  // namespace AsyncMqttClientInternals

#endif		// PUBLISH_PACKET_HPP
