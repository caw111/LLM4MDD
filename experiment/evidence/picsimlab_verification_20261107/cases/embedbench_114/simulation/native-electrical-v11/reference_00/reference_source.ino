#include <Arduino.h>
const int LED1_PIN = 2;  
const int LED2_PIN = 3;  
const int RGB1_R = 4, RGB1_G = 5, RGB1_B = 6;  
const int RGB2_R = 7, RGB2_G = 8, RGB2_B = 9;  
const int BAR_PINS[] = {10, 11, 12, 13, 44, 45};  
unsigned long previousMillis = 0;
int state = 0;
bool ledState = false;

void setup();
void loop();
void resetAll();
void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);
  for (int i = 0; i < 6; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;
    state = (state + 1) % 3;
    resetAll();
  }
  switch(state) {
    case 0:  
      digitalWrite(LED1_PIN, HIGH );
      digitalWrite(RGB1_R, HIGH);
      for (int i = 0; i < 3; i++) digitalWrite(BAR_PINS[i], HIGH);
      break;
    case 1:  
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(RGB2_G, HIGH);
      for (int i = 3; i < 6; i++) digitalWrite(BAR_PINS[i], HIGH);
      break;
    case 2:  
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(RGB1_B, HIGH);
      digitalWrite(RGB1_G, HIGH);
      digitalWrite(RGB2_B, HIGH);
      digitalWrite(RGB2_G, HIGH);
      for (int i = 0; i < 6; i++) digitalWrite(BAR_PINS[i], HIGH);
      break;
  }
}
void resetAll() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(RGB1_R, LOW);
  digitalWrite(RGB1_G, LOW);
  digitalWrite(RGB1_B, LOW);
  digitalWrite(RGB2_R, LOW);
  digitalWrite(RGB2_G, LOW);
  digitalWrite(RGB2_B, LOW);
  for (int i = 0; i < 6; i++) digitalWrite(BAR_PINS[i], LOW);
}