#include <Servo.h>

Servo servo1;

const int pot1Pin = A3; // Minimum angle
const int pot2Pin = A4; // Maximum angle
const int pot3Pin = A5; // Current position within [min, max]

int pot1MinDeg = 0;
int pot2MaxDeg = 90;

void setup() {
  servo1.attach(2);      // mega:2 -> servo1:PWM
  servo1.write(0);       // Default position: 0 degrees
}

void loop() {
  int pot1Val = analogRead(pot1Pin);
  int pot2Val = analogRead(pot2Pin);
  int pot3Val = analogRead(pot3Pin);

  // Map pot1: 0..90 degrees
  int minDeg = map(pot1Val, 0, 1023, 0, 90);

  // Map pot2: 90..180 degrees
  int maxDeg = map(pot2Val, 0, 1023, 90, 180);

  // Ensure min <= max (in case of any inversion due to mapping/reading)
  if (minDeg > maxDeg) {
    int tmp = minDeg;
    minDeg = maxDeg;
    maxDeg = tmp;
  }

  // Map pot3 to range [minDeg, maxDeg]
  int posDeg = map(pot3Val, 0, 1023, minDeg, maxDeg);

  servo1.write(posDeg);
}