#include <Arduino.h>

// Frozen interface contract bindings
// io_0: elapsed_ms (delta since previous control step), provided via fixed 10ms control loop timing.
// Outputs: io_1_segment_0..9 mapped to Mega pins D2..D11 (segments A1..A10 in provided order).

static constexpr uint8_t SEGMENT_PINS[10] = {
  2,  // io_1_segment_0 -> D2  (bargraph1:A1)
  3,  // io_1_segment_1 -> D3  (bargraph1:A2)
  4,  // io_1_segment_2 -> D4  (bargraph1:A3)
  5,  // io_1_segment_3 -> D5  (bargraph1:A4)
  6,  // io_1_segment_4 -> D6  (bargraph1:A5)
  7,  // io_1_segment_5 -> D7  (bargraph1:A6)
  8,  // io_1_segment_6 -> D8  (bargraph1:A7)
  9,  // io_1_segment_7 -> D9  (bargraph1:A8)
  10, // io_1_segment_8 -> D10 (bargraph1:A9)
  11  // io_1_segment_9 -> D11 (bargraph1:A10)
};

static int32_t accumulatedMs = 0; // initial 0
static bool toggle = false;       // initial false

// Control period: required 10ms
static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static constexpr int32_t TOGGLE_PERIOD_MS = 2000;

static void applyOutputs(bool t) {
  // Outputs:
  // io_1_segment_0 = (!toggle)
  // io_1_segment_1 = (toggle)
  // alternating as specified
  // Implement exactly:
  for (uint8_t i = 0; i < 10; i++) {
    // even i: segment_0,2,4,6,8 = (!toggle)
    // odd i:  segment_1,3,5,7,9 = (toggle)
    bool value = (i % 2 == 0) ? (!t) : (t);
    digitalWrite(SEGMENT_PINS[i], value ? HIGH : LOW); // active_high
  }
}

void setup() {
  // Initialize pin modes
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
    digitalWrite(SEGMENT_PINS[i], LOW);
  }

  // Controller initial state:
  accumulatedMs = 0;
  toggle = false;

  // Apply initial outputs corresponding to toggle=false:
  // segment_0,2,4,6,8 true; segment_1,3,5,7,9 false
  applyOutputs(toggle);
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    return;
  }
  // Ensure fixed number of 10ms steps; avoid drift by advancing lastTickMs in steps.
  lastTickMs += CONTROL_PERIOD_MS;

  // io_0 delta since previous step: fixed 10ms as per required sample/trace model.
  const int32_t io_0 = static_cast<int32_t>(CONTROL_PERIOD_MS);

  // ---- Controller tick (exact update order as in provided model) ----
  // tick:
  // first start;
  // then assign accumulatedMs := accumulatedMs + io_0;
  // then if accumulatedMs >= 2000 { ... }
  accumulatedMs = accumulatedMs + io_0;

  if (accumulatedMs >= TOGGLE_PERIOD_MS) {
    accumulatedMs = accumulatedMs - TOGGLE_PERIOD_MS;
    toggle = !toggle; // assign toggle := not toggle
  }

  // Update outputs every tick based on current toggle
  applyOutputs(toggle);
}