#include <Arduino.h>

// Arduino Mega 2560 wiring.
const uint8_t BUTTON_PINS[] = {2, 3, 4};
const uint8_t BUTTON_COUNT = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);

const uint8_t RGB1_PINS[] = {5, 6, 7};   // R, G, B; common cathode.
const uint8_t RGB2_PINS[] = {8, 9, 10};  // R, G, B; common cathode.

const uint8_t SHIFT_DATA_PIN = A0;
const uint8_t SHIFT_LATCH_PIN = A1;
const uint8_t SHIFT_CLOCK_PIN = A2;

const unsigned long DEBOUNCE_MS = 25;

// Bit 0 = A, bit 1 = B, ..., bit 6 = G, bit 7 = decimal point.
// These are active-high glyphs; invert for the common-anode display.
const uint8_t DIGIT_SEGMENTS[10] = {
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

struct ButtonState {
  uint8_t rawState;
  uint8_t stableState;
  unsigned long lastChangeMs;
};

ButtonState buttons[BUTTON_COUNT];
uint8_t count = 0;

void setRGB(const uint8_t pins[], bool red, bool green, bool blue) {
  digitalWrite(pins[0], red ? HIGH : LOW);
  digitalWrite(pins[1], green ? HIGH : LOW);
  digitalWrite(pins[2], blue ? HIGH : LOW);
}

void updateOutputs() {
  // COM.1 is connected to +5 V, so segment outputs are active LOW.
  const uint8_t segmentData =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[count % 10]);

  digitalWrite(SHIFT_LATCH_PIN, LOW);
  shiftOut(SHIFT_DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, segmentData);
  digitalWrite(SHIFT_LATCH_PIN, HIGH);

  // Zero is the initial/reset state, including after wrapping from 15.
  if (count == 0) {
    setRGB(RGB1_PINS, false, false, false);
    setRGB(RGB2_PINS, false, false, false);
  } else if (count < 5) {
    setRGB(RGB1_PINS, true, false, false);  // Red
    setRGB(RGB2_PINS, false, false, true);  // Blue
  } else if (count < 10) {
    setRGB(RGB1_PINS, false, true, false);  // Green
    setRGB(RGB2_PINS, true, true, false);   // Yellow
  } else {
    setRGB(RGB1_PINS, true, false, true);   // Purple
    setRGB(RGB2_PINS, false, true, true);   // Cyan
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(RGB1_PINS[i], LOW);
    digitalWrite(RGB2_PINS[i], LOW);
    pinMode(RGB1_PINS[i], OUTPUT);
    pinMode(RGB2_PINS[i], OUTPUT);
  }

  digitalWrite(SHIFT_DATA_PIN, LOW);
  digitalWrite(SHIFT_CLOCK_PIN, LOW);
  digitalWrite(SHIFT_LATCH_PIN, LOW);
  pinMode(SHIFT_DATA_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_LATCH_PIN, OUTPUT);

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    buttons[i].rawState = digitalRead(BUTTON_PINS[i]);
    buttons[i].stableState = HIGH;
    buttons[i].lastChangeMs = millis();
  }

  count = 0;
  updateOutputs();
}

void loop() {
  const unsigned long now = millis();

  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    const uint8_t reading = digitalRead(BUTTON_PINS[i]);

    if (reading != buttons[i].rawState) {
      buttons[i].rawState = reading;
      buttons[i].lastChangeMs = now;
    }

    if (reading != buttons[i].stableState &&
        now - buttons[i].lastChangeMs >= DEBOUNCE_MS) {
      buttons[i].stableState = reading;

      // Count only debounced press edges, never releases or held buttons.
      if (reading == LOW) {
        count = (count == 15) ? 0 : count + 1;
        updateOutputs();
      }
    }
  }
}