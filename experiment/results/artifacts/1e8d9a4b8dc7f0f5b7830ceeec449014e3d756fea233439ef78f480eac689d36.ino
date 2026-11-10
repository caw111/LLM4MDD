#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // Shared STCP
constexpr uint8_t CLOCK_PIN = A2;  // Shared SHCP
constexpr uint8_t POT_PIN   = A3;

// Common-anode displays: LOW illuminates a segment.
// Bits 0..6 correspond to A..G; bit 7 stays HIGH (unused).
constexpr uint8_t DIGIT_PATTERNS[10] = {
  0xC0, // 0
  0xF9, // 1
  0xA4, // 2
  0xB0, // 3
  0x99, // 4
  0x92, // 5
  0x82, // 6
  0xF8, // 7
  0x80, // 8
  0x90  // 9
};

int lastDisplayed = -1;

void displayNumber(int number) {
  if (number < 0 || number > 99) {
    number = 0;
  }

  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // First byte travels through sr1 into sr2 (units).
  // Second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  digitalWrite(LATCH_PIN, HIGH);
  lastDisplayed = number;
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  analogReference(DEFAULT);

  displayNumber(0);
  delay(250); // Make the required startup "00" visible.
}

void loop() {
  const int rawValue = analogRead(POT_PIN);
  long number = map(rawValue, 0, 1023, 0, 99);

  if (number < 0 || number > 99) {
    number = 0;
  }

  if (number != lastDisplayed) {
    displayNumber(static_cast<int>(number));
  }

  delay(10);
}