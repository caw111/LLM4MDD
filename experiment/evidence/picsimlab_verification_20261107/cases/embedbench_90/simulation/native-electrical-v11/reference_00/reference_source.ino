#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
const int LED1_PIN = 2;  
const int LED2_PIN = 3;  
const int LED3_PIN = 4;  
const int SERVO1_PIN = 5;  
const int SERVO2_PIN = 6;  
int servo1Angle = 0;
int servo2Angle = 180;

void setup();
void loop();
void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}
void loop() {
  servo1.write(0);
  servo1Angle = 0;
  while (servo1Angle < 180) {
    digitalWrite(LED1_PIN, HIGH);
    delay(2000);
    servo1Angle += 30;
    servo1.write(servo1Angle);
    digitalWrite(LED1_PIN, LOW);
  }
  while (servo2Angle > 0) {
    digitalWrite(LED2_PIN, HIGH);
    delay(2000);
    servo2Angle -= 30;
    servo2.write(servo2Angle);
    digitalWrite(LED2_PIN, LOW);
  }
  digitalWrite(LED3_PIN, HIGH);
  delay(2000);
  digitalWrite(LED3_PIN, LOW);
  servo1Angle = 0;
  servo2Angle = 180;
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
}