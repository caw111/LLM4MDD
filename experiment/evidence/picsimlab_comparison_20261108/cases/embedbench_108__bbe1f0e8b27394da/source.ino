#include <Arduino.h>

static const uint8_t LED1_PIN = 2;

static const uint8_t RGB1_R = 3;
static const uint8_t RGB1_G = 4;
static const uint8_t RGB1_B = 5;

static const uint8_t RGB2_R = 6;
static const uint8_t RGB2_G = 7;
static const uint8_t RGB2_B = 8;

static const uint8_t RGB3_R = 9;
static const uint8_t RGB3_G = 10;
static const uint8_t RGB3_B = 11;

struct Color {
  uint8_t r, g, b;
};

static const Color sequence[] = {
  {255, 0,   0},   // Red
  {0,   255, 0},   // Green
  {0,   0,   255}, // Blue
  {255, 255, 0},   // Yellow
  {0,   255, 255}, // Cyan
  {255, 0,   255}, // Magenta
  {255, 255, 255}  // White
};

static const uint8_t SEQ_LEN = sizeof(sequence) / sizeof(sequence[0]);

void setRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: HIGH turns LED ON, LOW turns OFF
  digitalWrite(rPin, r ? HIGH : LOW);
  digitalWrite(gPin, g ? HIGH : LOW);
  digitalWrite(bPin, b ? HIGH : LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  // Initial state: led1 off
  digitalWrite(LED1_PIN, LOW);

  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  // Initial state: RGBs show first color (Red)
  setRGB(RGB1_R, RGB1_G, RGB1_B, sequence[0].r, sequence[0].g, sequence[0].b);
  setRGB(RGB2_R, RGB2_G, RGB2_B, sequence[0].r, sequence[0].g, sequence[0].b);
  setRGB(RGB3_R, RGB3_G, RGB3_B, sequence[0].r, sequence[0].g, sequence[0].b);
}

void loop() {
  static unsigned long lastLedToggleMs = 0;
  static bool led1StateOn = false;

  static unsigned long lastColorChangeMs = 0;
  static uint8_t colorIndex = 0;

  const unsigned long intervalLed = 2000UL;    // 2 seconds on/off
  const unsigned long intervalRgb = 2000UL;    // 2 seconds per color

  unsigned long now = millis();

  // led1 blinking (independent, simultaneous)
  if (now - lastLedToggleMs >= intervalLed) {
    lastLedToggleMs = now;
    led1StateOn = !led1StateOn;
    digitalWrite(LED1_PIN, led1StateOn ? HIGH : LOW);
  }

  // RGB synchronous cycling (independent, simultaneous)
  if (now - lastColorChangeMs >= intervalRgb) {
    lastColorChangeMs = now;
    colorIndex = (colorIndex + 1) % SEQ_LEN;

    const Color &c = sequence[colorIndex];
    setRGB(RGB1_R, RGB1_G, RGB1_B, c.r, c.g, c.b);
    setRGB(RGB2_R, RGB2_G, RGB2_B, c.r, c.g, c.b);
    setRGB(RGB3_R, RGB3_G, RGB3_B, c.r, c.g, c.b);
  }
}