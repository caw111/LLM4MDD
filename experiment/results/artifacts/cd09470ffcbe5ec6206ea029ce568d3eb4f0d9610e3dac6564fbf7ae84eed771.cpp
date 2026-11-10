#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring:
// A0 -> sr1 DS; A1 -> both STCP; A2 -> both SHCP.
// sr1 Q7S -> sr2 DS.
// sr1 drives the tens digit; sr2 drives the units digit.
// Both displays are common-anode: LOW turns a segment on.
constexpr uint8_t DATA_PIN = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr uint8_t SERVO1_PIN = 2;
constexpr uint8_t SERVO2_PIN = 3;
constexpr uint8_t SERVO3_PIN = 4;

constexpr unsigned long INCREMENT_INTERVAL_MS = 2000UL;
constexpr int SERVO_MIN_US = 544;
constexpr int SERVO_MAX_US = 2400;

// Bits 0..6 correspond to segments A..G; bit 7 is unused.
const uint8_t DIGIT_PATTERNS[10] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F  // 9
};

Servo servo1;
Servo servo2;
Servo servo3;

uint8_t displayedNumber = 0;
unsigned long lastIncrementMs = 0;

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint8_t number);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void setServoAngleTenths(Servo &servo, uint16_t angleTenths);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void updateServos(uint8_t number);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_33\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // The first byte travels through sr1 into sr2.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[units]));
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,
           static_cast<uint8_t>(~DIGIT_PATTERNS[tens]));

  digitalWrite(LATCH_PIN, HIGH);
}

void setServoAngleTenths(Servo &servo, uint16_t angleTenths) {
  // Preserve fractional-degree targets using microsecond pulse resolution.
  const unsigned long pulseOffset =
      (static_cast<unsigned long>(angleTenths) *
           (SERVO_MAX_US - SERVO_MIN_US) +
       900UL) /
      1800UL;

  servo.writeMicroseconds(SERVO_MIN_US + static_cast<int>(pulseOffset));
}

void updateServos(uint8_t number) {
  setServoAngleTenths(servo1, static_cast<uint16_t>(number) * 18U);
  setServoAngleTenths(servo2, static_cast<uint16_t>(number) * 12U);
  setServoAngleTenths(servo3, static_cast<uint16_t>(number) * 6U);
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  displayedNumber = 0;
  updateDisplay(displayedNumber);

  // Preload zero-degree pulses before enabling servo outputs.
  servo1.writeMicroseconds(SERVO_MIN_US);
  servo2.writeMicroseconds(SERVO_MIN_US);
  servo3.writeMicroseconds(SERVO_MIN_US);

  servo1.attach(SERVO1_PIN, SERVO_MIN_US, SERVO_MAX_US);
  servo2.attach(SERVO2_PIN, SERVO_MIN_US, SERVO_MAX_US);
  servo3.attach(SERVO3_PIN, SERVO_MIN_US, SERVO_MAX_US);

  updateServos(displayedNumber);
  lastIncrementMs = millis();
}

void loop() {
  const unsigned long now = millis();

  // Unsigned subtraction remains valid across millis() rollover.
  if (now - lastIncrementMs >= INCREMENT_INTERVAL_MS) {
    lastIncrementMs += INCREMENT_INTERVAL_MS;

    displayedNumber = (displayedNumber + 1U) % 100U;
    updateDisplay(displayedNumber);
    updateServos(displayedNumber);
  }
}
