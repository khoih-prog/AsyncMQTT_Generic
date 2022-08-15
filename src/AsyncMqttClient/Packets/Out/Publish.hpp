/****************************************************************************************************************************
  Publish.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#pragma once

#ifndef PUBLISH_HPP
#define PUBLISH_HPP

#include <cstring>  // strlen
#include <vector>

#include "OutPacket.hpp"
#include "../../Flags.hpp"
#include "../../Helpers.hpp"
#include "../../Storage.hpp"

namespace AsyncMqttClientInternals 
{
  class PublishOutPacket : public OutPacket 
  {
    public:
      PublishOutPacket(const char* topic, uint8_t qos, bool retain, const char* payload, size_t length);
      const uint8_t* data(size_t index = 0) const;
      size_t size() const;

    void setDup();  // you cannot unset dup

    private:
      std::vector<uint8_t> _data;
  };
}  // namespace AsyncMqttClientInternals

#endif    // PUBLISH_HPP
