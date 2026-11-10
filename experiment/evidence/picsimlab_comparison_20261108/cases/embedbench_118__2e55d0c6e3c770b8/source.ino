#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_BTN1 = 3;  // io_0
static constexpr uint8_t PIN_BTN2 = 4;  // io_1
static constexpr uint8_t PIN_LED  = 2;  // io_3
static constexpr uint8_t PIN_SERVO_PWM = 5; // io_2 -> servoAngle (deg)

static constexpr float SERVO_INITIAL_DEG = 90.0f;
static constexpr float STEP_DEG = 30.0f;
static constexpr float SERVO_MIN_DEG = 0.0f;
static constexpr float SERVO_MAX_DEG = 180.0f;

static constexpr unsigned long CONTROL_PERIOD_MS = 5;

Servo servo1;

struct Controller {
  float servoAngle = SERVO_INITIAL_DEG;   // io_2
  bool previousBtn1Pressed = false;      // internal
  bool previousBtn2Pressed = false;      // internal
  bool io_3 = false;                     // LED output (modeled constant false in SysML)
};

static Controller ctrl;

static unsigned long nextTickMs = 0;

static inline bool readBtnPressed(uint8_t pin) {
  // Contract: active_high false with pullup => pressed is LOW.
  return digitalRead(pin) == LOW;
}

void applyServoAngle(float deg) {
  // Hardware contract says invalid_policy hold_last_valid; we always clamp within [0,180] anyway.
  if (deg < SERVO_MIN_DEG) deg = SERVO_MIN_DEG;
  if (deg > SERVO_MAX_DEG) deg = SERVO_MAX_DEG;
  servo1.write(static_cast<int>(deg)); // degree command
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW); // initial led off

  servo1.attach(PIN_SERVO_PWM);
  ctrl = Controller{}; // ensure initial state
  applyServoAngle(ctrl.servoAngle);

  nextTickMs = millis();
}

void loop() {
  unsigned long now = millis();
  if ((long)(now - nextTickMs) < 0) return;

  // enforce 5ms control_period
  nextTickMs += CONTROL_PERIOD_MS;

  bool io_0 = readBtnPressed(PIN_BTN1);
  bool io_1 = readBtnPressed(PIN_BTN2);

  // SysML tick logic (exact state update order)
  // first start; then button edge actions; then update previous states.
  if (io_0 && (!ctrl.previousBtn1Pressed)) {
    if (ctrl.servoAngle >= 150.0f) {
      // no change (value remains servoAngle)
    } else {
      ctrl.servoAngle = ctrl.servoAngle + STEP_DEG;
    }
  }

  if (io_1 && (!ctrl.previousBtn2Pressed)) {
    if (ctrl.servoAngle <= 30.0f) {
      // no change
    } else {
      ctrl.servoAngle = ctrl.servoAngle - STEP_DEG;
    }
  }

  ctrl.previousBtn1Pressed = io_0;
  ctrl.previousBtn2Pressed = io_1;

  // outputs: io_2 = servoAngle, io_3 = false (modeled)
  applyServoAngle(ctrl.servoAngle);
  ctrl.io_3 = false;
  digitalWrite(PIN_LED, ctrl.io_3 ? HIGH : LOW);
}