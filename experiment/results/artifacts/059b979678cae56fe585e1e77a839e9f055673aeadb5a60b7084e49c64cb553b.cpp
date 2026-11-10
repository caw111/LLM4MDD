#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

const int btn1Pin = 2;   // mega:2 -> btn1:1.l (green)
const int btn2Pin = 3;   // mega:3 -> btn2:1.l (green)

// RGB LED (common cathode)
const int rgbR = 4;      // mega:4 -> rgb1:R
const int rgbG = 5;      // mega:5 -> rgb1:G
const int rgbB = 6;      // mega:6 -> rgb1:B

// Servo PWM pins
const int servo1Pwm = 7; // mega:7 -> servo1:PWM
const int servo2Pwm = 8; // mega:8 -> servo2:PWM

// Debounce / state confirmation
const unsigned long CONFIRM_MS = 20;   // simple debounce interval
const unsigned long HOLD_MS = 2000;     // states must be maintained for at least 2 seconds

// Tracks current committed state for at least 2 seconds
enum State {
  OFF_INIT,
  BTN1_RED,
  BTN2_BLUE,
  BOTH_GREEN
};

State currentState = OFF_INIT;
unsigned long stateStartMs = 0;

// Debounced button logic
bool lastBtn1Reading = HIGH;
bool lastBtn2Reading = HIGH;
bool btn1Stable = HIGH;
bool btn2Stable = HIGH;
unsigned long lastDebounceMs1 = 0;
unsigned long lastDebounceMs2 = 0;

#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(bool r, bool g, bool b);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(State s);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void readButtonsDebounced(bool &b1, bool &b2);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void maybeTransition(bool btn1Pressed, bool btn2Pressed);
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 170 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_83\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(bool r, bool g, bool b) {
  // common cathode: HIGH turns on the channel
  digitalWrite(rgbR, r ? HIGH : LOW);
  digitalWrite(rgbG, g ? HIGH : LOW);
  digitalWrite(rgbB, b ? HIGH : LOW);
}

void applyState(State s) {
  switch (s) {
    case OFF_INIT:
      // Initial: servos at 0 degrees, RGB off
      servo1.write(0);
      servo2.write(0);
      setRGB(false, false, false);
      break;

    case BTN1_RED:
      servo1.write(90);
      // Requirement: if only btn1 pressed, servo2 returns? Not stated.
      // Keep existing unless overwritten by rule 3. We'll not force servo2 here.
      // However if coming from other states, rule 1 doesn't explicitly say to reset servo2,
      // so follow "If both buttons..." and "When one pressed and another already pressed..."
      // -> we leave servo2 as is.
      setRGB(true, false, false);  // red
      break;

    case BTN2_BLUE:
      servo2.write(180);
      setRGB(false, false, true);  // blue
      break;

    case BOTH_GREEN:
      servo1.write(0);
      servo2.write(0);
      setRGB(false, true, false);  // green
      break;
  }
}

void readButtonsDebounced(bool &b1, bool &b2) {
  // btn inputs are pulled up (since wiring shows switches to GND with internal pullup)
  int r1 = digitalRead(btn1Pin);
  int r2 = digitalRead(btn2Pin);

  unsigned long now = millis();

  if (r1 != lastBtn1Reading) {
    lastDebounceMs1 = now;
    lastBtn1Reading = r1;
  }
  if ((now - lastDebounceMs1) >= CONFIRM_MS) {
    btn1Stable = r1;
  }

  if (r2 != lastBtn2Reading) {
    lastDebounceMs2 = now;
    lastBtn2Reading = r2;
  }
  if ((now - lastDebounceMs2) >= CONFIRM_MS) {
    btn2Stable = r2;
  }

  b1 = btn1Stable;
  b2 = btn2Stable;
}

void maybeTransition(bool btn1Pressed, bool btn2Pressed) {
  unsigned long now = millis();

  // Maintain committed state for at least 2 seconds
  bool stateElapsed = (now - stateStartMs) >= HOLD_MS;

  // If currently maintaining a state and it's not elapsed, do not change behavior.
  if (!stateElapsed && currentState != OFF_INIT) {
    return;
  }

  if (btn1Pressed && btn2Pressed) {
    currentState = BOTH_GREEN;
    stateStartMs = now;
    applyState(currentState);
    // Buttons return to their unpressed state requirement is interpreted as next loop iteration
    // after release; we do not (and cannot) control physical button hardware.
    return;
  }

  if (btn1Pressed && !btn2Pressed) {
    currentState = BTN1_RED;
    stateStartMs = now;
    applyState(currentState);
    return;
  }

  if (!btn1Pressed && btn2Pressed) {
    currentState = BTN2_BLUE;
    stateStartMs = now;
    applyState(currentState);
    return;
  }

  // If no buttons pressed: RGB remains off and servos stay in their current positions.
  if (!btn1Pressed && !btn2Pressed) {
    setRGB(false, false, false); // ensure off
    // Do not modify servos.
    currentState = OFF_INIT; // mark as "no buttons action"
    stateStartMs = now;
  }
}

void setup() {
  // Buttons with internal pull-ups
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);

  // RGB LED pins
  pinMode(rgbR, OUTPUT);
  pinMode(rgbG, OUTPUT);
  pinMode(rgbB, OUTPUT);

  // Servos
  servo1.attach(servo1Pwm);
  servo2.attach(servo2Pwm);

  // Initial positions and RGB off
  currentState = OFF_INIT;
  stateStartMs = millis();
  applyState(currentState);
}

void loop() {
  bool b1Raw, b2Raw;
  readButtonsDebounced(b1Raw, b2Raw);

  // With INPUT_PULLUP: pressed = LOW
  bool btn1Pressed = (b1Raw == LOW);
  bool btn2Pressed = (b2Raw == LOW);

  maybeTransition(btn1Pressed, btn2Pressed);

  // Small delay to reduce loop jitter
  delay(5);
}
