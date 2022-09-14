# AsyncMQTT_Generic client for ESP8266, ESP32, etc.

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncMQTT_Generic.svg?)](https://www.ardu-badge.com/AsyncMQTT_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncMQTT_Generic.svg)](https://github.com/khoih-prog/AsyncMQTT_Generic/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncMQTT_Generic.svg)](http://github.com/khoih-prog/AsyncMQTT_Generic/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.7.0](#releases-v170)
	* [Releases v1.6.1](#releases-v161)
	* [Releases v1.6.0](#releases-v160)
	* [Releases v1.5.0](#releases-v150)
	* [Releases v1.4.0](#releases-v140)
	* [Releases v1.3.0](#releases-v130)
	* [Releases v1.2.1](#releases-v121)
	* [Releases v1.2.0](#releases-v120)
	* [Releases v1.1.0](#releases-v110)
	* [Releases v1.0.1](#releases-v101)
  * [Initial Releases v1.0.0](#Initial-Releases-v100)

---
---

## Changelog

### Releases v1.7.0

1. Fix ESP32 compile error. Check [Missing default case in switch causes #10](https://github.com/khoih-prog/AsyncMQTT_Generic/issues/10)
2. Change to new [Forked ESPAsyncTCP library](https://github.com/khoih-prog/ESPAsyncTCP) for ESP8266 boards, using WiFi or LwIP Ethernet, with [ESP8266 core v3.0.2+](https://github.com/esp8266/Arduino/releases/tag/3.0.2) or WiFi with [ESP8266 core v2.7.4](https://github.com/esp8266/Arduino/releases/tag/2.7.4) to avoid compile errors

### Releases v1.6.1

1. Workaround for RP2040W `WiFi.status()` bug using `ping()` to local gateway for example [FullyFeatured_RP2040W](examples/RP2040W/FullyFeatured_RP2040W)
2. Use WiFi macAddress for RP2040W `_clientId`

### Releases v1.6.0

1. Add support to **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, using [**arduino-pico core v2.4.0+**](https://github.com/earlephilhower/arduino-pico)
2. Add example [FullyFeatured_RP2040W](examples/RP2040W/FullyFeatured_RP2040W)
3. Update `Packages' Patches`

### Releases v1.5.0

1. Add support to ESP8266 W5x00 using [lwIP_w5100](https://github.com/esp8266/Arduino/tree/master/libraries/lwIP_w5100) or [lwIP_w5500](https://github.com/esp8266/Arduino/tree/master/libraries/lwIP_w5500) library
2. Add support to ESP8266 ENC28J60 using [lwIP_enc28j60](https://github.com/esp8266/Arduino/tree/master/libraries/lwIP_enc28j60) library
3. Add example [FullyFeatured_ESP8266_Ethernet](examples/ESP8266/FullyFeatured_ESP8266_Ethernet)
4. Update `Packages' Patches`

### Releases v1.4.0

1. Add support to **Teensy 4.1 using QNEthernet Library**
2. Add example for `QNEthernet`

### Releases v1.3.0

1. Add support to **Portenta_H7**, using either `Murata WiFi` or `Vision-shield Ethernet`
2. Add examples for `Portenta_H7_Ethernet` and `Portenta_H7_WiFi`

### Releases v1.2.1

1. Add support to many **STM32F4 and STM32F7 (without TLS/SSL)** using `LAN8720` Ethernet, such as F407xx, NUCLEO_F429ZI, DISCO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG, etc.
2. Add examples for `STM32_LAN8720`

### Releases v1.2.0

1. Add support to **STM32F/L/H/G/WB/MP1 (without TLS/SSL)** using `built-in LAN8742A` Ethernet, such as Nucleo-144, DISCOVERY, etc.
2. Add examples for `STM32`

### Releases v1.1.0

1. Add support to **WT32_ETH01 (SSL and non-SSL)**
2. Add examples for `WT32_ETH01`


### Releases v1.0.1

1. Fix Library Manager warnings
2. Suppress all compiler warnings
3. Optimize library code by using `reference-passing` instead of `value-passing`

### Initial Releases v1.0.0

1. Initial porting and coding to support **ESP32 (SSL and non-SSL) and ESP8266 (non-SSL)**
