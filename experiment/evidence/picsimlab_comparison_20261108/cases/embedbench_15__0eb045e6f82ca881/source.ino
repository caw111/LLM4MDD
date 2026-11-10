#include <Arduino.h>

// Mega 2560 wiring: DS=A0, STCP=A1, SHCP=A2.
// Both displays are common-anode, so LOW turns a segment on.
constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

// Bits 0..6 correspond to segments A..G; bit 7 remains HIGH.
constexpr uint8_t DIGITS[10] = {
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
constexpr uint8_t BLANK = 0xFF;
constexpr unsigned long INTERVAL_MS = 1000UL;

uint8_t displayedSeconds = 0;
unsigned long lastTickMs = 0;

void updateDisplay(uint8_t value) {
  const uint8_t tens = (value < 10) ? BLANK : DIGITS[value / 10];
  const uint8_t ones = DIGITS[value % 10];

  digitalWrite(LATCH_PIN, LOW);
  // sr2 is downstream: send its ones digit first, then sr1's tens digit.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ones);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tens);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  updateDisplay(0); // sevseg1 blank; sevseg2 shows 0.
  lastTickMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction handles millis() rollover. Catch up without drift.
  while (now - lastTickMs >= INTERVAL_MS) {
    lastTickMs += INTERVAL_MS;
    // Keeping the total modulo 100 preserves exactly its last two digits.
    displayedSeconds = (displayedSeconds + 6) % 100;
    updateDisplay(displayedSeconds);
  }
}