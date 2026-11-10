#include <Arduino.h>
const int RED_PIN = 4;       
const int GREEN_PIN = 5;     
const int BLUE_PIN = 6;       
const int BUTTON1_PIN = 2;    
const int BUTTON2_PIN = 3;    
const int POT1_PIN = A3;      
const int POT2_PIN = A4;      
const int POT3_PIN = A5;      
int mode = 1;

void setup();
void loop();
void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
}
void loop() {
  int pot1Value = analogRead(POT1_PIN);
  int pot2Value = analogRead(POT2_PIN);
  int pot3Value = analogRead(POT3_PIN);
  int redValue = map(pot1Value, 0, 1023, 0, 255);
  int greenValue = map(pot2Value, 0, 1023, 0, 255);
  int blueValue = map(pot3Value, 0, 1023, 0, 255);
  if (digitalRead(BUTTON1_PIN) == LOW) {
    delay(20);
    if (digitalRead(BUTTON1_PIN) == LOW) {
      mode++;
      if (mode > 4) {
        mode = 1;
      }
      while (digitalRead(BUTTON1_PIN) == LOW);
    }
  }
  if (digitalRead(BUTTON2_PIN) == LOW) {
    delay(20);
    if (digitalRead(BUTTON2_PIN) == LOW) {
      mode = 1;
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      while (digitalRead(BUTTON2_PIN) == LOW);
    }
  }
  switch (mode) {
    case 1:
      analogWrite(RED_PIN, redValue);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, 0);
      break;
    case 2:
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, greenValue);
      analogWrite(BLUE_PIN, 0);
      break;
    case 3:
      analogWrite(RED_PIN, 0);
      analogWrite(GREEN_PIN, 0);
      analogWrite(BLUE_PIN, blueValue);
      break;
    case 4:
      analogWrite(RED_PIN, redValue);
      analogWrite(GREEN_PIN, greenValue);
      analogWrite(BLUE_PIN, blueValue);
      break;
  }
  delay(10);
}