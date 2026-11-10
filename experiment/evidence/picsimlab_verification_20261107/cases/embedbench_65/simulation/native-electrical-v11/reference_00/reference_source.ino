#include <Arduino.h>
const int BUTTON_PIN = 2;  
const int RED_PIN = 3;     
const int GREEN_PIN = 4;   
const int BLUE_PIN = 5;    
int colorState = -1;  
unsigned long lastChangeTime = 0;  
bool buttonActive = false;  

void setup();
void loop();
void updateLED();
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
  if (digitalRead(BUTTON_PIN) == LOW && !buttonActive) {  
    delay(100);  
    if (digitalRead(BUTTON_PIN) == LOW) {  
      buttonActive = true;  
      colorState = (colorState + 1) % 8;  
      lastChangeTime = millis();  
      updateLED();  
    }  
  }  
  if (digitalRead(BUTTON_PIN) == HIGH) {  
    buttonActive = false;  
  }  
}  
void updateLED() {  
  switch(colorState) {  
    case 0:  
      analogWrite(RED_PIN, 255);  
      analogWrite(GREEN_PIN, 0);  
      analogWrite(BLUE_PIN, 0);  
      break;  
    case 1:  
      analogWrite(RED_PIN, 0);  
      analogWrite(GREEN_PIN, 255);  
      analogWrite(BLUE_PIN, 0);  
      break;  
    case 2:  
      analogWrite(RED_PIN, 0);  
      analogWrite(GREEN_PIN, 0);  
      analogWrite(BLUE_PIN, 255);  
      break;  
    case 3:  
      analogWrite(RED_PIN, 255);  
      analogWrite(GREEN_PIN, 255);  
      analogWrite(BLUE_PIN, 0);  
      break;  
    case 4:  
      analogWrite(RED_PIN, 0);  
      analogWrite(GREEN_PIN, 255);  
      analogWrite(BLUE_PIN, 255);  
      break;  
    case 5:  
      analogWrite(RED_PIN, 255);  
      analogWrite(GREEN_PIN, 0);  
      analogWrite(BLUE_PIN, 255);  
      break;  
    case 6:  
      analogWrite(RED_PIN, 255);  
      analogWrite(GREEN_PIN, 255);  
      analogWrite(BLUE_PIN, 255);  
      break;  
    case 7:  
      analogWrite(RED_PIN, 0);  
      analogWrite(GREEN_PIN, 0);  
      analogWrite(BLUE_PIN, 0);  
      break;  
  }  
}  