/****************************************************************************************************************************
  Helpers.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#pragma once

#ifndef HELPERS_HPP
#define HELPERS_HPP

namespace AsyncMqttClientInternals
{
class Helpers
{
  public:
    static uint32_t decodeRemainingLength(char* bytes)
    {
      uint32_t multiplier = 1;
      uint32_t value = 0;
      uint8_t currentByte = 0;
      uint8_t encodedByte;

      do
      {
        encodedByte = bytes[currentByte++];
        value += (encodedByte & 127) * multiplier;
        multiplier *= 128;
      } while ((encodedByte & 128) != 0);

      return value;
    }

    static uint8_t encodeRemainingLength(uint32_t remainingLength, char* destination)
    {
      uint8_t currentByte = 0;
      uint8_t bytesNeeded = 0;

      do
      {
        uint8_t encodedByte = remainingLength % 128;
        remainingLength /= 128;

        if (remainingLength > 0)
        {
          encodedByte = encodedByte | 128;
        }

        destination[currentByte++] = encodedByte;
        bytesNeeded++;
      } while (remainingLength > 0);

      return bytesNeeded;
    }
};

}  // namespace AsyncMqttClientInternals

#endif    // HELPERS_HPP
