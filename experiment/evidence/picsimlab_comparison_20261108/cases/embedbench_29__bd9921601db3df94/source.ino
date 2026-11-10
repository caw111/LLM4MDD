#include <Arduino.h>

const uint8_t POT1_PIN = A3;  // per wiring: pot1:SIG -> mega:A3
const uint8_t POT2_PIN = A4;  // per wiring: pot2:SIG -> mega:A4
const uint8_t POT3_PIN = A5;  // per wiring: pot3:SIG -> mega:A5

// Bargraph1 LED pins per wiring (A1-A3, A4-A6, A7-A10 are cathodes/anodes depending on module wiring;
// we follow the given pin connections exactly and drive them as outputs.)
const uint8_t SECTION1_PINS[3] = {A1, A2, A3};         // pot1 -> first 3 LEDs
const uint8_t SECTION2_PINS[3] = {A4, A5, A6};         // pot2 -> next 3 LEDs
const uint8_t SECTION3_PINS[4] = {A7, A8, A9, A10};     // pot3 -> last 4 LEDs

static int mapPotToCount(int potValue, int maxLeds) {
  // Map 0-1023 to 0-maxLeds inclusive
  // Use rounding down so that 1023 maps to maxLeds.
  return (potValue * (long)maxLeds) / 1023;
}

void setup() {
  // Ensure all LEDs are off at start/reset
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(SECTION1_PINS[i], OUTPUT);
    digitalWrite(SECTION1_PINS[i], LOW);
  }
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(SECTION2_PINS[i], OUTPUT);
    digitalWrite(SECTION2_PINS[i], LOW);
  }
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(SECTION3_PINS[i], OUTPUT);
    digitalWrite(SECTION3_PINS[i], LOW);
  }

  // Potentiometers analog inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);
}

void loop() {
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);
  int pot3 = analogRead(POT3_PIN);

  int count1 = mapPotToCount(pot1, 3); // 0..3
  int count2 = mapPotToCount(pot2, 3); // 0..3
  int count3 = mapPotToCount(pot3, 4); // 0..4

  // Section 1: sequential from first LED
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(SECTION1_PINS[i], (i < (uint8_t)count1) ? HIGH : LOW);
  }

  // Section 2: sequential from first LED of the section
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(SECTION2_PINS[i], (i < (uint8_t)count2) ? HIGH : LOW);
  }

  // Section 3: sequential from first LED of the section
  for (uint8_t i = 0; i < 4; i++) {
    digitalWrite(SECTION3_PINS[i], (i < (uint8_t)count3) ? HIGH : LOW);
  }

  // Continuous update behavior
}