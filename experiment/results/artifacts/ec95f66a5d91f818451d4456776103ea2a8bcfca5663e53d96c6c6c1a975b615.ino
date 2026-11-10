#include <Arduino.h>

const uint8_t BUTTON_PIN = 4;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }

  delay(20); // keep output frequency reasonable (matches short idle expectation)
}