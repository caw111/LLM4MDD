#include <Servo.h>

Servo servo1;
Servo servo2;

const int led1 = 2;  // from circuit: mega:2 -> led1:A
const int led2 = 3;  // from circuit: mega:3 -> led2:A
const int led3 = 4;  // from circuit: mega:4 -> led3:A

// PWM pins from circuit
const int servo1PWM = 5; // mega:5 -> servo1:PWM
const int servo2PWM = 6; // mega:6 -> servo2:PWM

static const unsigned long STEP_INTERVAL_MS = 2000;
static const int STEP_DEG = 30;

enum CyclePhase {
  PHASE_SERVO1_MOVING,
  PHASE_SERVO2_MOVING,
  PHASE_LED3
};

CyclePhase phase = PHASE_SERVO1_MOVING;

// Track step progression
int servo1Angle = 0;
int servo2Angle = 180;

// Timing control
unsigned long lastStepTime = 0;
bool started = false;

void setAllLEDsOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void applyLEDPhase() {
  // LEDs behavior per requirements:
  // - When Servo1 is moving: LED1 on
  // - When Servo2 is moving: LED2 on
  // - When both reach targets and LED3: LED3 on for 2 seconds
  // All others off.
  setAllLEDsOff();
  switch (phase) {
    case PHASE_SERVO1_MOVING:
      digitalWrite(led1, HIGH);
      break;
    case PHASE_SERVO2_MOVING:
      digitalWrite(led2, HIGH);
      break;
    case PHASE_LED3:
      digitalWrite(led3, HIGH);
      break;
  }
}

void resetCycleToInitialPositions() {
  // Initialization on reset/power-up behavior + cycle reset:
  // all LEDs off, both servos at 0 and 180 degrees.
  setAllLEDsOff();

  servo1Angle = 0;
  servo2Angle = 180;

  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  servo1.attach(servo1PWM);
  servo2.attach(servo2PWM);

  resetCycleToInitialPositions();

  // Start phase: Servo1 moving immediately (and stay for 2 seconds per step)
  phase = PHASE_SERVO1_MOVING;
  applyLEDPhase();

  // Requirement: increment/decrement every 2 seconds "immediately move and then stay for 2 seconds".
  // For the very first step in Servo1, we should perform the move right away.
  lastStepTime = millis();
  started = true;

  // Immediately move servo1 to 30 degrees (from 0) on entering moving phase.
  servo1Angle += STEP_DEG;
  if (servo1Angle > 180) servo1Angle = 180;
  servo1.write(servo1Angle);

  // Keep servo2 at 180 until its phase begins
}

void loop() {
  if (!started) return;

  unsigned long now = millis();

  if (phase == PHASE_SERVO1_MOVING) {
    // Wait 2 seconds per step, then increment by 30 until reaching 180
    if (now - lastStepTime >= STEP_INTERVAL_MS) {
      lastStepTime = now;

      if (servo1Angle >= 180) {
        // Already at target; transition to servo2 phase.
        // Requirement: after servo1 reaches 180, then servo2 starts at 180 and decrements.
        phase = PHASE_SERVO2_MOVING;
        applyLEDPhase();

        // On phase entry, servo2 should "immediately move" by one step.
        // Only do that if servo2 is not already at 0.
        if (servo2Angle > 0) {
          servo2Angle -= STEP_DEG; // 180 -> 150
          if (servo2Angle < 0) servo2Angle = 0;
          servo2.write(servo2Angle);
        }
      } else {
        servo1Angle += STEP_DEG;
        if (servo1Angle > 180) servo1Angle = 180;
        servo1.write(servo1Angle);

        if (servo1Angle >= 180) {
          // Next 2s boundary will advance to servo2 phase (as per sequencing),
          // but requirement states servo2 starts after servo1 reaches 180.
          // We'll transition immediately on the step that lands on 180 at the time boundary.
          // Since we just set it to 180 at this boundary, advance now.
          phase = PHASE_SERVO2_MOVING;
          applyLEDPhase();

          if (servo2Angle > 0) {
            servo2Angle -= STEP_DEG; // immediate move in servo2 phase
            if (servo2Angle < 0) servo2Angle = 0;
            servo2.write(servo2Angle);
          }
        }
      }
    }
  } else if (phase == PHASE_SERVO2_MOVING) {
    if (now - lastStepTime >= STEP_INTERVAL_MS) {
      lastStepTime = now;

      if (servo2Angle <= 0) {
        // Already at target, transition to LED3
        phase = PHASE_LED3;
        applyLEDPhase();

        // LED3 should be on for 2 seconds; the cycle will reset after that.
      } else {
        servo2Angle -= STEP_DEG;
        if (servo2Angle < 0) servo2Angle = 0;
        servo2.write(servo2Angle);

        if (servo2Angle <= 0) {
          // Both at targets: servo1 is already 180 and servo2 is now 0.
          // Turn on LED3 immediately (at the time boundary) and hold for 2 seconds.
          phase = PHASE_LED3;
          applyLEDPhase();
        }
      }
    }
  } else if (phase == PHASE_LED3) {
    // Hold LED3 for exactly 2 seconds, then reset and repeat.
    if (now - lastStepTime >= STEP_INTERVAL_MS) {
      // After LED3 lights up for 2 seconds, reset cycle
      resetCycleToInitialPositions();
      phase = PHASE_SERVO1_MOVING;
      applyLEDPhase();

      // Immediately move servo1 by one step (0 -> 30)
      lastStepTime = now;
      if (servo1Angle < 180) {
        servo1Angle += STEP_DEG;
        if (servo1Angle > 180) servo1Angle = 180;
        servo1.write(servo1Angle);
      }
    }
  }
}