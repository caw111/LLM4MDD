#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_028\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t LED_PIN = 13;
constexpr unsigned long DEBOUNCE_MS = 10;
constexpr unsigned long BLINK_INTERVAL_MS = 500;

bool running = false;
bool ledOn = false;
uint8_t lastRawButton = HIGH;
uint8_t stableButton = HIGH;

unsigned long rawChangedAt = 0;
unsigned long lastToggleAt = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_028\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_028\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_028\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);

  lastRawButton = digitalRead(BUTTON_PIN);
  stableButton = lastRawButton;
  rawChangedAt = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawButton = digitalRead(BUTTON_PIN);

  if (rawButton != lastRawButton) {
    lastRawButton = rawButton;
    rawChangedAt = now;
  }

  if (rawButton != stableButton &&
      (unsigned long)(now - rawChangedAt) >= DEBOUNCE_MS) {
    const uint8_t previousButton = stableButton;
    stableButton = rawButton;

    if (previousButton == HIGH && stableButton == LOW) {
      running = !running;
      ledOn = false;
      digitalWrite(LED_PIN, LOW);
      lastToggleAt = now;
    }
  }

  if (running &&
      (unsigned long)(now - lastToggleAt) >= BLINK_INTERVAL_MS) {
    lastToggleAt += BLINK_INTERVAL_MS;
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  }
}
