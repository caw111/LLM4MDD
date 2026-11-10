#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_54\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560; common-cathode RGB LED.
const uint8_t RGB_PINS[3] = {2, 3, 4};
const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
const uint8_t POT_PIN = A3;

const unsigned long MODE_DURATION_MS = 5000UL;

uint8_t colorLevels[3] = {0, 0, 0};
uint8_t activeMode = 0;  // Red, then green, then blue.
unsigned long modeStartedAt = 0;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_54\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_54\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_54\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Set output latches LOW before enabling outputs to start everything off.
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(RGB_PINS[i], LOW);
    pinMode(RGB_PINS[i], OUTPUT);
    analogWrite(RGB_PINS[i], 0);
  }

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  pinMode(POT_PIN, INPUT);
  modeStartedAt = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - modeStartedAt;

  // Advance against fixed boundaries rather than accumulating timing drift.
  if (elapsed >= MODE_DURATION_MS) {
    const unsigned long modesElapsed = elapsed / MODE_DURATION_MS;
    activeMode = (activeMode + (modesElapsed % 3UL)) % 3;
    modeStartedAt += modesElapsed * MODE_DURATION_MS;
  }

  const int potValue = analogRead(POT_PIN);

  // Only the active component changes; all other intensities persist.
  colorLevels[activeMode] =
      static_cast<uint8_t>(map(potValue, 0, 1023, 0, 255));

  for (uint8_t i = 0; i < 3; ++i) {
    analogWrite(RGB_PINS[i], colorLevels[i]);
  }

  const uint8_t litSegments =
      static_cast<uint8_t>(map(potValue, 0, 1023, 0, 10));

  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }
}
