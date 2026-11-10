#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_66\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

const uint8_t BTN_PIN = 2;

// Sequence states (1..5). After state 5, next press returns to state 1.
const uint8_t NUM_STATES = 5;

const int servo1Angles[NUM_STATES] = { 0,  90, 180,  45,   0 };
const int servo2Angles[NUM_STATES] = { 0,  45,  90, 135,   0 };
const int servo3Angles[NUM_STATES] = { 0, 135,   0,  90,   0 };

uint8_t currentState = 0; // 0 = initial not-yet-applied; pressing advances to state 1

// Debounce
const unsigned long DEBOUNCE_DELAY_MS = 50;
int lastReading = HIGH;
int stableReading = HIGH;
unsigned long lastDebounceTime = 0;

// Track button press edges to advance once per press
bool wasStablePressed = false;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_66\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(uint8_t stateNumber);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_66\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_66\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_66\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(uint8_t stateNumber) {
  // stateNumber is 1..5
  uint8_t idx = stateNumber - 1;
  servo1.write(servo1Angles[idx]);
  servo2.write(servo2Angles[idx]);
  servo3.write(servo3Angles[idx]);
}

void setup() {
  servo1.attach(3); // mega:3 -> servo1:PWM
  servo2.attach(4); // mega:4 -> servo2:PWM
  servo3.attach(5); // mega:5 -> servo3:PWM

  pinMode(BTN_PIN, INPUT_PULLUP); // Circuit uses btn1:1.l to mega:2, btn1:2.r to GND; pull-up matches wiring.

  // Initialize all servos to 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Initialize debounce tracking
  lastReading = digitalRead(BTN_PIN);
  stableReading = lastReading;
  lastDebounceTime = millis();

  currentState = 1;           // System starts at State 1 (0,0,0)
  applyState(currentState);   // Explicitly set to state angles
  wasStablePressed = (stableReading == LOW);

  // Ensure the requirement "each state maintained for at least 2 seconds before transitioning"
  // is respected by only allowing transitions on button *press*; additionally, enforce a 2s
  // minimum time between accepted transitions.
}

void loop() {
  // Read button with debouncing
  int reading = digitalRead(BTN_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if (millis() - lastDebounceTime >= DEBOUNCE_DELAY_MS) {
    if (reading != stableReading) {
      stableReading = reading;

      bool isPressed = (stableReading == LOW);

      // Detect a stable press transition (HIGH->LOW), advance once per press
      if (isPressed && !wasStablePressed) {
        static unsigned long lastAcceptedPressTime = 0;

        // Enforce minimum 2 seconds between state transitions (maintain each state for at least 2 seconds).
        unsigned long now = millis();
        if (lastAcceptedPressTime == 0 || (now - lastAcceptedPressTime) >= 2000UL) {
          lastAcceptedPressTime = now;

          // Advance state: 1->2->3->4->5->1
          currentState++;
          if (currentState > NUM_STATES) currentState = 1;

          applyState(currentState);
        }
      }

      wasStablePressed = isPressed;
    }
  }
}
