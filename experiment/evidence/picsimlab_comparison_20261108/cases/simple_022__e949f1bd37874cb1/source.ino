#include <Arduino.h>

const uint8_t BTN_PIN = 3;      // D3 button input
const uint8_t BUZZ_PIN = 6;     // D6 buzzer output

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // assumes button to GND when pressed
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);
}

void loop() {
  // When D3 button is pressed, make D6 buzzer continuously sound.
  // With INPUT_PULLUP: pressed -> LOW
  if (digitalRead(BTN_PIN) == LOW) {
    tone(BUZZ_PIN, 2000); // audible continuous tone while pressed
  } else {
    noTone(BUZZ_PIN);
  }
}