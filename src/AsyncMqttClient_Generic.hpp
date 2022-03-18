/****************************************************************************************************************************
  AsyncMqttClient_Generic.hpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.4.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
  1.2.0    K Hoang     15/03/2022 Add support to STM32 using LAN8742A (without TLS/SSL)
  1.2.1    K Hoang     16/03/2022 Add support to STM32 using LAN8720 (without TLS/SSL)
  1.3.0    K Hoang     16/03/2022 Add support to Portenta_H7 using built-in Ethernet or Murata WiFi (without TLS/SSL)
  1.4.0    K Hoang     17/03/2022 Add support to Teensy 4.1 using QNEthernet Library
 *****************************************************************************************************************************/

#pragma once

#ifndef ASYNC_MQTT_CLIENT_HPP
#define ASYNC_MQTT_CLIENT_HPP

#include <functional>
#include <vector>

#include "Arduino.h"

#ifndef MQTT_MIN_FREE_MEMORY
#define MQTT_MIN_FREE_MEMORY 4096
#endif

/////////////////////////////////////////////////////////

#define ASYNC_MQTT_GENERIC_SHORT_VERSION        "AsyncMQTT_Generic v1.4.0" 

/////////////////////////////////////////////////////////

#define ASYNC_MQTT_GENERIC_VERSION_MAJOR       1
#define ASYNC_MQTT_GENERIC_VERSION_MINOR       4
#define ASYNC_MQTT_GENERIC_VERSION_PATCH       0

#define ASYNC_MQTT_GENERIC_VERSION_INT         1004000

/////////////////////////////////////////////////////////

#ifdef ESP32

  #if ( ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) ) && ( ARDUINO_ESP32_GIT_VER != 0x46d5afb1 ) )
    #define USING_CORE_ESP32_CORE_V200_PLUS      true
    
    #if (_ASYNC_MQTT_LOGLEVEL_ > 3)
      #warning Using code for ESP32 core v2.0.0+ in AsyncMqttClient_Generic.hpp
    #endif
      
    #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for ESP32 core v2.0.0+")

  #else
    #if (_ASYNC_MQTT_LOGLEVEL_ > 3)
      #warning Using code for ESP32 core v1.0.6- in AsyncMqttClient_Generic.hpp
    #endif
    
    #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for ESP32 core v1.0.6-")
    
  #endif

  #if ASYNC_TCP_SSL_ENABLED
    #include <AsyncTCP_SSL.h>
    #warning ESP32 ASYNC_TCP_SSL_ENABLED
  #else
    #include <AsyncTCP.h>
  #endif
  
  #include <freertos/semphr.h>
  
#elif defined(ESP8266)

  #if ASYNC_TCP_SSL_ENABLED
    //#include <ESPAsyncTCP_SSL.h>
    #include <ESPAsyncTCP.h>
    //#warning ESP8266 ASYNC_TCP_SSL_ENABLED
    #error ESP8266 ASYNC_TCP_SSL_ENABLED not ready yet
  #else
    #include <ESPAsyncTCP.h>
    
    #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for ESP8266")
  #endif
  
#elif ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )

  #if ASYNC_TCP_SSL_ENABLED
    #error Portenta_H7 ASYNC_TCP_SSL_ENABLED not ready yet
    #include <Portenta_H7_AsyncTCP_SSL.h>
    #warning STM32 ASYNC_TCP_SSL_ENABLED   
  #else
    #include <Portenta_H7_AsyncTCP.h>
    
    #if defined(ARDUINO_PORTENTA_H7_M7)
      #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for Portenta_H7_M7")
    #else
      #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for Portenta_H7_M4")
    #endif
  #endif
  
  #define ASYNC_MQTT_USING_PORTENTA_H7    true   
    
#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
        defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
        defined(STM32WB) || defined(STM32MP1) )
       
  #if ASYNC_TCP_SSL_ENABLED
    #error STM32 ASYNC_TCP_SSL_ENABLED not ready yet
    #include <STM32AsyncTCP_SSL.h>
    #warning STM32 ASYNC_TCP_SSL_ENABLED   
  #else
    #include <STM32AsyncTCP.h>
    
    #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for STM32")
  #endif
  
  #define ASYNC_MQTT_USING_STM32      true

