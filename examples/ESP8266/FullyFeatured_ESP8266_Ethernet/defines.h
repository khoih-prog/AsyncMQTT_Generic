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

//////////////////////////////////////////////////////////

#define USING_W5500         true
#define USING_W5100         false
#define USING_ENC28J60      false

#include <SPI.h>

#define CSPIN       16      // 5

#if USING_W5500
  #include "W5500lwIP.h"
  #define SHIELD_TYPE       "ESP8266_W5500 Ethernet"
  
  Wiznet5500lwIP eth(CSPIN); 
   
#elif USING_W5100
  #include <W5100lwIP.h>
  #define SHIELD_TYPE       "ESP8266_W5100 Ethernet"
  
  Wiznet5100lwIP eth(CSPIN);

#elif USING_ENC28J60
  #include <ENC28J60lwIP.h>
  #define SHIELD_TYPE       "ESP8266_ENC28J60 Ethernet"
  
  ENC28J60lwIP eth(CSPIN);
#else
  // default if none selected
  #include "W5500lwIP.h"

  Wiznet5500lwIP eth(CSPIN);
#endif

#include <WiFiClient.h> // WiFiClient (-> TCPClient)

using TCPClient = WiFiClient;

//////////////////////////////////////////////////////////

#define USING_DHCP        true

#if !USING_DHCP
  IPAddress localIP(192, 168, 2, 222);
  IPAddress gateway(192, 168, 2, 1);
  IPAddress netMask(255, 255, 255, 0);
#endif

#endif    //defines_h
