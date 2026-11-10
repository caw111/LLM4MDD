#include <Arduino.h>

static bool readIo0() {
  // Frozen contract: io_0 is input bool, model true when pressed.
  // Hardware contract provided: active_high=true, pullup=false on D2.
  // So we read the pin as HIGH/LOW and interpret HIGH as true.
  return digitalRead(2) == HIGH;
}

static void writeIo1(bool v) {
  // Frozen contract: io_1 is output bool, LED on when HIGH.
  digitalWrite(13, v ? HIGH : LOW);
}

struct Controller {
  // Exact controller state per accepted model
  int32_t mode;              // i32
  bool previousPressed;     // bool
  bool blinkState;          // bool

  Controller()
      : mode(0), previousPressed(false), blinkState(false) {}

  // "tick" with exact ordered updates from the accepted model
  void tick(bool io_0) {
    // First: start (no-op for codegen correctness)

    // if (io_0 and (not previousPressed)) then assign mode := ...
    if (io_0 && (!previousPressed)) {
      int32_t newMode;
      if (mode == 0) {
        newMode = 1;
      } else if (mode == 1) {
        newMode = 2;
      } else {
        newMode = 0;
      }
      mode = newMode;
    }

    // assign previousPressed := io_0
    previousPressed = io_0;

    // if (mode == 1) then blinkState := not blinkState else blinkState := false
    if (mode == 1) {
      blinkState = !blinkState;
    } else {
      blinkState = false;
    }
  }

  bool outIo1() const {
    // output io_1 := (if mode==0 false else (if mode==1 blinkState else true))
    if (mode == 0) return false;
    if (mode == 1) return blinkState;
    return true; // mode == 2
  }
};

static Controller controller;

// Planning constraint: control_period_ms = 5
static const uint32_t control_period_ms = 5;

void setup() {
  // Pin bindings per frozen contract:
  // io_0: D2 digital_input active_high true, pullup false
  // io_1: D13 digital_output active_high true
  pinMode(2, INPUT);      // pullup disabled (default when not using INPUT_PULLUP)
  pinMode(13, OUTPUT);

  // Ensure deterministic initial output according to model state (mode=0 => io_1=false)
  writeIo1(false);
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t now = millis();
  // Run controller tick at fixed period without drift
  if (now - lastTickMs >= control_period_ms) {
    // Catch-up in case of long delays to preserve deterministic update count
    while (now - lastTickMs >= control_period_ms) {
      lastTickMs += control_period_ms;

      bool io_0 = readIo0();     // sample_period_ms implied by control_period_ms=5
      controller.tick(io_0);     // exact ordered updates

      bool io_1 = controller.outIo1(); // compute output expression after state updates
      writeIo1(io_1);
    }
  }
}