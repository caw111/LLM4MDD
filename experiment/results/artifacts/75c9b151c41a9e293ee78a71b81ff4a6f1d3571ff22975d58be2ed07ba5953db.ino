#include <Arduino.h>

// Frozen hardware contract / bindings
static constexpr uint8_t BUTTON_PIN = 4;          // D4
static constexpr uint8_t BAUD = 9600;             // serial baud
static constexpr uint32_t CONTROL_PERIOD_MS = 5; // control period

// Interface semantics
// io_0 is "pressed" as model boolean true when pressed.
// Given pull-up wiring with active_high=false:
// - button pressed => D4 reads LOW
// - button not pressed => D4 reads HIGH
static inline bool read_io_0_pressed_model() {
  int v = digitalRead(BUTTON_PIN); // LOW when pressed (pull-up)
  return (v == LOW);
}

// Serial line framing assumption: line_or_idle; idle_timeout_ms=20
// No extra commands required; only output when serial_0_trigger == true/false as per model.
static void emit_serial_report_discriminator(bool serial_0_trigger, int32_t serial_0_value_0) {
  // The frozen model provides a typed discriminator (0/1) used by SysML to select messages.
  // Hardware adapter not available here; we implement fixed text printing accordingly.
  // serial_0_trigger is equivalent to io_0, so printing is consistent with the model:
  // - when pressed (serial_0_trigger=true, value=1): print "Button pressed"
  // - otherwise: print "Button not pressed"
  if (serial_0_trigger) {
    // serial_0_value_0 should be 1
    (void)serial_0_value_0;
    Serial.println("Button pressed");
  } else {
    // serial_0_value_0 should be 0
    (void)serial_0_value_0;
    Serial.println("Button not pressed");
  }
}

struct Controller {
  // State: previousPressed initial true
  bool previousPressed = true;

  void tick(bool io_0) {
    // ACCEPTED executable model update logic (ordered, preserved):
    // if (io_0 and not previousPressed) { previousPressed := io_0 }
    if (io_0 && (!previousPressed)) {
      previousPressed = io_0;
    }
    // if (not (io_0 and not previousPressed)) { previousPressed := io_0 }
    if (!(io_0 && (!previousPressed))) {
      previousPressed = io_0;
    }
  }

  // Outputs
  bool serial_0_trigger(bool io_0) const {
    return io_0 ? true : false;
  }

  int32_t serial_0_value_0(bool io_0) const {
    return io_0 ? 1 : 0;
  }
};

static Controller controller;

// Timing to respect control_period_ms=5 (deterministic steps)
static uint32_t nextTickMs = 0;

void setup() {
  // 1) Initialize serial communication
  Serial.begin(BAUD);

  // 2) Configure button pin as INPUT (pull-up is not explicitly specified in requirements,
  //    but the circuit uses an external 10kΩ pull-up; Wokwi wiring provides correct level.
  //    Use plain INPUT to avoid enabling internal pull-up which would alter effective circuit.
  pinMode(BUTTON_PIN, INPUT);

  // align first tick to current time
  nextTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;

    // Sample io_0 (model pressed state)
    bool io_0 = read_io_0_pressed_model();

    // Controller tick (exact state update ordering)
    controller.tick(io_0);

    // Compute outputs from frozen model
    bool serial_0_trigger = controller.serial_0_trigger(io_0);
    int32_t serial_0_value_0 = controller.serial_0_value_0(io_0);

    // Emit fixed messages based on the frozen outputs
    // (Gate/typed discriminator represent the selection mechanism; printing fixed required text.)
    emit_serial_report_discriminator(serial_0_trigger, serial_0_value_0);
  }
}