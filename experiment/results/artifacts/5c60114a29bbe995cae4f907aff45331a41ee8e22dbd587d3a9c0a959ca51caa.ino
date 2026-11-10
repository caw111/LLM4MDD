#include <Arduino.h>

const int buttonPin = 4;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }

  delay(10);
}