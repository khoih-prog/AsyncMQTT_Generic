/****************************************************************************************************************************
  Callbacks.hpp
  
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

#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <functional>

#include "DisconnectReasons.hpp"
#include "MessageProperties.hpp"
#include "Errors.hpp"

namespace AsyncMqttClientInternals 
{
	// user callbacks
	typedef std::function<void(bool sessionPresent)> OnConnectUserCallback;
	typedef std::function<void(AsyncMqttClientDisconnectReason reason)> OnDisconnectUserCallback;
	typedef std::function<void(uint16_t packetId, uint8_t qos)> OnSubscribeUserCallback;
	typedef std::function<void(uint16_t packetId)> OnUnsubscribeUserCallback;
	typedef std::function<void(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)> OnMessageUserCallback;
	typedef std::function<void(uint16_t packetId)> OnPublishUserCallback;
	typedef std::function<void(uint16_t packetId, AsyncMqttClientError error)> OnErrorUserCallback;

	// internal callbacks
	typedef std::function<void(bool sessionPresent, uint8_t connectReturnCode)> OnConnAckInternalCallback;
	typedef std::function<void()> OnPingRespInternalCallback;
	typedef std::function<void(uint16_t packetId, char status)> OnSubAckInternalCallback;
	typedef std::function<void(uint16_t packetId)> OnUnsubAckInternalCallback;
	typedef std::function<void(char* topic, char* payload, uint8_t qos, bool dup, bool retain, size_t len, size_t index, size_t total, uint16_t packetId)> OnMessageInternalCallback;
	typedef std::function<void(uint16_t packetId, uint8_t qos)> OnPublishInternalCallback;
	typedef std::function<void(uint16_t packetId)> OnPubRelInternalCallback;
	typedef std::function<void(uint16_t packetId)> OnPubAckInternalCallback;
	typedef std::function<void(uint16_t packetId)> OnPubRecInternalCallback;
	typedef std::function<void(uint16_t packetId)> OnPubCompInternalCallback;
}  // namespace AsyncMqttClientInternals

#endif		// CALLBACKS_HPP
