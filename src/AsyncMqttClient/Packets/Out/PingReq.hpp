/****************************************************************************************************************************
  PingReq.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.2.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
  1.2.0    K Hoang     15/03/2022 Add support to STM32 using LAN8742A or LAN8720 (without TLS/SSL)
 *****************************************************************************************************************************/

#pragma once

#ifndef PING_REQ_HPP
#define PING_REQ_HPP

#include "OutPacket.hpp"
#include "../../Flags.hpp"
#include "../../Helpers.hpp"

namespace AsyncMqttClientInternals 
{
	class PingReqOutPacket : public OutPacket 
	{
		public:
			PingReqOutPacket();
			const uint8_t* data(size_t index = 0) const;
			size_t size() const;

		private:
			uint8_t _data[2];
	};
}  // namespace AsyncMqttClientInternals

#endif		// PING_REQ_HPP
