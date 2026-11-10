#include <Arduino.h>

// Frozen hardware contract
// io_0: D2 digital input with pullup, active-high (pressed => HIGH)
// io_1: D13 digital output, active-high (HIGH => LED ON)

static constexpr uint8_t PIN_IO_0 = 2;   // D2
static constexpr uint8_t PIN_IO_1 = 13;  // D13

// Accepted executable model (controller)
struct Controller {
  // states
  bool modeBlinking = false;   // initial false
  bool previousPressed = true; // initial true (so first sampled HIGH won't toggle unless rising edge)

  // update (tick action)
  void tick(bool io_0) {
    // if (io_0 and not previousPressed) then modeBlinking := !modeBlinking
    if (io_0 && !previousPressed) {
      modeBlinking = modeBlinking ? false : true;
    }
    // previousPressed := io_0
    previousPressed = io_0;
  }

  // output expression
  bool io_1() const {
    return modeBlinking ? true : false;
  }
};

static Controller controller;

// control period: 5 ms
static constexpr unsigned long CONTROL_PERIOD_MS = 5;

void setup() {
  // Hardware init
  pinMode(PIN_IO_0, INPUT_PULLUP); // pullup=true
  pinMode(PIN_IO_1, OUTPUT);       // active_high output

  // Ensure initial output consistent with model initial state (modeBlinking=false => io_1=false)
  digitalWrite(PIN_IO_1, LOW);
}

void loop() {
  static unsigned long lastTickMs = 0;

  unsigned long now = millis();
  if (now - lastTickMs >= CONTROL_PERIOD_MS) {
    // keep deterministic tick cadence as close as possible
    lastTickMs += CONTROL_PERIOD_MS;

    // sample inputs
    // active_high semantics: pressed gives HIGH due to pullup wiring assumption
    bool io_0 = (digitalRead(PIN_IO_0) == HIGH);

    // ordered updates exactly per model: update tick (which updates modeBlinking then previousPressed)
    controller.tick(io_0);

    // output after updates
    digitalWrite(PIN_IO_1, controller.io_1() ? HIGH : LOW);
  }
}