#include <Arduino.h>

const uint8_t BUTTON_PIN = 4;   // per circuit
// GND.2 is used by the button wiring; no need to reference it in code.

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for serial (optional on UNO) */ }

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // HIGH when not pressed, LOW when pressed

  if (buttonState == LOW) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }

  delay(20); // aligns with idle_timeout_ms=20 requirement for periodic output
}