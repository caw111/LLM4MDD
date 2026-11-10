#include <Arduino.h>
#include <Servo.h>

static const uint8_t SR1_SHCP = A1; // SHCP (clock) <- A1
static const uint8_t SR1_STCP = A0; // STCP (latch) <- A0
static const uint8_t SR1_DS   = A2; // DS (data)  <- A2

static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

static const uint8_t SERVO1_PWM_PIN = 2; // mega:2
static const uint8_t SERVO2_PWM_PIN = 3; // mega:3
static const uint8_t SERVO3_PWM_PIN = 4; // mega:4

// sevseg1 segment mapping from Q0..Q6 to A..G as per wiring:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Use standard common-cathode segment truth table for digits 0..9.
// If your display is common-anode, this would need inversion, but we must preserve behavior to wiring.
static uint8_t digitToSegmentsCommonCathode(uint8_t digit) {
  // bits: 0=A,1=B,2=C,3=D,4=E,5=F,6=G
  // 0: A B C D E F
  static const uint8_t segMap[10] = {
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),          // 0
    (1<<1)|(1<<2),                                      // 1
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                // 2
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                // 3
    (1<<1)|(1<<2)|(1<<5)|(1<<6),                       // 4
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                // 5
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),       // 6
    (1<<0)|(1<<1)|(1<<2),                               // 7
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6), // 8
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)          // 9
  };
  if (digit > 9) digit = 0;
  return segMap[digit];
}

// Shift register output: Q0->A, Q1->B, ... Q6->G
static void sr1WriteSegments(uint8_t segBitsAtoG) {
  // Create 8-bit value for SR outputs Q0..Q7:
  // Q0..Q6 correspond to A..G respectively, Q7 unused -> 0.
  // segBitsAtoG bits: 0=A..6=G -> directly to Q0..Q6.
  uint8_t out = segBitsAtoG & 0x7F;

  // For 74HC595: shift out MSB first or LSB first depends on our bit order.
  // We assume DS bit shifted corresponds to Q0..Q7 after 8 clocks when shifting MSB first
  // could be confusing; to guarantee correct Qn mapping, shift LSB-first so that the first
  // shifted bit ends up at Q0 after 8 clocks in the typical SPI-like behavior.
  // We'll implement explicit LSB-first for Q0..Q7.
  digitalWrite(SR1_STCP, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR1_SHCP, LOW);
    digitalWrite(SR1_DS, (out >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
  }
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(1);
}

Servo servo1, servo2, servo3;

static uint8_t readPot0to1023(uint8_t pin) {
  return (uint8_t)(analogRead(pin) & 0xFF);
}

static int mapPotToDigit0to9(int pot) {
  if (pot < 0) pot = 0;
  if (pot > 1023) pot = 1023;
  // Map 0..1023 -> 0..9
  return (pot * 10) / 1024; // ensures 1023 maps to 9
}

static int mapPotToAngle0to90(int pot) {
  if (pot < 0) pot = 0;
  if (pot > 1023) pot = 1023;
  return (pot * 90) / 1023;
}

static int mapPotToAngle90to180(int pot) {
  if (pot < 0) pot = 0;
  if (pot > 1023) pot = 1023;
  return 90 + (pot * 90) / 1023;
}

static int mapPotToAngle180to270(int pot) {
  if (pot < 0) pot = 0;
  if (pot > 1023) pot = 1023;
  return 180 + (pot * 90) / 1023;
}

static int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static void moveServoSmooth(Servo &s, int &currentAngle, int targetAngle) {
  // Smooth movement: step by 1 degree with a small delay.
  // Keep it responsive but maintain smoothness.
  const int stepDelayMs = 10;
  const int step = (targetAngle >= currentAngle) ? 1 : -1;

  while (currentAngle != targetAngle) {
    currentAngle += step;
    s.write(currentAngle);
    delay(stepDelayMs);
  }
}

void setup() {
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);
  servo3.attach(SERVO3_PWM_PIN);

  // Initialize with current pot readings
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);

  int digit = mapPotToDigit0to9(pot1);
  uint8_t segBits = digitToSegmentsCommonCathode((uint8_t)digit);
  sr1WriteSegments(segBits);

  int a1 = mapPotToAngle0to90(pot2);
  int a2 = mapPotToAngle90to180(pot2);
  int a3 = mapPotToAngle180to270(pot2);
  // Clamp servo3 if it doesn't support beyond 180.
  a3 = clampInt(a3, 0, 180);

  // Move smoothly to initial angles
  static int curr1 = 0, curr2 = 0, curr3 = 0;
  // For initial move, set current to target to avoid long start ramp.
  curr1 = a1; curr2 = a2; curr3 = a3;
  servo1.write(curr1);
  servo2.write(curr2);
  servo3.write(curr3);

  // State maintenance for at least 2 seconds
  delay(2000);
}

void loop() {
  static int prevDigit = -1;
  static int curr1 = 0, curr2 = 0, curr3 = 0;

  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);

  int digit = mapPotToDigit0to9(pot1);

  int target1 = mapPotToAngle0to90(pot2);
  int target2 = mapPotToAngle90to180(pot2);
  int target3 = mapPotToAngle180to270(pot2);
  target3 = clampInt(target3, 0, 180);

  bool changed = false;
  if (digit != prevDigit) changed = true;
  if (target1 != curr1) changed = true;
  if (target2 != curr2) changed = true;
  if (target3 != curr3) changed = true;

  if (changed) {
    // Update 7-seg display
    uint8_t segBits = digitToSegmentsCommonCathode((uint8_t)digit);
    sr1WriteSegments(segBits);
    prevDigit = digit;

    // Smoothly move servos to their respective angles
    moveServoSmooth(servo1, curr1, target1);
    moveServoSmooth(servo2, curr2, target2);
    moveServoSmooth(servo3, curr3, target3);

    // Maintain state for at least 2 seconds for verification
    delay(2000);
  } else {
    // Even if unchanged, keep continuous update behavior.
    // Ensure we still refresh display periodically without moving servos.
    // Also maintain state for at least 2 seconds when no change is detected.
    uint8_t segBits = digitToSegmentsCommonCathode((uint8_t)digit);
    sr1WriteSegments(segBits);
    delay(2000);
  }
}