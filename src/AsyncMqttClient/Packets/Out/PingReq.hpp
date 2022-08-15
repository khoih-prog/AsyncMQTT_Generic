/****************************************************************************************************************************
  PingReq.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
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

#endif    // PING_REQ_HPP
