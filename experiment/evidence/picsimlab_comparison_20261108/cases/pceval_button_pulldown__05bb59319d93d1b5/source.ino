#include <Arduino.h>

const uint8_t BUTTON_PIN = 4;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }

  // Small delay to avoid overwhelming the serial monitor
  delay(20);
}