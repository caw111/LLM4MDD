#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

constexpr uint8_t SERVO_PIN = 9;
constexpr uint8_t BUTTON_PIN = 3;
constexpr int PRESSED_ANGLE = 180;
constexpr int RELEASED_ANGLE = 90;

Servo servo;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_servo_motor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);  // External pull-up; pressed reads LOW.
  servo.attach(SERVO_PIN);
  servo.write(digitalRead(BUTTON_PIN) == LOW
                  ? PRESSED_ANGLE
                  : RELEASED_ANGLE);
}

void loop() {
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  servo.write(pressed ? PRESSED_ANGLE : RELEASED_ANGLE);
  Serial.println(servo.read());
}
