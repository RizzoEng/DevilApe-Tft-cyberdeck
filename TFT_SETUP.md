# TFT_eSPI Configuration for DEVILAPE_CYBERDECK

This project uses the TFT_eSPI library configured for a 240x320 ST7789 SPI TFT display with an ESP32 Dev Module.

---

# Library Required

TFT_eSPI by Bodmer

GitHub:
https://github.com/Bodmer/TFT_eSPI

---

# How To Configure

Open the following file inside your Arduino libraries folder:

```text
Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
```

Delete or comment out the existing display setup and replace it with the configuration below.

---

# DEVILAPE_CYBERDECK User_Setup.h Configuration

```cpp
// ##############################################################
// DEVILAPE_CYBERDECK TFT_eSPI CONFIG
// ESP32 + ST7789 240x320 SPI DISPLAY
// ##############################################################

// USER INFO
#define USER_SETUP_INFO "DEVILAPE_CYBERDECK_SETUP"

// ##############################################################
// DISPLAY DRIVER
// ##############################################################

#define ST7789_DRIVER

// DISPLAY RESOLUTION
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// COLOR ORDER
#define TFT_RGB_ORDER TFT_RGB

// COLOR INVERSION
#define TFT_INVERSION_OFF

// ##############################################################
// ESP32 SPI PINS
// ##############################################################

#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18

// TFT CONTROL PINS
#define TFT_CS    5
#define TFT_DC    2
#define TFT_RST   4

// ##############################################################
// SPI FREQUENCY
// ##############################################################

#define SPI_FREQUENCY       27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000

// ##############################################################
// FONT SETTINGS
// ##############################################################

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ##############################################################
// END CONFIG
// ##############################################################
```

---

# TFT Wiring

| TFT Pin    | ESP32 Pin |
| ---------- | --------- |
| VCC        | 3.3V      |
| GND        | GND       |
| SCK / CLK  | GPIO 18   |
| SDA / MOSI | GPIO 23   |
| MISO       | GPIO 19   |
| CS         | GPIO 5    |
| DC         | GPIO 2    |
| RST        | GPIO 4    |
| BL / LED   | 3.3V      |

---

# Rotary Encoder Wiring

| Encoder Pin | ESP32 Pin |
| ----------- | --------- |
| CLK         | GPIO 34   |
| DT          | GPIO 35   |
| SW          | GPIO 21   |
| +           | 3.3V      |
| GND         | GND       |

---

# Buzzer Wiring

| Buzzer Pin | ESP32 Pin |
| ---------- | --------- |
| +          | GPIO 22   |
| -          | GND       |

---

# Important Notes

* Only ONE display driver should be enabled.
* All other display drivers must remain commented out.
* This setup uses SPI mode only.
* DO NOT enable TFT_PARALLEL_8_BIT.
* Recompile and upload after editing User_Setup.h.
* If colors appear swapped, change:

```cpp
#define TFT_RGB_ORDER TFT_RGB
```

to:

```cpp
#define TFT_RGB_ORDER TFT_BGR
```

---

# Tested Hardware

* ESP32 Dev Module
* ST7789 240x320 TFT Display
* Rotary Encoder Module
* Passive Buzzer

---

# Tested Environment

* Arduino IDE
* TFT_eSPI
* ESP32 Arduino Core

---

# Troubleshooting

## White Screen

* Check TFT_CS
* Check TFT_DC
* Verify ST7789 driver enabled

## Wrong Colors

* Change TFT_RGB_ORDER

## No Display

* Verify SPI wiring
* Check 3.3V power
* Confirm TFT_RST wiring

## Display Flickering

* Lower SPI frequency:

```cpp
#define SPI_FREQUENCY 20000000
```

---

# Read_User_Setup Verification

After configuration, run:

```text
File → Examples → TFT_eSPI → Read_User_Setup
```

This confirms:

* Driver selection
* Resolution
* Pin mapping
* SPI speed

If the values displayed match the configuration above, the setup is correct.
