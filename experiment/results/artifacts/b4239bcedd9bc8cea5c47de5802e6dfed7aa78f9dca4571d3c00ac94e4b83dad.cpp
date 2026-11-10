#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const int buttonPin = 3;
const int servoPin = 9;

const int servoAnglePressed = 180;
const int servoAngleNotPressed = 90;

Servo servoMotor;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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
