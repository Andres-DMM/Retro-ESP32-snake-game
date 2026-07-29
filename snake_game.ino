#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

#define C_RED    0xFFE0 // (Yellow)
#define C_YELLOW 0xF800 // (Red)
#define C_GREEN  0xF81F // (Magenta)
#define C_WHITE  0x0000 // (Black)
#define C_BLACK  0xFFFF // (White)

#define CELL_S    8
#define GRID_W    30
#define GRID_H    37    // Reduced by 1 to make room for HUD (37 * 8 = 296 + 24 = 320)
#define MAX_SN    1200

#define JOY_X      34
#define JOY_Y      35
#define JOY_SW     25
#define DEAD_ZONE  1200

#define BUZZER 15

enum Dir { STOP, UP, DOWN, LEFT, RIGHT };
Dir dir = STOP, nxt = STOP;
int sx[MAX_SN], sy[MAX_SN];
int slen = 3, fx, fy, score = 0, highScore = 0, speed = 150;
bool over = false;
unsigned long last = 0;
uint8_t phase = 0;
bool joyFree = true;

#define SCR_OFF 24     
int prevScore = -1;

int rnd(int mn, int mx) { return mn + esp_random() % (mx - mn + 1); }

// --- Sound Effect Helpers ---
void soundEat() {
  tone(BUZZER, 1200, 40); // High short beep for eating food
}

void soundMove() {
  tone(BUZZER, 400, 15);  // Very short low click when turning
}

void soundStart() {
  tone(BUZZER, 523, 80); delay(100); // C5
  tone(BUZZER, 659, 80); delay(100); // E5
  tone(BUZZER, 784, 120);            // G5
}

void soundGameOver() {
  tone(BUZZER, 300, 150); delay(180);
  tone(BUZZER, 200, 150); delay(180);
  tone(BUZZER, 130, 300);
}
// ----------------------------

void spawnFood() {
  bool ok;
  do {
    ok = true;
    fx = rnd(1, GRID_W - 2);
    fy = rnd(1, GRID_H - 2);
    for (int i = 0; i < slen; i++)
      if (sx[i] == fx && sy[i] == fy) { ok = false; break; }
  } while (!ok);
}

void drawCell(int x, int y, uint16_t c) {
  tft.fillRect(x * CELL_S, SCR_OFF + y * CELL_S, CELL_S - 1, CELL_S - 1, c);
}

// Redesigned Score / Header display
void drawScoreBar() {
  // Clear the HUD header area
  tft.fillRect(0, 0, 240, SCR_OFF - 2, C_BLACK);
  
  // HUD Text Styling
  tft.setTextColor(C_YELLOW, C_BLACK);
  tft.setTextSize(2); // Slightly larger text for better readability
  
  // Current Score
  tft.setCursor(6, 4);
  tft.print("SCORE:");
  tft.print(score);
  
  // High Score
  tft.setCursor(140, 4);
  tft.print("HI:");
  tft.print(highScore);
  
  // Separator Line between Header and Grid
  tft.drawFastHLine(0, SCR_OFF - 2, 240, C_WHITE);
}

void drawScore() {
  if (score == prevScore) return;
  prevScore = score;
  if (score > highScore) highScore = score;
  drawScoreBar();
}

void initGame() {
  slen = 3;
  sx[0] = GRID_W / 2;     sy[0] = GRID_H / 2;
  sx[1] = sx[0] - 1;      sy[1] = sy[0];
  sx[2] = sx[1] - 1;      sy[2] = sy[0];
  dir = RIGHT; nxt = RIGHT;
  score = 0; speed = 150;
  over = false; joyFree = true;
  prevScore = -1;
  spawnFood();
}

void drawField() {
  tft.fillScreen(C_BLACK);
  for (int i = 0; i < slen; i++)
    drawCell(sx[i], sy[i], i == 0 ? C_WHITE : C_GREEN);
  drawCell(fx, fy, C_RED);
  drawScore();
}

