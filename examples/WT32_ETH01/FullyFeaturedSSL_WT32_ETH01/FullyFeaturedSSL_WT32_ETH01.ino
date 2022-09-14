/****************************************************************************************************************************
  FullyFeatureSSL_WT32_ETH01.ino
  
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support
  
  Based on and modified from :
  
  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)
  
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 *****************************************************************************************************************************/
 
#include "defines.h"

extern "C"
{
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}

#define ASYNC_TCP_SSL_ENABLED       true
//#define ASYNC_TCP_SSL_ENABLED       false

#include <AsyncMqtt_Generic.h>

//#define MQTT_HOST         IPAddress(192, 168, 2, 110)
#define MQTT_HOST         "broker.emqx.io"        // Broker address

#if ASYNC_TCP_SSL_ENABLED

  #define MQTT_SECURE     true
  
  const uint8_t MQTT_SERVER_FINGERPRINT[] = {0x7e, 0x36, 0x22, 0x01, 0xf9, 0x7e, 0x99, 0x2f, 0xc5, 0xdb, 0x3d, 0xbe, 0xac, 0x48, 0x67, 0x5b, 0x5d, 0x47, 0x94, 0xd2};
  const char *PubTopic  = "async-mqtt/WT32_ETH01_SSL_Pub";               // Topic to publish
  
  #define MQTT_PORT       8883
  
#else

  const char *PubTopic  = "async-mqtt/WT32_ETH01_Pub";                   // Topic to publish
  
  #define MQTT_PORT       1883
  
#endif

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void ETH_event(WiFiEvent_t event)
{
  switch (event)
  {
#if USING_CORE_ESP32_CORE_V200_PLUS
    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH starting");
      break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.println("ETH got IP");
      Serial.print("IP address: "); Serial.println(ETH.localIP());
      connectToMqtt();
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH lost connection");

      // ensure we don't reconnect to MQTT when no ETH
      xTimerStop(mqttReconnectTimer, 0);

      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH stops");

      // ensure we don't reconnect to MQTT when no ETH
      xTimerStop(mqttReconnectTimer, 0);

      break;
#else
    case SYSTEM_EVENT_ETH_CONNECTED:
      erial.println(F("ETH Connected"));
      break;

    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println("ETH connected");
      Serial.println("IP address: "); Serial.println(ETH.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH lost connection");

      // ensure we don't reconnect to MQTT when no ETH
      xTimerStop(mqttReconnectTimer, 0);

      break;
#endif
    default:
      break;
  }
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
  
  mqttClient.publish(PubTopic, 0, true, "WT32_ETH01 Test");
  Serial.println("Publishing at QoS 0");
  
  uint16_t packetIdPub1 = mqttClient.publish(PubTopic, 1, true, "test 2");
  Serial.print("Publishing at QoS 1, packetId: "); Serial.println(packetIdPub1);
  
  uint16_t packetIdPub2 = mqttClient.publish(PubTopic, 2, true, "test 3");
  Serial.print("Publishing at QoS 2, packetId: "); Serial.println(packetIdPub2);

  printSeparationLine();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  (void) reason;
  
  Serial.println("Disconnected from MQTT.");

  if (WT32_ETH01_isConnected())
  {
    xTimerStart(mqttReconnectTimer, 0);
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
  Serial.println("Publish acknowledged");
  Serial.print("  packetId: "); Serial.println(packetId);
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print("\nStarting FullyFeatureSSL_WT32_ETH01 on "); Serial.print(BOARD_NAME);
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);
  Serial.println(ASYNC_MQTT_GENERIC_VERSION);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

#if ASYNC_TCP_SSL_ENABLED
  mqttClient.setSecure(MQTT_SECURE);
  
  if (MQTT_SECURE) 
  {
    //mqttClient.addServerFingerprint((const uint8_t[])MQTT_SERVER_FINGERPRINT);
    mqttClient.addServerFingerprint((const uint8_t *)MQTT_SERVER_FINGERPRINT);
  }
#endif

  //////////////////////////////////////////////

  // To be called before ETH.begin()
  WiFi.onEvent(ETH_event);

  //bool begin(uint8_t phy_addr=ETH_PHY_ADDR, int power=ETH_PHY_POWER, int mdc=ETH_PHY_MDC, int mdio=ETH_PHY_MDIO,
  //           eth_phy_type_t type=ETH_PHY_TYPE, eth_clock_mode_t clk_mode=ETH_CLK_MODE);
  //ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER, ETH_PHY_MDC, ETH_PHY_MDIO, ETH_PHY_TYPE, ETH_CLK_MODE);
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);

  // Static IP, leave without this line to get IP via DHCP
  //bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = 0, IPAddress dns2 = 0);
  //ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();

  //////////////////////////////////////////////
}

void loop()
{
}
