#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
const int BUTTON_PIN = 2;  
const int SERVO1_PIN = 3;  
const int SERVO2_PIN = 4;  
int servo1Pos = 0;         
int servo2Pos = 180;       
bool buttonState = HIGH;   
bool lastButtonState = HIGH; 
unsigned long lastDebounceTime = 0; 
const unsigned long debounceDelay = 150; 

void setup();
void loop();
void toggleServoPositions();
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
}
void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        toggleServoPositions();
      }
    }
  }
  lastButtonState = reading;
}
void toggleServoPositions() {
  if (servo1Pos == 0 && servo2Pos == 180) {
    servo1Pos = 180;
    servo2Pos = 0;
  } else {
    servo1Pos = 0;
    servo2Pos = 180;
  }
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  delay(200);  
}