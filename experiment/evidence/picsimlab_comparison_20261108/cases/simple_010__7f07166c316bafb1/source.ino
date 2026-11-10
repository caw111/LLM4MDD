#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  static unsigned long lastPrintMs = 0;
  unsigned long now = millis();

  if (now - lastPrintMs >= 1000UL) { // 每秒
    lastPrintMs = now;
    int a0Value = analogRead(A0);
    Serial.println(a0Value); // line/or idle framing
  }
}