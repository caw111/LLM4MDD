#include <Arduino.h>

// --- Frozen hardware contract bindings ---
// Input: btn1 -> D5 (active LOW, pullup enabled). Press corresponds to LOW.
static constexpr uint8_t PIN_BTN1   = 5;

// Single-color LEDs (PWM capable) -> D2, D3, D4
static constexpr uint8_t PIN_LED1   = 2;
static constexpr uint8_t PIN_LED2   = 3;
static constexpr uint8_t PIN_LED3   = 4;

// RGB LEDs common to GND, cathode/common tied to GND.
// PWM channels:
// rgb1: R/G/B -> D6/D7/D8
static constexpr uint8_t PIN_RGB1_R = 6;
static constexpr uint8_t PIN_RGB1_G = 7;
static constexpr uint8_t PIN_RGB1_B = 8;
// rgb2: R/G/B -> D9/D10/D11
static constexpr uint8_t PIN_RGB2_R = 9;
static constexpr uint8_t PIN_RGB2_G = 10;
static constexpr uint8_t PIN_RGB2_B = 11;
// rgb3: R/G/B -> D12/D13/D44
static constexpr uint8_t PIN_RGB3_R = 12;
static constexpr uint8_t PIN_RGB3_G = 13;
static constexpr uint8_t PIN_RGB3_B = 44;

// Controller period (control_period_ms) is 5ms, but model doesn't require fixed cadence.
// We'll still run tick() as frequently as possible with a 5ms rhythm.
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// --- Generated model state (exact from ACCEPTED bundle) ---
static int32_t mode = 0;
static bool previousPressed = false;
static int32_t modeElapsedMs = 0;
static bool modeLed1On = false;
static bool modeLed2On = false;
static bool modeLed3On = false;
static int32_t rainbowPhaseMs = 0; // present in state; contract outputs don't use it

// Helper: write pwm_count in range [0..255] to a pin.
static inline void writePwm255(uint8_t pin, float pwmCount) {
  int v = (int)(pwmCount + 0.5f);
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  analogWrite(pin, v);
}

static inline void setAllOff() {
  // Initialize: all LEDs off (PWM outputs at 0).
  writePwm255(PIN_LED1, 0.0f);
  writePwm255(PIN_LED2, 0.0f);
  writePwm255(PIN_LED3, 0.0f);

  writePwm255(PIN_RGB1_R, 0.0f);
  writePwm255(PIN_RGB1_G, 0.0f);
  writePwm255(PIN_RGB1_B, 0.0f);

  writePwm255(PIN_RGB2_R, 0.0f);
  writePwm255(PIN_RGB2_G, 0.0f);
  writePwm255(PIN_RGB2_B, 0.0f);

  writePwm255(PIN_RGB3_R, 0.0f);
  writePwm255(PIN_RGB3_G, 0.0f);
  writePwm255(PIN_RGB3_B, 0.0f);
}

// --- Generated model tick per the frozen SysML2 logic ---
static void tick(bool io0_pressedLowAsBool) {
  // Model input naming: io_0 is Boolean, where "true_level" is LOW because active_high=false.
  // We'll treat io_0 as true when button is pressed => digitalRead == LOW.

  // perform action tick { first start; then if (io_0 and not previousPressed) { first start; then
  // if modeElapsedMs >= 2000 { ... assignments ... } } then assign previousPressed := io_0; }
  if (io0_pressedLowAsBool && (!previousPressed)) {
    if (modeElapsedMs >= 2000) {
      mode = (mode == 3) ? 0 : (mode + 1);
      modeElapsedMs = 0;
      rainbowPhaseMs = 0;
      modeLed1On = (mode == 1);
      modeLed2On = (mode == 2);
      modeLed3On = (mode == 3);
    }
  }
  previousPressed = io0_pressedLowAsBool;

  // Note: The frozen model does NOT increment modeElapsedMs nor rainbowPhaseMs anywhere else.
  // To preserve the exact generated model behavior, we intentionally do not update them here.
  // (Thus, modeElapsedMs stays at its initialized value unless the button gate sets it to 0.)
}

// Generated model output equations (exact structure from bundle)
static void updateOutputs() {
  // io_1..io_3 are led1..led3 mode indicators brightness
  float io_1 = (mode == 0) ? 0.0f : (modeLed1On ? 255.0f : 0.0f);
  float io_2 = (mode == 0) ? 0.0f : (modeLed2On ? 255.0f : 0.0f);
  float io_3 = (mode == 0) ? 0.0f : (modeLed3On ? 255.0f : 0.0f);

  // RGB channels io_4..io_12
  float io_4  = (mode == 0) ? 0.0f : (mode == 1 ? 255.0f : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_5  = (mode == 0) ? 0.0f : (mode == 1 ? 0.0f   : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_6  = (mode == 0) ? 0.0f : (mode == 1 ? 0.0f   : (mode == 2 ? 0.0f   : (mode == 3 ? 255.0f : 0.0f)));
  float io_7  = (mode == 0) ? 0.0f : (mode == 1 ? 255.0f : (mode == 2 ? 0.0f   : (mode == 3 ? 0.0f   : 0.0f)));
  float io_8  = (mode == 0) ? 0.0f : (mode == 1 ? 255.0f : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_9  = (mode == 0) ? 0.0f : (mode == 1 ? 0.0f   : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_10 = (mode == 0) ? 0.0f : (mode == 1 ? 255.0f : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_11 = (mode == 0) ? 0.0f : (mode == 1 ? 0.0f   : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));
  float io_12 = (mode == 0) ? 0.0f : (mode == 1 ? 255.0f : (mode == 2 ? 255.0f : (mode == 3 ? 255.0f : 0.0f)));

  // Apply to pins
  writePwm255(PIN_LED1, io_1);
  writePwm255(PIN_LED2, io_2);
  writePwm255(PIN_LED3, io_3);

  writePwm255(PIN_RGB1_R, io_4);
  writePwm255(PIN_RGB1_G, io_5);
  writePwm255(PIN_RGB1_B, io_6);

  writePwm255(PIN_RGB2_R, io_7);
  writePwm255(PIN_RGB2_G, io_8);
  writePwm255(PIN_RGB2_B, io_9);

  writePwm255(PIN_RGB3_R, io_10);
  writePwm255(PIN_RGB3_G, io_11);
  writePwm255(PIN_RGB3_B, io_12);
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);

  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);

  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);

  pinMode(PIN_RGB3_R, OUTPUT);
  pinMode(PIN_RGB3_G, OUTPUT);
  pinMode(PIN_RGB3_B, OUTPUT);

  // Initialize model state per bundle initial values
  mode = 0;
  previousPressed = false;
  modeElapsedMs = 0;
  modeLed1On = false;
  modeLed2On = false;
  modeLed3On = false;
  rainbowPhaseMs = 0;

  // Contract: all PWM outputs at 0 intensity (all LEDs off)
  setAllOff();
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < CONTROL_PERIOD_MS) {
    return;
  }
  lastTickMs = now;

  // io_0 semantics: active_high=false, pullup=true, pressed corresponds to LOW.
  bool io_0 = (digitalRead(PIN_BTN1) == LOW);

  tick(io_0);
  updateOutputs();
}