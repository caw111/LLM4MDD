#include <Arduino.h>

// Wiring for Arduino Mega 2560.
const uint8_t LED_PIN = 2;
const uint8_t POT_PIN = A3;
const uint8_t BAR_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

// Preserve the explicitly requested ranges:
// 0–102: off; 103–204: one segment; 921–1023: ten segments.
// The nine-segment interval is compressed to accommodate the final range.
const uint16_t SEGMENT_THRESHOLDS[10] = {
  103, 205, 307, 410, 512, 614, 717, 819, 920, 921
};

const unsigned long BLINK_HALF_PERIOD_MS = 2000UL;

bool blinking = false;
bool blinkOn = false;
unsigned long lastBlinkChange = 0;

void setup() {
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  pinMode(POT_PIN, INPUT);
}

void loop() {
  const int potValue = analogRead(POT_PIN);
  const unsigned long now = millis();

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i],
                 potValue >= SEGMENT_THRESHOLDS[i] ? HIGH : LOW);
  }

  if (potValue == 1023) {
    if (!blinking) {
      blinking = true;
      blinkOn = true;
      lastBlinkChange = now;
    }

    const unsigned long elapsed = now - lastBlinkChange;
    if (elapsed >= BLINK_HALF_PERIOD_MS) {
      const unsigned long intervals = elapsed / BLINK_HALF_PERIOD_MS;
      lastBlinkChange += intervals * BLINK_HALF_PERIOD_MS;

      if (intervals & 1UL) {
        blinkOn = !blinkOn;
      }
    }

    analogWrite(LED_PIN, blinkOn ? 255 : 0);
  } else {
    blinking = false;
    blinkOn = false;

    const uint8_t brightness =
        static_cast<uint8_t>((static_cast<unsigned long>(potValue) * 255UL
                              + 511UL) / 1023UL);
    analogWrite(LED_PIN, brightness);
  }
}