#include <Arduino.h>

namespace Model {
  // Frozen controller state
  static bool previousPressed = true;

  // Inputs/outputs
  static bool io_0 = true;  // D2 with INPUT_PULLUP: HIGH means released
  static bool io_1 = false; // D13 LED: HIGH means ON

  // control_period_ms = 5 (used for scheduling in loop())
  static const uint32_t CONTROL_PERIOD_US = 5000;

  inline void tickOnce() {
    // Apply "perform action tick" semantics, preserving exact structure:
    // if (io_0 and (not previousPressed)) then assign previousPressed := io_0
    // else assign previousPressed := io_0
    if (io_0 && (!previousPressed)) {
      previousPressed = io_0;
    } else {
      previousPressed = io_0;
    }

    // out attribute io_1 : Boolean = io_0;
    io_1 = io_0;
  }
} // namespace Model

// Hardware mapping (authoritative)
static const uint8_t PIN_D2  = 2;   // button input (INPUT_PULLUP)
static const uint8_t PIN_D13 = 13;  // LED output

void setup() {
  pinMode(PIN_D2, INPUT_PULLUP);  // low means pressed
  pinMode(PIN_D13, OUTPUT);

  // Ensure initial output consistent with model initialization.
  // Model: previousPressed=true initial, io_0 should be read at startup.
  // Output io_1 = io_0 after first tick; however we also set immediate output
  // from current pin to reduce latency at startup without violating model behavior.
  bool initial_io_0 = (digitalRead(PIN_D2) == HIGH);
  Model::io_0 = initial_io_0;
  Model::io_1 = Model::io_0;
  digitalWrite(PIN_D13, Model::io_1 ? HIGH : LOW);
}

void loop() {
  // 5ms periodic scheduling to satisfy <=20ms response time.
  static uint32_t nextTickUs = 0;
  uint32_t nowUs = micros();
  if (nextTickUs == 0) nextTickUs = nowUs;

  // Keep ticking until we're caught up, but still bounded naturally by small period.
  while ((int32_t)(nowUs - nextTickUs) >= 0) {
    // Sample input
    // io_0 is Boolean corresponding to pin level (HIGH = true/released, LOW = false/pressed)
    Model::io_0 = (digitalRead(PIN_D2) == HIGH);

    // Execute model tick (exact state update + output expression)
    Model::tickOnce();

    // Drive output
    digitalWrite(PIN_D13, Model::io_1 ? HIGH : LOW);

    // Advance schedule
    nextTickUs += Model::CONTROL_PERIOD_US;
  }
}