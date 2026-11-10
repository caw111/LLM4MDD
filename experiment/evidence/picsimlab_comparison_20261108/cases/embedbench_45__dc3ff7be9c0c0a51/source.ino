#include <Arduino.h>
#include <Servo.h>

static const uint8_t RGB1_R_PIN = 2;   // io_0
static const uint8_t RGB1_G_PIN = 3;   // io_1
static const uint8_t RGB1_B_PIN = 4;   // io_2

static const uint8_t RGB2_R_PIN = 5;   // io_3
static const uint8_t RGB2_G_PIN = 6;   // io_4
static const uint8_t RGB2_B_PIN = 7;   // io_5

static const uint8_t RGB3_R_PIN = 8;   // io_6
static const uint8_t RGB3_G_PIN = 9;   // io_7
static const uint8_t RGB3_B_PIN = 10;  // io_8

static const uint8_t SERVO1_PIN = 11;  // io_9
static const uint8_t SERVO2_PIN = 12;  // io_10
static const uint8_t SERVO3_PIN = 13;  // io_11

// Model contract: control_period_ms = 10. Also, color holds for 2 seconds.
// We advance colorIndex every 200 ticks (2s / 10ms).
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t STEP_HOLD_MS = 2000;
static const int32_t COLOR_MAX_INDEX = 6; // indices 0..6 inclusive

static const int SERVO1_INIT_DEG = 0;
static const int SERVO2_INIT_DEG = 0;
static const int SERVO3_INIT_DEG = 0;

Servo servo1;
Servo servo2;
Servo servo3;

// Model state
static int32_t colorIndex = 0;

// Timing for deterministic stepping with 10ms granularity
static uint32_t lastTickMs = 0;
static uint32_t stepElapsedMs = 0;

static inline uint8_t pwmFromModelReal(float v) {
  // Model outputs are exactly 0.0 or 255.0 (pwm_count).
  // Map pwm_count [0..255] to Arduino analogWrite [0..255] directly.
  if (v <= 0.0f) return 0;
  if (v >= 255.0f) return 255;
  return (uint8_t)lroundf(v);
}

static inline float model_io_0(int32_t ci) { // rgb1 red
  if (ci == 0) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_1(int32_t ci) { // rgb1 green
  if (ci == 1) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_2(int32_t ci) { // rgb1 blue
  if (ci == 2) return 255.0f;
  else if (ci == 3) return 0.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_3(int32_t ci) { // rgb2 red
  if (ci == 0) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_4(int32_t ci) { // rgb2 green
  if (ci == 1) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_5(int32_t ci) { // rgb2 blue
  if (ci == 2) return 255.0f;
  else if (ci == 3) return 0.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_6(int32_t ci) { // rgb3 red
  if (ci == 0) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_7(int32_t ci) { // rgb3 green
  if (ci == 1) return 255.0f;
  else if (ci == 3) return 255.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}
static inline float model_io_8(int32_t ci) { // rgb3 blue
  if (ci == 2) return 255.0f;
  else if (ci == 3) return 0.0f;
  else if (ci == 4) return 255.0f;
  else if (ci == 5) return 255.0f;
  else if (ci == 6) return 255.0f;
  else return 0.0f;
}

static inline float model_io_9(int32_t ci) { // servo1 target angle
  if (ci == 0) return 0.0f;
  else if (ci == 1) return 45.0f;
  else if (ci == 2) return 90.0f;
  else if (ci == 3) return 135.0f;
  else if (ci == 4) return 180.0f;
  else if (ci == 5) return 0.0f;
  else return 90.0f;
}
static inline float model_io_10(int32_t ci) { // servo2 target angle
  if (ci == 0) return 45.0f;
  else if (ci == 1) return 90.0f;
  else if (ci == 2) return 135.0f;
  else if (ci == 3) return 180.0f;
  else if (ci == 4) return 0.0f;
  else if (ci == 5) return 45.0f;
  else return 135.0f;
}
static inline float model_io_11(int32_t ci) { // servo3 target angle
  if (ci == 0) return 90.0f;
  else if (ci == 1) return 135.0f;
  else if (ci == 2) return 180.0f;
  else if (ci == 3) return 0.0f;
  else if (ci == 4) return 45.0f;
  else if (ci == 5) return 90.0f;
  else return 180.0f;
}

static void applyOutputsForCurrentColorIndex() {
  // RGB LEDs: common-cathode. Contract says use PWM outputs; full-on level is 255.0,
  // off is 0.0. (Model does not provide brightness scaling.)
  analogWrite(RGB1_R_PIN, pwmFromModelReal(model_io_0(colorIndex)));
  analogWrite(RGB1_G_PIN, pwmFromModelReal(model_io_1(colorIndex)));
  analogWrite(RGB1_B_PIN, pwmFromModelReal(model_io_2(colorIndex)));

  analogWrite(RGB2_R_PIN, pwmFromModelReal(model_io_3(colorIndex)));
  analogWrite(RGB2_G_PIN, pwmFromModelReal(model_io_4(colorIndex)));
  analogWrite(RGB2_B_PIN, pwmFromModelReal(model_io_5(colorIndex)));

  analogWrite(RGB3_R_PIN, pwmFromModelReal(model_io_6(colorIndex)));
  analogWrite(RGB3_G_PIN, pwmFromModelReal(model_io_7(colorIndex)));
  analogWrite(RGB3_B_PIN, pwmFromModelReal(model_io_8(colorIndex)));

  // Servos: update target angles at color transitions.
  // The contract specifies hold_last_valid for invalids; here we always provide valid angles.
  servo1.write((int)lroundf(model_io_9(colorIndex)));
  servo2.write((int)lroundf(model_io_10(colorIndex)));
  servo3.write((int)lroundf(model_io_11(colorIndex)));
}

static void advanceModelTickOnce() {
  // Controller update:
  // colorIndex := (if colorIndex >= 6 ? 0 else colorIndex + 1);
  if (colorIndex >= COLOR_MAX_INDEX) colorIndex = 0;
  else colorIndex = colorIndex + 1;
}

void setup() {
  // Initialize RGB outputs to OFF (initial=0 for io_0..io_8)
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);
  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);
  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);
  analogWrite(RGB3_R_PIN, 0);
  analogWrite(RGB3_G_PIN, 0);
  analogWrite(RGB3_B_PIN, 0);

  // Initialize servos to their 0-degree position at startup (contract initial=0 deg).
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  servo1.write(SERVO1_INIT_DEG);
  servo2.write(SERVO2_INIT_DEG);
  servo3.write(SERVO3_INIT_DEG);

  colorIndex = 0; // initial controller state
  lastTickMs = millis();
  stepElapsedMs = 0;

  // The model outputs depend on colorIndex; however initialization requirement says LEDs off at startup.
  // Contract specifies initial PWM outputs are 0 (initial=0), not computed from colorIndex at t=0.
  // Therefore, keep LEDs off until the first 2s color step completes and the first transition occurs.
}

void loop() {
  const uint32_t now = millis();

  // Deterministic 10ms control period stepping.
  while ((uint32_t)(now - lastTickMs) >= CONTROL_PERIOD_MS) {
    lastTickMs += CONTROL_PERIOD_MS;
    stepElapsedMs += CONTROL_PERIOD_MS;

    // Color should change every 2.0 seconds; servos update at color transitions.
    if (stepElapsedMs >= STEP_HOLD_MS) {
      stepElapsedMs = 0;

      // On color transition, advance the model and then apply outputs.
      // This preserves the exact ordered update: assign colorIndex (tick action) then outputs.
      advanceModelTickOnce();
      applyOutputsForCurrentColorIndex();
    }
  }
}