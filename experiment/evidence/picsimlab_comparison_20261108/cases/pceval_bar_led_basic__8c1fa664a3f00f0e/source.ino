#include <Arduino.h>

static constexpr uint8_t PIN_D2  = 2;  // io_0_segment_0
static constexpr uint8_t PIN_D3  = 3;  // io_0_segment_1
static constexpr uint8_t PIN_D4  = 4;  // io_0_segment_2
static constexpr uint8_t PIN_D5  = 5;  // io_0_segment_3
static constexpr uint8_t PIN_D6  = 6;  // io_0_segment_4
static constexpr uint8_t PIN_D7  = 7;  // io_0_segment_5
static constexpr uint8_t PIN_D8  = 8;  // io_0_segment_6
static constexpr uint8_t PIN_D9  = 9;  // io_0_segment_7
static constexpr uint8_t PIN_D10 = 10; // io_0_segment_8
static constexpr uint8_t PIN_D11 = 11; // io_0_segment_9

// ACCEPTED executable model outputs (startup, constant)
static constexpr bool io_0_segment_0 = true;
static constexpr bool io_0_segment_1 = true;
static constexpr bool io_0_segment_2 = true;
static constexpr bool io_0_segment_3 = true;
static constexpr bool io_0_segment_4 = true;
static constexpr bool io_0_segment_5 = true;
static constexpr bool io_0_segment_6 = false;
static constexpr bool io_0_segment_7 = false;
static constexpr bool io_0_segment_8 = false;
static constexpr bool io_0_segment_9 = false;

static inline void applyOutput(uint8_t pin, bool activeHigh, bool modelValue) {
  // active_high=true => model true => HIGH, model false => LOW
  digitalWrite(pin, (activeHigh ? modelValue : !modelValue) ? HIGH : LOW);
}

static void applyAllOutputs() {
  constexpr bool activeHigh = true;

  // ios io_0_segment_0..9 bound to D2..D11 respectively
  applyOutput(PIN_D2,  activeHigh, io_0_segment_0);
  applyOutput(PIN_D3,  activeHigh, io_0_segment_1);
  applyOutput(PIN_D4,  activeHigh, io_0_segment_2);
  applyOutput(PIN_D5,  activeHigh, io_0_segment_3);
  applyOutput(PIN_D6,  activeHigh, io_0_segment_4);
  applyOutput(PIN_D7,  activeHigh, io_0_segment_5);
  applyOutput(PIN_D8,  activeHigh, io_0_segment_6);
  applyOutput(PIN_D9,  activeHigh, io_0_segment_7);
  applyOutput(PIN_D10, activeHigh, io_0_segment_8);
  applyOutput(PIN_D11, activeHigh, io_0_segment_9);
}

void setup() {
  // Contract: set pins D2..D11 as OUTPUT
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);
  pinMode(PIN_D4, OUTPUT);
  pinMode(PIN_D5, OUTPUT);
  pinMode(PIN_D6, OUTPUT);
  pinMode(PIN_D7, OUTPUT);
  pinMode(PIN_D8, OUTPUT);
  pinMode(PIN_D9, OUTPUT);
  pinMode(PIN_D10, OUTPUT);
  pinMode(PIN_D11, OUTPUT);

  // Contract: LEDs must turn on immediately during setup and remain in pattern
  applyAllOutputs();
}

void loop() {
  // No actions required; behavior is constant.
  // Intentionally empty to preserve exact startup-only behavior.
}