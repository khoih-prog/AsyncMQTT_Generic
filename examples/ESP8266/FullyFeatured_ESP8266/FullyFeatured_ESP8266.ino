/****************************************************************************************************************************
  FullyFeature_ESP8266.ino
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

// Use ESP8266 core v2.7.4- for SSL as new cores don't use axtls anymore
// Use core v3.0.2+ for LwIP Ethernet W5500lwIP, W5100lwIP and ENC28J60lwIP libraries 
// Must use KH forked ESPAsyncTCP library or compile error

#include "defines.h"
 
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqtt_Generic.h>

//#define MQTT_HOST         IPAddress(192, 168, 2, 110)
#define MQTT_HOST         "broker.emqx.io"        // Broker address
#define MQTT_PORT         1883

const char *PubTopic  = "async-mqtt/ESP8266_Pub";               // Topic to publish

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

Ticker wifiReconnectTimer;

void connectToWifi() 
{
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() 
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) 
{
  (void) event;
  
  Serial.print("Connected to Wi-Fi. IP address: "); Serial.println(WiFi.localIP());
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) 
{
  (void) event;
  
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void printSeparationLine()
{
  Serial.println("************************************************");
}

void onMqttConnect(bool sessionPresent) 
{
  Serial.print("Connected to MQTT broker: "); Serial.print(MQTT_HOST);
  Serial.print(", port: "); Serial.println(MQTT_PORT);
  Serial.print("PubTopic: "); Serial.println(PubTopic);

  printSeparationLine();
  Serial.print("Session present: "); Serial.println(sessionPresent);
  
  uint16_t packetIdSub = mqttClient.subscribe(PubTopic, 2);
  Serial.print("Subscribing at QoS 2, packetId: "); Serial.println(packetIdSub);
  
  mqttClient.publish(PubTopic, 0, true, "ESP8266 Test1");
  Serial.println("Publishing at QoS 0");
  
  uint16_t packetIdPub1 = mqttClient.publish(PubTopic, 1, true, "ESP8266 Test2");
  Serial.print("Publishing at QoS 1, packetId: "); Serial.println(packetIdPub1);
  
  uint16_t packetIdPub2 = mqttClient.publish(PubTopic, 2, true, "ESP8266 Test3");
  Serial.print("Publishing at QoS 2, packetId: "); Serial.println(packetIdPub2);

  printSeparationLine();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) 
{
  (void) reason;
  
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) 
  {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(const uint16_t& packetId, const uint8_t& qos)
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: "); Serial.println(packetId);
  Serial.print("  qos: ");      Serial.println(qos);
}

void onMqttUnsubscribe(const uint16_t& packetId)
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: "); Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, const AsyncMqttClientMessageProperties& properties, 
                   const size_t& len, const size_t& index, const size_t& total)
{
  (void) payload;
  
  Serial.println("Publish received.");
  Serial.print("  topic: ");  Serial.println(topic);
  Serial.print("  qos: ");    Serial.println(properties.qos);
  Serial.print("  dup: ");    Serial.println(properties.dup);
  Serial.print("  retain: "); Serial.println(properties.retain);
  Serial.print("  len: ");    Serial.println(len);
  Serial.print("  index: ");  Serial.println(index);
  Serial.print("  total: ");  Serial.println(total);
}

void onMqttPublish(const uint16_t& packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: "); Serial.println(packetId);
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  delay(300);
  
  Serial.print("\nStarting FullyFeature_ESP8266 on "); Serial.println(ARDUINO_BOARD);
  Serial.println(ASYNC_MQTT_GENERIC_VERSION);

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() 
{
}
