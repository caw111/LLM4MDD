#include <Arduino.h>

static const uint8_t POT1_PIN = A3; // pot1: SIG -> mega:A3
static const uint8_t POT2_PIN = A4; // pot2: SIG -> mega:A4
static const uint8_t POT3_PIN = A5; // pot3: SIG -> mega:A5

// Section 1: first 3 LEDs (bargraph1 A1-A3)
static const uint8_t SECTION1_LEDS[3] = {2, 3, 4};  // mega:2->A1, mega:3->A2, mega:4->A3

// Section 2: next 3 LEDs (bargraph1 A4-A6)
static const uint8_t SECTION2_LEDS[3] = {5, 6, 7};  // mega:5->A4, mega:6->A5, mega:7->A6

// Section 3: last 4 LEDs (bargraph1 A7-A10)
static const uint8_t SECTION3_LEDS[4] = {8, 9, 10, 11}; // mega:8->A7, ... mega:11->A10

static uint16_t readPotClamped(uint8_t pin) {
  int v = analogRead(pin);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  return (uint16_t)v;
}

// Map 0..1023 to 0..maxLeds with inclusive behavior:
// 0 -> 0 LEDs, 1023 -> maxLeds LEDs
static uint8_t potToLeds(uint16_t potValue, uint8_t maxLeds) {
  // Use 32-bit math to avoid overflow
  uint32_t scaled = (uint32_t)potValue * (uint32_t)maxLeds; // 0..maxLeds*1023
  return (uint8_t)(scaled / 1023U); // 0..maxLeds
}

static void setSection(uint8_t maxLeds, const uint8_t *ledPins, uint8_t ledsToLight) {
  // Sequential from the first LED of the section
  for (uint8_t i = 0; i < maxLeds; i++) {
    digitalWrite(ledPins[i], (i < ledsToLight) ? HIGH : LOW);
  }
}

void setup() {
  // Ensure all LEDs are off on start/reset
  pinMode(SECTION1_LEDS[0], OUTPUT);
  pinMode(SECTION1_LEDS[1], OUTPUT);
  pinMode(SECTION1_LEDS[2], OUTPUT);

  pinMode(SECTION2_LEDS[0], OUTPUT);
  pinMode(SECTION2_LEDS[1], OUTPUT);
  pinMode(SECTION2_LEDS[2], OUTPUT);

  pinMode(SECTION3_LEDS[0], OUTPUT);
  pinMode(SECTION3_LEDS[1], OUTPUT);
  pinMode(SECTION3_LEDS[2], OUTPUT);
  pinMode(SECTION3_LEDS[3], OUTPUT);

  setSection(3, SECTION1_LEDS, 0);
  setSection(3, SECTION2_LEDS, 0);
  setSection(4, SECTION3_LEDS, 0);
}

void loop() {
  uint16_t pot1 = readPotClamped(POT1_PIN);
  uint16_t pot2 = readPotClamped(POT2_PIN);
  uint16_t pot3 = readPotClamped(POT3_PIN);

  uint8_t leds1 = potToLeds(pot1, 3); // 0-3 LEDs in Section 1 (A1-A3)
  uint8_t leds2 = potToLeds(pot2, 3); // 0-3 LEDs in Section 2 (A4-A6)
  uint8_t leds3 = potToLeds(pot3, 4); // 0-4 LEDs in Section 3 (A7-A10)

  setSection(3, SECTION1_LEDS, leds1);
  setSection(3, SECTION2_LEDS, leds2);
  setSection(4, SECTION3_LEDS, leds3);

  // Continuous update
}