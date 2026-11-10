#include <Arduino.h>

constexpr uint8_t LED_PIN = 13;
constexpr uint8_t CONTROL_PIN = A0;
constexpr unsigned long MIN_INTERVAL_MS = 200;
constexpr unsigned long MAX_INTERVAL_MS = 2000;

unsigned long lastToggleMs = 0;
bool ledState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(CONTROL_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  lastToggleMs = millis();
}

void loop() {
  const int reading = analogRead(CONTROL_PIN);

  // A0 控制 LED 每次亮灭切换的间隔：0.2～2 秒。
  const unsigned long intervalMs =
      MIN_INTERVAL_MS +
      (static_cast<unsigned long>(reading) *
       (MAX_INTERVAL_MS - MIN_INTERVAL_MS)) / 1023UL;

  const unsigned long now = millis();
  if (now - lastToggleMs >= intervalMs) {
    lastToggleMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}