/****************************************************************************************************************************
  Disconn.hpp
  
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

#pragma once

#ifndef DISCONN_HPP
#define DISCONN_HPP

#include "OutPacket.hpp"
#include "../../Flags.hpp"
#include "../../Helpers.hpp"

namespace AsyncMqttClientInternals 
{
	class DisconnOutPacket : public OutPacket 
	{
		public:
		DisconnOutPacket();
		const uint8_t* data(size_t index = 0) const;
		size_t size() const;

		private:
		uint8_t _data[2];
	};
}  // namespace AsyncMqttClientInternals

#endif		// DISCONN_HPP
