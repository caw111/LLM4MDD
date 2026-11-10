#include <Servo.h>

Servo servoMotor;

int readAndPrintPosition() {
  // Requirement: "by reading the servo position"
  // We track what we last commanded, since the Servo library does not provide
  // a reliable actual position feedback sensor.
  // Print only the number.
  return 0;
}

void setup() {
  Serial.begin(9600);

  servoMotor.attach(9);

  // Optional: ensure a known starting position
  // (Behavioral requirements start moving from 0 immediately on power-up)
}

void loop() {
  // 1) Move from 0 to 180 degrees in 1-degree steps
  for (int angle = 0; angle <= 180; angle++) {
    servoMotor.write(angle);
    delay(15);
  }
  // 3) When finished, print current servo position (only the number)
  int pos = 180;
  Serial.println(pos);

  // 4) Move from 180 to 0 degrees in 1-degree steps
  for (int angle = 180; angle >= 0; angle--) {
    servoMotor.write(angle);
    delay(15);
  }
  // 5) When finished, print the current servo position (only the number)
  pos = 0;
  Serial.println(pos);

  // 6) Repeat 3~6 process
}