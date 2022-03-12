/****************************************************************************************************************************
  ParsingInformation.hpp
  
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

#ifndef PARSING_INFORMATION_HPP
#define PARSING_INFORMATION_HPP

namespace AsyncMqttClientInternals 
{
	enum class BufferState : uint8_t 
	{
		NONE 							= 0,
		REMAINING_LENGTH 	= 2,
		VARIABLE_HEADER 	= 3,
		PAYLOAD 					= 4
	};

	struct ParsingInformation 
	{
		BufferState bufferState;

		uint16_t maxTopicLength;
		char* topicBuffer;

		uint8_t packetType;
		uint16_t packetFlags;
		uint32_t remainingLength;
	};
}  // namespace AsyncMqttClientInternals

#endif		// PARSING_INFORMATION_HPP
