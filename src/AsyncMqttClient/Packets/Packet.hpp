/****************************************************************************************************************************
  Packet.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.0.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
 *****************************************************************************************************************************/
 
#pragma once

#ifndef PACKET_HPP
#define PACKET_HPP

namespace AsyncMqttClientInternals 
{
	class Packet 
	{
	 public:
		virtual ~Packet() {}

		virtual void parseVariableHeader(char* data, size_t len, size_t* currentBytePosition) = 0;
		virtual void parsePayload(char* data, size_t len, size_t* currentBytePosition) = 0;
	};
}  // namespace AsyncMqttClientInternals

#endif		// PACKET_HPP
