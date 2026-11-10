#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // Button connected between D2 and GND
const uint8_t LED_PIN = 13;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long BLINK_INTERVAL_MS = 500;

bool blinking = false;
bool ledOn = false;
bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;

unsigned long lastDebounceTime = 0;
unsigned long lastBlinkTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();
  const bool buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastDebounceTime = now;
    lastButtonReading = buttonReading;
  }

  if (now - lastDebounceTime >= DEBOUNCE_MS &&
      buttonReading != stableButtonState) {
    stableButtonState = buttonReading;

    // Toggle modes once per debounced press, not while held.
    if (stableButtonState == LOW) {
      blinking = !blinking;
      ledOn = blinking;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
      lastBlinkTime = now;
    }
  }

  if (blinking && now - lastBlinkTime >= BLINK_INTERVAL_MS) {
    lastBlinkTime = now;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }
}