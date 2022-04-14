/****************************************************************************************************************************
  Flags.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.5.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
  1.2.0    K Hoang     15/03/2022 Add support to STM32 using LAN8742A (without TLS/SSL)
  1.2.1    K Hoang     16/03/2022 Add support to STM32 using LAN8720 (without TLS/SSL)
  1.3.0    K Hoang     16/03/2022 Add support to Portenta_H7 using built-in Ethernet or Murata WiFi (without TLS/SSL)
  1.4.0    K Hoang     17/03/2022 Add support to Teensy 4.1 using QNEthernet Library
  1.5.0    K Hoang     14/04/2022 Add support to ESP8266 W5x00/ENC28J60 using lwip_W5100/lwip_W5500 or lwip_enc28j60 library
 *****************************************************************************************************************************/

#pragma once

#ifndef FLAGS_HPP
#define FLAGS_HPP

namespace AsyncMqttClientInternals 
{
  constexpr struct 
  {
    const uint8_t RESERVED    = 0;
    const uint8_t CONNECT     = 1;
    const uint8_t CONNACK     = 2;
    const uint8_t PUBLISH     = 3;
    const uint8_t PUBACK      = 4;
    const uint8_t PUBREC      = 5;
    const uint8_t PUBREL      = 6;
    const uint8_t PUBCOMP     = 7;
    const uint8_t SUBSCRIBE   = 8;
    const uint8_t SUBACK      = 9;
    const uint8_t UNSUBSCRIBE = 10;
    const uint8_t UNSUBACK    = 11;
    const uint8_t PINGREQ     = 12;
    const uint8_t PINGRESP    = 13;
    const uint8_t DISCONNECT  = 14;
    const uint8_t RESERVED2   = 15;
  } PacketType;

  constexpr struct 
  {
    const uint8_t CONNECT_RESERVED     = 0x00;
    const uint8_t CONNACK_RESERVED     = 0x00;
    const uint8_t PUBLISH_DUP          = 0x08;
    const uint8_t PUBLISH_QOS0         = 0x00;
    const uint8_t PUBLISH_QOS1         = 0x02;
    const uint8_t PUBLISH_QOS2         = 0x04;
    const uint8_t PUBLISH_QOSRESERVED  = 0x06;
    const uint8_t PUBLISH_RETAIN       = 0x01;
    const uint8_t PUBACK_RESERVED      = 0x00;
    const uint8_t PUBREC_RESERVED      = 0x00;
    const uint8_t PUBREL_RESERVED      = 0x02;
    const uint8_t PUBCOMP_RESERVED     = 0x00;
    const uint8_t SUBSCRIBE_RESERVED   = 0x02;
    const uint8_t SUBACK_RESERVED      = 0x00;
    const uint8_t UNSUBSCRIBE_RESERVED = 0x02;
    const uint8_t UNSUBACK_RESERVED    = 0x00;
    const uint8_t PINGREQ_RESERVED     = 0x00;
    const uint8_t PINGRESP_RESERVED    = 0x00;
    const uint8_t DISCONNECT_RESERVED  = 0x00;
    const uint8_t RESERVED2_RESERVED   = 0x00;
  } HeaderFlag;

  constexpr struct 
  {
    const uint8_t USERNAME      = 0x80;
    const uint8_t PASSWORD      = 0x40;
    const uint8_t WILL_RETAIN   = 0x20;
    const uint8_t WILL_QOS0     = 0x00;
    const uint8_t WILL_QOS1     = 0x08;
    const uint8_t WILL_QOS2     = 0x10;
    const uint8_t WILL          = 0x04;
    const uint8_t CLEAN_SESSION = 0x02;
    const uint8_t RESERVED      = 0x00;
  } ConnectFlag;
}  // namespace AsyncMqttClientInternals

#endif    // FLAGS_HPP
