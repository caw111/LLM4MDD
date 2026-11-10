#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const int btn1Pin = 2;
const int btn2Pin = 3;
const int btn3Pin = 4;

const int servo1PwmPin = 8;
const int servo2PwmPin = 9;

// RGB LED (common cathode)
const int rgbRPin = 5;
const int rgbGPin = 6;
const int rgbBPin = 7;

Servo servo1;
Servo servo2;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(bool r, bool g, bool b);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void goToState0();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void stateBtn1();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void stateBtn2();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void stateBtn3();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool buttonPressed(int pin);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void waitForRelease(int pin);
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_82\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGB(bool r, bool g, bool b) {
  digitalWrite(rgbRPin, r ? HIGH : LOW);
  digitalWrite(rgbGPin, g ? HIGH : LOW);
  digitalWrite(rgbBPin, b ? HIGH : LOW);
}

void goToState0() {
  servo1.write(0);
  servo2.write(0);
  setRGB(false, false, false); // LED off on reset
  delay(2000);
}

void stateBtn1() {
  servo1.write(90);
  servo2.write(servo2.read()); // maintain current position if already moved
  setRGB(true, false, false);   // red
  delay(2000);
}

void stateBtn2() {
  servo2.write(90);
  servo1.write(servo1.read()); // maintain current position if already moved
  setRGB(false, true, false);   // green
  delay(2000);
}

void stateBtn3() {
  servo1.write(0);
  servo2.write(0);
  setRGB(false, false, true);   // blue
  delay(2000);
}

bool buttonPressed(int pin) {
  // Buttons connected with pull-up to +5V via wiring; switch to GND when pressed.
  // So pressed == LOW.
  return digitalRead(pin) == LOW;
}

void waitForRelease(int pin) {
  while (digitalRead(pin) == LOW) {
    delay(5);
  }
}

void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);

  pinMode(rgbRPin, OUTPUT);
  pinMode(rgbGPin, OUTPUT);
  pinMode(rgbBPin, OUTPUT);

  // Attach servos
  servo1.attach(servo1PwmPin);
  servo2.attach(servo2PwmPin);

  // Initialization: both servos at 0 degrees, RGB LED off
  setRGB(false, false, false);
  servo1.write(0);
  servo2.write(0);

  delay(2000); // allow verification after power/reset
}

void loop() {
  // Check buttons; when a button is pressed, execute its state
  if (buttonPressed(btn1Pin)) {
    waitForRelease(btn1Pin);
    stateBtn1();
  } else if (buttonPressed(btn2Pin)) {
    waitForRelease(btn2Pin);
    stateBtn2();
  } else if (buttonPressed(btn3Pin)) {
    waitForRelease(btn3Pin);
    stateBtn3();
  }
}
