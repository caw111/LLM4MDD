#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servoMotor;

const int POT_PIN = A0;   // potentiometer signal pin connected to a0
const int SERVO_PIN = 9;  // servo motor PWM pin connected to pin9

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_potentiometer_servo_motor\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // 1. Generate Servo object
  servoMotor.attach(SERVO_PIN);

  // 2. Set potentiometer signal pin a0 as INPUT
  pinMode(POT_PIN, INPUT);

  // For printing servo value to Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // 3. Read potentiometer value (0..1023)
  int potValue = analogRead(POT_PIN);

  // 4. Map to servo range (0..180)
  int servoValue = map(potValue, 0, 1023, 0, 180);

  // Write the mapped value to the servo motor
  servoMotor.write(servoValue);

  // 5. Continuously read the servo value and print it to Serial Monitor
  Serial.println(servoValue);
}
