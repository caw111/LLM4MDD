#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// Bargraph pins A1..A10 mapped per circuit
const uint8_t barPins[10] = {A1, A2, A3, A4, A5, A6, A7, A8, A9, A10};

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarAllOff();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarSegmentsOn(uint8_t startIndexInclusive, uint8_t endIndexInclusive);
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_28\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarAllOff() {
  for (uint8_t i = 0; i < 10; i++) digitalWrite(barPins[i], LOW);
}

static void setBarSegmentsOn(uint8_t startIndexInclusive, uint8_t endIndexInclusive) {
  // Indices: 0..9 correspond to A1..A10
  for (uint8_t i = 0; i < 10; i++) {
    bool on = (i >= startIndexInclusive && i <= endIndexInclusive);
    digitalWrite(barPins[i], on ? HIGH : LOW);
  }
}

void setup() {
  // Servos: PWM pins per circuit
  // servo1: mega pin 2
  // servo2: mega pin 3
  // servo3: mega pin 4
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);

  // All servos start at 0°
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // LED bargraph segments A1..A10
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW); // all segments off at start
  }

  // Optional: allow initial positioning
  delay(20);
}

void loop() {
  // --- Phase 1 ---
  // Servo1 to 180° and keep for 2 seconds
  servo1.write(180);

  // First 3 segments (A1..A3) on for 2 seconds
  setBarSegmentsOn(0, 2);

  delay(2000);

  // --- Phase 2 ---
  // Servo2 to 180° and keep for 2 seconds
  servo2.write(180);

  // Next 3 segments on (A1..A6 total 6 segments)
  setBarSegmentsOn(0, 5);

  delay(2000);

  // --- Phase 3 ---
  // Servo3 to 180° and keep for 2 seconds
  servo3.write(180);

  // Final 4 segments on (A1..A10 total 10 segments)
  setBarSegmentsOn(0, 9);

  delay(2000);

  // --- Reset ---
  // All servos back to 0° simultaneously
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // All LED segments off
  setBarAllOff();

  // Pause for 2 seconds before restarting
  delay(2000);
}
