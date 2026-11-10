#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_030\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr unsigned long DEBOUNCE_MS = 30;

unsigned long count = 0;
unsigned long lastChangeMs = 0;

uint8_t lastRawState = HIGH;
uint8_t stableState = HIGH;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_030\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_030\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_030\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Connect the button between D2 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);

  lastRawState = digitalRead(BUTTON_PIN);
  lastChangeMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawState) {
    lastRawState = rawState;
    lastChangeMs = now;
  }

  if (rawState != stableState &&
      (unsigned long)(now - lastChangeMs) >= DEBOUNCE_MS) {
    stableState = rawState;

    // Count only the debounced press, not the release or a held button.
    if (stableState == LOW) {
      ++count;
      Serial.println(count);
    }
  }
}
