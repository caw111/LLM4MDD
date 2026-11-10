#include <Arduino.h>
#include <Servo.h>
Servo servo1;
const int SERVO_PIN = 3;  
const int LED_PIN = 2;    
const int POT1_PIN = A3;  
const int POT2_PIN = A4;  
const int POT3_PIN = A5;  
int currentServoAngle = 0;
int targetServoAngle = 0;
int ledBrightness = 0;
int movementSpeed = 50;
unsigned long lasttime = 0;  

void setup();
void loop();
void setup() {
  pinMode(LED_PIN, OUTPUT);
  servo1.attach(SERVO_PIN);
  servo1.write(0); 
  analogWrite(LED_PIN, 0); 
}
void loop() {
  targetServoAngle = map(analogRead(POT1_PIN), 0, 1023, 0, 180);
  ledBrightness = map(analogRead(POT2_PIN), 0, 1023, 0, 255);
  movementSpeed = map(analogRead(POT3_PIN), 0, 1023, 10, 100);
  analogWrite(LED_PIN, ledBrightness);
  if (currentServoAngle < targetServoAngle && millis() - lasttime > movementSpeed ) {
    currentServoAngle++;
    servo1.write(currentServoAngle);
    lasttime = millis();
  } else if (currentServoAngle > targetServoAngle && millis() - lasttime > movementSpeed ) {
    currentServoAngle--;
    servo1.write(currentServoAngle);
    lasttime = millis();
  }
}