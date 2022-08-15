/****************************************************************************************************************************
  Storage.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#pragma once

#ifndef STORAGE_HPP
#define STORAGE_HPP

namespace AsyncMqttClientInternals 
{
  struct PendingPubRel 
  {
    uint16_t packetId;
  };

  struct PendingAck 
  {
    uint8_t packetType;
    uint8_t headerFlag;
    uint16_t packetId;
  };
}  // namespace AsyncMqttClientInternals

#endif    // STORAGE_HPP
