#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;   // D3 button input
const uint8_t LED_PIN     = 10; // D10 LED output

static bool ledState = false;
static bool lastButtonRaw = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long debounceDelayMs = 30;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button to GND: pressed = LOW
  pinMode(LED_PIN, OUTPUT);

  ledState = false;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void loop() {
  bool buttonRaw = digitalRead(BUTTON_PIN);

  if (buttonRaw != lastButtonRaw) {
    lastDebounceTime = millis();
    lastButtonRaw = buttonRaw;
  }

  if (millis() - lastDebounceTime >= debounceDelayMs) {
    // detect falling edge (HIGH -> LOW) meaning button press
    static bool stablePrev = HIGH;
    if (stablePrev == HIGH && buttonRaw == LOW) {
      ledState = !ledState; // toggle once per press
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
    stablePrev = buttonRaw;
  }
}