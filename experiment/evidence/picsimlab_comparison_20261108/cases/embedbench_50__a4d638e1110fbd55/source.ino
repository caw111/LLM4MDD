#include <Arduino.h>

// Arduino Mega 2560 wiring; all LEDs are active HIGH.
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

const uint8_t RGB1_R = 2;
const uint8_t RGB1_G = 3;
const uint8_t RGB1_B = 4;
const uint8_t RGB2_R = 5;
const uint8_t RGB2_G = 6;
const uint8_t RGB2_B = 7;

const unsigned long SEGMENT_DURATION_MS = 2000UL;

void initializeOff(uint8_t pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

void setColors(uint8_t segment) {
  const bool firstHalf = (segment < 4);

  // Common-cathode RGB LEDs: HIGH turns a color channel on.
  digitalWrite(RGB1_R, firstHalf ? HIGH : LOW);
  digitalWrite(RGB1_G, firstHalf ? LOW : HIGH);
  digitalWrite(RGB1_B, LOW);

  digitalWrite(RGB2_R, firstHalf ? LOW : HIGH);
  digitalWrite(RGB2_G, firstHalf ? LOW : HIGH);
  digitalWrite(RGB2_B, firstHalf ? HIGH : LOW);
}

void setup() {
  // Initialize every connected LED channel to OFF.
  for (uint8_t i = 0; i < 8; ++i) {
    initializeOff(BAR_PINS[i]);
  }

  initializeOff(RGB1_R);
  initializeOff(RGB1_G);
  initializeOff(RGB1_B);
  initializeOff(RGB2_R);
  initializeOff(RGB2_G);
  initializeOff(RGB2_B);
}

void loop() {
  for (uint8_t segment = 0; segment < 8; ++segment) {
    setColors(segment);
    digitalWrite(BAR_PINS[segment], HIGH);

    delay(SEGMENT_DURATION_MS);

    // Turn this segment off before activating the next one.
    digitalWrite(BAR_PINS[segment], LOW);
  }
}