void tick() {
  dir = nxt;
  int nx = sx[0], ny = sy[0];
  switch (dir) {
    case UP:    ny--; break;
    case DOWN:  ny++; break;
    case LEFT:  nx--; break;
    case RIGHT: nx++; break;
    default:    return;
  }

  if (nx < 0 || nx >= GRID_W || ny < 0 || ny >= GRID_H) { over = true; return; }
  for (int i = 0; i < slen; i++)
    if (sx[i] == nx && sy[i] == ny) { over = true; return; }

  bool ate = (nx == fx && ny == fy);
  int tx = sx[slen - 1], ty = sy[slen - 1];

  if (!ate) drawCell(tx, ty, C_BLACK);

  for (int i = slen - 1; i > 0; i--) { sx[i] = sx[i - 1]; sy[i] = sy[i - 1]; }
  sx[0] = nx; sy[0] = ny;

  drawCell(sx[0], sy[0], C_WHITE);
  if (slen > 1) drawCell(sx[1], sy[1], C_GREEN);

  if (ate) {
    soundEat(); // Play sound when food is eaten
    slen++;
    sx[slen - 1] = tx; sy[slen - 1] = ty;
    drawCell(tx, ty, C_GREEN);
    score++;
    if (speed > 50) speed -= 2;
    spawnFood();
    drawCell(fx, fy, C_RED);
  }

  drawScore();
}

void readJoy() {
  int x = analogRead(JOY_X), y = analogRead(JOY_Y);
  int cx = x - 2048, cy = y - 2048;

  if (abs(cx) <= DEAD_ZONE && abs(cy) <= DEAD_ZONE) {
    joyFree = true;
    return;
  }

  if (!joyFree) return;

  Dir oldNxt = nxt;

  if (abs(cx) > abs(cy)) {
    if (cx > 0 && dir != LEFT)  nxt = RIGHT;
    if (cx < 0 && dir != RIGHT) nxt = LEFT;
  } else {
    if (cy > 0 && dir != UP)    nxt = DOWN;
    if (cy < 0 && dir != DOWN)  nxt = UP;
  }

  if (nxt != oldNxt) {
    soundMove(); // Play short click on valid direction change
  }

  joyFree = false;
}

void showStart() {
  phase = 0;
  tft.fillScreen(C_BLACK);
  tft.setTextColor(C_YELLOW, C_BLACK);
  tft.setTextSize(4);
  tft.setCursor(40, 50);
  tft.println("SNAKE");
  tft.setTextColor(C_GREEN, C_BLACK);
  tft.setTextSize(2);
  tft.setCursor(30, 120);
  tft.println("ESP32 RETRO");
  tft.setTextColor(C_WHITE, C_BLACK);
  tft.setTextSize(1);
  tft.setCursor(20, 190);
  tft.println("MOVE JOYSTICK TO START");
  tft.setCursor(20, 210);
  tft.println("OR PRESS BUTTON");
}

void showGameOver() {
  phase = 2;
  soundGameOver(); // Play sad tune on game over
  tft.fillScreen(C_BLACK);
  tft.setTextColor(C_RED, C_BLACK);
  tft.setTextSize(3);
  tft.setCursor(30, 50);
  tft.println("GAME OVER");
  
  tft.setTextColor(C_YELLOW, C_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 110);
  tft.print("SCORE: ");
  tft.println(score);
  
  tft.setCursor(40, 140);
  tft.print("BEST:  ");
  tft.println(highScore);

  tft.setTextColor(C_WHITE, C_BLACK);
  tft.setTextSize(1);
  tft.setCursor(40, 200);
  tft.println("PRESS BUTTON");
  tft.setCursor(40, 215);
  tft.println("TO RESTART");
}

void setup() {
  Serial.begin(115200);
  pinMode(JOY_SW, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT); // Ensure BUZZER pin mode is configured
  tft.init();
  tft.setRotation(0);
  initGame();
  showStart();
}

void loop() {
  readJoy();
  bool btn = (digitalRead(JOY_SW) == LOW);

  if (phase == 0) {
    if (nxt != STOP || btn) {
      soundStart(); // Play startup jingle when game begins
      delay(200);
      initGame();
      phase = 1;
      drawField();
      last = millis();
    }
    return;
  }

  if (phase == 2) {
    if (btn) {
      delay(250);
      initGame();
      showStart();
    }
    return;
  }

  if (over) {
    showGameOver();
    return;
  }

  if (millis() - last >= (unsigned long)speed) {
    last = millis();
    tick();
  }
}