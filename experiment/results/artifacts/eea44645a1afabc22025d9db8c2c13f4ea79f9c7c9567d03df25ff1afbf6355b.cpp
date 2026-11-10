#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

const uint8_t led1 = 2;
const uint8_t led2 = 3;
const uint8_t led3 = 4;

enum State : uint8_t {
  S_SERVO1_MOVING,
  S_SERVO2_MOVING,
  S_LED3_PAUSE,
  S_RESET
};

static State state = S_RESET;
static unsigned long stateStartMs = 0;

static int s1Angle = 0;   // servo1 target/current step tracking
static int s2Angle = 180; // servo2 target/current step tracking

const int stepSize = 30;
const int minAngle = 0;
const int maxAngle = 180;
const unsigned long stepPeriodMs = 2000;
const unsigned long led3DurationMs = 2000;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLEDs(bool l1, bool l2, bool l3);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_90\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLEDs(bool l1, bool l2, bool l3) {
  digitalWrite(led1, l1 ? HIGH : LOW);
  digitalWrite(led2, l2 ? HIGH : LOW);
  digitalWrite(led3, l3 ? HIGH : LOW);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  setLEDs(false, false, false);

  servo1.attach(5); // mega pin 5 -> servo1 PWM
  servo2.attach(6); // mega pin 6 -> servo2 PWM

  // Initialization: servos at 0° and 180°
  s1Angle = 0;
  s2Angle = 180;
  servo1.write(s1Angle);
  servo2.write(s2Angle);

  state = S_RESET;
  stateStartMs = millis();
}

void loop() {
  unsigned long now = millis();

  switch (state) {
    case S_RESET: {
      // Reset cycle: all LEDs off, servos to initial positions, then start servo1 movement
      setLEDs(false, false, false);

      s1Angle = 0;
      s2Angle = 180;

      servo1.write(s1Angle);
      servo2.write(s2Angle);

      // Move servo1 immediately to 30°, then stay for 2 seconds before next increment.
      s1Angle = minAngle + stepSize;
      if (s1Angle > maxAngle) s1Angle = maxAngle;
      servo1.write(s1Angle);

      setLEDs(true, false, false); // LED1 on while servo1 moving

      state = S_SERVO1_MOVING;
      stateStartMs = now;
      break;
    }

    case S_SERVO1_MOVING: {
      // After each 2 seconds, increment servo1 by 30° until reaching 180°
      if (now - stateStartMs >= stepPeriodMs) {
        if (s1Angle >= maxAngle) {
          // Shouldn't happen with current flow, but ensure transition.
          setLEDs(false, false, false);
          // Start servo2 immediately by moving to 150° (decrement by 30 from 180)
          s2Angle = maxAngle - stepSize;
          servo2.write(s2Angle);

          setLEDs(false, true, false); // LED2 on while servo2 moving

          state = S_SERVO2_MOVING;
          stateStartMs = now;
        } else {
          // Next increment
          s1Angle += stepSize;
          if (s1Angle > maxAngle) s1Angle = maxAngle;
          servo1.write(s1Angle);

          if (s1Angle >= maxAngle) {
            // Once at 180°, next stepPeriod ends will transition to servo2 moving
            // We keep LED1 on for the just-finished period per timing rules.
          }
          stateStartMs = now;

          // If reached exactly 180 at the increment, transition after the current 2s wait ends.
          if (s1Angle >= maxAngle) {
            // Keep the state; transition logic occurs next tick after period.
          }
        }
      }

      // Transition condition: if servo1 has reached target (180°) and we've waited the last 2s period
      if (s1Angle >= maxAngle && (now - stateStartMs >= stepPeriodMs)) {
        // Start servo2 immediately decrementing by 30° every 2 seconds
        setLEDs(false, true, false);

        s2Angle = maxAngle - stepSize; // 150°
        servo2.write(s2Angle);

        state = S_SERVO2_MOVING;
        stateStartMs = now;
      }
      break;
    }

    case S_SERVO2_MOVING: {
      // After each 2 seconds, decrement servo2 by 30° until reaching 0°
      if (now - stateStartMs >= stepPeriodMs) {
        if (s2Angle <= minAngle) {
          // Shouldn't happen with current flow, but ensure transition.
          setLEDs(false, false, false);
          setLEDs(false, false, true); // LED3 on when both targets reached
          state = S_LED3_PAUSE;
          stateStartMs = now;
        } else {
          s2Angle -= stepSize;
          if (s2Angle < minAngle) s2Angle = minAngle;
          servo2.write(s2Angle);

          stateStartMs = now;

          if (s2Angle <= minAngle) {
            // Wait the required 2s after the last move, then LED3 for 2 seconds.
            // Transition happens below when condition is met and period elapses.
          }
        }
      }

      // Transition to LED3: when servo2 at 0° and the last 2s period has elapsed
      if (s2Angle <= minAngle && (now - stateStartMs >= stepPeriodMs)) {
        setLEDs(false, false, true); // LED3 on for 2 seconds
        state = S_LED3_PAUSE;
        stateStartMs = now;
      }
      break;
    }

    case S_LED3_PAUSE: {
      // LED3 lights up for 2 seconds before repeating cycle
      if (now - stateStartMs >= led3DurationMs) {
        setLEDs(false, false, false);
        state = S_RESET;
        stateStartMs = now;
      }
      break;
    }
  }
}
