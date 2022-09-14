/****************************************************************************************************************************
  AsyncMqttClient_Generic_Impl.h
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.7.0
  
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
  1.6.0    K Hoang     14/08/2022 Add support to RP2040W with CYW43439 WiFi using arduino-pico core
  1.6.1    K Hoang     17/08/2022 Better workaround for RP2040W WiFi.status() bug using ping() to local gateway
  1.7.0    K Hoang     13/09/2022 Fix ESP32 and ESP8266 compile error
 *****************************************************************************************************************************/

#pragma once

#ifndef ASYNC_MQTT_CLIENT_IMPL_H
#define ASYNC_MQTT_CLIENT_IMPL_H

#if !defined(ASYNC_TCP_SSL_ENABLED)
  #define ASYNC_TCP_SSL_ENABLED       false
#endif

/////////////////////////////////////////////////////////

const char* PacketTypeName[] = 
{ 
  "RESERVED",
  "CONNECT",
  "CONNACK",
  "PUBLISH",
  "PUBACK",
  "PUBREC",
  "PUBREL",
  "PUBCOMP",
  "SUBSCRIBE",
  "SUBACK",
  "UNSUBSCRIBE",
  "UNSUBACK",
  "PINGREQ",
  "PINGRESP",
  "DISCONNECT",
  "RESERVED2" 
};

/////////////////////////////////////////////////////////

AsyncMqttClient::AsyncMqttClient()
  : _client()
  , _head(nullptr)
  , _tail(nullptr)
  , _sent(0)
  , _state(DISCONNECTED)
  , _disconnectReason(AsyncMqttClientDisconnectReason::TCP_DISCONNECTED)
  , _lastClientActivity(0)
  , _lastServerActivity(0)
  , _lastPingRequestTime(0)
  , _generatedClientId{0}
  , _ip()
  , _host(nullptr)
  , _useIp(false)
#if ASYNC_TCP_SSL_ENABLED
  , _secure(false)
#endif
  , _port(0)
  , _keepAlive(15)
  , _cleanSession(true)
  , _clientId(nullptr)
  , _username(nullptr)
  , _password(nullptr)
  , _willTopic(nullptr)
  , _willPayload(nullptr)
  , _willPayloadLength(0)
  , _willQos(0)
  , _willRetain(false)
#if ASYNC_TCP_SSL_ENABLED
  , _secureServerFingerprints()
#endif
  , _onConnectUserCallbacks()
  , _onDisconnectUserCallbacks()
  , _onSubscribeUserCallbacks()
  , _onUnsubscribeUserCallbacks()
  , _onMessageUserCallbacks()
  , _onPublishUserCallbacks()
  , _parsingInformation() 
  , _currentParsedPacket(nullptr)
  , _remainingLengthBufferPosition(0)
  , _remainingLengthBuffer{0}
  , _pendingPubRels()
{
  _parsingInformation.bufferState = AsyncMqttClientInternals::BufferState::NONE;
  
#if ASYNC_TCP_SSL_ENABLED
  _client.onConnect([](void* obj, AsyncSSLClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onConnect();
  }, this);
  
  _client.onDisconnect([](void* obj, AsyncSSLClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onDisconnect();
  }, this);
  
  // _client.onError([](void* obj, AsyncSSLClient* c, int8_t error) { (static_cast<AsyncMqttClient*>(obj))->_onError(error); }, this);
  // _client.onTimeout([](void* obj, AsyncSSLClient* c, uint32_t time) { (static_cast<AsyncMqttClient*>(obj))->_onTimeout(); }, this);
  
  _client.onAck([](void* obj, AsyncSSLClient * c, size_t len, uint32_t time) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onAck(len);
  }, this);
  
  _client.onData([](void* obj, AsyncSSLClient * c, void* data, size_t len) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onData(static_cast<char*>(data), len);
  }, this);
  
  _client.onPoll([](void* obj, AsyncSSLClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onPoll();
  }, this);
  
#else

  _client.onConnect([](void* obj, AsyncClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onConnect();
  }, this);
  
  _client.onDisconnect([](void* obj, AsyncClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onDisconnect();
  }, this);
  
  // _client.onError([](void* obj, AsyncClient* c, int8_t error) { (static_cast<AsyncMqttClient*>(obj))->_onError(error); }, this);
  // _client.onTimeout([](void* obj, AsyncClient* c, uint32_t time) { (static_cast<AsyncMqttClient*>(obj))->_onTimeout(); }, this);
  
  _client.onAck([](void* obj, AsyncClient * c, size_t len, uint32_t time) 
  {
    (void) c;
    (void) time;
    (static_cast<AsyncMqttClient*>(obj))->_onAck(len);
  }, this);
  
  _client.onData([](void* obj, AsyncClient * c, void* data, size_t len) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onData(static_cast<char*>(data), len);
  }, this);
  
  _client.onPoll([](void* obj, AsyncClient * c) 
  {
    (void) c;
    (static_cast<AsyncMqttClient*>(obj))->_onPoll();
  }, this);
  
