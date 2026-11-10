#include <Arduino.h>
#include <Servo.h>
Servo servo1;
const int LED_PIN = 2;
const int BTN1_PIN = 3;
const int BTN2_PIN = 4;
const int SERVO_PIN = 5;
int currentAngle = 90;

void setup();
void loop();
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  servo1.attach(SERVO_PIN);
  servo1.write(currentAngle);
  digitalWrite(LED_PIN, LOW);
}
void loop() {
  if (digitalRead(BTN1_PIN) == LOW) {
    delay(150); 
    if (digitalRead(BTN1_PIN) == LOW) {
      if (currentAngle < 180) {
        currentAngle += 30;
        if (currentAngle > 180) currentAngle = 180;
        digitalWrite(LED_PIN, HIGH);
        servo1.write(currentAngle);
        delay(2000); 
        digitalWrite(LED_PIN, LOW);
      }
      while (digitalRead(BTN1_PIN) == LOW); 
    }
  }
  if (digitalRead(BTN2_PIN) == LOW) {
    delay(150); 
    if (digitalRead(BTN2_PIN) == LOW) {
      if (currentAngle > 0) {
        currentAngle -= 30;
        if (currentAngle < 0) currentAngle = 0;
        digitalWrite(LED_PIN, HIGH);
        servo1.write(currentAngle);
        delay(2000); 
        digitalWrite(LED_PIN, LOW);
      }
      while (digitalRead(BTN2_PIN) == LOW); 
    }
  }
}