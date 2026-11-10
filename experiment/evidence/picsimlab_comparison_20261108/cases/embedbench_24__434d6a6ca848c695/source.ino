#include <Servo.h>

const uint8_t POT1_PIN = A3;
const uint8_t POT2_PIN = A4;
const uint8_t POT3_PIN = A5;
const uint8_t SERVO_PIN = 2;

Servo servo1;

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  servo1.write(0);
  servo1.attach(SERVO_PIN);
  servo1.write(0);
  delay(500);  // Allow the servo to reach its startup position.
}

void loop() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);
  const int pot3Value = analogRead(POT3_PIN);

  const int minAngle = map(pot1Value, 0, 1023, 0, 90);
  const int maxAngle = map(pot2Value, 0, 1023, 90, 180);
  const int currentAngle = map(pot3Value, 0, 1023, minAngle, maxAngle);

  servo1.write(currentAngle);
  delay(20);
}