#include <Arduino.h>

// Frozen interface contract (control_period_ms = 5)
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Pin bindings (authoritative)
static constexpr uint8_t PIN_D3 = 3; // digital_input io_0
static constexpr uint8_t PIN_D6 = 6; // tone_output io_1 (treated as silence/constant frequency)

// Controller state (exactly as model bundle)
static bool previousPressed = false;      // initial false
static bool releaseDelayActive = false;  // initial false

// SysML tick semantics: executed periodically at CONTROL_PERIOD_MS
static void tick(bool io_0) {
  // Updates (ordered as in model)
  // 1) if io_0 then releaseDelayActive := false
  if (io_0) {
    releaseDelayActive = false;
  }

  // 2) if (not io_0) and previousPressed then releaseDelayActive := true
  if ((!io_0) && previousPressed) {
    releaseDelayActive = true;
  }

  // 3) previousPressed := io_0
  previousPressed = io_0;
}

// Output expression (exact as model bundle)
// io_1 = (if io_0 ? 440 else (if releaseDelayActive ? 0 else 0))
static void updateOutputs(bool io_0) {
  int32_t io_1 = 0;
  if (io_0) {
    io_1 = 440;
  } else {
    if (releaseDelayActive) {
      io_1 = 0;
    } else {
      io_1 = 0;
    }
  }

  // Apply to hardware contract: tone_output invalid_policy = silence on 0.
  if (io_1 > 0) {
    // Use tone on D6 at constant frequency while commanded non-zero.
    tone(PIN_D6, static_cast<unsigned int>(io_1));
  } else {
    // Silence
    noTone(PIN_D6);
  }
}

void setup() {
  // Hardware: digital_input (active_high=true, pullup=false)
  pinMode(PIN_D3, INPUT);

  // tone_output
  pinMode(PIN_D6, OUTPUT);
  noTone(PIN_D6);

  // Ensure initial controller state matches model initial values
  previousPressed = false;
  releaseDelayActive = false;
}

void loop() {
  static uint32_t nextTickMs = 0;
  uint32_t now = millis();
  if (now < nextTickMs) {
    return;
  }
  nextTickMs = now + CONTROL_PERIOD_MS;

  // Sample io_0
  // active_high semantics: io_0 = (D3 reads HIGH)
  bool io_0 = (digitalRead(PIN_D3) == HIGH);

  // Execute model tick and update outputs in the same control period
  tick(io_0);
  updateOutputs(io_0);
}