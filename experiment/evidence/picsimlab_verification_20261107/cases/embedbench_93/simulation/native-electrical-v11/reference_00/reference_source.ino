#include <Arduino.h>
#include <Servo.h>
Servo servo1;
const int LED1_PIN = 2;  
const int LED2_PIN = 3;  
const int POT_PIN = A3;  
const int SERVO_PIN = 4; 
int previousAngle = 0;

void setup();
void loop();
void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(0); 
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}
void loop() {
  int potValue = analogRead(POT_PIN);
  int angle = map(potValue, 0, 1023, 0, 180);
  servo1.write(angle);
  if (angle > previousAngle) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
  } else if (angle < previousAngle) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
  }
  previousAngle = angle;
  delay(2000); 
}