# DEVILAPE_CYBERDECK

ESP32-powered cyberdeck interface featuring a tactical TFT UI, rotary encoder navigation, WiFi/BLE monitoring tools, packet visualization, and animated system telemetry.

---

## Overview

DEVILAPE_CYBERDECK is a standalone ESP32 cyberdeck interface designed around a TFT display and rotary encoder control system.

The project combines:
- Real-time WiFi analysis
- BLE device scanning
- Packet monitoring
- Deauthentication detection
- Beacon frame simulation
- Radar-style signal visualization
- Animated telemetry dashboards
- System logging

The interface is fully menu-driven and optimized for embedded hardware aesthetics.

---

## Hardware Used

### Core Components
- ESP32 Dev Module
- TFT Display (TFT_eSPI compatible)
- Rotary Encoder
- Passive Buzzer

### Pin Configuration

| Component | GPIO |
|---|---|
| Buzzer | 22 |
| Encoder CLK | 34 |
| Encoder DT | 35 |
| Encoder SW | 21 |

---

## Libraries Used

```cpp
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "AiEsp32RotaryEncoder.h"
#include "esp_wifi.h"
#include <BLEDevice.h>
```

Required libraries:
- TFT_eSPI
- AiEsp32RotaryEncoder
- ESP32 WiFi
- ESP32 BLE Arduino

---

## Features

## 1. Spectrum Analyzer

Visualizes nearby WiFi activity across channels using animated bar graphs.

### Displays:
- WiFi channel usage
- Signal distribution
- Real-time scan updates

---

## 2. Packet Monitor

Monitors captured WiFi management packets in promiscuous mode.

### Includes:
- Live traffic graph
- Surge detection
- Audible alert system
- Historical packet visualization

---

## 3. Cyber Core Telemetry

Displays system diagnostics and simulated telemetry.

### Metrics:
- Connected node
- Ping RTT
- Packet loss
- Upload/download estimates
- Status indicators

---

## 4. Radar Scan

Animated radar-style interface showing signal intensity.

### Features:
- Rotating radar sweep
- RSSI visualization
- Dynamic scanning animation

---

## 5. BLE Capture

Scans and displays nearby BLE devices.

### Displays:
- Device names
- MAC addresses
- Signal strength references

---

## 6. Net Runner

WiFi network browser interface.

### Displays:
- SSIDs
- Channels
- RSSI values
- Scrollable network list

---

## 7. Deauth Sniffer

Detects WiFi deauthentication/disassociation frames.

### Features:
- Channel hopping
- Packet counting
- Threat detection alerts
- Last detected MAC target
- Real-time monitoring

---

## 8. Beacon Flood Simulator

Generates custom beacon frames using rotating SSID names.

### Included SSIDs:
- X_HACKED_X
- SYSTEM_FAIL
- VOID_SIG
- ROOT_ACCESS
- SQL_INJECT
- CYBER_PUNK

### Notes
This mode is intended for controlled lab/testing environments only.

---

## 9. System Logs

Embedded rolling event logger.

### Logs:
- Boot events
- Scan activity
- Alerts
- Start/stop actions
- Threat notifications

---

## Controls

| Action | Function |
|---|---|
| Rotate Encoder | Navigate menus |
| Press Encoder | Enter/Exit mode |

---

## UI Design

The interface uses:
- Yellow/Magenta tactical color palette
- Animated transitions
- Cyberpunk-inspired diagnostics
- Real-time rendering
- Embedded HUD layout

---

## Technical Details

### WiFi Features
- Promiscuous packet capture
- Channel hopping
- Raw 802.11 frame transmission
- WiFi network scanning

### BLE Features
- BLE device discovery
- Device metadata collection

### Display Features
- Real-time graph rendering
- Dynamic UI refresh
- Animated radar sweep
- TFT_eSPI acceleration

---

## Boot Sequence

The device performs:
1. TFT initialization
2. Rotary encoder setup
3. WiFi initialization
4. BLE initialization
5. Audio startup tones
6. Animated boot screen

---

## Lessons Learned

- Efficient TFT rendering matters on ESP32
- Partial screen redraws improve responsiveness
- Promiscuous mode handling requires careful timing
- Rotary encoder input debouncing is important
- Embedded UI design benefits from persistent state systems

---

## Future Improvements

- SD card logging
- Battery monitoring
- GPS integration
- OLED secondary display
- RF analysis expansion
- Custom enclosure
- Audio spectrum visualization

---

## Media

Add project photos, wiring diagrams, and demo videos here.

Example:
```text
/images/cyberdeck_front.jpg
/images/ui_demo.jpg
/images/wiring.png
```

---

## Disclaimer

This project is intended for educational, research, and authorized testing environments only.

Users are responsible for complying with local laws and wireless regulations.

---

## License

MIT License
