# DEVILAPE_CYBERDECK

ESP32-powered cyberdeck interface featuring a tactical TFT UI, rotary encoder navigation, WiFi/BLE monitoring tools, packet visualization, radar scanning, animated telemetry, and embedded cyberpunk-inspired diagnostics.

---

# Overview

DEVILAPE_CYBERDECK is a standalone ESP32 cyberdeck project built around a 240x320 ST7789 TFT display and rotary encoder control system.

The project combines:

* Real-time WiFi analysis
* BLE device scanning
* Packet monitoring
* Deauthentication detection
* Beacon frame simulation
* Radar-style signal visualization
* Animated telemetry dashboards
* System logging

The interface is fully menu-driven and optimized for embedded hardware aesthetics.

---

# Features

## Spectrum Analyzer

Visualizes nearby WiFi activity across channels using animated bar graphs.

## Packet Monitor

Captures WiFi management packets in promiscuous mode with live traffic visualization.

## Cyber Core Telemetry

Displays simulated network diagnostics and cyberdeck telemetry.

## Radar Scan

Animated radar-style signal scanner using RSSI values.

## BLE Capture

Scans and displays nearby BLE devices and addresses.

## Net Runner

WiFi network browser with RSSI and channel information.

## Deauth Sniffer

Detects deauthentication/disassociation packets with threat alerts.

## Beacon Flood Simulator

Broadcasts rotating custom SSIDs using raw beacon frames.

## System Logs

Embedded rolling event logger for system activity and alerts.

---

# Hardware Used

## Core Components

* ESP32 Dev Module
* ST7789 240x320 TFT Display
* Rotary Encoder
* Passive Buzzer

---

# TFT Wiring

| TFT Pin    | ESP32 GPIO |
| ---------- | ---------- |
| VCC        | 3.3V       |
| GND        | GND        |
| SCK / CLK  | GPIO 18    |
| SDA / MOSI | GPIO 23    |
| MISO       | GPIO 19    |
| CS         | GPIO 5     |
| DC         | GPIO 2     |
| RST        | GPIO 4     |
| BL / LED   | 3.3V       |

---

# Rotary Encoder Wiring

| Encoder Pin | ESP32 GPIO |
| ----------- | ---------- |
| CLK         | GPIO 34    |
| DT          | GPIO 35    |
| SW          | GPIO 21    |
| +           | 3.3V       |
| GND         | GND        |

---

# Buzzer Wiring

| Buzzer Pin | ESP32 GPIO |
| ---------- | ---------- |
| +          | GPIO 22    |
| -          | GND        |

---

# Libraries Used

```cpp
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "AiEsp32RotaryEncoder.h"
#include "esp_wifi.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
```

Required libraries:

* TFT_eSPI
* AiEsp32RotaryEncoder
* ESP32 WiFi
* ESP32 BLE Arduino

---

# Controls

| Action         | Function          |
| -------------- | ----------------- |
| Rotate Encoder | Navigate menus    |
| Press Encoder  | Enter / Exit mode |

---

# TFT_eSPI Setup

This project requires custom TFT_eSPI configuration.

See:

```text
TFT_SETUP.md
```

for the complete User_Setup.h configuration.

---

# User Interface

The UI includes:

* Tactical yellow/magenta color palette
* Animated radar sweep
* Real-time graphs
* Dynamic telemetry
* Cyberpunk HUD styling
* Embedded audio feedback

---

# Technical Features

## WiFi Features

* Promiscuous packet capture
* Channel hopping
* Raw 802.11 frame transmission
* WiFi scanning

## BLE Features

* BLE discovery
* Device metadata collection

## Display Features

* Real-time graph rendering
* Dynamic screen refresh
* Animated UI transitions
* TFT_eSPI acceleration

---

# Project Structure

```text
DEVILAPE_CYBERDECK/
├── DEVILAPE_CYBERDECK.ino
├── README.md
├── TFT_SETUP.md
├── LICENSE
├── images/
│   ├── cyberdeck_front.jpg
│   ├── ui_demo.jpg
│   ├── wiring.png
│   └── radar_screen.jpg
```

---

# Media


---

# Future Improvements

* SD card logging
* Battery monitoring
* GPS integration
* RF analysis expansion
* Custom enclosure

---

# Disclaimer

This project is intended for educational, research, and authorized testing environments only.

Users are responsible for complying with local wireless regulations and laws.

---

# License

MIT License
