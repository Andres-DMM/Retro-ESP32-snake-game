# Snake Game - ESP32 + TFT + Joystick

## Connections

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

## TFT_eSPI Library Setup

1. Install **TFT_eSPI** via Arduino Library Manager
2. Go to `Arduino/libraries/TFT_eSPI/`
3. **Replace** `User_Setup.h` with the one I provided in this folder

OR manually edit `User_Setup.h` to contain exactly:
```cpp
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
```

4. Select **ESP32 Dev Module** as board
5. Upload `snake_game.ino`

---

## TFT_eSPI Driver Note

The original code used `ILI9341_DRIVER` for ILI9341 displays.
ST7789 needs `ST7789_DRIVER` instead - the init sequence is different.

---

## Controls

- **Joystick** - move the snake
- **Button press (SW)** - start / restart
- Moving the joystick also starts the game
