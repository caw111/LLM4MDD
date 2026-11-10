#include <Arduino.h>

const uint8_t buttonPin = 3;
const uint8_t buzzerPin = 9;
const unsigned int toneFreq = 262; // Hz

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin); // LOW when pressed, HIGH when not pressed

  if (buttonState == LOW) {
    tone(buzzerPin, toneFreq);
  } else {
    noTone(buzzerPin);
  }
}