#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>
#include <stdint.h>

Servo servo1;
Servo servo2;
Servo servo3;

// 74HC595 output pins (wired from Mega A0/A1/A2 per circuit)
static const uint8_t DS_PIN  = A0; // DS -> blue -> sevseg1 segments via SR
static const uint8_t STCP_PIN = A1; // STCP -> purple
static const uint8_t SHCP_PIN = A2; // SHCP -> gray

// 7-seg mapping (common wiring depends on the SR/segment polarity in the circuit).
// We'll assume a typical active-high segment driver as displayed.
// Segment bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G.
// We'll generate bitmasks for digits accordingly.
static const uint8_t DIGIT_MASK[10] = {
  // A B C D E F G
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

// Servo angles per number
static const uint8_t SERVO1_ANGLE[10] = {0, 0, 0, 45, 45, 45, 90, 90, 90, 0};
static const uint8_t SERVO2_ANGLE[10] = {45, 45, 45, 90, 90, 90, 135, 135, 135, 0};
static const uint8_t SERVO3_ANGLE[10] = {90, 90, 90, 135, 135, 135, 180, 180, 180, 0};

// Shift out MSB->LSB for 74HC595
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut595(uint8_t value);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(uint8_t d);
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_34\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut595(uint8_t value)
{
  // Typical 74HC595 uses 8-bit shift register:
  // We'll shift out bits Q7..Q0; only Q0..Q6 are connected to segments A..G.
  for (int8_t i = 7; i >= 0; --i)
  {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(STCP_PIN, LOW);

    digitalWrite(DS_PIN, (value & (1 << i)) ? HIGH : LOW);
    digitalWrite(SHCP_PIN, HIGH);
  }

  // Latch
  digitalWrite(STCP_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(STCP_PIN, LOW);
}

// Display digit 0..9 on sevseg1
static void displayDigit(uint8_t d)
{
  if (d > 9) d = 0;

  // DIGIT_MASK uses bits 0..6 -> segments A..G via Q0..Q6.
  // We set bit7 (Q7) to 0 (unused).
  uint8_t out = DIGIT_MASK[d] & 0x7F;
  shiftOut595(out);
}

// Move a servo to angle and wait until movement likely finished.
// Since requirement says "complete its movement before next one starts",
// we use a conservative time based on angle delta.
static void moveServoAndWait(Servo &s, uint8_t targetDeg, uint8_t &currentDeg, unsigned long maxStepMs = 15)
{
  uint8_t startDeg = currentDeg;
  s.write(targetDeg);

  // Estimate travel time: assume up to ~180deg with maxStepMs per degree step.
  // Clamp to reasonable bound to ensure sequential behavior.
  int diff = (int)targetDeg - (int)startDeg;
  if (diff < 0) diff = -diff;

  unsigned long waitMs = (unsigned long)diff * maxStepMs + 100UL;
  delay(waitMs);

  currentDeg = targetDeg;
}

uint8_t currentDigit = 0;
uint8_t s1Deg = 0, s2Deg = 0, s3Deg = 0;

void setup()
{
  // 74HC595 pins
  pinMode(DS_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);

  digitalWrite(DS_PIN, LOW);
  digitalWrite(STCP_PIN, LOW);
  digitalWrite(SHCP_PIN, LOW);

  // Servos attached to PWM pins per circuit:
  // servo1 PWM -> Mega pin 2
  // servo2 PWM -> Mega pin 3
  // servo3 PWM -> Mega pin 4
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);

  // Initialization: display 0 and servos at 0 degrees
  displayDigit(0);
  s1Deg = 0; s2Deg = 0; s3Deg = 0;
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Let servos settle briefly
  delay(300);
}

void loop()
{
  // Display each number for 2 seconds.
  // When the number changes, move servos in sequence (servo1 -> servo2 -> servo3),
  // each completing before the next begins.
  for (uint8_t d = 0; d <= 9; ++d)
  {
    // Show digit
    displayDigit(d);

    // Move servos only when number changes from previous value
    if (d != currentDigit)
    {
      moveServoAndWait(servo1, SERVO1_ANGLE[d], s1Deg);
      moveServoAndWait(servo2, SERVO2_ANGLE[d], s2Deg);
      moveServoAndWait(servo3, SERVO3_ANGLE[d], s3Deg);
    }
    else
    {
      // For the first digit on the first run (0), ensure servos are at initial state
      // (already set in setup, but keep behavior consistent)
      if (SERVO1_ANGLE[d] != s1Deg) moveServoAndWait(servo1, SERVO1_ANGLE[d], s1Deg);
      if (SERVO2_ANGLE[d] != s2Deg) moveServoAndWait(servo2, SERVO2_ANGLE[d], s2Deg);
      if (SERVO3_ANGLE[d] != s3Deg) moveServoAndWait(servo3, SERVO3_ANGLE[d], s3Deg);
    }

    currentDigit = d;

    // Keep number visible for 2 seconds
    delay(2000);
  }

  // After reaching 9, reset to 0 and repeat (handled by for-loop).
}
