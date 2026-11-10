#include <Servo.h>

Servo servo1;
Servo servo2;

const int led1Pin = 2;   // mega:2 -> led1:A (red)
const int led2Pin = 3;   // mega:3 -> led2:A (red)

// Bargraph segments A1..A10 are on pins 4..13 respectively
const int barPins[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

static void setBargraphForServo1Angle(int angleDeg) {
  // 0° -> 0 segments, 90° -> 5 segments, 180° -> 10 segments
  // Use 10 segments across 0..180 inclusive:
  // segments = round(angle/180 * 10) => 90 -> 5, 180 -> 10, 0 -> 0
  int segments = (int)((long long)angleDeg * 10 + 90) / 180; // rounded
  if (segments < 0) segments = 0;
  if (segments > 10) segments = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(barPins[i], (i < segments) ? HIGH : LOW);
  }
}

static void updateLEDsForAngles(int servo1AngleDeg, int servo2AngleDeg) {
  // LED1 on when Servo1 between 0 and 90 (inclusive of 0,90), off when 91..180
  if (servo1AngleDeg >= 0 && servo1AngleDeg <= 90) digitalWrite(led1Pin, HIGH);
  else digitalWrite(led1Pin, LOW);

  // LED2 on when Servo2 between 90 and 180 (inclusive of 90,180), off when 0..89
  if (servo2AngleDeg >= 90 && servo2AngleDeg <= 180) digitalWrite(led2Pin, HIGH);
  else digitalWrite(led2Pin, LOW);
}

void setup() {
  // Initialize LED pins
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // Initialize bargraph pins
  for (int i = 0; i < 10; i++) {
    pinMode(barPins[i], OUTPUT);
    digitalWrite(barPins[i], LOW);
  }

  // Start with all LEDs off
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  // Attach servos to PWM pins as per wiring: servo1 PWM on mega:44, servo2 PWM on mega:45
  servo1.attach(44);
  servo2.attach(45);

  // Start servos at 0°
  servo1.write(0);
  servo2.write(0);

  // Keep LEDs/bargraph off initially (consistent with 0° rules)
  updateLEDsForAngles(0, 0);
  setBargraphForServo1Angle(0);
}

void loop() {
  // Servo1: 0 to 180 in steps of 10, pause 2000ms at each step
  // Servo2: 180 to 0 in decrements of 10, pause 2000ms at each step
  for (int s1 = 0, s2 = 180; s1 <= 180 && s2 >= 0; s1 += 10, s2 -= 10) {
    servo1.write(s1);
    servo2.write(s2);

    updateLEDsForAngles(s1, s2);
    setBargraphForServo1Angle(s1);

    delay(2000);
  }
}