#include <Arduino.h>

// Arduino Mega 2560 wiring.
constexpr uint8_t RGB_RED_PIN   = 2;
constexpr uint8_t RGB_GREEN_PIN = 3;
constexpr uint8_t RGB_BLUE_PIN  = 4;

constexpr uint8_t SR_DATA_PIN  = A0; // DS
constexpr uint8_t SR_LATCH_PIN = A1; // STCP
constexpr uint8_t SR_CLOCK_PIN = A2; // SHCP

// Bar graph anodes A1 through A10, in order.
constexpr uint8_t BAR_PINS[10] = {
  5, 6, 7, 8, 9, 10, 11, 12, 13, 44
};

// Bits 0..6 represent segments A..G; bit 7 is unused.
// These masks are inverted before output because COM.1 is wired to +5V.
constexpr uint8_t DIGIT_MASKS[10] = {
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

constexpr unsigned long UPDATE_INTERVAL_MS = 2000UL;

uint8_t currentNumber = 0;
unsigned long lastUpdateMs = 0;

void updateDisplays(uint8_t number) {
  const uint8_t segmentOutputs =
      static_cast<uint8_t>(~DIGIT_MASKS[number]);

  digitalWrite(SR_LATCH_PIN, LOW);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, MSBFIRST, segmentOutputs);
  digitalWrite(SR_LATCH_PIN, HIGH);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < number ? HIGH : LOW);
  }

  // Common-cathode RGB LED: HIGH turns a color on.
  digitalWrite(RGB_RED_PIN,   number <= 3 ? HIGH : LOW);
  digitalWrite(RGB_GREEN_PIN,
               (number >= 4 && number <= 6) ? HIGH : LOW);
  digitalWrite(RGB_BLUE_PIN,  number >= 7 ? HIGH : LOW);
}

void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  currentNumber = 0;
  updateDisplays(currentNumber);
  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction remains correct across millis() rollover.
  if (now - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    lastUpdateMs += UPDATE_INTERVAL_MS;
    currentNumber = (currentNumber + 1) % 10;
    updateDisplays(currentNumber);
  }
}