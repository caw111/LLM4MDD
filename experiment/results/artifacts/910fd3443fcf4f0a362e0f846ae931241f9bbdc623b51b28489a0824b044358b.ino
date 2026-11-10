#include <Servo.h>

const int buttonPin = 3;
const int servoPin = 9;

const int servoAnglePressed = 180;
const int servoAngleNotPressed = 90;

Servo servoMotor;

void setup() {
  Serial.begin(9600);

  servoMotor.attach(servoPin);

  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonVal = digitalRead(buttonPin);

  int angle;
  if (buttonVal == HIGH) {
    angle = servoAngleNotPressed;
  } else {
    angle = servoAnglePressed;
  }

  servoMotor.write(angle);

  Serial.println(angle);

  delay(15);
}