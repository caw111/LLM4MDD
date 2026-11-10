#include <Arduino.h>
const int BUTTON1_PIN = 2;  
const int BUTTON2_PIN = 3;  
const int POT_PIN = A3;     
const int RED_PIN = 4;      
const int GREEN_PIN = 5;   
const int BLUE_PIN = 6;    
const unsigned long DEBOUNCE_TIME = 50;
int colorMode = 0;  
unsigned long lastColorChangeTime = 0;  
int button1State = HIGH;
int lastButton1State = HIGH;
unsigned long lastButton1DebounceTime = 0;
int button2State = HIGH;
int lastButton2State = HIGH;
unsigned long lastButton2DebounceTime = 0;
bool button1Pressed = false;
bool button2Pressed = false;

void setup();
void loop();
void handleButton1Press();
void handleButton2Press();
void setColorMode(int brightness);
void resetState();
void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}
void loop() {
  int reading1 = digitalRead(BUTTON1_PIN);
  int reading2 = digitalRead(BUTTON2_PIN);
  if (reading1 != lastButton1State) {
    lastButton1DebounceTime = millis();
  }
  if ((millis() - lastButton1DebounceTime) > DEBOUNCE_TIME) {
    if (reading1 != button1State) {
      button1State = reading1;
      if (button1State == LOW) {
        handleButton1Press();
      }
    }
  }
  if (reading2 != lastButton2State) {
    lastButton2DebounceTime = millis();
  }
  if ((millis() - lastButton2DebounceTime) > DEBOUNCE_TIME) {
    if (reading2 != button2State) {
      button2State = reading2;
      if (button2State == LOW) {
        handleButton2Press();
      }
    }
  }
  lastButton1State = reading1;
  lastButton2State = reading2;
  int brightness = analogRead(POT_PIN) / 4;  
  setColorMode(brightness);
  if (button1Pressed && button2Pressed) {
    resetState();
    button1Pressed = false;
    button2Pressed = false;
  }
}
void handleButton1Press() {
  button1Pressed = true;
  if (button2Pressed) {
    return;
  }
  if (colorMode < 2) {
    colorMode++;
  } else {
    colorMode = 0;
  }
  lastColorChangeTime = millis();
}
void handleButton2Press() {
  button2Pressed = true;
  if (button1Pressed) {
    return;
  }
  if (colorMode >= 3 && colorMode < 5) {
    colorMode++;
  } else {
    colorMode = 3;
  }
  lastColorChangeTime = millis();
}
void setColorMode(int brightness) {
  switch (colorMode) {
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
    case 3:  
      analogWrite(RED_PIN, brightness);
      analogWrite(GREEN_PIN, brightness);
      analogWrite(BLUE_PIN, 0);
      break;
    case 4:  
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, brightness);
      analogWrite(BLUE_PIN, brightness);
      break;
    case 5:  
      analogWrite(RED_PIN, brightness);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, brightness);
      break;
    case 6:  
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      break;
  }
}
void resetState() {
  colorMode = 6;
  setColorMode(0);
}