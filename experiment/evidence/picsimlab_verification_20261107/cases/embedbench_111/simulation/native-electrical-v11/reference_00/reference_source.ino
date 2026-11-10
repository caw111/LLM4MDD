#include <Arduino.h>
const int LED1_PIN = 2;  
const int LED2_PIN = 3;  
const int LED3_PIN = 4;  
const int RGB_R_PIN = 5; 
const int RGB_G_PIN = 6; 
const int RGB_B_PIN = 7; 
const int POT_PIN = A3;  

void setup();
void loop();
void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
}
void loop() {
  int potValue = analogRead(POT_PIN);
  if (potValue <= 341) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    analogWrite(RGB_R_PIN, 255);
    analogWrite(RGB_G_PIN, 0);
    analogWrite(RGB_B_PIN, 0);
  } else if (potValue <= 682) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW);
    analogWrite(RGB_R_PIN, 0);
    analogWrite(RGB_G_PIN, 255);
    analogWrite(RGB_B_PIN, 0);
  } else {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, HIGH);
    analogWrite(RGB_R_PIN, 0);
    analogWrite(RGB_G_PIN, 0);
    analogWrite(RGB_B_PIN, 255);
  }
}