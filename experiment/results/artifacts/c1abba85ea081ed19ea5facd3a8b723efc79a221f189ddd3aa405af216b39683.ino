#include <Arduino.h>

const uint8_t BUTTON_PIN = 4;      // from circuit config: button_pin = 4
const uint8_t GND_PIN = 2;         // from circuit config: gnd_pin = "GND.2"

const unsigned long BOUNCE_DELAY_MS = 25; // short delay in the loop to handle switch bouncing

// For pull-up wiring: button pressed => pin reads LOW, not pressed => HIGH
static bool lastStablePressed = false;
static unsigned long pressStartMs = 0;
static bool measuring = false;

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT); // pull-up resistor is provided by the external 10k resistor

  // Initialize state
  bool pressedNow = (digitalRead(BUTTON_PIN) == LOW);
  lastStablePressed = pressedNow;

  if (pressedNow) {
    measuring = true;
    pressStartMs = millis();
  }
}

void loop() {
  // Read raw state
  bool pressedRaw = (digitalRead(BUTTON_PIN) == LOW);

  // Debounce: wait briefly and confirm
  delay(BOUNCE_DELAY_MS);
  bool pressedStable = (digitalRead(BUTTON_PIN) == LOW);

  // If state changed and is stable, handle press/release timing
  if (pressedStable != lastStablePressed) {
    lastStablePressed = pressedStable;

    if (pressedStable) {
      // Button just pressed
      measuring = true;
      pressStartMs = millis();
    } else {
      // Button just released
      if (measuring) {
        unsigned long durationMs = millis() - pressStartMs;
        float durationSeconds = durationMs / 1000.0f;

        Serial.print("Button pressed duration: ");
        Serial.print(durationSeconds, 2);
        Serial.println("s");
      }
      measuring = false;
    }
  }
}