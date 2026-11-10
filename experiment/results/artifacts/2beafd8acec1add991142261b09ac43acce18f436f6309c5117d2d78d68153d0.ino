#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t SERVO_PIN = 9;

// -------- Accepted executable model (controller) state --------
static double servoAngleDeg = 0.0;   // io_0, unit deg, f64
static bool directionUp = true;       // internal
static bool started = false;         // internal
static bool reportPending = true;    // internal, drives serial_0_trigger

// -------- Servo and serial reporting --------
static Servo servoMotor;

// Track last printed gate to ensure "asserted exactly when move reaches endpoint"
// (model sets reportPending to true for the endpoint step and false otherwise)
static bool lastReportPending = false;

// Apply model "tick" updates deterministically (executed once per loop call)
static void controllerTick()
{
  // perform action tick:
  // first start; then if not started { ... } else {}
  if (!started) {
    started = true;
    directionUp = true;
    servoAngleDeg = 0.0;
    reportPending = true;
  } else {
    if (directionUp) {
      // assign servoAngleDeg := (if servoAngleDeg >= 180.0 then 180.0 else servoAngleDeg + 1.0);
      if (servoAngleDeg >= 180.0) {
        servoAngleDeg = 180.0;
      } else {
        servoAngleDeg = servoAngleDeg + 1.0;
      }

      // if servoAngleDeg >= 179.0 then { directionUp := false; reportPending := true } else { reportPending := false }
      if (servoAngleDeg >= 179.0) {
        directionUp = false;
        reportPending = true;
      } else {
        reportPending = false;
      }
    } else {
      // assign servoAngleDeg := (if servoAngleDeg <= 0.0 then 0.0 else servoAngleDeg - 1.0);
      if (servoAngleDeg <= 0.0) {
        servoAngleDeg = 0.0;
      } else {
        servoAngleDeg = servoAngleDeg - 1.0;
      }

      // if servoAngleDeg <= 1.0 then { directionUp := true; reportPending := true } else { reportPending := false }
      if (servoAngleDeg <= 1.0) {
        directionUp = true;
        reportPending = true;
      } else {
        reportPending = false;
      }
    }
  }
}

static inline int32_t realFloorToInt(double x)
{
  // Model uses RealFunctions::floor(servoAngleDeg). For 0..180 with unit steps this is exact.
  // Still, compute floor correctly for safety.
  if (x >= 0.0) return (int32_t)x; // floor for non-negative
  // For completeness (should not occur with this model):
  int32_t i = (int32_t)x;
  return (x == (double)i) ? i : (i - 1);
}

static void updateOutputs()
{
  // io_0 output: servo angle in degrees
  // Servo library expects an integer angle 0..180; model io_0 changes in exact 1-degree steps and saturates.
  int angleInt = realFloorToInt(servoAngleDeg);
  if (angleInt < 0) angleInt = 0;
  if (angleInt > 180) angleInt = 180;
  servoMotor.write(angleInt);

  // serial_0_trigger output: reportPending
  // Print only the number when reporting (no extra text).
  // "Gate serial print ... after each completed movement; exact condition ... modeled ..."
  // So we print exactly when reportPending is true at the controller step boundary.
  if (reportPending) {
    // Ensure we only emit once per asserted step (guard against any potential re-assertions without change)
    // The model guarantees reportPending is true at endpoint steps; other steps set it false.
    // So simple condition is enough, but we keep a state guard to be robust.
    if (!lastReportPending) {
      int32_t value = realFloorToInt(servoAngleDeg); // serial_0_value_0
      Serial.println(value);
    }
  }

  lastReportPending = reportPending;
}

void setup()
{
  Serial.begin(9600);

  servoMotor.attach(SERVO_PIN);

  // Initialize controller state as in model initial values:
  // servoAngleDeg=0.0, directionUp=true, started=false, reportPending=true
  servoAngleDeg = 0.0;
  directionUp = true;
  started = false;
  reportPending = true;

  lastReportPending = false;

  // Perform first tick immediately so the model's "when not started" branch is executed
  // and outputs are applied without waiting an extra period.
  controllerTick();
  updateOutputs();
}

void loop()
{
  // Control period is 5 ms
  delay(5);

  controllerTick();
  updateOutputs();
}