#elif ( defined(CORE_TEENSY) && defined(__IMXRT1062__) && defined(ARDUINO_TEENSY41) )

  #if ASYNC_TCP_SSL_ENABLED
    #error Teensy 4.1 QNEthernet ASYNC_TCP_SSL_ENABLED not ready yet
    #include <Teensy41_AsyncTCP_SSL.h>
    #warning Teensy 4.1 QNEthernet ASYNC_TCP_SSL_ENABLED   
  #else
    #include <Teensy41_AsyncTCP.h>
    
    #define ASYNC_MQTT_GENERIC_VERSION        (ASYNC_MQTT_GENERIC_SHORT_VERSION " for Teensy 4.1 QNEthernet")
  #endif
  
  #define ASYNC_MQTT_USING_TEENSY41_QNETHERNET      true        
#else
  #error Platform not supported
#endif


/////////////////////////////////////////////////////////

#if ASYNC_TCP_SSL_ENABLED
  #if (ESP32)
    #include <tcp_mbedtls.h>
  #elif defined(ESP8266)
    #include <tcp_axtls.h>
  #elif ASYNC_MQTT_USING_PORTENTA_H7
    #include <tcp_axtls.h>  
  #elif ASYNC_MQTT_USING_STM32
    #include <tcp_axtls.h>
  #elif ASYNC_MQTT_USING_TEENSY41_QNETHERNET
    #include <tcp_axtls.h>
  #endif
  
  #define SHA1_SIZE 20
#endif

/////////////////////////////////////////////////////////

#if defined(ARDUINO_ARCH_ESP32)
  #define SEMAPHORE_TAKE()      xSemaphoreTake(_xSemaphore, portMAX_DELAY)
  #define SEMAPHORE_GIVE()      xSemaphoreGive(_xSemaphore)
  #define GET_FREE_MEMORY()     ESP.getMaxAllocHeap()
  #include <esp32-hal-log.h>
#elif defined(ARDUINO_ARCH_ESP8266)
  #define SEMAPHORE_TAKE(X) while (_xSemaphore) { /*ESP.wdtFeed();*/ } _xSemaphore = true
  #define SEMAPHORE_GIVE()      _xSemaphore = false
  #define GET_FREE_MEMORY()     ESP.getMaxFreeBlockSize()
#elif ASYNC_MQTT_USING_PORTENTA_H7
  #define SEMAPHORE_TAKE(X) while (_xSemaphore) {  } _xSemaphore = true
  #define SEMAPHORE_GIVE()      _xSemaphore = false
  #define GET_FREE_MEMORY()     MQTT_MIN_FREE_MEMORY  
#elif ASYNC_MQTT_USING_STM32 
  #define SEMAPHORE_TAKE(X) while (_xSemaphore) {  } _xSemaphore = true
  #define SEMAPHORE_GIVE()      _xSemaphore = false
  #define GET_FREE_MEMORY()     MQTT_MIN_FREE_MEMORY
#elif ASYNC_MQTT_USING_TEENSY41_QNETHERNET
  #define SEMAPHORE_TAKE(X) while (_xSemaphore) {  } _xSemaphore = true
  #define SEMAPHORE_GIVE()      _xSemaphore = false
  #define GET_FREE_MEMORY()     MQTT_MIN_FREE_MEMORY  
#else
  #pragma error "No valid architecture"
#endif

/////////////////////////////////////////////////////////

#include "AsyncMqttClient_Generic_Debug.h"

#include "AsyncMqttClient/Flags.hpp"
#include "AsyncMqttClient/ParsingInformation.hpp"
#include "AsyncMqttClient/MessageProperties.hpp"
#include "AsyncMqttClient/Helpers.hpp"
#include "AsyncMqttClient/Callbacks.hpp"
#include "AsyncMqttClient/DisconnectReasons.hpp"
#include "AsyncMqttClient/Storage.hpp"

