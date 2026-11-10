#include <Arduino.h>

// Frozen interface contract:
// - control_period_ms: 5
// - io_0 is a bool output, active_high=true, pin D13
// - initial=true => set HIGH after initialization and keep HIGH
//
// Acceptance:
// - after initialization completes, within 20ms LED must be ON
// - then remain ON continuously

static constexpr uint8_t LED_PIN = 13;
static constexpr unsigned long CONTROL_PERIOD_MS = 5;
static constexpr unsigned long START_ON_DELAY_MS = 20;

static unsigned long t0_ms = 0;

static void controller_init() {
  // Initialize controller outputs according to the frozen model:
  // io_0 := true
  // The hardware contract states active_high => HIGH turns LED ON.
  digitalWrite(LED_PIN, HIGH);
}

static void controller_step() {
  // Controller has no internal states/inputs; outputs remain constant:
  // io_0 always true.
  digitalWrite(LED_PIN, HIGH);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  // Start time reference immediately after setting pin mode.
  t0_ms = millis();

  // Ensure output is set to required value.
  // The acceptance requires "within 20ms", so we set it immediately,
  // which satisfies the constraint and preserves "initial=true".
  controller_init();

  // Optional: enforce that we remain in ON state after 20ms.
  // No change required; kept for timing clarity.
  while (millis() - t0_ms < START_ON_DELAY_MS) {
    // busy wait; output already HIGH
  }
}

void loop() {
  // Periodic stepping at 5ms to match control_period_ms in the contract.
  static unsigned long last_ms = 0;
  unsigned long now = millis();
  if (now - last_ms >= CONTROL_PERIOD_MS) {
    last_ms = now;
    controller_step();
  }
}