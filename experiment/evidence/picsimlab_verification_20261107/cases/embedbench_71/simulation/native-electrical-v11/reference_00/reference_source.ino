#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
const int BUTTON1_PIN = 2;  
const int BUTTON2_PIN = 3;  
const int POT1_PIN = A3;    
const int POT2_PIN = A4;    
const int SERVO1_PIN = 4;   
const int SERVO2_PIN = 5;   
int angle1 = 0;
int angle2 = 0;

void setup();
void loop();
void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(0);
  servo2.write(0);
}
void loop() {
  if (digitalRead(BUTTON1_PIN) == LOW) {
    delay(100); 
    if (digitalRead(BUTTON1_PIN) == LOW) {
      angle1 = map(analogRead(POT1_PIN), 0, 1023, 0, 180);
      servo1.write(angle1);
    }
  }
  if (digitalRead(BUTTON2_PIN) == LOW) {
    delay(100); 
    if (digitalRead(BUTTON2_PIN) == LOW) {
      angle2 = map(analogRead(POT2_PIN), 0, 1023, 0, 180);
      servo2.write(angle2);
    }
  }
}