#include <Arduino.h>

// Pin mapping per provided wiring
// Regular LED (led1)
static const uint8_t LED1_PIN = 2;   // mega:2 -> led1:A (red)

// RGB LEDs (common cathode)
static const uint8_t RGB1_R = 3;       // mega:3 -> rgb1:R
static const uint8_t RGB1_G = 4;       // mega:4 -> rgb1:G
static const uint8_t RGB1_B = 5;       // mega:5 -> rgb1:B

static const uint8_t RGB2_R = 6;       // mega:6 -> rgb2:R
static const uint8_t RGB2_G = 7;       // mega:7 -> rgb2:G
static const uint8_t RGB2_B = 8;       // mega:8 -> rgb2:B

static const uint8_t RGB3_R = 9;       // mega:9 -> rgb3:R
static const uint8_t RGB3_G = 10;      // mega:10 -> rgb3:G
static const uint8_t RGB3_B = 11;      // mega:11 -> rgb3:B

// Bar graph segments (4 segments)
static const uint8_t BAR_A1 = 12;      // mega:12 -> bargraph1:A1
static const uint8_t BAR_A2 = 13;      // mega:13 -> bargraph1:A2
static const uint8_t BAR_A3 = 44;      // mega:44 -> bargraph1:A3
static const uint8_t BAR_A4 = 45;      // mega:45 -> bargraph1:A4

static inline void setRGBCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                       bool rOn, bool gOn, bool bOn) {
  // Common cathode: driving an output HIGH turns that color ON (as expected with typical wiring).
  // (COM is already tied to GND in the circuit.)
  digitalWrite(rPin, rOn ? HIGH : LOW);
  digitalWrite(gPin, gOn ? HIGH : LOW);
  digitalWrite(bPin, bOn ? HIGH : LOW);
}

static inline void allOff() {
  digitalWrite(LED1_PIN, LOW);

  // RGBs off
  setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, false);
  setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, false, false, false);

  // Bar graph off
  digitalWrite(BAR_A1, LOW);
  digitalWrite(BAR_A2, LOW);
  digitalWrite(BAR_A3, LOW);
  digitalWrite(BAR_A4, LOW);
}

static inline void setBarSegments(uint8_t segments) {
  // segments: 0..4
  digitalWrite(BAR_A1, (segments >= 1) ? HIGH : LOW);
  digitalWrite(BAR_A2, (segments >= 2) ? HIGH : LOW);
  digitalWrite(BAR_A3, (segments >= 3) ? HIGH : LOW);
  digitalWrite(BAR_A4, (segments >= 4) ? HIGH : LOW);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);

  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(RGB3_R, OUTPUT);
  pinMode(RGB3_G, OUTPUT);
  pinMode(RGB3_B, OUTPUT);

  pinMode(BAR_A1, OUTPUT);
  pinMode(BAR_A2, OUTPUT);
  pinMode(BAR_A3, OUTPUT);
  pinMode(BAR_A4, OUTPUT);

  // Initialization: all LEDs off
  allOff();
}

void loop() {
  const unsigned long stageDurationMs = 2000UL; // each stage lasts exactly 2 seconds

  unsigned long t = millis();
  uint8_t stage = (t / stageDurationMs) % 4; // Stage 1..4 repeating

  // Ensure deterministic per-stage behavior
  allOff();

  switch (stage) {
    case 0: // Stage 1 (0-2s)
      // regular led off already
      // RGB1 red, RGB2 off, RGB3 off
      setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, true, false, false);
      setBarSegments(1);
      break;

    case 1: // Stage 2 (2-4s)
      // regular led on
      digitalWrite(LED1_PIN, HIGH);
      // RGB1 green, RGB2 blue, RGB3 off
      setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, true, false);
      setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, true);
      setBarSegments(2);
      break;

    case 2: // Stage 3 (4-6s)
      // regular led off already
      // RGB1 blue, RGB2 red, RGB3 green
      setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, true);
      setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, true, false, false);
      setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, false, true, false);
      setBarSegments(3);
      break;

    case 3: // Stage 4 (6-8s)
      // regular led on
      digitalWrite(LED1_PIN, HIGH);
      // All RGB LEDs white (all colors activated)
      setRGBCommonCathode(RGB1_R, RGB1_G, RGB1_B, true, true, true);
      setRGBCommonCathode(RGB2_R, RGB2_G, RGB2_B, true, true, true);
      setRGBCommonCathode(RGB3_R, RGB3_G, RGB3_B, true, true, true);
      setBarSegments(4);
      break;
  }
}