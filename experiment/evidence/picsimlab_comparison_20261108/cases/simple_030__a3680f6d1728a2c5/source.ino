#include <Arduino.h>

const uint8_t BUTTON_PIN = 2; // D2

volatile unsigned long counterValue = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  // No extra serial framing settings needed on Arduino; baud=9600.
}

void loop() {
  static bool lastStable = HIGH;
  static unsigned long lastChangeMs = 0;
  static bool latchedPressed = false;

  bool reading = digitalRead(BUTTON_PIN);

  // Simple debounce
  if (reading != lastStable) {
    lastStable = reading;
    lastChangeMs = millis();
  }

  // When stable for a short time, accept state
  if (millis() - lastChangeMs >= 25) {
    if (reading == LOW) { // pressed (INPUT_PULLUP)
      if (!latchedPressed) {
        latchedPressed = true;
        counterValue++;
        Serial.println(counterValue);
      }
    } else {
      latchedPressed = false;
    }
  }
}