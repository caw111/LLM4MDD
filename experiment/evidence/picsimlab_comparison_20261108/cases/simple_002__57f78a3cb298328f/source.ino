#include <Arduino.h>

static const uint8_t LED_PIN = 12;

// Frozen model interfaces
// io_1: i32, unit ms, delta_since_previous_step
// io_0: bool, LED state (active HIGH)
static int32_t accumulatedMs = 0; // initial 0
static bool ledOn = false;        // initial false

// Elapsed timing implementation (control_period_ms = 5ms)
static unsigned long lastStepMs = 0;

static inline void model_tick(int32_t io_1_delta_ms) {
  // Controller action tick (exact update ordering as provided)
  accumulatedMs = accumulatedMs + io_1_delta_ms;

  if (accumulatedMs >= 500) {
    accumulatedMs = accumulatedMs - 500;
    ledOn = !ledOn;
  }

  if (accumulatedMs >= 500) {
    accumulatedMs = accumulatedMs - 500;
    ledOn = !ledOn;
  }

  // Output assignment: io_0 = ledOn
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // initial OFF

  accumulatedMs = 0;
  ledOn = false;

  lastStepMs = millis();
  // Perform no initial tick; first tick occurs when control period elapses.
}

void loop() {
  unsigned long now = millis();
  // control_period_ms = 5
  if ((unsigned long)(now - lastStepMs) >= 5UL) {
    // Maintain delta_since_previous_step via actual elapsed time.
    // Note: With scheduling every 5ms, this stays aligned within tolerance.
    unsigned long elapsed = now - lastStepMs;
    lastStepMs = now;

    // Clamp to i32-safe range; elapsed from millis() is within uint32 range.
    int32_t io_1_delta_ms = (elapsed > 0x7FFFFFFFUL) ? 0x7FFFFFFF : (int32_t)elapsed;

    model_tick(io_1_delta_ms);
  }
}