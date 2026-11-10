#include <Arduino.h>
#include <Servo.h>

static const uint8_t SERVO1_PIN = 5;   // D5
static const uint8_t SERVO2_PIN = 6;   // D6
static const uint8_t SERVO3_PIN = 7;   // D7

static const uint8_t RGB_RED_PIN   = 2;  // D2 (R)
static const uint8_t RGB_GREEN_PIN = 3;  // D3 (G)
static const uint8_t RGB_BLUE_PIN  = 4;  // D4 (B)

// Bargraph segments A1..A8 use D8..D13 and D44..D45
static const uint8_t BAR_SEG_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};

static Servo s1, s2, s3;

// Controller model state (Accepted executable model)
static int32_t servoIndex = 0;
static int32_t phaseMs = 0;
static double servo1Angle = 0.0;
static double servo2Angle = 0.0;
static double servo3Angle = 0.0;

// Timing
static uint32_t lastTickMs = 0;

static inline uint8_t angleToPwm255(double angleDeg) {
  // Model outputs: 0.0 -> 0.0 else 255.0 (boolean based on equality to 0.0)
  return (angleDeg == 0.0) ? 0 : 255;
}

static inline void applyOutputs() {
  // io_0..io_2: servo angles drive the servos (hold_last_valid semantics)
  // (We set servos directly; model state already contains exact 0.0 or 180.0.)
  s1.write((int)servo1Angle);
  s2.write((int)servo2Angle);
  s3.write((int)servo3Angle);

  // io_3..io_5: pwm outputs depend on whether corresponding servo angle equals 0.0
  analogWrite(RGB_RED_PIN, angleToPwm255(servo1Angle));     // io_3
  analogWrite(RGB_GREEN_PIN, angleToPwm255(servo2Angle));   // io_4
  analogWrite(RGB_BLUE_PIN, angleToPwm255(servo3Angle));     // io_5

  // io_6_segment_0..7:
  // if ((R255 + G255 + B255) >= threshold) then true else false
  // where thresholds are 1,3,5,7,9,11,13,15.
  // Note: R/G/B values are 0 or 255; comparisons are performed in the model against 1..15.
  // If any channel is non-zero, sum is 255..765, always >= all thresholds (>=1..15),
  // hence only the all-off case yields false for all segments.
  uint16_t r = (servo1Angle == 0.0) ? 0 : 255;
  uint16_t g = (servo2Angle == 0.0) ? 0 : 255;
  uint16_t b = (servo3Angle == 0.0) ? 0 : 255;
  uint16_t sum = r + g + b;

  // Implement exactly the model thresholds.
  const uint16_t thresholds[8] = {1, 3, 5, 7, 9, 11, 13, 15};
  for (int i = 0; i < 8; i++) {
    digitalWrite(BAR_SEG_PINS[i], (sum >= thresholds[i]) ? HIGH : LOW);
  }
}

void setup() {
  // Initialize bar graph pins
  for (int i = 0; i < 8; i++) {
    pinMode(BAR_SEG_PINS[i], OUTPUT);
    digitalWrite(BAR_SEG_PINS[i], LOW);
  }

  // Initialize RGB PWM pins
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  analogWrite(RGB_RED_PIN, 0);
  analogWrite(RGB_GREEN_PIN, 0);
  analogWrite(RGB_BLUE_PIN, 0);

  // Initialize servos to 0°
  s1.attach(SERVO1_PIN);
  s2.attach(SERVO2_PIN);
  s3.attach(SERVO3_PIN);

  s1.write(0);
  s2.write(0);
  s3.write(0);

  // Model initial state already set in globals:
  // servoIndex=0, phaseMs=0, servo angles = 0.0
  applyOutputs();

  lastTickMs = millis();
}

void loop() {
  // Controller contract: control_period_ms = 10
  // Advance phaseMs based on elapsed time; perform tick(s) as needed.
  uint32_t now = millis();
  uint32_t elapsed = now - lastTickMs;

  // Keep deterministic stepping at 10ms granularity.
  while (elapsed >= 10) {
    lastTickMs += 10;
    elapsed -= 10;

    // tick action:
    // first start;
    // then if phaseMs >= 2000 ... update model state and subtract 2000
    phaseMs += 10;

    if (phaseMs >= 2000) {
      // assign phaseMs := phaseMs - 2000;
      phaseMs = phaseMs - 2000;

      // servo1Angle update
      if (servoIndex == 0) {
        servo1Angle = (servo1Angle == 0.0) ? 180.0 : 0.0;
      }

      // servo2Angle update
      if (servoIndex == 1) {
        servo2Angle = (servo2Angle == 0.0) ? 180.0 : 0.0;
      }

      // servo3Angle update
      if (servoIndex == 2) {
        servo3Angle = (servo3Angle == 0.0) ? 180.0 : 0.0;
      }

      // servoIndex update
      servoIndex = (servoIndex == 2) ? 0 : (servoIndex + 1);
    }

    // outputs are continuously derived from model state; apply each control step
    applyOutputs();
  }
}