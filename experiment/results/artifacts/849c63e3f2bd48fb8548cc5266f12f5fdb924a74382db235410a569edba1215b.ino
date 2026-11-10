#include <Arduino.h>

// Arduino Mega 2560 wiring to the 74HC595.
constexpr uint8_t DATA_PIN   = A0;  // DS
constexpr uint8_t LATCH_PIN  = A1;  // STCP
constexpr uint8_t CLOCK_PIN  = A2;  // SHCP
constexpr uint8_t BUTTON_PIN = 2;   // Button connects to GND when pressed

constexpr unsigned long DEBOUNCE_MS = 30;

// Q0..Q6 drive segments A..G. COM is wired to +5V (common anode),
// so LOW illuminates a segment. Q7 is unused and kept HIGH.
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

uint8_t count = 0;
uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void displayDigit(uint8_t digit) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[digit]);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  displayDigit(count);

  lastRawButtonState = digitalRead(BUTTON_PIN);
  lastButtonChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= DEBOUNCE_MS &&
      rawButtonState != stableButtonState) {
    stableButtonState = rawButtonState;

    // Count only debounced press edges, not releases or a held button.
    if (stableButtonState == LOW) {
      count = (count >= 9) ? 0 : count + 3;
      displayDigit(count);
    }
  }
}