#include <Arduino.h>

const uint8_t POT_PIN = A0;
const uint8_t BTN_PIN = 2;   // D2

void setup() {
  Serial.begin(9600);

  pinMode(BTN_PIN, INPUT_PULLUP); // preserves typical D2 button usage with pull-up
}

void loop() {
  static unsigned long lastPrintMs = 0;

  unsigned long now = millis();
  if (now - lastPrintMs >= 1000UL) { // 每秒
    lastPrintMs = now;

    int potVal = analogRead(POT_PIN);
    int btnState = digitalRead(BTN_PIN); // 按钮状态(高/低，依赖接线与INPUT_PULLUP)

    Serial.print("A0=");
    Serial.print(potVal);
    Serial.print(", D2=");
    Serial.println(btnState);
  }
}