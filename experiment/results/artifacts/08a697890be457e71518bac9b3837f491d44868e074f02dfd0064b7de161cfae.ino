#include <Arduino.h>

// Frozen hardware contract bindings
// io_0: red   -> D2 (PWM output, scale=1.0, initial=0)
// io_1: green -> D3 (PWM output, scale=1.0, initial=0)
// io_2: blue  -> D4 (PWM output, scale=1.0, initial=0)
// io_3: button input (active_low, pullup=true) -> D5, model true when pressed (LOW)

static constexpr uint8_t PIN_RED   = 2;  // D2
static constexpr uint8_t PIN_GREEN = 3;  // D3
static constexpr uint8_t PIN_BLUE  = 4;  // D4
static constexpr uint8_t PIN_K1    = 5;  // D5

static constexpr uint32_t CONTROL_PERIOD_MS = 5;

static int32_t colorIndex = 0;        // i32, initial 0
static bool previousPressed = false;  // bool, initial false

static inline void setPwmCount(uint8_t pin, float pwmCount) {
  // Model outputs are pwm_count in range [0..255] (scale=1.0).
  // Use analogWrite to implement PWM output with corresponding count.
  uint8_t value = (pwmCount <= 0.0f) ? 0u : (pwmCount >= 255.0f ? 255u : (uint8_t)(pwmCount + 0.5f));
  analogWrite(pin, value);
}

static inline bool readButtonIo3() {
  // active_high=false, pullup=true, pressed model true when LOW.
  return digitalRead(PIN_K1) == LOW;
}

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);

  pinMode(PIN_K1, INPUT_PULLUP);

  // Initialization: RGB LED should be off after reset.
  setPwmCount(PIN_RED, 0.0f);
  setPwmCount(PIN_GREEN, 0.0f);
  setPwmCount(PIN_BLUE, 0.0f);

  // Ensure model initial state
  colorIndex = 0;
  previousPressed = false;
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs += CONTROL_PERIOD_MS;

  // io_3
  bool io_3 = readButtonIo3();

  // Controller tick logic (exactly as given)
  if (io_3 && (!previousPressed)) {
    // assign colorIndex := (if colorIndex == 0 ? 1 else (if colorIndex == 1 ? 2 else (if colorIndex == 2 ? 3 else 0)))
    if (colorIndex == 0) {
      colorIndex = 1;
    } else if (colorIndex == 1) {
      colorIndex = 2;
    } else if (colorIndex == 2) {
      colorIndex = 3;
    } else {
      colorIndex = 0;
    }
    previousPressed = io_3;
  }

  if (!(io_3 && (!previousPressed))) {
    previousPressed = io_3;
  }

  // Outputs:
  // io_0 = (if colorIndex == 1 ? 255.0 else 0.0)
  // io_1 = (if colorIndex == 2 ? 255.0 else 0.0)
  // io_2 = (if colorIndex == 3 ? 255.0 else 0.0)
  setPwmCount(PIN_RED,   (colorIndex == 1) ? 255.0f : 0.0f);
  setPwmCount(PIN_GREEN, (colorIndex == 2) ? 255.0f : 0.0f);
  setPwmCount(PIN_BLUE,  (colorIndex == 3) ? 255.0f : 0.0f);
}