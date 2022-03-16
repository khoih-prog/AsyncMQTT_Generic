/****************************************************************************************************************************
  Connect.cpp
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 
  Version: 1.3.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     10/03/2022 Initial coding to support only ESP32 (with SSL) and ESP8266 (without SSL)
  1.0.1    K Hoang     10/03/2022 Fix Library Manager warnings
  1.1.0    K Hoang     11/03/2022 Add support to WT32_ETH01 (with or without TLS/SSL)
  1.2.0    K Hoang     15/03/2022 Add support to STM32 using LAN8742A (without TLS/SSL)
  1.2.1    K Hoang     16/03/2022 Add support to STM32 using LAN8720 (without TLS/SSL)
  1.3.0    K Hoang     16/03/2022 Add support to Portenta_H7 using built-in Ethernet or Murata WiFi (without TLS/SSL)
 *****************************************************************************************************************************/

#include "Connect.hpp"

using AsyncMqttClientInternals::ConnectOutPacket;

ConnectOutPacket::ConnectOutPacket(bool cleanSession,
                                   const char* username,
                                   const char* password,
                                   const char* willTopic,
                                   bool willRetain,
                                   uint8_t willQos,
                                   const char* willPayload,
                                   uint16_t willPayloadLength,
                                   uint16_t keepAlive,
                                   const char* clientId)
{
  char fixedHeader[5];
  fixedHeader[0] = AsyncMqttClientInternals::PacketType.CONNECT;
  fixedHeader[0] = fixedHeader[0] << 4;
  fixedHeader[0] = fixedHeader[0] | AsyncMqttClientInternals::HeaderFlag.CONNECT_RESERVED;

  uint16_t protocolNameLength = 4;
  char protocolNameLengthBytes[2];
  protocolNameLengthBytes[0] = protocolNameLength >> 8;
  protocolNameLengthBytes[1] = protocolNameLength & 0xFF;

  char protocolLevel[1];
  protocolLevel[0] = 0x04;

  char connectFlags[1];
  connectFlags[0] = 0;

  if (cleanSession)
    connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.CLEAN_SESSION;

  if (username != nullptr)
    connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.USERNAME;

  if (password != nullptr)
    connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.PASSWORD;

  if (willTopic != nullptr)
  {
    connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.WILL;

    if (willRetain)
      connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.WILL_RETAIN;

    switch (willQos)
    {
      case 0:
        connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.WILL_QOS0;
        break;

      case 1:
        connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.WILL_QOS1;
        break;

      case 2:
        connectFlags[0] |= AsyncMqttClientInternals::ConnectFlag.WILL_QOS2;
        break;
    }
  }

  char keepAliveBytes[2];
  keepAliveBytes[0] = keepAlive >> 8;
  keepAliveBytes[1] = keepAlive & 0xFF;

  uint16_t clientIdLength = strlen(clientId);
  char clientIdLengthBytes[2];
  clientIdLengthBytes[0] = clientIdLength >> 8;
  clientIdLengthBytes[1] = clientIdLength & 0xFF;

  // Optional fields
  uint16_t willTopicLength = 0;
  char willTopicLengthBytes[2];
  char willPayloadLengthBytes[2];

  if (willTopic != nullptr)
  {
    willTopicLength = strlen(willTopic);
    willTopicLengthBytes[0] = willTopicLength >> 8;
    willTopicLengthBytes[1] = willTopicLength & 0xFF;

    if (willPayload != nullptr && willPayloadLength == 0)
      willPayloadLength = strlen(willPayload);

    willPayloadLengthBytes[0] = willPayloadLength >> 8;
    willPayloadLengthBytes[1] = willPayloadLength & 0xFF;
  }

  uint16_t usernameLength = 0;
  char usernameLengthBytes[2];

  if (username != nullptr)
  {
    usernameLength = strlen(username);
    usernameLengthBytes[0] = usernameLength >> 8;
    usernameLengthBytes[1] = usernameLength & 0xFF;
  }

  uint16_t passwordLength = 0;
  char passwordLengthBytes[2];

  if (password != nullptr)
  {
    passwordLength = strlen(password);
    passwordLengthBytes[0] = passwordLength >> 8;
    passwordLengthBytes[1] = passwordLength & 0xFF;
  }

  uint32_t remainingLength = 2 + protocolNameLength + 1 + 1 + 2 + 2 + clientIdLength;  // always present

  if (willTopic != nullptr)
    remainingLength += 2 + willTopicLength + 2 + willPayloadLength;

  if (username != nullptr)
    remainingLength += 2 + usernameLength;

  if (password != nullptr)
    remainingLength += 2 + passwordLength;

  uint8_t remainingLengthLength = AsyncMqttClientInternals::Helpers::encodeRemainingLength(remainingLength, fixedHeader + 1);

  uint32_t neededSpace = 1 + remainingLengthLength;
  neededSpace += 2;
  neededSpace += protocolNameLength;
  neededSpace += 1;
  neededSpace += 1;
  neededSpace += 2;
  neededSpace += 2;
  neededSpace += clientIdLength;

  if (willTopic != nullptr)
  {
    neededSpace += 2;
    neededSpace += willTopicLength;

    neededSpace += 2;

    if (willPayload != nullptr)
      neededSpace += willPayloadLength;
  }

  if (username != nullptr)
  {
    neededSpace += 2;
    neededSpace += usernameLength;
  }

  if (password != nullptr)
  {
    neededSpace += 2;
    neededSpace += passwordLength;
  }

  _data.reserve(neededSpace);

  _data.insert(_data.end(), fixedHeader, fixedHeader + 1 + remainingLengthLength);

  _data.push_back(protocolNameLengthBytes[0]);
  _data.push_back(protocolNameLengthBytes[1]);

  _data.push_back('M');
  _data.push_back('Q');
  _data.push_back('T');
  _data.push_back('T');

  _data.push_back(protocolLevel[0]);
  _data.push_back(connectFlags[0]);
  _data.push_back(keepAliveBytes[0]);
  _data.push_back(keepAliveBytes[1]);
  _data.push_back(clientIdLengthBytes[0]);
  _data.push_back(clientIdLengthBytes[1]);

  _data.insert(_data.end(), clientId, clientId + clientIdLength);

  if (willTopic != nullptr)
  {
    _data.insert(_data.end(), willTopicLengthBytes, willTopicLengthBytes + 2);
    _data.insert(_data.end(), willTopic, willTopic + willTopicLength);

    _data.insert(_data.end(), willPayloadLengthBytes, willPayloadLengthBytes + 2);
    if (willPayload != nullptr) _data.insert(_data.end(), willPayload, willPayload + willPayloadLength);
  }

  if (username != nullptr)
  {
    _data.insert(_data.end(), usernameLengthBytes, usernameLengthBytes + 2);
    _data.insert(_data.end(), username, username + usernameLength);
  }

  if (password != nullptr)
  {
    _data.insert(_data.end(), passwordLengthBytes, passwordLengthBytes + 2);
    _data.insert(_data.end(), password, password + passwordLength);
  }
}

const uint8_t* ConnectOutPacket::data(size_t index) const
{
  return &_data.data()[index];
}

size_t ConnectOutPacket::size() const
{
  return _data.size();
}
