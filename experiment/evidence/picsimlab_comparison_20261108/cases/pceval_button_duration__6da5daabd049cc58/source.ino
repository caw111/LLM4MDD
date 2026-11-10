#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 4;
constexpr unsigned long DEBOUNCE_MS = 20;

int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastChangeTime = 0;
unsigned long pressStartTime = 0;
bool pressInProgress = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);  // External 10k ohm pull-up to 5V.

  lastReading = digitalRead(BUTTON_PIN);
  lastChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const int reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = now;
  }

  if (reading != stableState &&
      now - lastChangeTime >= DEBOUNCE_MS) {
    stableState = reading;

    if (stableState == LOW) {
      pressStartTime = lastChangeTime;
      pressInProgress = true;
    } else if (pressInProgress) {
      const unsigned long durationMs = lastChangeTime - pressStartTime;
      pressInProgress = false;

      Serial.print(F("Button pressed duration: "));
      Serial.print(durationMs / 1000.0, 3);
      Serial.println(F("s"));
    }
  }

  delay(5);  // Short polling delay; stable-state filtering handles bouncing.
}