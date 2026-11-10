#include <Arduino.h>
const int LED_PIN = 2;  
const int POT1_PIN = A3;  
const int POT2_PIN = A4;  

void setup();
void loop();
void setup() {
  pinMode(LED_PIN, OUTPUT);  
  analogWrite(LED_PIN, 0);  
}
void loop() {
  int pot1Value = analogRead(POT1_PIN);  
  int pot2Value = analogRead(POT2_PIN);  
  int brightness = map(pot1Value, 0, 1023, 0, 255);  
  if (pot1Value > pot2Value) {
    analogWrite(LED_PIN, brightness);  
  } else {
    analogWrite(LED_PIN, 0);  
  }
  delay(10);  
}