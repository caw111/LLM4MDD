#include <Arduino.h>

const uint8_t buttonPin = 3; // INPUT reads LOW when pressed, HIGH when not pressed
const uint8_t buzzerPin = 9; // Piezo buzzer positive

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin); // LOW when pressed, HIGH when not pressed

  if (buttonState == LOW) {
    tone(buzzerPin, 262); // 262 Hz when pressed
  } else {
    noTone(buzzerPin);
  }
}