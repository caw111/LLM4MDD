#include <Arduino.h>
const int BUTTON_PIN = 2;  
const int POT_PIN = A3;   
const int RED_PIN = 3;    
const int GREEN_PIN = 4;  
const int BLUE_PIN = 5;   
int currentMode = -1; 
int lastButtonState = HIGH;
int currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const int DEBOUNCE_DELAY = 50; 

void setup();
void loop();
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}
void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); 
  }
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != currentButtonState) {
      currentButtonState = reading;
      if (currentButtonState == LOW) {
        if (currentMode == -1) {
          currentMode = 0; 
        } else {
          currentMode = (currentMode + 1) % 3; 
        }
      }
    }
  }
  lastButtonState = reading; 
  int brightness = map(analogRead(POT_PIN), 0, 1023, 0, 255);
  switch (currentMode) {
    case -1: 
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      break;
    case 0: 
      analogWrite(RED_PIN, brightness);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      break;
    case 1: 
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, brightness);
      analogWrite(BLUE_PIN, 0);
      break;
    case 2: 
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, brightness);
      break;
  }
}