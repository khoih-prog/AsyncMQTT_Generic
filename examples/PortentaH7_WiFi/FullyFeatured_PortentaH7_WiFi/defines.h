/****************************************************************************************************************************
  defines.h
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define ASYNC_MQTT_DEBUG_PORT               Serial
#define _ASYNC_MQTT_LOGLEVEL_               1

#define USE_ETHERNET_PORTENTA_H7            false
#define USE_WIFI_PORTENTA_H7                true

#include <WiFi.h>

#if (_ASYNC_MQTT_LOGLEVEL_ > 3)
  #warning Using WiFi for Portenta_H7
#endif

#define WIFI_SSID         "yourSSID"    // your network SSID (name)
#define WIFI_PASSWORD     "yourpass"    // your network password (use for WPA, or use as key for WEP), length must be 8+

int status = WL_IDLE_STATUS;

#endif    //defines_h
