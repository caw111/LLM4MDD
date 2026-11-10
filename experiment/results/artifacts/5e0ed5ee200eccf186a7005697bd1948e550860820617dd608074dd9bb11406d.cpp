#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

// Potentiometer input
static const uint8_t POT_PIN = A3;

// Bargraph LED pins (A1 to A10 as required)
static const uint8_t LED_PINS[10] = {
  A1, A2, A3, A4, A5, A6, A7, A8, A9, A10
};

// Ensure sequential fully on/off (no PWM to LEDs)
static const uint8_t LED_ON_STATE = HIGH;

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapToLEDCount(int potValue);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapToServo1Angle(int potValue);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapToServo2Angle(int potValue);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapToLEDCount(int potValue) {
  // 0..1023 -> 0..10
  long v = potValue;
  long count = (v * 10L) / 1023L;
  if (count < 0) count = 0;
  if (count > 10) count = 10;
  return (int)count;
}

static int mapToServo1Angle(int potValue) {
  // 0% -> 0°, 100% -> 180°
  long v = potValue;
  long angle = (v * 180L) / 1023L;
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return (int)angle;
}

static int mapToServo2Angle(int potValue) {
  // 0% -> 180°, 100% -> 0° (opposite direction)
  int a1 = mapToServo1Angle(potValue); // 0..180
  return 180 - a1; // 180..0
}

void setup() {
  servo1.attach(2); // mega:2 -> servo1:PWM
  servo2.attach(3); // mega:3 -> servo2:PWM

  // Initialize servos at 0° as required
  servo1.write(0);
  servo2.write(0);

  // Bargraph pins as outputs, all off initially
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], !LED_ON_STATE); // OFF
  }
}

void loop() {
  int pot = analogRead(POT_PIN); // 0..1023

  // Map potentiometer linearly
  int servo1Angle = mapToServo1Angle(pot);
  int servo2Angle = mapToServo2Angle(pot);
  int ledCount = mapToLEDCount(pot); // 0..10

  // Synchronize servos
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);

  // Sequential fully-on LEDs from left to right
  for (int i = 0; i < 10; i++) {
    if (i < ledCount) {
      digitalWrite(LED_PINS[i], LED_ON_STATE);
    } else {
      digitalWrite(LED_PINS[i], !LED_ON_STATE);
    }
  }

  // Small delay to reduce rapid updates without causing flicker
  delay(20);
}
