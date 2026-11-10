#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

static const uint8_t servoPin = 9;
static const uint8_t buttonPin = 3;

static const int servoAngle1 = 180; // when pressed
static const int servoAngle2 = 90;  // when not pressed

Servo servoObj;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);

  servoObj.attach(servoPin);

  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonValue = digitalRead(buttonPin);

  // Expected behavior: print the angle of the servo motor continuously
  // Set servo angle based on button pressed/not pressed
  int angle = (buttonValue == HIGH) ? servoAngle2 : servoAngle1;

  servoObj.write(angle);
  Serial.println(angle);

  delay(20);
}
