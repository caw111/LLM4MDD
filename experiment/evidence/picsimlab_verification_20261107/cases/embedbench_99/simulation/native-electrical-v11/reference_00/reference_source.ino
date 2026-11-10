#include <Arduino.h>
const int ledPin = 2;          
const int potPin = A3;         
const int barGraphPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; 
int potValue = 0;             
int brightness = 0;           
int segmentsToLight = 0;      
unsigned long previousMillis = 0; 
bool ledState = LOW;          

void setup();
void loop();
void setup() {
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 10; i++) {
    pinMode(barGraphPins[i], OUTPUT);
    digitalWrite(barGraphPins[i], LOW); 
  }
}
void loop() {
  potValue = analogRead(potPin);
  brightness = map(potValue, 0, 1023, 0, 255);
  segmentsToLight = map(potValue, 0, 1023, 0, 10);
  for (int i = 0; i < 10; i++) {
    if (i < segmentsToLight) {
      digitalWrite(barGraphPins[i], HIGH); 
    } else {
      digitalWrite(barGraphPins[i], LOW);  
    }
  }
  if (potValue == 1023) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 2000) {
      previousMillis = currentMillis;
      ledState = !ledState; 
      digitalWrite(ledPin, ledState);
    }
  } else {
    analogWrite(ledPin, brightness);
  }
}