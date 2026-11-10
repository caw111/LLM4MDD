#include <Arduino.h>

void setup() {
  pinMode(12, OUTPUT); // D12
  pinMode(13, OUTPUT); // D13
}

void loop() {
  static bool state = false;

  // 每 1 秒切换一次两个 LED 的状态
  // LED1: D12, LED2: D13
  state = !state;
  digitalWrite(12, state ? HIGH : LOW);
  digitalWrite(13, state ? LOW : HIGH);

  delay(1000);
}