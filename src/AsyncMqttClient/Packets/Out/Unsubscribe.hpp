/****************************************************************************************************************************
  Unsubscribe.hpp
  
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

#ifndef UNSUBSCRIBE_HPP
#define UNSUBSCRIBE_HPP

#include <cstring>  // strlen
#include <vector>

#include "OutPacket.hpp"
#include "../../Flags.hpp"
#include "../../Helpers.hpp"
#include "../../Storage.hpp"

namespace AsyncMqttClientInternals 
{
	class UnsubscribeOutPacket : public OutPacket 
	{
		public:
			explicit UnsubscribeOutPacket(const char* topic);
			const uint8_t* data(size_t index = 0) const;
			size_t size() const;

		private:
			std::vector<uint8_t> _data;
	};
}  // namespace AsyncMqttClientInternals

#endif		// UNSUBSCRIBE_HPP
