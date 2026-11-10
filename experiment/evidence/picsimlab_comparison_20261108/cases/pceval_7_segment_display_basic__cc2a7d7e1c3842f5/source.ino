#include <Arduino.h>

static constexpr uint8_t PIN_IO_0 = 2; // segment A
static constexpr uint8_t PIN_IO_1 = 3; // segment B
static constexpr uint8_t PIN_IO_2 = 4; // segment C
static constexpr uint8_t PIN_IO_3 = 5; // segment D
static constexpr uint8_t PIN_IO_4 = 6; // segment E
static constexpr uint8_t PIN_IO_5 = 7; // segment F
static constexpr uint8_t PIN_IO_6 = 8; // segment G
static constexpr uint8_t PIN_IO_7 = 9; // segment DP

// Accepted executable model outputs (controller state, ordered updates not applicable beyond startup):
// io_0=false, io_1=false, io_2=false, io_3=true, io_4=true, io_5=false, io_6=false, io_7=true
static constexpr bool IO_0 = false;
static constexpr bool IO_1 = false;
static constexpr bool IO_2 = false;
static constexpr bool IO_3 = true;
static constexpr bool IO_4 = true;
static constexpr bool IO_5 = false;
static constexpr bool IO_6 = false;
static constexpr bool IO_7 = true;

static inline void writeBoolToPin(uint8_t pin, bool value, bool active_high = true) {
  digitalWrite(pin, (active_high ? (value ? HIGH : LOW) : (value ? LOW : HIGH)));
}

void setup() {
  // Set 2~9 pin as output
  pinMode(PIN_IO_0, OUTPUT);
  pinMode(PIN_IO_1, OUTPUT);
  pinMode(PIN_IO_2, OUTPUT);
  pinMode(PIN_IO_3, OUTPUT);
  pinMode(PIN_IO_4, OUTPUT);
  pinMode(PIN_IO_5, OUTPUT);
  pinMode(PIN_IO_6, OUTPUT);
  pinMode(PIN_IO_7, OUTPUT);

  // Apply frozen hardware contract (active_high=true, model ON=HIGH)
  writeBoolToPin(PIN_IO_0, IO_0, true);
  writeBoolToPin(PIN_IO_1, IO_1, true);
  writeBoolToPin(PIN_IO_2, IO_2, true);
  writeBoolToPin(PIN_IO_3, IO_3, true);
  writeBoolToPin(PIN_IO_4, IO_4, true);
  writeBoolToPin(PIN_IO_5, IO_5, true);
  writeBoolToPin(PIN_IO_6, IO_6, true);
  writeBoolToPin(PIN_IO_7, IO_7, true);
}

void loop() {
  // No additional inputs/events; preserve exact controller outputs indefinitely.
  // Re-assert at control period granularity (5ms) to match "control_period_ms": 5.
  // (Behavior remains constant.)
  static unsigned long nextTick = 0;
  const unsigned long now = millis();
  if (now < nextTick) return;
  nextTick = now + 5;

  writeBoolToPin(PIN_IO_0, IO_0, true);
  writeBoolToPin(PIN_IO_1, IO_1, true);
  writeBoolToPin(PIN_IO_2, IO_2, true);
  writeBoolToPin(PIN_IO_3, IO_3, true);
  writeBoolToPin(PIN_IO_4, IO_4, true);
  writeBoolToPin(PIN_IO_5, IO_5, true);
  writeBoolToPin(PIN_IO_6, IO_6, true);
  writeBoolToPin(PIN_IO_7, IO_7, true);
}