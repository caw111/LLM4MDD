#include <Arduino.h>

namespace GeneratedModel {
  struct Controller {
    // Inputs
    bool io_2 = false; // btn1 (active when pressed -> LOW in hardware, but we treat as model-true when pressed)
    bool io_3 = false; // btn2
    bool io_4 = false; // btn3
    int32_t io_5 = 0;  // delta ms since previous step

    // State variables
    bool led1Stable = false;
    bool led2Stable = false;
    bool prevBtn1 = false;
    bool prevBtn2 = false;
    bool prevBtn3 = false;

    int32_t btn1DebounceMs = 0;
    int32_t btn2DebounceMs = 0;
    int32_t btn3DebounceMs = 0;

    int32_t btn1HoldMs = 0;
    int32_t btn2HoldMs = 0;
    int32_t btn3HoldMs = 0;

    void tick() {
      // order-preserving updates from model
      // Debounce accumulation
      if (io_2 ? true : false) {
        btn1DebounceMs = btn1DebounceMs + io_5;
      } else {
        btn1DebounceMs = 0;
      }

      if (io_3 ? true : false) {
        btn2DebounceMs = btn2DebounceMs + io_5;
      } else {
        btn2DebounceMs = 0;
      }

      if (io_4 ? true : false) {
        btn3DebounceMs = btn3DebounceMs + io_5;
      } else {
        btn3DebounceMs = 0;
      }

      // Hold countdowns (clamped by else branches to 0)
      if ((btn1HoldMs) >= (1)) {
        btn1HoldMs = btn1HoldMs - io_5;
      } else {
        btn1HoldMs = 0;
      }

      if ((btn2HoldMs) >= (1)) {
        btn2HoldMs = btn2HoldMs - io_5;
      } else {
        btn2HoldMs = 0;
      }

      if ((btn3HoldMs) >= (1)) {
        btn3HoldMs = btn3HoldMs - io_5;
      } else {
        btn3HoldMs = 0;
      }

      // Toggle/reset on debounced press edges
      if ((io_2) && ((not prevBtn1)) && (((btn1DebounceMs) >= (20)))) {
        led1Stable = (not led1Stable);
        btn1HoldMs = 2000;
      }

      if ((io_3) && ((not prevBtn2)) && (((btn2DebounceMs) >= (20)))) {
        led2Stable = (not led2Stable);
        btn2HoldMs = 2000;
      }

      if ((io_4) && ((not prevBtn3)) && (((btn3DebounceMs) >= (20)))) {
        led1Stable = false;
        led2Stable = false;
        btn3HoldMs = 2000;
      }

      // btn3Hold immediate effect and interactions
      if ((btn3HoldMs) >= (1)) {
        led1Stable = false;
        led2Stable = false;
      }

      // Additional effects (keep exact model order)
      if (((btn1HoldMs) >= (1)) && ((btn3HoldMs) >= (1))) {
        led1Stable = false;
      }

      if (((btn2HoldMs) >= (1)) && ((btn3HoldMs) >= (1))) {
        led2Stable = false;
      }

      // Update prevs
      prevBtn1 = io_2;
      prevBtn2 = io_3;
      prevBtn3 = io_4;
    }

    // Outputs mapping
    bool io_0() const { return led1Stable; } // led1 state
    bool io_1() const { return led2Stable; } // led2 state
  };
}

using GeneratedModel::Controller;

static Controller controller;

static constexpr uint8_t PIN_LED1 = 2; // D2
static constexpr uint8_t PIN_LED2 = 3; // D3
static constexpr uint8_t PIN_BTN1 = 4; // D4
static constexpr uint8_t PIN_BTN2 = 5; // D5
static constexpr uint8_t PIN_BTN3 = 6; // D6

static uint32_t lastTickUs = 0;
static uint32_t nextPlannedMs = 0;

static inline int32_t clampI32(int64_t v, int32_t lo, int32_t hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return (int32_t)v;
}

void setup() {
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  // Inputs use pullup, active_high=false (pressed => LOW => model true)
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);

  // Model initial state: both LEDs off
  controller.led1Stable = false;
  controller.led2Stable = false;

  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  lastTickUs = micros();
  nextPlannedMs = millis();
}

void loop() {
  // control_period_ms = 5
  // We'll schedule based on millis and compute delta from micros.
  uint32_t nowMs = millis();
  if (nowMs < nextPlannedMs) return;
  nextPlannedMs = nowMs + 5;

  uint32_t nowUs = micros();
  uint32_t dtUs = nowUs - lastTickUs;
  lastTickUs = nowUs;

  int32_t dtMs = (int32_t)(dtUs / 1000UL);
  if (dtMs <= 0) dtMs = 1; // ensure model countdown semantics progress (>=1 used)
  controller.io_5 = clampI32(dtMs, 1, 1000000);

  // Read buttons: active when pressed => input LOW => model true
  controller.io_2 = (digitalRead(PIN_BTN1) == LOW);
  controller.io_3 = (digitalRead(PIN_BTN2) == LOW);
  controller.io_4 = (digitalRead(PIN_BTN3) == LOW);

  controller.tick();

  // Outputs: active_high=true for LEDs, model true->HIGH
  digitalWrite(PIN_LED1, controller.io_0() ? HIGH : LOW);
  digitalWrite(PIN_LED2, controller.io_1() ? HIGH : LOW);
}