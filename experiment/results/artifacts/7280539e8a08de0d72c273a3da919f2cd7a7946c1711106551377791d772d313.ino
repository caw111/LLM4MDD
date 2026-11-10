#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 4;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);  // External 10kΩ pull-down resistor to GND.
}

void loop() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    Serial.println("Button pressed");
  } else {
    Serial.println("Button not pressed");
  }
}