#endif

  _client.setNoDelay(true);  // send small packets immediately (PINGREQ/DISCONN are only 2 bytes)

#ifdef ESP32
  sprintf(_generatedClientId, "esp32-%06llx", ESP.getEfuseMac());
  _xSemaphore = xSemaphoreCreateMutex();
#elif defined(ESP8266)
  sprintf(_generatedClientId, "esp8266-%06x", ESP.getChipId());
#elif ASYNC_MQTT_USING_PORTENTA_H7
  // Will create _clientId from macAddress later in connect() as ID not available now  
#elif ASYNC_MQTT_USING_STM32
  // Will create _clientId from macAddress later in connect() as ID not available now
#elif ASYNC_MQTT_USING_TEENSY41_QNETHERNET
  // Will create _clientId from TensyID
#elif ASYNC_MQTT_USING_RP2040W
  // Will create _clientId from unique hardware ID later in connect() as ID not available now 
#endif
  
  _clientId = _generatedClientId;

  setMaxTopicLength(128);
}

/////////////////////////////////////////////////////////

AsyncMqttClient::~AsyncMqttClient()
{
  delete _currentParsedPacket;
  delete[] _parsingInformation.topicBuffer;
  
  _clear();
  _pendingPubRels.clear();
  _pendingPubRels.shrink_to_fit();
  _clearQueue(false);  // _clear() doesn't clear session data

#ifdef ESP32
  vSemaphoreDelete(_xSemaphore);
#endif
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setKeepAlive(uint16_t keepAlive)
{
  _keepAlive = keepAlive;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setClientId(const char* clientId)
{
  _clientId = clientId;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setCleanSession(bool cleanSession)
{
  _cleanSession = cleanSession;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setMaxTopicLength(uint16_t maxTopicLength)
{
  _parsingInformation.maxTopicLength = maxTopicLength;
  delete[] _parsingInformation.topicBuffer;
  _parsingInformation.topicBuffer = new char[maxTopicLength + 1];
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setCredentials(const char* username, const char* password)
{
  _username = username;
  _password = password;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setWill(const char* topic, uint8_t qos, bool retain, const char* payload, size_t length)
{
  _willTopic = topic;
  _willQos = qos;
  _willRetain = retain;
  _willPayload = payload;
  _willPayloadLength = length;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setServer(IPAddress ip, uint16_t port)
{
  _useIp = true;
  _ip = ip;
  _port = port;
  
  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::setServer(const char* host, uint16_t port)
{
  _useIp = false;
  _host = host;
  _port = port;
  
  return *this;
}

/////////////////////////////////////////////////////////

#if ASYNC_TCP_SSL_ENABLED
AsyncMqttClient& AsyncMqttClient::setSecure(bool secure)
{
  _secure = secure;
  
  return *this;
}

AsyncMqttClient& AsyncMqttClient::addServerFingerprint(const uint8_t* fingerprint)
{
  std::array<uint8_t, SHA1_SIZE> newFingerprint;
  memcpy(newFingerprint.data(), fingerprint, SHA1_SIZE);
  _secureServerFingerprints.push_back(newFingerprint);

  return *this;
}
#endif

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onConnect(AsyncMqttClientInternals::OnConnectUserCallback callback)
{
  _onConnectUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onDisconnect(AsyncMqttClientInternals::OnDisconnectUserCallback callback)
{
  _onDisconnectUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onSubscribe(AsyncMqttClientInternals::OnSubscribeUserCallback callback)
{
  _onSubscribeUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onUnsubscribe(AsyncMqttClientInternals::OnUnsubscribeUserCallback callback)
{
  _onUnsubscribeUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onMessage(AsyncMqttClientInternals::OnMessageUserCallback callback)
{
  _onMessageUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

AsyncMqttClient& AsyncMqttClient::onPublish(AsyncMqttClientInternals::OnPublishUserCallback callback)
{
  _onPublishUserCallbacks.push_back(callback);

  return *this;
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_freeCurrentParsedPacket()
{
  delete _currentParsedPacket;
  _currentParsedPacket = nullptr;
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_clear()
{
  _lastPingRequestTime = 0;
  _freeCurrentParsedPacket();
  _clearQueue(true);  // keep session data for now

  _parsingInformation.bufferState = AsyncMqttClientInternals::BufferState::NONE;

  _client.setRxTimeout(0);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/* TCP */
void AsyncMqttClient::_onConnect()
{
  AMQTT_LOGINFO("TCP conn, MQTT CONNECT");

#if ASYNC_TCP_SSL_ENABLED

#if 0

  if (_secure && _secureServerFingerprints.size() > 0)
  {
    SSL* clientSsl = _client.getSSL();

    bool sslFoundFingerprint = false;

    for (std::array<uint8_t, SHA1_SIZE> fingerprint : _secureServerFingerprints)
    {
      if (ssl_match_fingerprint(clientSsl, fingerprint.data()) == SSL_OK)
      {
        sslFoundFingerprint = true;
        break;
      }
    }

    if (!sslFoundFingerprint)
    {
      _disconnectReason = AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT;
      _client.close(true);
      return;
    }
  }
#endif

#endif

  AsyncMqttClientInternals::OutPacket* msg =
    new AsyncMqttClientInternals::ConnectOutPacket(_cleanSession,
        _username,
        _password,
        _willTopic,
        _willRetain,
        _willQos,
        _willPayload,
        _willPayloadLength,
        _keepAlive,
        _clientId);
        
  _addFront(msg);
  _handleQueue();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onDisconnect()
{
  AMQTT_LOGINFO("_onDisconnect: TCP disconn");

  _state = DISCONNECTED;

  _clear();

  for (auto callback : _onDisconnectUserCallbacks)
    callback(_disconnectReason);
}

/////////////////////////////////////////////////////////

/*
  void AsyncMqttClient::_onError(int8_t error)
  {
  (void)error;
  // _onDisconnect called anyway
  }

  void AsyncMqttClient::_onTimeout()
  {
  // disconnection will be handled by ping/pong management
  }
*/

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onAck(size_t len)
{
  AMQTT_LOGINFO1("_onAck: ack len =", len);

  _handleQueue();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onData(char* data, size_t len)
{
  AMQTT_LOGINFO1("_onData : data rcv len =", len);

  size_t currentBytePosition = 0;
  char currentByte;
  _lastServerActivity = millis();

  do
  {
    switch (_parsingInformation.bufferState)
    {
      case AsyncMqttClientInternals::BufferState::NONE:
        currentByte = data[currentBytePosition++];
        _parsingInformation.packetType = currentByte >> 4;
        _parsingInformation.packetFlags = (currentByte << 4) >> 4;
        _parsingInformation.bufferState = AsyncMqttClientInternals::BufferState::REMAINING_LENGTH;

        switch (_parsingInformation.packetType)
        {
          case AsyncMqttClientInternals::PacketType.CONNACK:
            AMQTT_LOGINFO("_onData: rcv CONNACK");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::ConnAckPacket(&_parsingInformation, 
                                                          std::bind(&AsyncMqttClient::_onConnAck, this, 
                                                          std::placeholders::_1, std::placeholders::_2));
            _client.setRxTimeout(0);
            break;

          case AsyncMqttClientInternals::PacketType.PINGRESP:
            AMQTT_LOGINFO("_onData: rcv PINGRESP");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PingRespPacket(&_parsingInformation, 
                                                           std::bind(&AsyncMqttClient::_onPingResp, this));
            break;

          case AsyncMqttClientInternals::PacketType.SUBACK:
            AMQTT_LOGINFO("_onData: rcv SUBACK");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::SubAckPacket(&_parsingInformation, 
                                                         std::bind(&AsyncMqttClient::_onSubAck, this, 
                                                         std::placeholders::_1, std::placeholders::_2));
            break;

          case AsyncMqttClientInternals::PacketType.UNSUBACK:
            AMQTT_LOGINFO("_onData: rcv UNSUBACK");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::UnsubAckPacket(&_parsingInformation, 
                                                           std::bind(&AsyncMqttClient::_onUnsubAck, this, 
                                                           std::placeholders::_1));
            break;

          case AsyncMqttClientInternals::PacketType.PUBLISH:
            AMQTT_LOGINFO("_onData: rcv PUBLISH");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PublishPacket(&_parsingInformation, 
                                                          std::bind(&AsyncMqttClient::_onMessage, this, 
                                                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                                          std::placeholders::_4, std::placeholders::_5, std::placeholders::_6,
                                                          std::placeholders::_7, std::placeholders::_8, std::placeholders::_9),
                                                          std::bind(&AsyncMqttClient::_onPublish, this, std::placeholders::_1,
                                                          std::placeholders::_2));
            break;

          case AsyncMqttClientInternals::PacketType.PUBREL:
            AMQTT_LOGINFO("_onData: rcv PUBREL");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PubRelPacket(&_parsingInformation, 
                                                         std::bind(&AsyncMqttClient::_onPubRel, this,
                                                         std::placeholders::_1));
            break;

          case AsyncMqttClientInternals::PacketType.PUBACK:
            AMQTT_LOGINFO("_onData: rcv PUBACK");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PubAckPacket(&_parsingInformation, 
                                                         std::bind(&AsyncMqttClient::_onPubAck, this,
                                                         std::placeholders::_1));
            break;

          case AsyncMqttClientInternals::PacketType.PUBREC:
            AMQTT_LOGINFO("_onData: rcv PUBREC");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PubRecPacket(&_parsingInformation, 
                                                         std::bind(&AsyncMqttClient::_onPubRec, this, 
                                                         std::placeholders::_1));
            break;

          case AsyncMqttClientInternals::PacketType.PUBCOMP:
            AMQTT_LOGINFO("_onData: rcv PUBCOMP");

            _currentParsedPacket = 
              new AsyncMqttClientInternals::PubCompPacket(&_parsingInformation, 
                                                          std::bind(&AsyncMqttClient::_onPubComp, this, 
                                                          std::placeholders::_1));
            break;

          default:
            AMQTT_LOGERROR("_onData: rcv PROTOCOL VIOLATION");

            disconnect(true);
            break;
        }

        break;
        
      case AsyncMqttClientInternals::BufferState::REMAINING_LENGTH:
        currentByte = data[currentBytePosition++];
        _remainingLengthBuffer[_remainingLengthBufferPosition++] = currentByte;

        if (currentByte >> 7 == 0)
        {
          _parsingInformation.remainingLength = AsyncMqttClientInternals::Helpers::decodeRemainingLength(_remainingLengthBuffer);
          _remainingLengthBufferPosition = 0;

          if (_parsingInformation.remainingLength > 0)
          {
            _parsingInformation.bufferState = AsyncMqttClientInternals::BufferState::VARIABLE_HEADER;
          }
          else
          {
            // PINGRESP is a special case where it has no variable header, so the packet ends right here
            _parsingInformation.bufferState = AsyncMqttClientInternals::BufferState::NONE;
            _onPingResp();
          }
        }
        
        break;
        
      case AsyncMqttClientInternals::BufferState::VARIABLE_HEADER:
        _currentParsedPacket->parseVariableHeader(data, len, &currentBytePosition);
        break;
        
      case AsyncMqttClientInternals::BufferState::PAYLOAD:
        _currentParsedPacket->parsePayload(data, len, &currentBytePosition);
        break;
        
      default:
        currentBytePosition = len;
        break;
    }
  } while (currentBytePosition != len);
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPoll()
{
  // if there is too much time the client has sent a ping request without a response, disconnect client to avoid half open connections
  if (_lastPingRequestTime != 0 && (millis() - _lastPingRequestTime) >= (_keepAlive * 1000 * 2))
  {
    AMQTT_LOGERROR("_onPoll: PING timeout, disconnecting");

    disconnect(true);

    return;
  }

  // send ping to ensure the server will receive at least one message inside keepalive window
  if (_state == CONNECTED && _lastPingRequestTime == 0 && (millis() - _lastClientActivity) >= (_keepAlive * 1000 * 0.7))
  {
    _sendPing();
    // send ping to verify if the server is still there (ensure this is not a half connection)
  }
  else if (_state == CONNECTED && _lastPingRequestTime == 0 && (millis() - _lastServerActivity) >= (_keepAlive * 1000 * 0.7))
  {
    _sendPing();
  }

  _handleQueue();
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/* QUEUE */

void AsyncMqttClient::_insert(AsyncMqttClientInternals::OutPacket* packet)
{
  // We only use this for QoS2 PUBREL so there must be a PUBLISH packet present.
  // The queue therefore cannot be empty and _head points to this PUBLISH packet.
  SEMAPHORE_TAKE();

  AMQTT_LOGINFO1("_insert: new insert, packetType =", PacketTypeName[packet->packetType()]);

  packet->next = _head->next;
  _head->next = packet;

  if (_head == _tail)
  {
    // PUB packet is the only one in the queue
    _tail = packet;
  }

  SEMAPHORE_GIVE();
  _handleQueue();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_addFront(AsyncMqttClientInternals::OutPacket* packet)
{
  // This is only used for the CONNECT packet, to be able to establish a connection
  // before anything else. The queue can be empty or has packets from the continued session.
  // In both cases, _head should always point to the CONNECT packet afterwards.
  SEMAPHORE_TAKE();

  AMQTT_LOGINFO1("_addFront: new front, packetType =", PacketTypeName[packet->packetType()]);

  if (_head == nullptr)
  {
    _tail = packet;
  }
  else
  {
    packet->next = _head;
  }

  _head = packet;
  SEMAPHORE_GIVE();
  _handleQueue();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_addBack(AsyncMqttClientInternals::OutPacket* packet)
{
  SEMAPHORE_TAKE();

  AMQTT_LOGINFO1("_addBack: new back, packetType =", PacketTypeName[packet->packetType()]);

  if (!_tail)
  {
    _head = packet;
  }
  else
  {
    _tail->next = packet;
  }

  _tail = packet;
  _tail->next = nullptr;
  SEMAPHORE_GIVE();
  _handleQueue();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_handleQueue()
{
  SEMAPHORE_TAKE();
  // On ESP32, onDisconnect is called within the close()-call. So we need to make sure we don't lock
  bool disconnect = false;

  while (_head && _client.space() > 10)
  {
    // safe but arbitrary value, send at least 10 bytes
    // 1. try to send
    if (_head->size() > _sent)
    {
      // On SSL the TCP library returns the total amount of bytes, not just the unencrypted payload length.
      // So we calculate the amount to be written ourselves.
      size_t willSend = std::min(_head->size() - _sent, _client.space());
      
      // flag is set by LWIP anyway, added for clarity
      size_t realSent = _client.add(reinterpret_cast<const char*>(_head->data(_sent)), willSend, ASYNC_WRITE_FLAG_COPY);
      
      _sent += willSend;
      (void)realSent;
      _client.send();
      _lastClientActivity = millis();
      _lastPingRequestTime = 0;

#if ASYNC_TCP_SSL_ENABLED
      AMQTT_LOGINFO3("_handleQueue: snd, packetType #", PacketTypeName[_head->packetType()], ", tls: realSent =", realSent);
      AMQTT_LOGINFO3("_handleQueue: sent / _headsize =", _sent, "/", _head->size());
#else
      AMQTT_LOGINFO1("_handleQueue: snd, packetType #", PacketTypeName[_head->packetType()]);
      AMQTT_LOGINFO3("_handleQueue: sent / _headsize =", _sent, "/", _head->size());
#endif

      if (_head->packetType() == AsyncMqttClientInternals::PacketType.DISCONNECT)
      {
        disconnect = true;
      }
    }

    // 2. stop processing when we have to wait for an MQTT acknowledgment
    if (_head->size() == _sent)
    {
      if (_head->released())
      {
        AMQTT_LOGINFO1("_handleQueue: released packetType #", PacketTypeName[_head->packetType()]);

        AsyncMqttClientInternals::OutPacket* tmp = _head;
        _head = _head->next;

        if (!_head)
          _tail = nullptr;

        delete tmp;
        _sent = 0;
      }
      else
      {
        break;  // sending is complete however send next only after mqtt confirmation
      }
    }
  }

  SEMAPHORE_GIVE();

  if (disconnect)
  {
    AMQTT_LOGINFO("_handleQueue: snd DISCONN, disconnecting");

    _client.close();
  }
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_clearQueue(bool keepSessionData)
{
  SEMAPHORE_TAKE();
  AsyncMqttClientInternals::OutPacket* packet = _head;
  _head = nullptr;
  _tail = nullptr;

  while (packet)
  {
    /* MQTT spec 3.1.2.4 Clean Session:
        - QoS 1 and QoS 2 messages which have been sent to the Server, but have not been completely acknowledged.
        - QoS 2 messages which have been received from the Server, but have not been completely acknowledged.
       + (unsent PUB messages with QoS > 0)

       To be kept:
       - possibly first message (sent to server but not acked)
       - PUBREC messages (QoS 2 PUB received but not acked)
       - PUBCOMP messages (QoS 2 PUBREL received but not acked)
    */
    if (keepSessionData)
    {
      if (packet->qos() > 0 && packet->size() <= _sent)
      {
        // check for qos includes check for PUB-packet type
        reinterpret_cast<AsyncMqttClientInternals::PublishOutPacket*>(packet)->setDup();
        AsyncMqttClientInternals::OutPacket* next = packet->next;

        AMQTT_LOGINFO1("_clearQueue: keep, packetType =", PacketTypeName[packet->packetType()]);

        SEMAPHORE_GIVE();
        _addBack(packet);
        SEMAPHORE_TAKE();
        packet = next;
      }
      else if (packet->qos() > 0 ||
               packet->packetType() == AsyncMqttClientInternals::PacketType.PUBREC ||
               packet->packetType() == AsyncMqttClientInternals::PacketType.PUBCOMP)
      {
        AsyncMqttClientInternals::OutPacket* next = packet->next;

        AMQTT_LOGINFO1("_clearQueue: keep, packetType =", PacketTypeName[packet->packetType()]);

        SEMAPHORE_GIVE();
        _addBack(packet);
        SEMAPHORE_TAKE();
        packet = next;
      }
      else
      {
        AsyncMqttClientInternals::OutPacket* next = packet->next;
        delete packet;
        packet = next;
      }
      /* Delete everything when not keeping session data
      */
    }
    else
    {
      AsyncMqttClientInternals::OutPacket* next = packet->next;
      delete packet;
      packet = next;
    }
  }

  _sent = 0;
  SEMAPHORE_GIVE();
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/* MQTT */
void AsyncMqttClient::_onPingResp()
{
  AMQTT_LOGINFO("PINGRESP");

  _freeCurrentParsedPacket();
  _lastPingRequestTime = 0;
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onConnAck(bool sessionPresent, uint8_t connectReturnCode)
{
  AMQTT_LOGINFO("CONNACK");

  _freeCurrentParsedPacket();

  if (!sessionPresent)
  {
    _pendingPubRels.clear();
    _pendingPubRels.shrink_to_fit();
    _clearQueue(false);  // remove session data
  }

  if (connectReturnCode == 0)
  {
    _state = CONNECTED;

    for (auto callback : _onConnectUserCallbacks)
      callback(sessionPresent);
  }
  else
  {
    // Callbacks are handled by the onDisconnect function which is called from the AsyncTcp lib
    _disconnectReason = static_cast<AsyncMqttClientDisconnectReason>(connectReturnCode);
    return;
  }

  _handleQueue();  // send any remaining data from continued session
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onSubAck(uint16_t packetId, char status)
{
  AMQTT_LOGINFO("SUBACK");

  _freeCurrentParsedPacket();
  SEMAPHORE_TAKE();

  if (_head && _head->packetId() == packetId)
  {
    _head->release();

    AMQTT_LOGINFO("SUB released");
  }

  SEMAPHORE_GIVE();

  for (auto callback : _onSubscribeUserCallbacks)
    callback(packetId, status);

  _handleQueue();  // subscribe confirmed, ready to send next queued item
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onUnsubAck(uint16_t packetId)
{
  AMQTT_LOGINFO("UNSUBACK");

  _freeCurrentParsedPacket();
  SEMAPHORE_TAKE();

  if (_head && _head->packetId() == packetId)
  {
    _head->release();

    AMQTT_LOGINFO("UNSUB released");
  }

  SEMAPHORE_GIVE();

  for (auto callback : _onUnsubscribeUserCallbacks)
    callback(packetId);

  _handleQueue();  // unsubscribe confirmed, ready to send next queued item
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onMessage(char* topic, char* payload, uint8_t qos, bool dup, bool retain, size_t len, size_t index, size_t total, uint16_t packetId)
{
  bool notifyPublish = true;

  if (qos == 2)
  {
    for (AsyncMqttClientInternals::PendingPubRel pendingPubRel : _pendingPubRels)
    {
      if (pendingPubRel.packetId == packetId)
      {
        notifyPublish = false;
        break;
      }
    }
  }

  if (notifyPublish)
  {
    AsyncMqttClientMessageProperties properties;
    properties.qos = qos;
    properties.dup = dup;
    properties.retain = retain;

    for (auto callback : _onMessageUserCallbacks)
      callback(topic, payload, properties, len, index, total);
  }
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPublish(uint16_t packetId, uint8_t qos)
{
  AsyncMqttClientInternals::PendingAck pendingAck;

  if (qos == 1)
  {
    pendingAck.packetType = AsyncMqttClientInternals::PacketType.PUBACK;
    pendingAck.headerFlag = AsyncMqttClientInternals::HeaderFlag.PUBACK_RESERVED;
    pendingAck.packetId = packetId;
    AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PubAckOutPacket(pendingAck);
    _addBack(msg);
  }
  else if (qos == 2)
  {
    pendingAck.packetType = AsyncMqttClientInternals::PacketType.PUBREC;
    pendingAck.headerFlag = AsyncMqttClientInternals::HeaderFlag.PUBREC_RESERVED;
    pendingAck.packetId = packetId;
    AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PubAckOutPacket(pendingAck);
    _addBack(msg);

    bool pubRelAwaiting = false;

    for (AsyncMqttClientInternals::PendingPubRel pendingPubRel : _pendingPubRels)
    {
      if (pendingPubRel.packetId == packetId)
      {
        pubRelAwaiting = true;
        break;
      }
    }

    if (!pubRelAwaiting)
    {
      AsyncMqttClientInternals::PendingPubRel pendingPubRel;
      pendingPubRel.packetId = packetId;
      _pendingPubRels.push_back(pendingPubRel);
    }
  }

  _freeCurrentParsedPacket();
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPubRel(uint16_t packetId)
{
  _freeCurrentParsedPacket();

  AsyncMqttClientInternals::PendingAck pendingAck;
  pendingAck.packetType = AsyncMqttClientInternals::PacketType.PUBCOMP;
  pendingAck.headerFlag = AsyncMqttClientInternals::HeaderFlag.PUBCOMP_RESERVED;
  pendingAck.packetId = packetId;

  if (_head && _head->packetId() == packetId)
  {
    AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PubAckOutPacket(pendingAck);
    _head->release();
    _insert(msg);

    AMQTT_LOGINFO("PUBREC released");
  }

  for (size_t i = 0; i < _pendingPubRels.size(); i++)
  {
    if (_pendingPubRels[i].packetId == packetId)
    {
      _pendingPubRels.erase(_pendingPubRels.begin() + i);
      _pendingPubRels.shrink_to_fit();
    }
  }
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPubAck(uint16_t packetId)
{
  _freeCurrentParsedPacket();

  if (_head && _head->packetId() == packetId)
  {
    _head->release();

    AMQTT_LOGINFO("PUB released");
  }

  for (auto callback : _onPublishUserCallbacks)
    callback(packetId);
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPubRec(uint16_t packetId) {

  _freeCurrentParsedPacket();

  // We will only be sending 1 QoS>0 PUB message at a time (to honor message
  // ordering). So no need to store ACKS in a separate container as it will
  // be stored in the outgoing queue until a PUBCOMP comes in.
  AsyncMqttClientInternals::PendingAck pendingAck;
  pendingAck.packetType = AsyncMqttClientInternals::PacketType.PUBREL;
  pendingAck.headerFlag = AsyncMqttClientInternals::HeaderFlag.PUBREL_RESERVED;
  pendingAck.packetId = packetId;

  AMQTT_LOGINFO("snd PUBREL");

  AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PubAckOutPacket(pendingAck);

  if (_head && _head->packetId() == packetId)
  {
    _head->release();

    AMQTT_LOGINFO("PUB released");
  }

  _insert(msg);
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_onPubComp(uint16_t packetId)
{
  _freeCurrentParsedPacket();

  // _head points to the PUBREL package
  if (_head && _head->packetId() == packetId)
  {
    _head->release();

    AMQTT_LOGINFO("PUBREL released");
  }

  for (auto callback : _onPublishUserCallbacks)
    callback(packetId);
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::_sendPing()
{
  AMQTT_LOGINFO("PING");

  _lastPingRequestTime = millis();
  AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PingReqOutPacket;
  _addBack(msg);
}

/////////////////////////////////////////////////////////

bool AsyncMqttClient::connected() const
{
  return _state == CONNECTED;
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::connect()
{
  if (_state != DISCONNECTED)
    return;

#if (ASYNC_MQTT_USING_PORTENTA_H7)

  #if (USE_ETHERNET_PORTENTA_H7)
  
  // 6 HEX bytes
  uint8_t macPortenta[6];
  
	Ethernet.MACAddress(macPortenta);
	
	snprintf(_generatedClientId, sizeof(_generatedClientId), "h7m7-%02X%02X%02X%02X%02X%02X", 
           macPortenta[0], macPortenta[1], macPortenta[2], macPortenta[3], macPortenta[4], macPortenta[5]);
  
  _clientId = _generatedClientId;
  
  #else
   
  // For WiFi. TODO Get Portenta unique hardwareID to use for both Ethernet and WiFi
  
  snprintf(_generatedClientId, sizeof(_generatedClientId), "h7m7-%06lx", micros());

  _clientId = _generatedClientId;
  
  #endif
  
#elif ASYNC_MQTT_USING_STM32

	uint8_t* macSTM32ptr;
	
	macSTM32ptr = Ethernet.MACAddress();
	
	snprintf(_generatedClientId, sizeof(_generatedClientId), "stm32-%02X%02X%02X%02X%02X%02X", 
           macSTM32ptr[0], macSTM32ptr[1], macSTM32ptr[2], macSTM32ptr[3], macSTM32ptr[4], macSTM32ptr[5]);
  Serial.printf("Array MAC Address: %02X:%02X:%02X:%02X:%02X:%02X \n", 
                macSTM32ptr[0], macSTM32ptr[1], macSTM32ptr[2], macSTM32ptr[3], macSTM32ptr[4], macSTM32ptr[5]);
		
  _clientId = _generatedClientId;
  
#elif ASYNC_MQTT_USING_TEENSY41_QNETHERNET
  uint8_t macTeensy[6];
  
  getTeensyMac(macTeensy);   
  snprintf(_generatedClientId, sizeof(_generatedClientId), "t41-%02X%02X%02X%02X%02X%02X", 
           macTeensy[0], macTeensy[1], macTeensy[2], macTeensy[3], macTeensy[4], macTeensy[5]);
  
  _clientId = _generatedClientId;

#elif ASYNC_MQTT_USING_RP2040W
  // For WiFi. Use macAddress for WiFi  
  // 6 HEX bytes
  uint8_t macRP2040W[6];
  
	WiFi.macAddress(macRP2040W);
	
	snprintf(_generatedClientId, sizeof(_generatedClientId), "2040w-%02X%02X%02X%02X%02X%02X", 
           macRP2040W[0], macRP2040W[1], macRP2040W[2], macRP2040W[3], macRP2040W[4], macRP2040W[5]);
  
  _clientId = _generatedClientId;
    
#endif

  AMQTT_LOGINFO("CONNECTING");

  AMQTT_LOGINFO1("ClientID =", _clientId);

  _state = CONNECTING;
  _disconnectReason = AsyncMqttClientDisconnectReason::TCP_DISCONNECTED;  // reset any previous

  _client.setRxTimeout(_keepAlive);

#if ASYNC_TCP_SSL_ENABLED
  if (_useIp)
  {
    _client.connect(_ip, _port, _secure);
  }
  else
  {
    _client.connect(_host, _port, _secure);
  }
#else
  if (_useIp)
  {
    _client.connect(_ip, _port);
  }
  else
  {
    _client.connect(_host, _port);
  }
#endif
}

/////////////////////////////////////////////////////////

void AsyncMqttClient::disconnect(bool force)
{
  if (_state == DISCONNECTED)
    return;

  AMQTT_LOGINFO1("disconnect: DISCONNECT force =", force);

  if (force)
  {
    _state = DISCONNECTED;
    _client.close(true);
  }
  else if (_state != DISCONNECTING)
  {
    _state = DISCONNECTING;
    AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::DisconnOutPacket;
    _addBack(msg);
  }
}

/////////////////////////////////////////////////////////

uint16_t AsyncMqttClient::subscribe(const char* topic, uint8_t qos)
{
  if (_state != CONNECTED)
    return 0;

  AMQTT_LOGINFO("SUBSCRIBE");

  AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::SubscribeOutPacket(topic, qos);
  _addBack(msg);
  return msg->packetId();
}

/////////////////////////////////////////////////////////

uint16_t AsyncMqttClient::unsubscribe(const char* topic)
{
  if (_state != CONNECTED)
    return 0;

  AMQTT_LOGINFO("UNSUBSCRIBE");

  AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::UnsubscribeOutPacket(topic);
  _addBack(msg);
  return msg->packetId();
}

/////////////////////////////////////////////////////////

uint16_t AsyncMqttClient::publish(const char* topic, uint8_t qos, bool retain, const char* payload, size_t length, bool dup, uint16_t message_id)
{
  (void) dup;
  (void) message_id;
  
  if (_state != CONNECTED || GET_FREE_MEMORY() < MQTT_MIN_FREE_MEMORY)
    return 0;

  AMQTT_LOGINFO("PUBLISH");

  AsyncMqttClientInternals::OutPacket* msg = new AsyncMqttClientInternals::PublishOutPacket(topic, qos, retain, payload, length);
  _addBack(msg);

  return msg->packetId();
}

/////////////////////////////////////////////////////////

bool AsyncMqttClient::clearQueue()
{
  if (_state != DISCONNECTED)
    return false;

  _clearQueue(false);

  return true;
}

/////////////////////////////////////////////////////////

const char* AsyncMqttClient::getClientId() const
{
  return _clientId;
}

/////////////////////////////////////////////////////////

#if ASYNC_MQTT_USING_TEENSY41_QNETHERNET
uint8_t* AsyncMqttClient::getTeensyMac(uint8_t* _macAddress)
{
  // 6 bytes
  uint32_t m1 = HW_OCOTP_MAC1;
  uint32_t m2 = HW_OCOTP_MAC0;
  
  _macAddress[0] = m1 >> 8;
  _macAddress[1] = m1 >> 0;
  _macAddress[2] = m2 >> 24;
  _macAddress[3] = m2 >> 16;
  _macAddress[4] = m2 >> 8;
  _macAddress[5] = m2 >> 0;
  
  return _macAddress;
}
#endif

/////////////////////////////////////////////////////////

#endif    // ASYNC_MQTT_CLIENT_IMPL_H
