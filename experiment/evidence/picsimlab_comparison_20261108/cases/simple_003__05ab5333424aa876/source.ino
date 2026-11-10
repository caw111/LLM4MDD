#include <Arduino.h>

void setup() {
  pinMode(2, INPUT_PULLUP); // D2, 按下为低电平
  pinMode(13, OUTPUT);     // D13 LED
  digitalWrite(13, LOW);  // 初始熄灭
}

void loop() {
  // 低电平表示按下
  if (digitalRead(2) == LOW) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}