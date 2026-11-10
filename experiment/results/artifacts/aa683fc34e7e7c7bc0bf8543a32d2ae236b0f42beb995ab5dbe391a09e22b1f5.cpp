#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_smart_led_system\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_smart_led_system\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_smart_led_system\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_smart_led_system\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // LEDs 1..5 -> Arduino pins 2..6
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // Photoresistors 1..5 -> Arduino analog pins A0..A4
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  // Start with LEDs off
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

void loop() {
  const int threshold = 500;

  int sensor1 = analogRead(A0);
  int sensor2 = analogRead(A1);
  int sensor3 = analogRead(A2);
  int sensor4 = analogRead(A3);
  int sensor5 = analogRead(A4);

  // If sensor value is greater than threshold, turn corresponding LED on
  // Photoresistor1 -> led1 (pin 2), ..., Photoresistor5 -> led5 (pin 6)
  digitalWrite(2, (sensor1 > threshold) ? HIGH : LOW);
  digitalWrite(3, (sensor2 > threshold) ? HIGH : LOW);
  digitalWrite(4, (sensor3 > threshold) ? HIGH : LOW);
  digitalWrite(5, (sensor4 > threshold) ? HIGH : LOW);
  digitalWrite(6, (sensor5 > threshold) ? HIGH : LOW);
}
