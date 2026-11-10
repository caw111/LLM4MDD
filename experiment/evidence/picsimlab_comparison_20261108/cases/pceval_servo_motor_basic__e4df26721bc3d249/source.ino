#include <Servo.h>

Servo servoMotor;

const int SERVO_PIN = 9;
const int STEP_DELAY_MS = 15;

void setup() {
  Serial.begin(9600);
  servoMotor.attach(SERVO_PIN);
}

void loop() {
  for (int angle = 0; angle <= 180; ++angle) {
    servoMotor.write(angle);
    delay(STEP_DELAY_MS);
  }
  Serial.println(servoMotor.read());

  for (int angle = 180; angle >= 0; --angle) {
    servoMotor.write(angle);
    delay(STEP_DELAY_MS);
  }
  Serial.println(servoMotor.read());
}