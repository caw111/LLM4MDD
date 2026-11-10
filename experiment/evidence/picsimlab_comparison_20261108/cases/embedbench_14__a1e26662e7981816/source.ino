#include <Arduino.h>

// Arduino Mega 2560 -> 74HC595
constexpr uint8_t DATA_PIN  = A0; // DS
constexpr uint8_t LATCH_PIN = A1; // STCP
constexpr uint8_t CLOCK_PIN = A2; // SHCP

// Q0..Q6 connect to segments A..G.
// COM is connected to +5V, so segments are active LOW.
// Q7 is unused and is kept HIGH.
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

constexpr uint8_t DISPLAY_OFF = 0xFF;
constexpr uint32_t TICK_MS = 1000UL;

uint64_t timerSeconds = 0;
uint32_t lastTickMs = 0;
uint8_t displayedPattern = DISPLAY_OFF;

void writeDisplay(uint8_t pattern) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
  digitalWrite(LATCH_PIN, HIGH);
  displayedPattern = pattern;
}

void setup() {
  digitalWrite(DATA_PIN, HIGH);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  writeDisplay(DISPLAY_OFF);
  lastTickMs = millis();
}

void loop() {
  const uint32_t now = millis();
  const uint32_t elapsed = now - lastTickMs;

  if (elapsed >= TICK_MS) {
    const uint32_t ticks = elapsed / TICK_MS;
    lastTickMs += ticks * TICK_MS;
    timerSeconds += static_cast<uint64_t>(ticks) * 2ULL;
  }

  // Remain blank until the first real-time second has elapsed.
  if (timerSeconds == 0) {
    return;
  }

  uint8_t digit;

  if (timerSeconds == 10) {
    // A single digit cannot show "10" simultaneously. Present both
    // digits in order during this second: "1", followed by "0".
    digit = (static_cast<uint32_t>(now - lastTickMs) < 500UL) ? 1 : 0;
  } else if (timerSeconds > 10) {
    digit = static_cast<uint8_t>(timerSeconds % 10ULL);
  } else {
    digit = static_cast<uint8_t>(timerSeconds);
  }

  const uint8_t pattern = DIGIT_PATTERNS[digit];
  if (pattern != displayedPattern) {
    writeDisplay(pattern);
  }
}