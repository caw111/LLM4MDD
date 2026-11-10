#include <Arduino.h>
#include <math.h>

// Frozen interface contract
// io_0: D2 button, INPUT_PULLUP, pressed => LOW (model semantics: pressed=>true)
// io_1: A0 potentiometer ADC counts 0..1023 -> f64 (model semantics expects 0..1023)
// io_2: D9 PWM 0..255 (model: floor(io_1 * 255 / 1023), else 0)

static const uint8_t PIN_BUTTON = 2;   // D2
static const uint8_t PIN_POT    = A0;  // A0
static const uint8_t PIN_LED_PWM = 9;  // D9 PWM

// Controller state (exact): previousPressed : bool initial false
static bool previousPressed = false;

static inline bool readIo0_pressedModelTrue() {
  // D2 INPUT_PULLUP: LOW means pressed. Contract says model true corresponds to pressed-state.
  // Therefore: model io_0 = (digitalRead(D2) == LOW)
  return (digitalRead(PIN_BUTTON) == LOW);
}

static inline float readIo1_adcCounts() {
  // ADC count 0..1023 as f64
  return (float)analogRead(PIN_POT);
}

static inline int32_t model_floor_f64_to_int(float x) {
  // Model uses RealFunctions::floor(io_1 * 255.0 / 1023.0)
  // Convert safely to int range 0..255.
  // Use floorf for AVR.
  float f = floorf(x);
  // Clamp to 0..255 to avoid any edge rounding artifacts outside expected range.
  if (f < 0.0f) f = 0.0f;
  if (f > 255.0f) f = 255.0f;
  return (int32_t)f;
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_PWM, OUTPUT);

  // Initialize output to match model initial behavior:
  // initial io_0 is read at runtime; but controller initial previousPressed is false.
  // Model output depends on io_0 only, so set PWM after first sampling in loop().
  analogWrite(PIN_LED_PWM, 0);
}

void loop() {
  // control_period_ms = 10 (acceptance: <= 20ms response)
  // To meet the control-period constraint, sample/update once per 10ms.
  static unsigned long lastTickMs = 0;
  unsigned long now = millis();

  if (now - lastTickMs < 10) {
    // Busy-wait lightly to avoid missing the tick; still respects <=20ms response.
    return;
  }
  lastTickMs = now;

  // === Read inputs (sample_period_ms = 10) ===
  const bool io_0 = readIo0_pressedModelTrue();     // model input
  const float io_1 = readIo1_adcCounts();           // 0..1023 in f64 domain

  // === Controller tick ===
  // perform action tick { first start; then assign previousPressed := io_0; }
  previousPressed = io_0;

  // out io_2 = (if io_0 ? floor(io_1 * 255.0 / 1023.0) else 0.0)
  float io_2_real = 0.0f;
  if (io_0) {
    // Compute floor(io_1 * 255 / 1023)
    float scaled = (io_1 * 255.0f) / 1023.0f;
    io_2_real = (float)model_floor_f64_to_int(scaled);
  }

  // === Output binding: pwm_output scale 255 offset 0 ===
  // Contract expects PWM count 0..255, using io_2_real directly.
  int32_t pwmCount = (int32_t)io_2_real; // already floor'ed and clamped 0..255
  analogWrite(PIN_LED_PWM, (uint8_t)pwmCount);
}