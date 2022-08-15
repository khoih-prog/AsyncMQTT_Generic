/****************************************************************************************************************************
  DisconnectReasons.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#pragma once

#ifndef DISCONNECT_REASONS_HPP
#define DISCONNECT_REASONS_HPP

enum class AsyncMqttClientDisconnectReason : uint8_t 
{
  TCP_DISCONNECTED                    = 0,

  MQTT_UNACCEPTABLE_PROTOCOL_VERSION  = 1,
  MQTT_IDENTIFIER_REJECTED            = 2,
  MQTT_SERVER_UNAVAILABLE             = 3,
  MQTT_MALFORMED_CREDENTIALS          = 4,
  MQTT_NOT_AUTHORIZED                 = 5,

  ESP8266_NOT_ENOUGH_SPACE            = 6,

  TLS_BAD_FINGERPRINT                 = 7
};

#endif    // DISCONNECT_REASONS_HPP
