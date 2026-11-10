#include <Arduino.h>

static const uint32_t CONTROL_PERIOD_MS = 5;
static const uint32_t PRINT_PERIOD_MS   = 2000;

// Model state (exact per ACCEPTED executable model)
static int32_t  accumulatedMs = 0;
static bool     printGate     = false;

// Serial configuration (frozen contract)
static const uint32_t SERIAL_BAUD = 9600;

static uint32_t lastStepMs = 0;

static inline void controllerTick(int32_t io_0_ms) {
  // Updates in order:
  printGate = false;
  accumulatedMs = accumulatedMs + io_0_ms;

  if (accumulatedMs >= (int32_t)PRINT_PERIOD_MS) {
    accumulatedMs = accumulatedMs - (int32_t)PRINT_PERIOD_MS;
    printGate = true;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  // Ensure initial conditions: accumulatedMs = 0, printGate = false
  accumulatedMs = 0;
  printGate = false;

  lastStepMs = millis();
}

void loop() {
  const uint32_t now = millis();
  uint32_t elapsed = now - lastStepMs;

  // Run the control model at the configured 5ms period
  if (elapsed >= CONTROL_PERIOD_MS) {
    // Preserve "delta_since_previous_step" semantics using the actual elapsed time since last step.
    // If multiple periods elapsed, step through them to preserve deterministic control timing.
    while (elapsed >= CONTROL_PERIOD_MS) {
      // delta for this specific control step is exactly 5ms
      int32_t io_0 = (int32_t)CONTROL_PERIOD_MS;

      controllerTick(io_0);

      // Output expression: io_1 = printGate
      if (printGate) {
        // serial_text_output binding: print fixed text line
        Serial.println(F("Hello Arduino"));
      }

      lastStepMs += CONTROL_PERIOD_MS;
      elapsed = now - lastStepMs;
    }
  }
}