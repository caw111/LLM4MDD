#include <Arduino.h>

const uint8_t LED_PIN = 12;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // 初始熄灭
}

void loop() {
  static unsigned long lastToggleMs = 0;
  static bool ledState = false; // 当前状态：先保持熄灭

  unsigned long now = millis();
  if (now - lastToggleMs >= 500) { // 每500ms翻转
    lastToggleMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
}