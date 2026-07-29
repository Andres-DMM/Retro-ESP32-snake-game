# Retro-ESP32-snake-game

This is a simple vibe coded project that uses a 2.4" tft spi screen, a joystick, and a esp32 dev kit v1 (you can use whatever esp32 you want) to make
a playable replica of the famous snake game.

#Connections:
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
