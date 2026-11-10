#include <Arduino.h>

// Bargraph pins (A1 to A8)
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

// RGB1 pins (R, G, B)
const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;

// RGB2 pins (R, G, B)
const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

// Common cathode LEDs:
// - HIGH turns a color ON
// - LOW turns a color OFF
static void setRgbCommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                  bool rOn, bool gOn, bool bOn) {
  digitalWrite(rPin, rOn ? HIGH : LOW);
  digitalWrite(gPin, gOn ? HIGH : LOW);
  digitalWrite(bPin, bOn ? HIGH : LOW);
}

static void allOff() {
  // Bargraph off
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_PINS[i], LOW);
  }

  // RGB off
  setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, false);
}

void setup() {
  // Bargraph pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // RGB pins
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  allOff();
}

void loop() {
  static uint8_t idx = 0;
  static unsigned long lastChange = 0;

  unsigned long now = millis();
  if (now - lastChange >= 2000UL) {
    lastChange = now;

    // Turn off all bargraph LEDs before turning on the next one
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(BAR_PINS[i], LOW);
    }

    // Determine which bar LED is active
    uint8_t activePin = idx;
    digitalWrite(BAR_PINS[activePin], HIGH);

    // RGB behavior based on active segment A1-A4 vs A5-A8
    // idx: 0..3 => A1..A4, idx: 4..7 => A5..A8
    if (activePin <= 3) {
      // rgb1 red, rgb2 blue
      setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, true, false, false);  // Red
      setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, false, false, true);  // Blue
    } else {
      // rgb1 green, rgb2 yellow (red + green)
      setRgbCommonCathode(RGB1_R, RGB1_G, RGB1_B, false, true, false);  // Green
      setRgbCommonCathode(RGB2_R, RGB2_G, RGB2_B, true, true, false);   // Yellow
    }

    idx++;
    if (idx >= 8) idx = 0;
  }
}