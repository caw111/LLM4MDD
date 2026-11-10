#include <Arduino.h>
const int LED1_PIN = 2;  
const int LED2_PIN = 3;  
const int BARGRAPH_PINS[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};  

void setup();
void loop();
void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  for (int i = 0; i < 10; i++) {
    pinMode(BARGRAPH_PINS[i], OUTPUT);
  }
}
void loop() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(BARGRAPH_PINS[i], HIGH);  
    if (i % 2 == 0) {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, LOW);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }
    delay(2000);  
  }
  for (int i = 0; i < 10; i++) {
    digitalWrite(BARGRAPH_PINS[i], LOW);  
  }
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  delay(200);  
}