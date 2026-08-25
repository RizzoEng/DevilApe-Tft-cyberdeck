# TFT_eSPI Parallel Configuration

DEVILAPE_CYBERDECK uses a 240x320 ST7789 TFT display connected through the ESP32 8-bit parallel interface using `TFT_eSPI`.

This is **NOT** a standard SPI TFT setup.

---

## Library Required

* **Library:** `TFT_eSPI` by Bodmer
* **GitHub Repository:** https://github.com/Bodmer/TFT_eSPI

---

## Configuration File Path

Open the setup file located at:

```text
Documents/Arduino/libraries/TFT_eSPI/User_Setup.h

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
#define SPI_TOUCH_FREQUENCY  2500000

// ##############################################################
// END CONFIG
// ##############################################################
