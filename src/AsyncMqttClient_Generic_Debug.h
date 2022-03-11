/****************************************************************************************************************************
  AsyncMqttClient_Generic_Debug.h
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.0.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
 *****************************************************************************************************************************/

#pragma once

#ifndef ASYNC_MQTT_CLIENT_DEBUG_H
#define ASYNC_MQTT_CLIENT_DEBUG_H

#ifdef ASYNC_MQTT_DEBUG_PORT
  #define DBG_PORT_AMQTT      ASYNC_MQTT_DEBUG_PORT
#else
  #define DBG_PORT_AMQTT      Serial
#endif

// Change _ASYNC_MQTT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _ASYNC_MQTT_LOGLEVEL_
  #define _ASYNC_MQTT_LOGLEVEL_       1
#endif

/////////////////////////////////////////////////////////

#define AMQTT_PRINT_MARK      AMQTT_PRINT("[AMQTT] ")
#define AMQTT_PRINT_SP        DBG_PORT_AMQTT.print(" ")
#define AMQTT_PRINT_SP0X      DBG_PORT_AMQTT.print(" 0x")

#define AMQTT_PRINT           DBG_PORT_AMQTT.print
#define AMQTT_PRINTLN         DBG_PORT_AMQTT.println

/////////////////////////////////////////////////////////

#define AMQTT_LOGERROR(x)         if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINTLN(x); }
#define AMQTT_LOGERROR0(x)        if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT(x); }
#define AMQTT_LOGERROR1(x,y)      if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINTLN(y); }
#define AMQTT_HEXLOGERROR1(x,y)   if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(y, HEX); }
#define AMQTT_LOGERROR2(x,y,z)    if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINTLN(z); }
#define AMQTT_HEXLOGERROR2(x,y,z) if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINT(y, HEX); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(z, HEX); }
#define AMQTT_LOGERROR3(x,y,z,w)  if(_ASYNC_MQTT_LOGLEVEL_>0) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINT(z); AMQTT_PRINT_SP; AMQTT_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define AMQTT_LOGWARN(x)          if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINTLN(x); }
#define AMQTT_LOGWARN0(x)         if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT(x); }
#define AMQTT_LOGWARN1(x,y)       if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINTLN(y); }
#define AMQTT_HEXLOGWARN1(x,y)    if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(y, HEX); }
#define AMQTT_LOGWARN2(x,y,z)     if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINTLN(z); }
#define AMQTT_HEXLOGWARN2(x,y,z)  if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINT(y, HEX); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(z, HEX); }
#define AMQTT_LOGWARN3(x,y,z,w)   if(_ASYNC_MQTT_LOGLEVEL_>1) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINT(z); AMQTT_PRINT_SP; AMQTT_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define AMQTT_LOGINFO(x)          if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINTLN(x); }
#define AMQTT_LOGINFO0(x)         if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT(x); }
#define AMQTT_LOGINFO1(x,y)       if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINTLN(y); }
#define AMQTT_HEXLOGINFO1(x,y)    if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(y, HEX); }
#define AMQTT_LOGINFO2(x,y,z)     if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINTLN(z); }
#define AMQTT_HEXLOGINFO2(x,y,z)  if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINT(y, HEX); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(z, HEX); }
#define AMQTT_LOGINFO3(x,y,z,w)   if(_ASYNC_MQTT_LOGLEVEL_>2) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINT(z); AMQTT_PRINT_SP; AMQTT_PRINTLN(w); }

/////////////////////////////////////////////////////////

#define AMQTT_LOGDEBUG(x)         if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINTLN(x); }
#define AMQTT_LOGDEBUG0(x)        if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT(x); }
#define AMQTT_LOGDEBUG1(x,y)      if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINTLN(y); }
#define AMQTT_HEXLOGDEBUG1(x,y)   if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(y, HEX); }
#define AMQTT_LOGDEBUG2(x,y,z)    if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINTLN(z); }
#define AMQTT_HEXLOGDEBUG2(x,y,z) if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP0X; AMQTT_PRINT(y, HEX); AMQTT_PRINT_SP0X; AMQTT_PRINTLN(z, HEX); }
#define AMQTT_LOGDEBUG3(x,y,z,w)  if(_ASYNC_MQTT_LOGLEVEL_>3) { AMQTT_PRINT_MARK; AMQTT_PRINT(x); AMQTT_PRINT_SP; AMQTT_PRINT(y); AMQTT_PRINT_SP; AMQTT_PRINT(z); AMQTT_PRINT_SP; AMQTT_PRINTLN(w); }

/////////////////////////////////////////////////////////

#endif    //ASYNC_MQTT_CLIENT_DEBUG_H
