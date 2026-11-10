#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 3;
constexpr uint8_t LED_PIN = 10;
constexpr unsigned long DEBOUNCE_MS = 30;

bool ledOn = false;
uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastChangeTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button connects D3 to GND.
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;
  lastChangeTime = millis();
}

void loop() {
  const uint8_t reading = digitalRead(BUTTON_PIN);
  const unsigned long now = millis();

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastChangeTime = now;
  }

  if (now - lastChangeTime >= DEBOUNCE_MS &&
      reading != stableButtonState) {
    stableButtonState = reading;

    if (stableButtonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }
}