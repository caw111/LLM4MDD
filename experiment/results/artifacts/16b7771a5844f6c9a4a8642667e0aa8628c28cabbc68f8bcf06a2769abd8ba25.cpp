#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_108\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED1_PIN = 2;

// RGB1 pins
static const uint8_t RGB1_R = 3;
static const uint8_t RGB1_G = 4;
static const uint8_t RGB1_B = 5;

// RGB2 pins
static const uint8_t RGB2_R = 6;
static const uint8_t RGB2_G = 7;
static const uint8_t RGB2_B = 8;

// RGB3 pins
static const uint8_t RGB3_R = 9;
static const uint8_t RGB3_G = 10;
static const uint8_t RGB3_B = 11;

struct Color {
  uint8_t r, g, b;
};

static const Color sequence[] = {
  {255,   0,   0}, // Red
  {  0, 255,   0}, // Green
  {  0,   0, 255}, // Blue
  {255, 255,   0}, // Yellow
  {  0, 255, 255}, // Cyan
  {255,   0, 255}, // Magenta
  {255, 255, 255}  // White
};
static const size_t SEQ_LEN = sizeof(sequence) / sizeof(sequence[0]);

// Common cathode LEDs => logic HIGH turns LED ON, LOW turns LED OFF.

#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_108\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, const Color &c);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_108\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_108\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_108\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, const Color &c) {
  digitalWrite(rPin, c.r ? HIGH : LOW);
  digitalWrite(gPin, c.g ? HIGH : LOW);
  digitalWrite(bPin, c.b ? HIGH : LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW); // Initial state: off

  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT); pinMode(RGB3_G, OUTPUT); pinMode(RGB3_B, OUTPUT);

  // Initial state: first color (Red)
  const Color &first = sequence[0];
  setRGB(RGB1_R, RGB1_G, RGB1_B, first);
  setRGB(RGB2_R, RGB2_G, RGB2_B, first);
  setRGB(RGB3_R, RGB3_G, RGB3_B, first);
}

void loop() {
  static unsigned long lastBlinkToggleMs = 0;
  static bool led1On = false;

  static unsigned long lastColorChangeMs = 0;
  static size_t colorIndex = 0;

  const unsigned long now = millis();

  // Single-color LED blink: 2s on, 2s off indefinitely
  if (now - lastBlinkToggleMs >= 2000UL) {
    lastBlinkToggleMs = now;
    led1On = !led1On;
    digitalWrite(LED1_PIN, led1On ? HIGH : LOW);
  }

  // RGB LEDs: synchronous color sequence, each displayed for 2s
  if (now - lastColorChangeMs >= 2000UL) {
    lastColorChangeMs = now;
    colorIndex = (colorIndex + 1) % SEQ_LEN;
    const Color &c = sequence[colorIndex];

    setRGB(RGB1_R, RGB1_G, RGB1_B, c);
    setRGB(RGB2_R, RGB2_G, RGB2_B, c);
    setRGB(RGB3_R, RGB3_G, RGB3_B, c);
  }
}
