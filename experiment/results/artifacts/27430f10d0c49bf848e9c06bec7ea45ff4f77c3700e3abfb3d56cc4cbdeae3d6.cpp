#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_24\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

Servo servo1;

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_24\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotDegrees(int analogPin);
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_24\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_24\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_24\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readPotDegrees(int analogPin) {
  int raw = analogRead(analogPin); // 0..1023
  return raw;
}

void setup() {
  // Default position on power-up/reset
  servo1.attach(2);     // mega:2 -> servo1:PWM
  servo1.write(0);      // 0 degrees

  // Optional stabilization delay
  delay(50);
}

void loop() {
  // Read potentiometers
  int pot1Raw = analogRead(A3); // pot1:SIG -> mega:A3
  int pot2Raw = analogRead(A4); // pot2:SIG -> mega:A4
  int pot3Raw = analogRead(A5); // pot3:SIG -> mega:A5

  // Map pot1 to minimum angle: 0..90 degrees
  int minAngle = map(pot1Raw, 0, 1023, 0, 90);

  // Map pot2 to maximum angle: 90..180 degrees
  int maxAngle = map(pot2Raw, 0, 1023, 90, 180);

  // Ensure valid bounds (servo expects min <= max)
  if (maxAngle < minAngle) {
    int tmp = minAngle;
    minAngle = maxAngle;
    maxAngle = tmp;
  }

  // Map pot3 to current position between minAngle and maxAngle
  int currentAngle = map(pot3Raw, 0, 1023, minAngle, maxAngle);

  servo1.write(currentAngle);

  // Update continuously
  delay(15);
}
