/****************************************************************************************************************************
  Helpers.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.2.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
  1.2.0    K Hoang     15/03/2022 Add support to STM32 using LAN8742A (without TLS/SSL)
  1.2.1    K Hoang     16/03/2022 Add support to STM32 using LAN8720 (without TLS/SSL)
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
