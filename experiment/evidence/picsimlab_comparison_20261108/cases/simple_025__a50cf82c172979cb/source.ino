#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2
const uint8_t LED_PIN     = 13;  // D13

// Debounce
static const unsigned long DEBOUNCE_MS = 30;

// Toggle state
static bool isBlinking = false;
static bool lastButtonReading = HIGH;
static bool stableButtonState = HIGH;
static unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // typical wiring for a button to GND
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize debounce tracking
  bool reading = digitalRead(BUTTON_PIN);
  lastButtonReading = reading;
  stableButtonState = reading;
  lastDebounceTime = millis();
}

void loop() {
  // Read button with debounce
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // Button pressed when reading transitions to LOW (INPUT_PULLUP)
      if (stableButtonState == LOW) {
        // Toggle: first press -> start blinking; second press -> stop blinking and turn off
        isBlinking = !isBlinking;

        if (!isBlinking) {
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
  }

  // LED blinking behavior
  static unsigned long lastToggleTime = 0;
  static bool ledState = false;

  if (isBlinking) {
    // Blink rate: 1 Hz (toggle every 500 ms)
    const unsigned long BLINK_INTERVAL_MS = 500;
    unsigned long now = millis();

    if (now - lastToggleTime >= BLINK_INTERVAL_MS) {
      lastToggleTime = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  }
}