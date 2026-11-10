#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

int currentAngle = 0;
int step = 10;

unsigned long holdStart = 0;
bool holding = false;
const unsigned long holdDurationMs = 2000;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_20\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  servo1.attach(9);          // mega:2 -> PWM in provided wiring context
  servo1.write(0);          // start at 0° upon power/reset
  currentAngle = 0;
  step = 10;

  holdStart = millis();
  holding = true;
}

void loop() {
  // Hold each position for 2 seconds
  if (holding) {
    if (millis() - holdStart >= holdDurationMs) {
      holding = false;
    } else {
      return;
    }
  }

  // Move to next angle in the sweep
  currentAngle += step;

  if (currentAngle >= 180) {
    currentAngle = 180;
    step = -10;
  } else if (currentAngle <= 0) {
    currentAngle = 0;
    step = 10;
  }

  servo1.write(currentAngle);

  // Start hold timer at the new position
  holding = true;
  holdStart = millis();
}
