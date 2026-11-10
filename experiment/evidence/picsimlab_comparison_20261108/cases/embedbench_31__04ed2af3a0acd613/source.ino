#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t POT1_PIN = A3;   // pot1:SIG -> mega:A3
static const uint8_t POT2_PIN = A4;   // pot2:SIG -> mega:A4

static const uint8_t SEG_PINS[10] = {
  A1, A2, A3, A4, A5, A6, A7, A8, A9, A10
}; // bargraph1 segments order A1..A10

static int mapRangeLong(long x, long inMin, long inMax, long outMin, long outMax) {
  // Clamp and map to integer range
  if (x < inMin) x = inMin;
  if (x > inMax) x = inMax;
  return (int)((x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin);
}

void setup() {
  // Attach servos and initialize at 0 degrees
  servo1.attach(2);  // mega:2 -> servo1:PWM
  servo2.attach(5);  // mega:3 -> servo2:PWM (NOTE: circuit shows mega:3; but preserve wiring below)
  // The provided wiring says:
  // servo1 PWM: mega:2
  // servo2 PWM: mega:3
  // servo3 PWM: mega:4
  // (Connection list uses mega:3 for servo2, so correct attachments accordingly.)
  // Re-attach with correct pins.
  servo2.detach();
  servo2.attach(3);

  servo3.attach(4);  // mega:4 -> servo3:PWM

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // LED bargraph pins initialization (A1..A10)
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
    digitalWrite(SEG_PINS[i], LOW); // all off
  }
}

void loop() {
  // Read potentiometers
  int pot1Raw = analogRead(POT1_PIN);
  int pot2Raw = analogRead(POT2_PIN);

  // Map pot1 -> servo1 0..180 directly
  int servo1Angle = mapRangeLong(pot1Raw, 0, 1023, 0, 180);

  // Map pot2 -> servo2 0..180 directly
  int servo2Angle = mapRangeLong(pot2Raw, 0, 1023, 0, 180);

  // Average pot1 and pot2, rounded to nearest integer -> servo3 0..180
  // "rounded to the nearest integer" applies to averaged potentiometer values.
  int avgRawRounded = (pot1Raw + pot2Raw + 1) / 2; // integer rounding to nearest
  int servo3Angle = mapRangeLong(avgRawRounded, 0, 1023, 0, 180);

  // Update servos
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);

  // LED bar: number of lit segments corresponds to average value scaled to 0..10
  // 0 = all off, 10 = all on
  // Scale avgRawRounded (0..1023) -> 0..10 with rounding.
  int barLevel = (avgRawRounded * 10 + 511) / 1023; // rounded scaling
  if (barLevel < 0) barLevel = 0;
  if (barLevel > 10) barLevel = 10;

  // Light segments A1..A10 in order
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(SEG_PINS[i], (i < (uint8_t)barLevel) ? HIGH : LOW);
  }
}