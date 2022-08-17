/****************************************************************************************************************************
  defines.h
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, Teensy41_QNEthernet, STM32F7, etc. 
  with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#if !( defined(ARDUINO_RASPBERRY_PI_PICO_W) )
  #error For RASPBERRY_PI_PICO_W only
#endif

// Debug Level from 0 to 4
#define _ASYNCTCP_RP2040W_LOGLEVEL_         1
#define _ASYNC_MQTT_LOGLEVEL_               1

#if (_ASYNC_MQTT_LOGLEVEL_ > 3)
  #warning Using RASPBERRY_PI_PICO_W with CYC43439 WiFi
#endif

#define WIFI_SSID         "yourSSID"    // your network SSID (name)
#define WIFI_PASSWORD     "yourpass"    // your network password (use for WPA, or use as key for WEP), length must be 8+

#endif    //defines_h
