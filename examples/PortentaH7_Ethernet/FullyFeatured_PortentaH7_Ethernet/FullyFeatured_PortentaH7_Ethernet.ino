/****************************************************************************************************************************
  FullyFeatured_PortentaH7_Ethernet.ino
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#include "defines.h"

#include <AsyncMqtt_Generic.h>

using namespace rtos;
Thread connectThread;

//#define MQTT_HOST         IPAddress(192, 168, 2, 110)
#define MQTT_HOST         "broker.emqx.io"        // Broker address
#define MQTT_PORT         1883

const char *PubTopic  = "async-mqtt/Portenta_H7_Ethernet_Pub";               // Topic to publish

AsyncMqttClient mqttClient;

bool connectedEthernet  = false;
bool connectedMQTT      = false;

// Check connection every 1s
#define MQTT_CHECK_INTERVAL_MS     1000

void connectToMqttLoop()
{
  while (true)
  {
    if (Ethernet.linkStatus() == LinkON)
    { 
      if (!connectedMQTT)
      {
        mqttClient.connect();
      }
  
      if (!connectedEthernet)
      {
        Serial.println("Ethernet reconnected.");
        connectedEthernet = true;
      }
    }
    else
    {
      if (connectedEthernet)
      {
        Serial.println("Ethernet disconnected");      
        connectedEthernet = false;
      }
    }
     
    delay(MQTT_CHECK_INTERVAL_MS);
  }
}

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
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

  connectedMQTT = true;
  
  printSeparationLine();
  Serial.print("Session present: "); Serial.println(sessionPresent);
  
  uint16_t packetIdSub = mqttClient.subscribe(PubTopic, 2);
  Serial.print("Subscribing at QoS 2, packetId: "); Serial.println(packetIdSub);
  
  mqttClient.publish(PubTopic, 0, true, "Portenta_H7_Ethernet Test1");
  Serial.println("Publishing at QoS 0");
  
  uint16_t packetIdPub1 = mqttClient.publish(PubTopic, 1, true, "Portenta_H7_Ethernet Test2");
  Serial.print("Publishing at QoS 1, packetId: "); Serial.println(packetIdPub1);
  
  uint16_t packetIdPub2 = mqttClient.publish(PubTopic, 2, true, "Portenta_H7_Ethernet Test3");
  Serial.print("Publishing at QoS 2, packetId: "); Serial.println(packetIdPub2);

  printSeparationLine();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  (void) reason;

  connectedMQTT = false;
  
  Serial.println("Disconnected from MQTT.");
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
  char message[len + 1];

  memcpy(message, payload, len);
  message[len] = 0;
  
  Serial.println("Publish received.");
  Serial.print("  topic: ");    Serial.println(topic);
  Serial.print("  message: ");  Serial.println(message);
  Serial.print("  qos: ");      Serial.println(properties.qos);
  Serial.print("  dup: ");      Serial.println(properties.dup);
  Serial.print("  retain: ");   Serial.println(properties.retain);
  Serial.print("  len: ");      Serial.println(len);
  Serial.print("  index: ");    Serial.println(index);
  Serial.print("  total: ");    Serial.println(total);
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

  Serial.print("\nStarting FullyFeatured_PortentaH7_Ethernet on "); Serial.println(BOARD_NAME);
  Serial.println(ASYNC_MQTT_GENERIC_VERSION);

  // start the ethernet connection and the server

  // Use Static IP
  //Ethernet.begin(mac[index], ip);
  // Use DHCP dynamic IP and built-in mac
  Ethernet.begin();
  // Use DHCP dynamic IP and random mac
  //uint16_t index = micros() % NUMBER_OF_MAC;
  //Ethernet.begin(mac[index]);

  Serial.print("Connected to network. IP = ");
  Serial.println(Ethernet.localIP());
  
  connectedEthernet = true;

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  // Add "connectToMqttLoop" loop to control connection To Mqtt
  connectThread.start(connectToMqttLoop);

  connectToMqtt();
}

void loop()
{
}
