#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_72\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_COUNT = 10;
constexpr uint8_t LED_PINS[LED_COUNT] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
constexpr unsigned long DEBOUNCE_MS = 150UL;

uint8_t progress = 0;
uint8_t lastReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastChangeTime = 0;

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_72\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_72\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_72\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_72\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], i < progress ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  // The button connects pin 2 to GND when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  progress = 0;
  updateDisplay();
  lastReading = digitalRead(BUTTON_PIN);
  lastChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = now;
  }

  // Accept a transition only after the input remains stable for 150 ms.
  if (reading != stableButtonState &&
      now - lastChangeTime >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // Count only the pressed edge; holding the button does not repeat.
    if (stableButtonState == LOW) {
      progress = (progress + 1) % (LED_COUNT + 1);
      updateDisplay();
    }
  }
}
