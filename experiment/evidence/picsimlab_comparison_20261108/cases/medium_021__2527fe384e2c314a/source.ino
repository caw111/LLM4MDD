#include <Arduino.h>

const uint8_t LED_PIN = 9;     // D9
const uint8_t BUZZER_PIN = 6;  // D6 (buzzer)

const int THRESHOLD_A0 = 700;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  int a0 = analogRead(A0);

  if (a0 > THRESHOLD_A0) {
    digitalWrite(LED_PIN, HIGH);  // LED 全亮
    tone(BUZZER_PIN, 2000);       // 蜂鸣器报警
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}