#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2
const uint8_t LED_PIN     = 13;  // D13

// Modes: 0 = blink, 1 = constant on, 2 = off
volatile uint8_t mode = 0;

// Simple debounce
const unsigned long DEBOUNCE_MS = 25;
unsigned long lastDebounceTime = 0;
int lastButtonReading = HIGH;   // because we use INPUT_PULLUP
int buttonState = HIGH;         // debounced state

// Blink timing (preserved behavior intent: LED( D13 )闪烁/常亮/关闭)
const unsigned long BLINK_INTERVAL_MS = 500;
unsigned long lastBlinkToggle = 0;
bool ledBlinkState = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // assumes button wiring to GND when pressed
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize button tracking
  lastButtonReading = digitalRead(BUTTON_PIN);
  buttonState = lastButtonReading;
}

void loop() {
  // Read with debounce
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (reading != buttonState) {
      buttonState = reading;

      // Toggle mode on button press (active LOW)
      if (buttonState == LOW) {
        mode = (mode + 1) % 3; // blink -> on -> off -> blink
      }
    }
  }

  // Apply mode to LED
  if (mode == 0) {
    // Blink
    unsigned long now = millis();
    if (now - lastBlinkToggle >= BLINK_INTERVAL_MS) {
      lastBlinkToggle = now;
      ledBlinkState = !ledBlinkState;
      digitalWrite(LED_PIN, ledBlinkState ? HIGH : LOW);
    }
  } else if (mode == 1) {
    // Constant ON
    digitalWrite(LED_PIN, HIGH);
  } else {
    // OFF
    digitalWrite(LED_PIN, LOW);
  }
}