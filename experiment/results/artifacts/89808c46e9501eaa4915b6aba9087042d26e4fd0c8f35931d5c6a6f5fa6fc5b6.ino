#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;  // btn1 wired to mega:2
static const uint8_t LED_BAR_COUNT = 10;

// bargraph1 segment pins (A1..A10) wired to mega:3..12 respectively
static const uint8_t BARGRAPH_PINS[LED_BAR_COUNT] = {
  3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

static const unsigned long DEBOUNCE_MS = 150;

void setup() {
  // Ensure all LEDs are off at startup/reset
  for (uint8_t i = 0; i < LED_BAR_COUNT; i++) {
    pinMode(BARGRAPH_PINS[i], OUTPUT);
    digitalWrite(BARGRAPH_PINS[i], LOW);
  }

  // Button with internal pull-up (btn1 left is connected to mega:2; other side to GND)
  pinMode(BTN1_PIN, INPUT_PULLUP);
}

void loop() {
  static unsigned long lastDebounceTime = 0;
  static int lastButtonState = HIGH;
  static int stableButtonState = HIGH;

  static uint8_t litCount = 0;

  int reading = digitalRead(BTN1_PIN);

  // Detect change
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
    lastButtonState = reading;
  }

  // If the reading has been stable for debounce period, accept it
  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS && reading != stableButtonState) {
    stableButtonState = reading;

    // Valid press: button goes LOW with INPUT_PULLUP
    if (stableButtonState == LOW) {
      // Increase lit LEDs by 1, cycling 0..10
      litCount++;
      if (litCount > LED_BAR_COUNT) {
        litCount = 0;
      }

      // Update bargraph display
      for (uint8_t i = 0; i < LED_BAR_COUNT; i++) {
        digitalWrite(BARGRAPH_PINS[i], (i < litCount) ? HIGH : LOW);
      }
    }
  }
}