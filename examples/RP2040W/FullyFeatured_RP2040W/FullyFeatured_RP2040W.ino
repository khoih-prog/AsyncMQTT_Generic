/****************************************************************************************************************************
  FullyFeatured_RP2040W.ino
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, Teensy41_QNEthernet, STM32F7, etc. 
  with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/

#include "defines.h"

#include <WiFi.h>
#include <Ticker.h>

// Check connection every 1s
#define MQTT_CHECK_INTERVAL_MS     1000 

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include <AsyncMqtt_Generic.h>

#include <Ticker.h>                   // https://github.com/sstaub/Ticker

//#define MQTT_HOST         IPAddress(192, 168, 2, 110)
#define MQTT_HOST         "broker.emqx.io"        // Broker address
#define MQTT_PORT         1883

const char *PubTopic  = "async-mqtt/RP2040W_Pub";               // Topic to publish

AsyncMqttClient mqttClient;

int status = WL_IDLE_STATUS;

void connectToMqtt();
void connectToMqttLoop();

// Repeat forever, millis() resolution
Ticker connectToMqttTicker(connectToMqttLoop, MQTT_CHECK_INTERVAL_MS, 0, MILLIS); 

bool connectedWiFi  = false;
bool connectedMQTT  = false;

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

bool connectToWifi()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print(F("Connecting to SSID: ")); Serial.println(WIFI_SSID);

#define MAX_NUM_WIFI_CONNECT_TRIES_PER_LOOP       20 

  uint8_t numWiFiConnectTries = 0;
  
  // attempt to connect to WiFi network
  while ( (status != WL_CONNECTED) && (numWiFiConnectTries++ < MAX_NUM_WIFI_CONNECT_TRIES_PER_LOOP) )
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    delay(500);
  }

  if (status != WL_CONNECTED)
  {
    // Restart for Portenta as something is very wrong
    Serial.println("Resetting. Can't connect to any WiFi");
    
    NVIC_SystemReset();
  }

  printWifiStatus();

  connectedWiFi = (status == WL_CONNECTED);

  return (status == WL_CONNECTED);
}

bool isWiFiConnected()
{
  // You can change longer or shorter depending on your network response
  // Shorter => more responsive, but more ping traffic
  static uint8_t theTTL = 10;

  // Use ping() to test TCP connections
  if (WiFi.ping(WiFi.gatewayIP(), theTTL) == theTTL)
  {
    return true;
  }

  return false;
}

void connectToMqttLoop()
{
  //if ( (WiFi.status() == WL_CONNECTED) && (WiFi.RSSI() != 0) )      // temporary workaround
  if (isWiFiConnected())
  {     
    if (!connectedMQTT)
    {
      mqttClient.connect();
    }

    if (!connectedWiFi)
    {
      Serial.println("WiFi reconnected");
      connectedWiFi = true;
    }
  }
  else
  {          
    if (connectedWiFi)
    {
      Serial.println("WiFi disconnected. Reconnecting");
      connectedWiFi = false;

      connectToWifi();
    }
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
  
  mqttClient.publish(PubTopic, 0, true, "RP2040W Test1");
  Serial.println("Publishing at QoS 0");
  
  uint16_t packetIdPub1 = mqttClient.publish(PubTopic, 1, true, "RP2040W Test2");
  Serial.print("Publishing at QoS 1, packetId: "); Serial.println(packetIdPub1);
  
  uint16_t packetIdPub2 = mqttClient.publish(PubTopic, 2, true, "RP2040W Test3");
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

  Serial.print("\nStarting FullyFeature_RP2040W on "); Serial.println(BOARD_NAME);
  Serial.println(ASYNC_MQTT_GENERIC_VERSION);

  ///////////////////////////////////

  connectToWifi();

  ///////////////////////////////////

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToMqttTicker.start(); //start the ticker.

  connectToMqtt();
}

void loop()
{
  connectToMqttTicker.update(); //update the ticker.
}
