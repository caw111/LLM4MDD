#include <Arduino.h>

const uint8_t LIGHT_SENSOR_PIN = A0;
const uint8_t LED_PWM_PIN = 9;
const uint8_t BUZZER_PIN = 6;
const uint8_t AUX_OUTPUT_PIN = 13;

const int EXTREME_LIGHT_THRESHOLD = 800;
const unsigned int BEEP_FREQUENCY_HZ = 2000;
const unsigned long BEEP_DURATION_MS = 100;
const unsigned long SAMPLE_INTERVAL_MS = 10;

bool wasExtremeLight = false;
unsigned long lastSampleTime = 0;

void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(AUX_OUTPUT_PIN, OUTPUT);

  analogWrite(LED_PWM_PIN, 0);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(AUX_OUTPUT_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();
  if (now - lastSampleTime < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleTime = now;

  const int lightLevel = analogRead(LIGHT_SENSOR_PIN);

  // 光线越强，LED 越暗。
  const int brightness = map(lightLevel, 0, 1023, 255, 0);
  analogWrite(LED_PWM_PIN, brightness);

  // 每次进入极强光状态时短鸣一次，持续强光时不连续鸣叫。
  const bool isExtremeLight = lightLevel > EXTREME_LIGHT_THRESHOLD;
  if (isExtremeLight && !wasExtremeLight) {
    tone(BUZZER_PIN, BEEP_FREQUENCY_HZ, BEEP_DURATION_MS);
  }
  wasExtremeLight = isExtremeLight;
}