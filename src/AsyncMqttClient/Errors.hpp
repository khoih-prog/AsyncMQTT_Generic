/****************************************************************************************************************************
  Errors.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#pragma once

#ifndef ERRORS_HPP
#define ERRORS_HPP

enum class AsyncMqttClientError : uint8_t 
{
  MAX_RETRIES   = 0,
  OUT_OF_MEMORY = 1
};

#endif    // ERRORS_HPP
