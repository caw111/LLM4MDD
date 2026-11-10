#include <Arduino.h>

static const int SENSOR_PIN = A1;   // 光敏电阻检测光强
static const int LED_PIN = 8;       // D8 LED（注意：Arduino UNO 的 D8 实际引脚号为 8）

// 阈值：低于该值点亮 LED（可按需要校准）
static const int LIGHT_THRESHOLD = 500;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int lightLevel = analogRead(SENSOR_PIN); // 读取光敏电阻光强

  if (lightLevel < LIGHT_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}