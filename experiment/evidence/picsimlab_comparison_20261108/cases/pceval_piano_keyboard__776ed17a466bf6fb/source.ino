#include <Arduino.h>

const uint8_t BUTTON_COUNT = 4;
const uint8_t buttonPins[BUTTON_COUNT] = {2, 3, 4, 5};
const uint8_t buzzerPins[BUTTON_COUNT] = {9, 10, 11, 12};
const unsigned int noteFrequencies[BUTTON_COUNT] = {262, 294, 330, 349};

int8_t activeNote = -1;

void setup() {
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    // Buttons use external pull-down resistors; pressed reads HIGH.
    pinMode(buttonPins[i], INPUT);
    pinMode(buzzerPins[i], OUTPUT);
    digitalWrite(buzzerPins[i], LOW);
  }
}

void loop() {
  int8_t requestedNote = -1;

  // Only one button is pressed at a time.
  for (uint8_t i = 0; i < BUTTON_COUNT; ++i) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      requestedNote = i;
      break;
    }
  }

  if (requestedNote != activeNote) {
    // The Uno supports one tone() output at a time.
    // Stop the old output before starting a different buzzer.
    if (activeNote >= 0) {
      noTone(buzzerPins[activeNote]);
      digitalWrite(buzzerPins[activeNote], LOW);
    }

    activeNote = requestedNote;

    if (activeNote >= 0) {
      tone(buzzerPins[activeNote], noteFrequencies[activeNote]);
    }
  }
}