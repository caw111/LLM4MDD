#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t DATA_PIN = A0;
const uint8_t LATCH_PIN = A1;
const uint8_t CLOCK_PIN = A2;

const uint8_t BAR_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Bits 0..6 correspond to segments A..G on shift-register Q0..Q6.
// The display common is connected to +5V, so segments are active LOW.
const uint8_t DIGIT_PATTERNS[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

const unsigned long DEBOUNCE_MS = 25;

uint8_t displayedNumber = 0;
uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

void updateDisplays() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[displayedNumber]));
  digitalWrite(LATCH_PIN, HIGH);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < displayedNumber ? HIGH : LOW);
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  displayedNumber = 0;
  updateDisplays();

  lastRawButtonState = digitalRead(BUTTON_PIN);
  stableButtonState = lastRawButtonState;
  lastButtonChangeMs = millis();
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

    if (stableButtonState == LOW) {
      displayedNumber = (displayedNumber + 1) % 10;
      updateDisplays();
    }
  }
}