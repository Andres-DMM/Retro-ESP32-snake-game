# Retro-ESP32-snake-game

This is a simple vibe coded project that uses a 2.4" tft spi screen, a joystick, and a esp32 dev kit v1 (you can use whatever esp32 you want) to make
a playable replica of the famous snake game.

# Connections:
### 2.4" TFT (ST7789) -> ESP32 Dev Kit V1

| TFT Pin | ESP32 GPIO |
|---------|-----------|
| CS      | GPIO5     |
| DC      | GPIO17    |
| RST     | GPIO16    |
| MOSI    | GPIO23    |
| SCLK    | GPIO18    |
| MISO    | GPIO19    |
| LED     | 3.3V      |
| VCC     | 3.3V or 5V |
| GND     | GND       |

### 2-Axis Joystick -> ESP32 Dev Kit V1

| Joystick | ESP32 GPIO |
|----------|-----------|
| VRx (X)  | GPIO34    |
| VRy (Y)  | GPIO35    |
| SW       | GPIO25    |
| VCC      | 3.3V      |
| GND      | GND       |


### Buzzer -> ESP32 Dev Kit V1
| Buzzer | ESP32 GPIO |
|----------|-----------|
| VCC      | GPIO15    |
| GND      | GND       |

---
# You will need the TFT_eSPI library:
https://github.com/Bodmer/TFT_espi

You will need to edit the setup file with this or directly use the one that has been provided.


# SETUP:
#define ST7789_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_CS   5
#define TFT_DC   17
#define TFT_RST  16
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_MISO 19

#define SPI_FREQUENCY  40000000
#define SPI_READ_FREQUENCY  20000000

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH

#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters

#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters




