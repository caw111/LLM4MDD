#include <Arduino.h>

static const uint8_t BUTTON_PINS[4] = {2, 3, 4, 5};     // Do, Re, Mi, Fa
static const uint8_t BUZZER_PINS[4] = {9, 10, 11, 12};   // Do, Re, Mi, Fa

static const uint16_t FREQUENCIES[4] = {262, 294, 330, 349}; // Do, Re, Mi, Fa

void setup() {
  // 1. Configure button pins as INPUT
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT);
  }

  // 2. Configure buzzer pins as OUTPUT
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(BUZZER_PINS[i], OUTPUT);
    digitalWrite(BUZZER_PINS[i], LOW);
  }
}

void loop() {
  bool anyPressed = false;

  // 3. Continuously monitor button state, play corresponding tone if HIGH
  // 4. If no button is pressed, keep all buzzers silent
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t state = digitalRead(BUTTON_PINS[i]); // HIGH when pressed
    if (state == HIGH) {
      anyPressed = true;
      tone(BUZZER_PINS[i], FREQUENCIES[i]);
    } else {
      noTone(BUZZER_PINS[i]);
    }
  }

  // Ensures silence on all buzzers when none pressed
  if (!anyPressed) {
    for (uint8_t i = 0; i < 4; i++) {
      noTone(BUZZER_PINS[i]);
    }
  }

  // Small delay to reduce rapid toggling due to bounce
  delay(1);
}