#include "AsyncMqttClient/Packets/Packet.hpp"
#include "AsyncMqttClient/Packets/ConnAckPacket.hpp"
#include "AsyncMqttClient/Packets/PingRespPacket.hpp"
#include "AsyncMqttClient/Packets/SubAckPacket.hpp"
#include "AsyncMqttClient/Packets/UnsubAckPacket.hpp"
#include "AsyncMqttClient/Packets/PublishPacket.hpp"
#include "AsyncMqttClient/Packets/PubRelPacket.hpp"
#include "AsyncMqttClient/Packets/PubAckPacket.hpp"
#include "AsyncMqttClient/Packets/PubRecPacket.hpp"
#include "AsyncMqttClient/Packets/PubCompPacket.hpp"

#include "AsyncMqttClient/Packets/Out/Connect.hpp"
#include "AsyncMqttClient/Packets/Out/PingReq.hpp"
#include "AsyncMqttClient/Packets/Out/PubAck.hpp"
#include "AsyncMqttClient/Packets/Out/Disconn.hpp"
#include "AsyncMqttClient/Packets/Out/Subscribe.hpp"
#include "AsyncMqttClient/Packets/Out/Unsubscribe.hpp"
#include "AsyncMqttClient/Packets/Out/Publish.hpp"

/////////////////////////////////////////////////////////

class AsyncMqttClient 
{
 public:
  AsyncMqttClient();
  ~AsyncMqttClient();

  AsyncMqttClient& setKeepAlive(uint16_t keepAlive);
  AsyncMqttClient& setClientId(const char* clientId);
  AsyncMqttClient& setCleanSession(bool cleanSession);
  AsyncMqttClient& setMaxTopicLength(uint16_t maxTopicLength);
  AsyncMqttClient& setCredentials(const char* username, const char* password = nullptr);
  AsyncMqttClient& setWill(const char* topic, uint8_t qos, bool retain, const char* payload = nullptr, size_t length = 0);
  AsyncMqttClient& setServer(IPAddress ip, uint16_t port);
  AsyncMqttClient& setServer(const char* host, uint16_t port);
  
#if ASYNC_TCP_SSL_ENABLED
  AsyncMqttClient& setSecure(bool secure);
  AsyncMqttClient& addServerFingerprint(const uint8_t* fingerprint);
#endif

  AsyncMqttClient& onConnect(AsyncMqttClientInternals::OnConnectUserCallback callback);
  AsyncMqttClient& onDisconnect(AsyncMqttClientInternals::OnDisconnectUserCallback callback);
  AsyncMqttClient& onSubscribe(AsyncMqttClientInternals::OnSubscribeUserCallback callback);
  AsyncMqttClient& onUnsubscribe(AsyncMqttClientInternals::OnUnsubscribeUserCallback callback);
  AsyncMqttClient& onMessage(AsyncMqttClientInternals::OnMessageUserCallback callback);
  AsyncMqttClient& onPublish(AsyncMqttClientInternals::OnPublishUserCallback callback);

  bool connected() const;
  void connect();
  void disconnect(bool force = false);
  uint16_t subscribe(const char* topic, uint8_t qos);
  uint16_t unsubscribe(const char* topic);
  uint16_t publish(const char* topic, uint8_t qos, bool retain, const char* payload = nullptr, 
                   size_t length = 0, bool dup = false, uint16_t message_id = 0);
                   
  bool clearQueue();  // Not MQTT compliant!

  const char* getClientId() const;

 private:
 
#if ASYNC_TCP_SSL_ENABLED
  AsyncSSLClient _client;
#else
  AsyncClient _client;
#endif  
  
  AsyncMqttClientInternals::OutPacket* _head;
  AsyncMqttClientInternals::OutPacket* _tail;
  size_t _sent;
  
  enum 
  {
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED
  } _state;
  
