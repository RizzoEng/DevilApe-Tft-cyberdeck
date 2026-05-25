# TFT_eSPI Parallel Configuration

DEVILAPE_CYBERDECK uses a 240x320 ST7789 TFT display connected through the ESP32 8-bit parallel interface using TFT_eSPI.

This is NOT a standard SPI TFT setup.

---

# Library Required

TFT_eSPI by Bodmer

GitHub:
https://github.com/Bodmer/TFT_eSPI

---

# Open User_Setup.h

Open:

```text id="rq7h8o"
Documents/Arduino/libraries/TFT_eSPI/User_Setup.h
```

Replace the display configuration with the setup below.

---

# DEVILAPE_CYBERDECK User_Setup.h

```cpp id="qwv1h8"
// ##############################################################
// DEVILAPE_CYBERDECK PARALLEL TFT CONFIG
// ESP32 + ST7789 240x320
// ##############################################################

#define USER_SETUP_INFO "User_Setup"

// ##############################################################
// DISPLAY DRIVER
// ##############################################################

#define ST7789_DRIVER

// ##############################################################
// DISPLAY RESOLUTION
// ##############################################################

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ##############################################################
// COLOR SETTINGS
// ##############################################################

#define TFT_RGB_ORDER TFT_RGB
#define TFT_INVERSION_OFF

// ##############################################################
// ENABLE 8-BIT PARALLEL MODE
// ##############################################################

#define TFT_PARALLEL_8_BIT

// ##############################################################
// PARALLEL DATA PINS
// ##############################################################

#define TFT_D0 12
#define TFT_D1 13
#define TFT_D2 26
#define TFT_D3 25
#define TFT_D4 17
#define TFT_D5 16
#define TFT_D6 27
#define TFT_D7 14

// ##############################################################
// CONTROL PINS
// ##############################################################

#define TFT_CS   33
#define TFT_DC   15
#define TFT_WR    4
#define TFT_RD    2
#define TFT_RST  32

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
// SPI FREQUENCY SETTINGS
// ##############################################################

#define SPI_FREQUENCY       27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000

// ##############################################################
// END CONFIG
// ##############################################################
```

---

# Parallel TFT Wiring

| TFT Pin | ESP32 GPIO |
| ------- | ---------- |
| D0      | GPIO 12    |
| D1      | GPIO 13    |
| D2      | GPIO 26    |
| D3      | GPIO 25    |
| D4      | GPIO 17    |
| D5      | GPIO 16    |
| D6      | GPIO 27    |
| D7      | GPIO 14    |
| CS      | GPIO 33    |
| DC      | GPIO 15    |
| WR      | GPIO 4     |
| RD      | GPIO 2     |
| RST     | GPIO 32    |
| VCC     | 3.3V       |
| GND     | GND        |

---

# Important Notes

* This project uses 8-bit parallel TFT mode.
* Do NOT use SPI pin definitions.
* Only ONE display driver should be enabled.
* Comment out all unused drivers.
* Recompile after editing User_Setup.h.

---

# Tested Hardware

* ESP32 Dev Module
* ST7789 240x320 TFT
* TFT_eSPI Library
* Arduino IDE

---

# Troubleshooting

## White Screen

* Verify parallel data pins
* Check TFT_RST wiring
* Confirm ST7789 driver enabled

## Wrong Colors

Try:

```cpp id="1o86e4"
#define TFT_RGB_ORDER TFT_BGR
```

instead of:

```cpp id="n2rxmo"
#define TFT_RGB_ORDER TFT_RGB
```

## Mirrored or Inverted Display

Try:

```cpp id="gb3u68"
#define TFT_INVERSION_ON
```

or:

```cpp id="4hqlfv"
#define TFT_INVERSION_OFF
```

depending on the panel revision.

---

# Verify Setup

Run:

```text id="5n6oyg"
File → Examples → TFT_eSPI → Read_User_Setup
```

Verify:

* ST7789 driver
* 240x320 resolution
* Parallel mode enabled
* Correct GPIO assignments
