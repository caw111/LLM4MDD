#include <Arduino.h>

constexpr uint8_t LED_PIN = 2;

// Common-cathode RGB LEDs: R, G, B.
constexpr uint8_t RGB_PINS[3][3] = {
  {3, 4, 5},
  {6, 7, 8},
  {9, 10, 11}
};

// Bar graph terminals A1-A4, not Arduino analog pins.
constexpr uint8_t BAR_PINS[4] = {12, 13, 44, 45};

constexpr unsigned long STAGE_DURATION_MS = 2000UL;

uint8_t currentStage = 0;
unsigned long stageStartMs = 0;

void setRgb(uint8_t index, bool red, bool green, bool blue) {
  digitalWrite(RGB_PINS[index][0], red ? HIGH : LOW);
  digitalWrite(RGB_PINS[index][1], green ? HIGH : LOW);
  digitalWrite(RGB_PINS[index][2], blue ? HIGH : LOW);
}

void applyStage(uint8_t stage) {
  digitalWrite(LED_PIN, (stage == 1 || stage == 3) ? HIGH : LOW);

  switch (stage) {
    case 0:  // Stage 1
      setRgb(0, true, false, false);
      setRgb(1, false, false, false);
      setRgb(2, false, false, false);
      break;

    case 1:  // Stage 2
      setRgb(0, false, true, false);
      setRgb(1, false, false, true);
      setRgb(2, false, false, false);
      break;

    case 2:  // Stage 3
      setRgb(0, false, false, true);
      setRgb(1, true, false, false);
      setRgb(2, false, true, false);
      break;

    case 3:  // Stage 4
      setRgb(0, true, true, true);
      setRgb(1, true, true, true);
      setRgb(2, true, true, true);
      break;
  }

  for (uint8_t i = 0; i < 4; ++i) {
    digitalWrite(BAR_PINS[i], i <= stage ? HIGH : LOW);
  }
}

void initializeOff(uint8_t pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

void setup() {
  initializeOff(LED_PIN);

  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      initializeOff(RGB_PINS[led][channel]);
    }
  }

  for (uint8_t i = 0; i < 4; ++i) {
    initializeOff(BAR_PINS[i]);
  }

  currentStage = 0;
  stageStartMs = millis();
  applyStage(currentStage);
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - stageStartMs;

  if (elapsed >= STAGE_DURATION_MS) {
    const unsigned long stagesElapsed = elapsed / STAGE_DURATION_MS;

    // Advance on the original time grid to prevent cumulative timing drift.
    stageStartMs += stagesElapsed * STAGE_DURATION_MS;
    currentStage = (currentStage + (stagesElapsed % 4UL)) % 4;
    applyStage(currentStage);
  }
}