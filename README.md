# AsyncMQTT_Generic client for ESP8266, ESP32, etc.

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncMQTT_Generic.svg?)](https://www.ardu-badge.com/AsyncMQTT_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncMQTT_Generic.svg)](https://github.com/khoih-prog/AsyncMQTT_Generic/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncMQTT_Generic.svg)](http://github.com/khoih-prog/AsyncMQTT_Generic/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Why do we need this AsyncMQTT_Generic library](#why-do-we-need-this-AsyncMQTT_Generic-library)
  * [Features](#features)
  * [Why Async is better](#why-async-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [To-be-supported Boards](#To-be-supported-Boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For STM32 boards](#1-for-stm32-boards) 
    * [1.1. For STM32 boards to use LAN8720](#11-for-stm32-boards-to-use-lan8720)
    * [1.2. For STM32 boards to use Serial1](#12-for-stm32-boards-to-use-serial1)
  * [2. For Portenta_H7 boards using Arduino IDE in Linux](#2-for-portenta_h7-boards-using-arduino-ide-in-linux)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [Basic Usage](#Basic-Usage)
* [Examples](#examples)
  * [1. For ESP32](#1-for-ESP32)
    * [1. FullyFeatured_ESP32](examples/ESP32/FullyFeatured_ESP32)
    * [2. FullyFeaturedSSL_ESP32](examples/ESP32/FullyFeaturedSSL_ESP32)
  * [2. For ESP8266](#1-for-ESP8266)
    * [1. FullyFeatured_ESP8266](examples/ESP8266/FullyFeatured_ESP8266)
  * [3. For WT32_ETH01](#1-for-WT32_ETH01)
    * [1. FullyFeatured_WT32_ETH01](examples/WT32_ETH01/FullyFeatured_WT32_ETH01) **New**
    * [2. FullyFeaturedSSL_WT32_ETH01](examples/WT32_ETH01/FullyFeaturedSSL_WT32_ETH01) **New**
* [Example FullyFeaturedSSL_ESP32](#example-FullyFeaturedSSL_ESP32)
  * [1. File FullyFeaturedSSL_ESP32.ino](#1-file-FullyFeaturedSSL_ESP32ino)
  * [2. File defines.h](#2-file-definesh)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [ 1. FullyFeature_ESP8266 on ESP8266_NODEMCU_ESP12E](#1-FullyFeature_ESP8266-on-ESP8266_NODEMCU_ESP12E) 
  * [ 2. FullyFeature_ESP32 on ESP32_DEV](#2-FullyFeature_ESP32-on-ESP32_DEV)
  * [ 3. FullyFeatureSSL_ESP32 on ESP32_DEV](#3-FullyFeatureSSL_ESP32-on-ESP32_DEV) 
  * [ 4. FullyFeatureSSL_ESP32 on ESP32_DEV with _ASYNC_MQTT_LOGLEVEL_](#4-fullyfeaturessl_esp32-on-esp32_dev-with-async_mqtt_loglevel)
  * [ 5. FullyFeatureSSL_WT32_ETH01 on WT32-ETH01 with ETH_PHY_LAN8720](#5-FullyFeatureSSL_WT32_ETH01-on-WT32-ETH01-with-ETH_PHY_LAN8720)
  
  5. FullyFeatureSSL_WT32_ETH01 on WT32-ETH01 with ETH_PHY_LAN8720
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [AsyncMQTT_Generic library](https://github.com/khoih-prog/AsyncMQTT_Generic)

#### Features

This [**AsyncMQTT_Generic library**](https://github.com/khoih-prog/AsyncMQTT_Generic) is based on and modified from [**Marvin Roger's async-mqtt-client Library**](https://github.com/marvinroger/async-mqtt-client), to provide support to many more boards besides ESP32/ESP8266, such as **STM32F, Portenta_H7, etc. boards**. Those supported boards can be used with **ESP8266/ESP32’s WiFi, LAN8742A Ethernet, Portenta_H7 WiFi/Ethernet.**

#### Why Async is better

- Using asynchronous network means that you can handle **more than one connection at the same time**
- **You are called once the request is ready and parsed**
- When you send the response, you are **immediately ready** to handle other connections while the server is taking care of sending the response in the background
- **Speed is OMG**
- **Easy to use API, HTTP Basic and Digest MD5 Authentication (default), ChunkedResponse**
- Easily extensible to handle **any type of content**
- Supports Continue 100
- **Async WebSocket plugin offering different locations without extra servers or ports**
- Async EventSource (Server-Sent Events) plugin to send events to the browser
- URL Rewrite plugin for conditional and permanent url rewrites
- ServeStatic plugin that supports cache, Last-Modified, default index and more
- Simple template processing engine to handle templates


---

#### Currently supported Boards

This [**AsyncMQTT_Generic** library](https://github.com/khoih-prog/AsyncMQTT_Generic) currently supports these following boards:

 1. **ESP32**
 2. **ESP8266**
 3. **WT32_ETH01 (ESP32 + LAN8720A)** from v1.1.0
 
--- 
 
#### To-be-supported Boards
 
 1. **STM32F/L/H/G/WB/MP1 boards with built-in Ethernet LAN8742A** such as :

  - **Nucleo-144 (F429ZI, F767ZI)**
  - **Discovery (STM32F746G-DISCOVERY)**
  - **All STM32 boards (STM32F/L/H/G/WB/MP1) with 32K+ Flash, with Built-in Ethernet**
  - See [EthernetWebServer_STM32 Support and Test Results](https://github.com/khoih-prog/EthernetWebServer_STM32/issues/1)
  
 2. **STM32F4/F7 boards using Ethernet LAN8720** such as :

  - **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
  - **Discovery (DISCO_F746NG)**
  - **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

 3. **Portenta_H7** using either `Murata WiFi` or `Vision-shield Ethernet`


---
---

 
## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`ESP32 Core 2.0.2+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/)
 3. [`ESP8266 Core 3.0.2+`](https://github.com/esp8266/Arduino) for ESP8266-based boards. [![Latest release](https://img.shields.io/github/release/esp8266/Arduino.svg)](https://github.com/esp8266/Arduino/releases/latest/).
 4. [`Arduino Core for STM32 v2.2.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32F/L/H/G/WB/MP1 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest). **Not ready yet**
 5. [`Arduino mbed_portenta core 2.8.0+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) Portenta_H7-based boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest). **Not ready yet**
 6. [`WebServer_WT32_ETH01 library v1.4.1+`](https://github.com/khoih-prog/WebServer_WT32_ETH01) to use WT32_ETH01 (ESP32 + LAN8720). To install, check [![arduino-library-badge](https://www.ardu-badge.com/badge/WebServer_WT32_ETH01.svg?)](https://www.ardu-badge.com/WebServer_WT32_ETH01). **Ready from v1.1.0**

---
---

## Installation

### Use Arduino Library Manager
The best and easiest way is to use `Arduino Library Manager`. Search for `AsyncMQTT_Generic`, then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncMQTT_Generic.svg?)](https://www.ardu-badge.com/AsyncMQTT_Generic) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**AsyncMQTT_Generic**](https://github.com/khoih-prog/AsyncMQTT_Generic) page.
2. Download the latest release `AsyncMQTT_Generic-master.zip`.
3. Extract the zip file to `AsyncMQTT_Generic-master` directory 
4. Copy whole `AsyncMQTT_Generic-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AsyncMQTT_Generic** library](https://registry.platformio.org/libraries/khoih-prog/AsyncMQTT_Generic) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/AsyncMQTT_Generic/installation). Search for **AsyncMQTT_Generic** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### Packages' Patches

#### 1. For STM32 boards

#### 1.1 For STM32 boards to use LAN8720

To use LAN8720 on some STM32 boards 

- **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
- **Discovery (DISCO_F746NG)**
- **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

you have to copy the files [stm32f4xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.2.0/system/STM32F4xx) and [stm32f7xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/2.2.0/system/STM32F7xx) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system) to overwrite the old files.

Supposing the STM32 stm32 core version is 2.2.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system/STM32F4xx/stm32f4xx_hal_conf_default.h` for STM32F4.
- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/system/STM32F7xx/stm32f7xx_hal_conf_default.h` for Nucleo-144 STM32F7.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F4xx/stm32f4xx_hal_conf_default.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F7xx/stm32f7xx_hal_conf_default.h


#### 1.2 For STM32 boards to use Serial1

**To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/2.2.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/2.2.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 2.2.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/2.2.0/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32F7xx/F765Z(G-I)T_F767Z(G-I)T_F777ZIT/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/STM32L0xx/L052R(6-8)T_L053R(6-8)T_L063R8T/NUCLEO_L053R8/variant.h`


#### 2. For Portenta_H7 boards using Arduino IDE in Linux

  **To be able to upload firmware to Portenta_H7 using Arduino IDE in Linux (Ubuntu, etc.)**, you have to copy the file [portenta_post_install.sh](Packages_Patches/arduino/hardware/mbed_portenta/2.8.0/portenta_post_install.sh) into mbed_portenta directory (~/.arduino15/packages/arduino/hardware/mbed_portenta/2.8.0/portenta_post_install.sh). 
  
  Then run the following command using `sudo`
  
```
$ cd ~/.arduino15/packages/arduino/hardware/mbed_portenta/2.8.0
$ chmod 755 portenta_post_install.sh
$ sudo ./portenta_post_install.sh
```

This will create the file `/etc/udev/rules.d/49-portenta_h7.rules` as follows:

```
# Portenta H7 bootloader mode UDEV rules

SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="035b", GROUP="plugdev", MODE="0666"
```

Supposing the ArduinoCore-mbed core version is 2.8.0. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/2.8.0/portenta_post_install.sh`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/mbed_portenta/x.yy.zz/portenta_post_install.sh`


---
---


### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

    
---
---

## Basic Usage

Please check [API documentation](https://github.com/khoih-prog/AsyncMQTT_Generic/blob/main/docs/2.-API-reference.md).


---
---

### Examples

#### 1. For ESP32

 1. [FullyFeatured_ESP32](examples/ESP32/FullyFeatured_ESP32)
 2. [FullyFeaturedSSL_ESP32](examples/ESP32/FullyFeaturedSSL_ESP32)

#### 2. For ESP8266

 1. [FullyFeatured_ESP8266](examples/ESP8266/FullyFeatured_ESP8266)

#### 3. For WT32_ETH01

 1. [FullyFeatured_WT32_ETH01](examples/WT32_ETH01/FullyFeatured_WT32_ETH01) **New**
 2. [FullyFeaturedSSL_WT32_ETH01](examples/WT32_ETH01/FullyFeaturedSSL_WT32_ETH01) **New**

---
---

### Example [FullyFeaturedSSL_ESP32](examples/ESP32/FullyFeaturedSSL_ESP32)

#### 1. File [FullyFeaturedSSL_ESP32.ino](examples/ESP32/FullyFeaturedSSL_ESP32/FullyFeaturedSSL_ESP32.ino)

https://github.com/khoih-prog/AsyncMQTT_Generic/blob/a2db21ca2e68487560c0718529e5e83dee4d6b3f/examples/ESP32/FullyFeaturedSSL_ESP32/FullyFeaturedSSL_ESP32.ino#L13-L225



#### 2. File [defines.h](examples/ESP32/FullyFeaturedSSL_ESP32/defines.h)

https://github.com/khoih-prog/AsyncMQTT_Generic/blob/a2db21ca2e68487560c0718529e5e83dee4d6b3f/examples/ESP32/FullyFeaturedSSL_ESP32/defines.h#L12-L20


---
---

### Debug Terminal Output Samples

#### 1. FullyFeature_ESP8266 on ESP8266_NODEMCU_ESP12E

This is terminal debug output when running [FullyFeature_ESP8266](examples/ESP8266/FullyFeature_ESP8266) on **ESP8266_NODEMCU_ESP12E** connecting to `broker.emqx.io` MQTT server.

```
Starting FullyFeature_ESP8266 on ESP8266_NODEMCU_ESP12E
AsyncMQTT_Generic v1.1.0 for ESP8266
Connecting to Wi-Fi...
Connected to Wi-Fi. IP address: 192.168.2.82
Connecting to MQTT...
Connected to MQTT broker: broker.emqx.io, port: 1883
PubTopic: async-mqtt/ESP8266_Pub
************************************************
Session present: 0
Subscribing at QoS 2, packetId: 1
Publishing at QoS 0
Publishing at QoS 1, packetId: 2
Publishing at QoS 2, packetId: 3
************************************************
Subscribe acknowledged.
  packetId: 1
  qos: 2
Publish received.
  topic: async-mqtt/ESP8266_Pub
  qos: 2
  dup: 0
  retain: 1
  len: 13
  index: 0
  total: 13
Publish acknowledged.
  packetId: 2
Publish received.
  topic: async-mqtt/ESP8266_Pub
  qos: 0
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish received.
  topic: async-mqtt/ESP8266_Pub
  qos: 1
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish received.
  topic: async-mqtt/ESP8266_Pub
  qos: 2
  dup: 0
  retain: 0
  len: 13
  index: 0
  total: 13
Publish acknowledged.
  packetId: 3
```

---

#### 2. FullyFeature_ESP32 on ESP32_DEV

This is terminal debug output when running [FullyFeature_ESP32](examples/ESP32/FullyFeature_ESP32) on **ESP32_DEV** connecting to `broker.emqx.io` MQTT server.

```
Starting FullyFeature_ESP32 on ESP32_DEV
AsyncMQTT_Generic v1.1.0 for ESP32 core v2.0.0+
Connecting to Wi-Fi...
WiFi ready
WiFi STA starting
WiFi STA connected
WiFi connected
IP address: 192.168.2.81
Connecting to MQTT...
Connected to MQTT broker: broker.emqx.io, port: 1883
PubTopic: async-mqtt/ESP32_Pub
************************************************
Session present: 0
Subscribing at QoS 2, packetId: 1
Publishing at QoS 0
Publishing at QoS 1, packetId: 2
Publishing at QoS 2, packetId: 3
************************************************
Subscribe acknowledged.
  packetId: 1
  qos: 2
Publish received.
  topic: async-mqtt/ESP32_Pub
  qos: 2
  dup: 0
  retain: 1
  len: 6
  index: 0
  total: 6
Publish received.
  topic: async-mqtt/ESP32_Pub
  qos: 0
  dup: 0
  retain: 0
  len: 10
  index: 0
  total: 10
Publish acknowledged.
  packetId: 2
Publish received.
  topic: async-mqtt/ESP32_Pub
  qos: 1
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish received.
  topic: async-mqtt/ESP32_Pub
  qos: 2
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish acknowledged.
  packetId: 3
```

---

#### 3. FullyFeatureSSL_ESP32 on ESP32_DEV

This is terminal debug output when running [FullyFeatureSSL_ESP32](examples/ESP32/FullyFeatureSSL_ESP32) on **ESP32_DEV** connecting to `broker.emqx.io` secured MQTT server (port 8883).

```
Starting FullyFeatureSSL_ESP32 on ESP32_DEV
AsyncMQTT_Generic v1.1.0 for ESP32 core v2.0.0+
Connecting to Wi-Fi...
WiFi ready
WiFi STA starting
WiFi STA connected
WiFi connected
IP address: 192.168.2.81
Connecting to MQTT...
Connected to MQTT broker: broker.emqx.io, port: 8883
PubTopic: async-mqtt/ESP32_SSL_Pub
************************************************
Session present: 0
Subscribing at QoS 2, packetId: 1
Publishing at QoS 0
Publishing at QoS 1, packetId: 2
Publishing at QoS 2, packetId: 3
************************************************
Subscribe acknowledged.
  packetId: 1
  qos: 2
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 2
  dup: 0
  retain: 1
  len: 6
  index: 0
  total: 6
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 0
  dup: 0
  retain: 0
  len: 10
  index: 0
  total: 10
Publish acknowledged
  packetId: 2
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 1
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 2
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish acknowledged
  packetId: 3
```

---

#### 4. FullyFeatureSSL_ESP32 on ESP32_DEV with _ASYNC_MQTT_LOGLEVEL_

This is terminal debug output when running [FullyFeatureSSL_ESP32](examples/ESP32/FullyFeatureSSL_ESP32) on **ESP32_DEV** connecting to `broker.emqx.io` secured MQTT server (port 8883). The _ASYNC_MQTT_LOGLEVEL_ is set at 4 to show all the debugging messages.


```
Starting FullyFeatureSSL_ESP32 on ESP32_DEV
AsyncMQTT_Generic v1.1.0 for ESP32 core v2.0.0+
Connecting to Wi-Fi...
WiFi ready
WiFi STA starting
WiFi STA connected
WiFi connected
IP address: 192.168.2.81
Connecting to MQTT...
[AMQTT] CONNECTING
[AMQTT] _onAck: ack len = 303
[AMQTT] _onAck: ack len = 75
[AMQTT] _onAck: ack len = 51
[AMQTT] TCP conn, MQTT CONNECT
[AMQTT] _addFront: new front, packetType = CONNECT
[AMQTT] _handleQueue: snd, packetType # CONNECT , tls: realSent = 61
[AMQTT] _handleQueue: sent / _headsize = 32 / 32
[AMQTT] _handleQueue: released packetType # CONNECT
[AMQTT] _onAck: ack len = 61
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv CONNACK
[AMQTT] CONNACK
Connected to MQTT broker: broker.emqx.io, port: 8883
PubTopic: async-mqtt/ESP32_SSL_Pub
************************************************
Session present: 0
[AMQTT] SUBSCRIBE
[AMQTT] _addBack: new back, packetType = SUBSCRIBE
[AMQTT] _handleQueue: snd, packetType # SUBSCRIBE , tls: realSent = 60
[AMQTT] _handleQueue: sent / _headsize = 31 / 31
Subscribing at QoS 2, packetId: 1
[AMQTT] PUBLISH
[AMQTT] _addBack: new back, packetType = PUBLISH
Publishing at QoS 0
[AMQTT] PUBLISH
[AMQTT] _addBack: new back, packetType = PUBLISH
Publishing at QoS 1, packetId: 2
[AMQTT] PUBLISH
[AMQTT] _addBack: new back, packetType = PUBLISH
Publishing at QoS 2, packetId: 3
************************************************
[AMQTT] _onAck: ack len = 60
[AMQTT] _onData : data rcv len = 5
[AMQTT] _onData: rcv SUBACK
[AMQTT] SUBACK
[AMQTT] SUB released
Subscribe acknowledged.
  packetId: 1
  qos: 2
[AMQTT] _handleQueue: released packetType # SUBSCRIBE
[AMQTT] _handleQueue: snd, packetType # PUBLISH , tls: realSent = 67
[AMQTT] _handleQueue: sent / _headsize = 38 / 38
[AMQTT] _handleQueue: released packetType # PUBLISH
[AMQTT] _handleQueue: snd, packetType # PUBLISH , tls: realSent = 65
[AMQTT] _handleQueue: sent / _headsize = 36 / 36
[AMQTT] _onData : data rcv len = 36
[AMQTT] _onData: rcv PUBLISH
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 2
  dup: 0
  retain: 1
  len: 6
  index: 0
  total: 6
[AMQTT] _addBack: new back, packetType = PUBREC
[AMQTT] _onAck: ack len = 67
[AMQTT] _onData : data rcv len = 38
[AMQTT] _onData: rcv PUBLISH
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 0
  dup: 0
  retain: 0
  len: 10
  index: 0
  total: 10
[AMQTT] _onAck: ack len = 65
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv PUBACK
[AMQTT] PUB released
Publish acknowledged
  packetId: 2
[AMQTT] _onData : data rcv len = 36
[AMQTT] _onData: rcv PUBLISH
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 1
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
[AMQTT] _addBack: new back, packetType = PUBACK
[AMQTT] _handleQueue: released packetType # PUBLISH
[AMQTT] _handleQueue: snd, packetType # PUBLISH , tls: realSent = 65
[AMQTT] _handleQueue: sent / _headsize = 36 / 36
[AMQTT] _onAck: ack len = 65
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv PUBREC
[AMQTT] snd PUBREL
[AMQTT] PUB released
[AMQTT] _insert: new insert, packetType = PUBREL
[AMQTT] _handleQueue: released packetType # PUBLISH
[AMQTT] _handleQueue: snd, packetType # PUBREL , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] _onData : data rcv len = 36
[AMQTT] _onData: rcv PUBLISH
Publish received.
  topic: async-mqtt/ESP32_SSL_Pub
  qos: 2
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
[AMQTT] _addBack: new back, packetType = PUBREC
[AMQTT] _onAck: ack len = 33
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv PUBCOMP
[AMQTT] PUBREL released
Publish acknowledged
  packetId: 3
[AMQTT] _handleQueue: released packetType # PUBREL
[AMQTT] _handleQueue: snd, packetType # PUBREC , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] _onAck: ack len = 33
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv PUBREL
[AMQTT] _insert: new insert, packetType = PUBCOMP
[AMQTT] _handleQueue: released packetType # PUBREC
[AMQTT] _handleQueue: snd, packetType # PUBCOMP , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] _handleQueue: released packetType # PUBCOMP
[AMQTT] _handleQueue: snd, packetType # PUBACK , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] _handleQueue: released packetType # PUBACK
[AMQTT] _handleQueue: snd, packetType # PUBREC , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] PUBREC released
[AMQTT] _onAck: ack len = 33
[AMQTT] _onAck: ack len = 66
[AMQTT] _onData : data rcv len = 4
[AMQTT] _onData: rcv PUBREL
[AMQTT] _insert: new insert, packetType = PUBCOMP
[AMQTT] _handleQueue: released packetType # PUBREC
[AMQTT] _handleQueue: snd, packetType # PUBCOMP , tls: realSent = 33
[AMQTT] _handleQueue: sent / _headsize = 4 / 4
[AMQTT] _handleQueue: released packetType # PUBCOMP
[AMQTT] PUBREC released
[AMQTT] _onAck: ack len = 33
[AMQTT] PING
[AMQTT] _addBack: new back, packetType = PINGREQ
[AMQTT] _handleQueue: snd, packetType # PINGREQ , tls: realSent = 31
[AMQTT] _handleQueue: sent / _headsize = 2 / 2
[AMQTT] _handleQueue: released packetType # PINGREQ
[AMQTT] _onAck: ack len = 31
[AMQTT] _onData : data rcv len = 2
[AMQTT] _onData: rcv PINGRESP
[AMQTT] PINGRESP
```

---

#### 5. FullyFeatureSSL_WT32_ETH01 on WT32-ETH01 with ETH_PHY_LAN8720

This is terminal debug output when running [FullyFeatureSSL_WT32_ETH01](examples/WT32_ETH01/FullyFeatureSSL_WT32_ETH01) on **WT32-ETH01** connecting to `broker.emqx.io` secured MQTT server (port 8883).


```
Starting FullyFeatureSSL_WT32_ETH01 on WT32-ETH01 with ETH_PHY_LAN8720
WebServer_WT32_ETH01 v1.4.1 for core v2.0.0+
AsyncMQTT_Generic v1.1.0 for ESP32 core v2.0.0+
ETH starting
ETH connected
ETH got IP
IP address: 192.168.2.97
Connecting to MQTT...
Connected to MQTT broker: broker.emqx.io, port: 8883
PubTopic: async-mqtt/WT32_ETH01_SSL_Pub
************************************************
Session present: 0
Subscribing at QoS 2, packetId: 1
Publishing at QoS 0
Publishing at QoS 1, packetId: 2
Publishing at QoS 2, packetId: 3
************************************************
Subscribe acknowledged.
  packetId: 1
  qos: 2
Publish received.
  topic: async-mqtt/WT32_ETH01_SSL_Pub
  qos: 0
  dup: 0
  retain: 0
  len: 15
  index: 0
  total: 15
Publish acknowledged
  packetId: 2
Publish received.
  topic: async-mqtt/WT32_ETH01_SSL_Pub
  qos: 1
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish received.
  topic: async-mqtt/WT32_ETH01_SSL_Pub
  qos: 2
  dup: 0
  retain: 0
  len: 6
  index: 0
  total: 6
Publish acknowledged
  packetId: 3
```

---
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the _MYSQL_LOGLEVEL_ to 0

```cpp
#define ASYNC_MQTT_DEBUG_PORT      Serial

// Debug Level from 0 to 4
#define _ASYNC_MQTT_LOGLEVEL_      1
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---


### Issues ###

Submit issues to: [AsyncMQTT_Generic issues](https://github.com/khoih-prog/AsyncMQTT_Generic/issues)

---
---

### TO DO

 1. Add support to **Portenta_H7**, using either `Murata WiFi` or `Vision-shield Ethernet`
 2. Add support to **Ethernet LAN8742A**, using [`STM32Ethernet library`](https://github.com/stm32duino/STM32Ethernet) and [`STM32duino_LwIP library`](https://github.com/stm32duino/LwIP).
 3. Support ESP8266 using SSL/TLS
 4. Bug fixing.

---

### DONE

 1. Add support to **ESP32 (SSL and non-SSL)**
 2. Add support to **ESP8266 (non-SSL)**
 3. Add support to **WT32_ETH01 (SSL and non-SSL)**

---
---

### Contributions and Thanks

1. Based on and modified from [**Marvin Roger's async-mqtt-client Library**](https://github.com/marvinroger/async-mqtt-client)


<table>
  <tr>
    <td align="center"><a href="https://github.com/marvinroger"><img src="https://github.com/marvinroger.png" width="100px;" alt="marvinroger"/><br /><sub><b>⭐️ Marvin Roger</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- Most of the credits go to original author [Marvin Roger](https://github.com/marvinroger)

- The library is licensed under [MIT](https://github.com/khoih-prog/AsyncMQTT_Generic/blob/main/LICENSE)

---

## Copyright

1. Copyright (c) 2017- Marvin Roger
2. Copyright (c) 2022- Khoi Hoang

