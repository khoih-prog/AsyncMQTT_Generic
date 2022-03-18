/****************************************************************************************************************************
  defines.h
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support

  Based on and modified from :

  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)

  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#if !( defined(CORE_TEENSY) && defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41) )
#error Only Teensy 4.1 supported
#endif

// Debug Level from 0 to 4
#define _TEENSY41_ASYNC_TCP_LOGLEVEL_       1
#define _ASYNC_MQTT_LOGLEVEL_               4

#define SHIELD_TYPE     "Teensy4.1 QNEthernet"

#include "QNEthernet.h"       // https://github.com/ssilverman/QNEthernet
using namespace qindesign::network;

#if (_ASYNC_MQTT_LOGLEVEL_ > 3)
#warning Using QNEthernet lib for Teensy 4.1. Must also use Teensy Packages Patch or error
#endif
#define USING_DHCP            true
//#define USING_DHCP            false

#if !USING_DHCP
// Set the static IP address to use if the DHCP fails to assign
IPAddress myIP(192, 168, 2, 222);
IPAddress myNetmask(255, 255, 255, 0);
IPAddress myGW(192, 168, 2, 1);
//IPAddress mydnsServer(192, 168, 2, 1);
IPAddress mydnsServer(8, 8, 8, 8);
#endif

#endif    //defines_h
