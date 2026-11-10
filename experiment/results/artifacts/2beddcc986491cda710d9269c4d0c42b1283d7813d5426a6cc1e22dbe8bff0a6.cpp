#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// Pins from provided circuit
// LEDs: led1=A2, led2=A3, led3=A4 (through mega:2/3/4)
const uint8_t led1Pin = 2;
const uint8_t led2Pin = 3;
const uint8_t led3Pin = 4;

// Servos PWM: servo1=5, servo2=6, servo3=7
const uint8_t servo1Pin = 5;
const uint8_t servo2Pin = 6;
const uint8_t servo3Pin = 7;

// Bar graph segments: A1-A8 on mega pins 8-13, 44-45
const uint8_t barPins[8] = {8, 9, 10, 11, 12, 13, 44, 45};

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllLEDsOff();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllBarOff();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllServosTo0();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarRangeOn(uint8_t startSegInclusive, uint8_t endSegInclusive);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllLEDsOff() {
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
}

static void setAllBarOff() {
  for (uint8_t i = 0; i < 8; i++) digitalWrite(barPins[i], LOW);
}

static void setAllServosTo0() {
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

static void setBarRangeOn(uint8_t startSegInclusive, uint8_t endSegInclusive) {
  // segments are 1..8 logically; map to array indices 0..7
  for (uint8_t seg = 1; seg <= 8; seg++) {
    uint8_t idx = seg - 1;
    if (seg >= startSegInclusive && seg <= endSegInclusive) {
      digitalWrite(barPins[idx], HIGH);
    } else {
      digitalWrite(barPins[idx], LOW);
    }
  }
}

void setup() {
  // LEDs
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);

  // Bar graph
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(barPins[i], OUTPUT);
  }

  // Initialize servos at 0 degrees
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  setAllLEDsOff();
  setAllBarOff();
  setAllServosTo0();

  // Persist initial state for 2 seconds
  delay(2000);
}

void loop() {
  const unsigned long stateDurationMs = 2000;

  // ----- State 1 (2 seconds) -----
  servo1.write(60);
  // Ensure others stay at their current/previous values (they start at 0 and should change only per state)
  // But to avoid overlap ambiguity, keep servo2/servo3 at 0 until their states.
  servo2.write(0);
  servo3.write(0);

  digitalWrite(led1Pin, HIGH);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);

  // First two segments (A1-A2)
  setBarRangeOn(1, 2);

  delay(stateDurationMs);

  // ----- State 2 (2 seconds) -----
  servo1.write(60);   // keep configuration for this state
  servo2.write(120);
  servo3.write(0);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, HIGH);
  digitalWrite(led3Pin, LOW);

  // Next three segments (A3-A5)
  setBarRangeOn(3, 5);

  delay(stateDurationMs);

  // ----- State 3 (2 seconds) -----
  servo1.write(60);
  servo2.write(120);
  servo3.write(180);

  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, HIGH);

  // Final three segments (A6-A8)
  setBarRangeOn(6, 8);

  delay(stateDurationMs);

  // ----- Reset (2 seconds) -----
  setAllServosTo0();
  setAllLEDsOff();
  setAllBarOff();

  delay(stateDurationMs);

  // Sequence repeats indefinitely
}
