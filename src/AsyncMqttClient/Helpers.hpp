/****************************************************************************************************************************
  Helpers.hpp
  
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

#if defined(ARDUINO_ARCH_ESP32)
  #define SEMAPHORE_TAKE() xSemaphoreTake(_xSemaphore, portMAX_DELAY)
  #define SEMAPHORE_GIVE() xSemaphoreGive(_xSemaphore)
  #define GET_FREE_MEMORY() ESP.getMaxAllocHeap()
  #include <esp32-hal-log.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #define SEMAPHORE_TAKE(X) while (_xSemaphore) { /*ESP.wdtFeed();*/ } _xSemaphore = true
  #define SEMAPHORE_GIVE() _xSemaphore = false
  #define GET_FREE_MEMORY() ESP.getMaxFreeBlockSize()
#else
  //#pragma error "No valid architecture"
#endif

}  // namespace AsyncMqttClientInternals

#endif		// HELPERS_HPP
