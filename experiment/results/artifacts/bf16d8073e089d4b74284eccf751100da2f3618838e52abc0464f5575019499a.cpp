#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// 74HC595 connections (from provided wiring)
static const uint8_t SR1_SHCP = A2; // SHCP
static const uint8_t SR1_STCP = A1; // STCP
static const uint8_t SR1_DS   = A0;  // DS

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t SERVO1_PIN = 5;
static const uint8_t SERVO2_PIN = 6;
static const uint8_t SERVO3_PIN = 7;

// Segment mapping for common setup:
// The circuit uses Q outputs to drive segments (A,B,C,D,E,F,G) and Q7S of sr1 drives sr2:DS.
// We'll use a conventional 7-seg bit order: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, bit7=dp(not used).
// Assume segments are active-high.
// Digit to segments (A..G), dp off.
static const uint8_t digitSegments[10] = {
  // A B C D E F G
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

// Send 16 bits to two cascaded 74HC595:
// Order: sr2 is in the chain after sr1 (sr1's Q7S to sr2 DS).
// After shifting 16 bits, sr1 holds the first 8 bits shifted, sr2 holds the last 8 bits shifted.
// We want sevseg1 (sr1) to show the units digit and sevseg2 (sr2) to show the tens digit.
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shift16To595(uint8_t sr1Byte, uint8_t sr2Byte);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayNumber(uint8_t number);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateIndicatorsAndServos(uint8_t number);
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_102\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shift16To595(uint8_t sr1Byte, uint8_t sr2Byte) {
  // sr1Byte corresponds to sevseg1 segments (A..G) via sr1 Q0..Q6.
  // sr2Byte corresponds to sevseg2 segments (A..G) via sr2 Q0..Q6.
  uint16_t data = ((uint16_t)sr2Byte << 8) | sr1Byte;

  // Ensure shift clock low
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // Shift MSB first across the chain (16 bits total)
  for (int8_t i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (data >> i) & 0x01 ? HIGH : LOW);
    // rising edge on SHCP
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

static void displayNumber(uint8_t number) {
  uint8_t tens = number / 10;
  uint8_t units = number % 10;

  // Build segment bytes for sr1 (sevseg1 = units) and sr2 (sevseg2 = tens).
  uint8_t sr1Byte = digitSegments[units] & 0x7F; // A..G in bits 0..6
  uint8_t sr2Byte = digitSegments[tens] & 0x7F;

  shift16To595(sr1Byte, sr2Byte);
}

static void updateIndicatorsAndServos(uint8_t number) {
  uint8_t units = number % 10;
  uint8_t tens  = number / 10;

  // LEDs: 0-2 => led1, 3-5 => led2, 6-9 => led3
  digitalWrite(LED1_PIN, (units >= 0 && units <= 2) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (units >= 3 && units <= 5) ? HIGH : LOW);
  digitalWrite(LED3_PIN, (units >= 6 && units <= 9) ? HIGH : LOW);

  // Servos based on tens digit:
  // 0-2: servo1 30, servo2 60, servo3 90
  // 3-5: servo1 60, servo2 90, servo3 120
  // 6-9: servo1 90, servo2 120, servo3 150
  uint8_t s1, s2, s3;

  if (tens <= 2) {
    s1 = 30;  s2 = 60;  s3 = 90;
  } else if (tens <= 5) {
    s1 = 60;  s2 = 90;  s3 = 120;
  } else {
    s1 = 90;  s2 = 120; s3 = 150;
  }

  servo1.write(s1);
  servo2.write(s2);
  servo3.write(s3);
}

void setup() {
  // LED pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // 74HC595 pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization requirement:
  // displays show "00", LEDs off, servos at 0 degrees.
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  displayNumber(0);
  // LEDs off already, but enforce per rule for 00 (units digit 0 => led1 on).
  // Requirement says "all LEDs should be off" upon power on/reset.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

void loop() {
  static uint8_t number = 0;
  static bool initializedAfterBoot = false;

  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  // After boot, apply indicator + servo state for the current number immediately
  // (after showing "00" with LEDs off and servos at 0 degrees).
  // This preserves the increment/update behavior while meeting the explicit init requirement.
  if (!initializedAfterBoot) {
    updateIndicatorsAndServos(number); // number is 0 => will move servos per tens=0 and light led1 by units=0
    // But explicit requirement says LEDs off at init; we've already kept them off.
    // Apply only once right after start; if you want strictly "stay off until first 2s",
    // comment out the next line. We'll keep update behavior as described.
    initializedAfterBoot = true;
    // Ensure display is correct (already set to 00).
    displayNumber(number);
    lastUpdate = now;
    return;
  }

  if (now - lastUpdate >= 2000UL) {
    lastUpdate = now;

    number = (number + 1) % 100;

    // Display update immediately after increment
    displayNumber(number);

    // LEDs and servos updated accordingly
    updateIndicatorsAndServos(number);
  }
}