  AsyncMqttClientDisconnectReason _disconnectReason;
  uint32_t _lastClientActivity;
  uint32_t _lastServerActivity;
  uint32_t _lastPingRequestTime;

  char _generatedClientId[18 + 1];  // esp8266-abc123, esp32-abcdef123456 or stm32-abcdef123456
  IPAddress _ip;
  const char* _host;
  bool _useIp;
  
#if ASYNC_TCP_SSL_ENABLED
  bool _secure;
#endif

  uint16_t _port;
  uint16_t _keepAlive;
  bool _cleanSession;
  const char* _clientId;
  const char* _username;
  const char* _password;
  const char* _willTopic;
  const char* _willPayload;
  uint16_t _willPayloadLength;
  uint8_t _willQos;
  bool _willRetain;

#if ASYNC_TCP_SSL_ENABLED
  std::vector<std::array<uint8_t, SHA1_SIZE>> _secureServerFingerprints;
#endif

  std::vector<AsyncMqttClientInternals::OnConnectUserCallback> _onConnectUserCallbacks;
  std::vector<AsyncMqttClientInternals::OnDisconnectUserCallback> _onDisconnectUserCallbacks;
  std::vector<AsyncMqttClientInternals::OnSubscribeUserCallback> _onSubscribeUserCallbacks;
  std::vector<AsyncMqttClientInternals::OnUnsubscribeUserCallback> _onUnsubscribeUserCallbacks;
  std::vector<AsyncMqttClientInternals::OnMessageUserCallback> _onMessageUserCallbacks;
  std::vector<AsyncMqttClientInternals::OnPublishUserCallback> _onPublishUserCallbacks;

  AsyncMqttClientInternals::ParsingInformation _parsingInformation;
  AsyncMqttClientInternals::Packet* _currentParsedPacket;
  uint8_t _remainingLengthBufferPosition;
  char _remainingLengthBuffer[4];

  std::vector<AsyncMqttClientInternals::PendingPubRel> _pendingPubRels;

#if defined(ESP32)
  SemaphoreHandle_t _xSemaphore = nullptr;
#elif defined(ESP8266)
  bool _xSemaphore = false;
#elif ASYNC_MQTT_USING_PORTENTA_H7
  bool _xSemaphore = false;  
#elif ASYNC_MQTT_USING_STM32 
  bool _xSemaphore = false;
#elif ASYNC_MQTT_USING_TEENSY41_QNETHERNET
  bool _xSemaphore = false;  
#endif

  void _clear();
  void _freeCurrentParsedPacket();

  // TCP
  void _onConnect();
  void _onDisconnect();
  // void _onError(int8_t error);
  // void _onTimeout();
  void _onAck(size_t len);
  void _onData(char* data, size_t len);
  void _onPoll();

  // QUEUE
  void _insert(AsyncMqttClientInternals::OutPacket* packet);    // for PUBREL
  void _addFront(AsyncMqttClientInternals::OutPacket* packet);  // for CONNECT
  void _addBack(AsyncMqttClientInternals::OutPacket* packet);   // all the rest
  void _handleQueue();
  void _clearQueue(bool keepSessionData);

  // MQTT
  void _onPingResp();
  void _onConnAck(bool sessionPresent, uint8_t connectReturnCode);
  void _onSubAck(uint16_t packetId, char status);
  void _onUnsubAck(uint16_t packetId);
  void _onMessage(char* topic, char* payload, uint8_t qos, bool dup, bool retain, size_t len, 
                  size_t index, size_t total, uint16_t packetId);
  void _onPublish(uint16_t packetId, uint8_t qos);
  void _onPubRel(uint16_t packetId);
  void _onPubAck(uint16_t packetId);
  void _onPubRec(uint16_t packetId);
  void _onPubComp(uint16_t packetId);

  void _sendPing();
  
#if ASYNC_MQTT_USING_TEENSY41_QNETHERNET
  uint8_t* getTeensyMac(uint8_t* _macAddress);
#endif  
};

#endif    // ASYNC_MQTT_CLIENT_HPP
