#include <Arduino.h>

// Arduino Mega 2560: preserve the supplied wiring.
constexpr uint8_t DATA_PIN  = A0;  // sr1 DS
constexpr uint8_t LATCH_PIN = A1;  // Shared STCP
constexpr uint8_t CLOCK_PIN = A2;  // Shared SHCP

// Common-cathode RGB LEDs: HIGH turns a channel on.
constexpr uint8_t RGB_PINS[3][3] = {
  {2, 3, 4},   // rgb1: R, G, B
  {5, 6, 7},   // rgb2: R, G, B
  {8, 9, 10}   // rgb3: R, G, B
};

// Bits 0..8 correspond to rgb1 R/G/B, rgb2 R/G/B, rgb3 R/G/B.
constexpr uint16_t COLOR_MASKS[] = {
  (1U << 0),              // Red: rgb1
  (1U << 4),              // Green: rgb2
  (1U << 8),              // Blue: rgb3
  (1U << 0) | (1U << 2),  // Purple: rgb1
  (1U << 3) | (1U << 4),  // Yellow: rgb2
  (1U << 7) | (1U << 8)   // Cyan: rgb3
};

constexpr uint8_t COLOR_COUNT =
    sizeof(COLOR_MASKS) / sizeof(COLOR_MASKS[0]);

// Active-high reference patterns: bits 0..6 represent segments A..G.
// Display commons are wired to +5V, so actual outputs are inverted.
constexpr uint8_t DIGIT_SEGMENTS[10] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};

constexpr unsigned long STEP_INTERVAL_MS = 2000UL;

uint8_t countdown = 99;
uint8_t colorIndex = 0;
unsigned long lastStepMs = 0;

void showCountdown(uint8_t value) {
  const uint8_t tens = value / 10;
  const uint8_t ones = value % 10;

  const uint8_t tensOutputs =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[tens]);
  const uint8_t onesOutputs =
      static_cast<uint8_t>(~DIGIT_SEGMENTS[ones]);

  digitalWrite(LATCH_PIN, LOW);

  // sr1 Q7S feeds sr2 DS: send sr2 (right/ones) first.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, onesOutputs);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, tensOutputs);

  // Both displays update together.
  digitalWrite(LATCH_PIN, HIGH);
}

void showColor(uint8_t index) {
  // Clear all channels before enabling the selected color.
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
    }
  }

  const uint16_t mask = COLOR_MASKS[index];
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      const uint8_t bitIndex = led * 3 + channel;
      if (mask & (1U << bitIndex)) {
        digitalWrite(RGB_PINS[led][channel], HIGH);
      }
    }
  }
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  showCountdown(countdown);
  showColor(colorIndex);
  lastStepMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction handles millis() rollover.
  while (now - lastStepMs >= STEP_INTERVAL_MS) {
    lastStepMs += STEP_INTERVAL_MS;

    --countdown;

    if (countdown == 0) {
      // Reset immediately upon reaching zero, without a separate zero dwell.
      countdown = 99;
      colorIndex = 0;
    } else {
      colorIndex = (colorIndex + 1) % COLOR_COUNT;
    }

    showCountdown(countdown);
    showColor(colorIndex);
  }
}