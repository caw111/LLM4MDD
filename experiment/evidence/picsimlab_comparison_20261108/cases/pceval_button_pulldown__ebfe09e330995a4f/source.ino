#include <Arduino.h>

// Frozen configuration contract
static const uint8_t BUTTON_PIN = 4;      // D4
static const unsigned long SERIAL_BAUD = 9600;

static const char MSG_PRESSED[]     = "Button pressed";
static const char MSG_NOT_PRESSED[] = "Button not pressed";

// ACCEPTED executable model (controller outputs)
// io_0 : input  (button pressed state; true => HIGH)
// io_1 : output (serial gate for "Button pressed") = io_0
// io_2 : output (serial gate for "Button not pressed") = !io_0
struct Controller {
  bool io_0; // input
  bool io_1; // output
  bool io_2; // output
  void eval() {
    io_1 = io_0;
    io_2 = !io_0;
  }
};

Controller controller;

// Serial protocol note: framing is line_or_idle with idle_timeout_ms=20.
// We emit fixed text lines; Arduino println() is line-based (newline delimited).
static void emitSerialLineIf(bool gate, const char* msg) {
  if (gate) {
    Serial.println(msg);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(BUTTON_PIN, INPUT);

  // Initial state (match controller initial semantics: io_0 initial false)
  controller.io_0 = false;
  controller.eval();
  // The original requirements do not state to print immediately on boot,
  // only "continuously check ... and print ... if ...".
}

void loop() {
  // ACCEPTED controller state preservation:
  // There are no stored internal states in the given model; outputs are pure functions of io_0.
  // sample period and control period are not enforced by code in the frozen contract,
  // so we simply evaluate as fast as possible while continuously checking.
  bool buttonHigh = (digitalRead(BUTTON_PIN) == HIGH); // active_high: true_level=HIGH
  controller.io_0 = buttonHigh;
  controller.eval();

  // Emit outputs exactly as the model indicates (gates).
  emitSerialLineIf(controller.io_1, MSG_PRESSED);
  emitSerialLineIf(controller.io_2, MSG_NOT_PRESSED);

  // Do not add extra behavior (e.g., rate limiting) not specified in the contract.
}