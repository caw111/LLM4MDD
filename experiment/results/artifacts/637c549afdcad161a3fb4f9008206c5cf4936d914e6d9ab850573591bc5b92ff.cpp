#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_potentiometer_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const uint8_t POTENTIOMETER_PIN = A0;
const uint8_t SERVO_PIN = 9;

Servo servoMotor;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_potentiometer_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_potentiometer_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_potentiometer_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(POTENTIOMETER_PIN, INPUT);
  Serial.begin(9600);
  servoMotor.attach(SERVO_PIN);
}

void loop() {
  const int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  const int servoAngle = map(potentiometerValue, 0, 1023, 0, 180);

  servoMotor.write(servoAngle);

  // read() returns the commanded angle, not physical position feedback.
  Serial.println(servoMotor.read());

  delay(15);
}
