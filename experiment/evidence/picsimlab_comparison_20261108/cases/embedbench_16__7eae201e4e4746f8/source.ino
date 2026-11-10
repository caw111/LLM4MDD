#include <Arduino.h>

constexpr uint8_t DATA_PIN   = A0;
constexpr uint8_t CLOCK_PIN  = A2;
constexpr uint8_t LATCH_PIN  = A1;
constexpr uint8_t BUTTON_PIN = 2;

constexpr unsigned long DEBOUNCE_MS = 10;

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
// Common-anode displays require LOW to illuminate a segment.
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

uint8_t displayedNumber = 0;
uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void updateDisplays() {
  const uint8_t tens = displayedNumber / 10;
  const uint8_t units = displayedNumber % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2 (units).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  // The second byte remains in sr1 (tens).
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  updateDisplays(); // Initially show "00".
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (rawButtonState != stableButtonState &&
      now - lastButtonChangeMs >= DEBOUNCE_MS) {
    stableButtonState = rawButtonState;

    // Act only on a debounced press, never on release or while held.
    if (stableButtonState == LOW) {
      const uint16_t nextNumber = displayedNumber + 37;
      displayedNumber = (nextNumber > 99) ? 0 : nextNumber;
      updateDisplays();
    }
  }
}