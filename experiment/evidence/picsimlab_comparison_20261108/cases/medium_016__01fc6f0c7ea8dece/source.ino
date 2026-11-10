#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;  // Button between D2 and GND
const uint8_t LED_PIN = 13;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long BLINK_INTERVAL_MS = 500;

enum LedMode : uint8_t {
  MODE_BLINK,
  MODE_ON,
  MODE_OFF
};

LedMode mode = MODE_BLINK;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;
unsigned long lastBlinkMs = 0;
bool ledState = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  ledState = true;
  digitalWrite(LED_PIN, HIGH);
  lastBlinkMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t buttonReading = digitalRead(BUTTON_PIN);

  if (buttonReading != lastButtonReading) {
    lastButtonReading = buttonReading;
    lastButtonChangeMs = now;
  }

  if ((now - lastButtonChangeMs >= DEBOUNCE_MS) &&
      (buttonReading != stableButtonState)) {
    stableButtonState = buttonReading;

    // Advance once per debounced press; holding the button does not repeat.
    if (stableButtonState == LOW) {
      switch (mode) {
        case MODE_BLINK:
          mode = MODE_ON;
          ledState = true;
          break;

        case MODE_ON:
          mode = MODE_OFF;
          ledState = false;
          break;

        case MODE_OFF:
          mode = MODE_BLINK;
          ledState = true;
          lastBlinkMs = now;
          break;
      }

      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  }

  if (mode == MODE_BLINK && now - lastBlinkMs >= BLINK_INTERVAL_MS) {
    lastBlinkMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}