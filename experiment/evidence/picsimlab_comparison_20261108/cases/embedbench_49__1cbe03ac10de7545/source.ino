#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_POT = A3;

// RGB1: D2/D3/D4 ; RGB2: D5/D6/D7 (common cathode)
static constexpr uint8_t PIN_RGB1_R = 2;
static constexpr uint8_t PIN_RGB1_G = 3;
static constexpr uint8_t PIN_RGB1_B = 4;
static constexpr uint8_t PIN_RGB2_R = 5;
static constexpr uint8_t PIN_RGB2_G = 6;
static constexpr uint8_t PIN_RGB2_B = 7;

// Servos: D8/D9/D10
static constexpr uint8_t PIN_SERVO1 = 8;
static constexpr uint8_t PIN_SERVO2 = 9;
static constexpr uint8_t PIN_SERVO3 = 10;

static inline float mapRealTo255_piecewise(float pot)
{
  // R: pot in [0..341] => pot * 255 / 341, else 0 when pot > 341
  if (pot <= 341.0f) return (pot * 255.0f / 341.0f);
  return 0.0f;
}

static inline float mapRealTo255_piecewise_green(float pot)
{
  // Green: pot in [342..682] => (pot-342)*255/340, else 0
  if (pot >= 342.0f && pot <= 682.0f) return ((pot - 342.0f) * 255.0f / 340.0f);
  return 0.0f;
}

static inline float mapRealTo255_piecewise_blue(float pot)
{
  // Blue: pot >= 683 => (pot-683)*255/340, else 0
  if (pot >= 683.0f) return ((pot - 683.0f) * 255.0f / 340.0f);
  return 0.0f;
}

static inline uint8_t clampToU8f(float x)
{
  if (x <= 0.0f) return 0;
  if (x >= 255.0f) return 255;
  return (uint8_t)x; // keep deterministic truncation
}

static inline int clampToServoDeg(float deg)
{
  if (deg < 0.0f) return 0;
  if (deg > 180.0f) return 180;
  // Deterministic: servo expects int; keep truncation
  return (int)deg;
}

// Models states per accepted executable model
static float prevPot = 0.0f;
static bool servoHold1 = false;
static bool servoHold2 = false;
static bool servoHold3 = false;

static Servo servo1;
static Servo servo2;
static Servo servo3;

static void applyRGB(float pot)
{
  // outputs io_0..io_5 (shared gradient, rgb1==rgb2)
  float r = mapRealTo255_piecewise(pot);        // io_0 and io_3
  float g = mapRealTo255_piecewise_green(pot);  // io_1 and io_4
  float b = mapRealTo255_piecewise_blue(pot);   // io_2 and io_5

  uint8_t ur = clampToU8f(r);
  uint8_t ug = clampToU8f(g);
  uint8_t ub = clampToU8f(b);

  // Use PWM for RGB channels
  analogWrite(PIN_RGB1_R, ur);
  analogWrite(PIN_RGB1_G, ug);
  analogWrite(PIN_RGB1_B, ub);

  analogWrite(PIN_RGB2_R, ur);
  analogWrite(PIN_RGB2_G, ug);
  analogWrite(PIN_RGB2_B, ub);
}

static void applyServos(float pot)
{
  // outputs io_7..io_9
  // io_k = io_6 * 180.0 / 1023.0
  float deg = pot * 180.0f / 1023.0f;
  int ideg = clampToServoDeg(deg);

  // Model does not include hold logic in the provided accepted bundle.
  servo1.write(ideg);
  servo2.write(ideg);
  servo3.write(ideg);
}

void setup() {
  // Initialize outputs to model initial values: io_0..io_5 = 0; io_7..io_9 = 0.
  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);
  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);

  analogWrite(PIN_RGB1_R, 0);
  analogWrite(PIN_RGB1_G, 0);
  analogWrite(PIN_RGB1_B, 0);
  analogWrite(PIN_RGB2_R, 0);
  analogWrite(PIN_RGB2_G, 0);
  analogWrite(PIN_RGB2_B, 0);

  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  // Move to initial positions (0 degrees)
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Model state init (per bundle)
  prevPot = 0.0f;
  servoHold1 = false;
  servoHold2 = false;
  servoHold3 = false;
}

void loop() {
  // Control period is 5ms; do best-effort timing.
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if (now - lastTickMs < 5) {
    return;
  }
  lastTickMs = now;

  // Read io_6 (pot1 SIG on A3) as f64 equivalent. analogRead gives 0..1023.
  int potRaw = analogRead(PIN_POT);
  float io6 = (float)potRaw;

  // tick: first start; then updates:
  prevPot = io6;
  servoHold1 = false;
  servoHold2 = false;
  servoHold3 = false;

  // tick: outputs:
  applyRGB(io6);
  applyServos(io6);
}