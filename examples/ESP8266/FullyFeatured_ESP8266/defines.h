/****************************************************************************************************************************
  defines.h
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 ***************************************************************************************************************************************/

// Use ESP8266 core v2.7.4- for SSL as new cores don't use axtls anymore
// Use core v3.0.2+ for LwIP Ethernet W5500lwIP, W5100lwIP and ENC28J60lwIP libraries 
// Must use KH forked ESPAsyncTCP library or compile error

#ifndef defines_h
#define defines_h

#define _ASYNC_MQTT_LOGLEVEL_               1

#define WIFI_SSID         "yourSSID"
#define WIFI_PASSWORD     "yourpass"

#endif    //defines_h
