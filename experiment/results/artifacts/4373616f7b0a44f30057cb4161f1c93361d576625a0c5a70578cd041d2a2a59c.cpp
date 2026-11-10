#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_servo_motor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servoMotor;

const int SERVO_PIN = 9;
const int STEP_DELAY_MS = 15;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_servo_motor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_servo_motor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_servo_motor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
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
