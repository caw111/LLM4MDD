#include <Arduino.h>
#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
const int BUTTON_PIN = 2; 
int currentState = 0;     
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 150; 
bool buttonPressed = false; 

void setup();
void loop();
void setServoAngles(int angle1, int angle2, int angle3);
void setup() {
  servo1.attach(3); 
  servo2.attach(4); 
  servo3.attach(5); 
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  setServoAngles(0, 0, 0); 
}
void loop() {
  int buttonState = digitalRead(BUTTON_PIN); 
  if (buttonState == LOW && !buttonPressed && (millis() - lastDebounceTime) > debounceDelay) {
    buttonPressed = true; 
    lastDebounceTime = millis(); 
    currentState = (currentState + 1) % 5;
    switch (currentState) {
      case 0:
        setServoAngles(0, 0, 0);
        break;
      case 1:
        setServoAngles(90, 45, 135);
        break;
      case 2:
        setServoAngles(180, 90, 0);
        break;
      case 3:
        setServoAngles(45, 135, 90);
        break;
      case 4:
        setServoAngles(0, 0, 0);
        break;
    }
  }
  if (buttonState == HIGH && buttonPressed && (millis() - lastDebounceTime) > debounceDelay) {
    buttonPressed = false; 
    lastDebounceTime = millis(); 
  }
}
void setServoAngles(int angle1, int angle2, int angle3) {
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
}