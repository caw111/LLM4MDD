#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_123\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring.
// A1-A10 refer to bargraph terminals, not Arduino analog pins.
constexpr uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
constexpr uint8_t LED1_PIN = 2;  // Most significant overflow bit.
constexpr uint8_t LED2_PIN = 3;  // Least significant overflow bit.
constexpr uint8_t BUTTON_PIN = 4;

constexpr unsigned long DEBOUNCE_MS = 150UL;

uint8_t counter = 0;
uint8_t overflowCounter = 0;

uint8_t lastRawButtonState = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_123\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_123\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_123\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_123\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs() {
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], i < counter ? HIGH : LOW);
  }

  // Read as led1,led2: 00 -> 01 -> 10 -> 11 -> 00.
  digitalWrite(LED1_PIN, (overflowCounter & 0x02) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (overflowCounter & 0x01) ? HIGH : LOW);
}

void setup() {
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Button connects pin 4 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  updateOutputs();

  lastRawButtonState = digitalRead(BUTTON_PIN);
  stableButtonState = HIGH;
  lastButtonChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeTime = now;
  }

  // Debounce both press and release. A held button counts only once.
  if (rawButtonState != stableButtonState &&
      (unsigned long)(now - lastButtonChangeTime) >= DEBOUNCE_MS) {
    stableButtonState = rawButtonState;

    if (stableButtonState == LOW) {
      ++counter;

      if (counter > 10) {
        counter = 0;
        overflowCounter = (overflowCounter + 1) & 0x03;
      }

      updateOutputs();
    }
  }
}
