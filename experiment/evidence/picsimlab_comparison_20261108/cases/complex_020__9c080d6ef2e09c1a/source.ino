#include <Arduino.h>

const uint8_t LIGHT_PIN = A0;
const uint8_t TEMPERATURE_PIN = A1;
const uint8_t LED_PIN = 13;

// 可根据实验要求调整阈值。
const int LIGHT_THRESHOLD = 700;             // 光照 ADC 原始值：0～1023
const float TEMPERATURE_THRESHOLD_C = 30.0;   // 温度阈值：摄氏度

const float ADC_REFERENCE_VOLTAGE = 5.0;       // Uno 默认参考电压
const float LM35_VOLTS_PER_C = 0.01;          // LM35：10 mV/°C
const unsigned long REPORT_INTERVAL_MS = 1000UL;

unsigned long lastReportTime = 0;

void setup() {
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TEMPERATURE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  analogReference(DEFAULT);
  Serial.begin(9600);

  lastReportTime = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastReportTime >= REPORT_INTERVAL_MS) {
    lastReportTime += REPORT_INTERVAL_MS;

    // 丢弃切换 ADC 通道后的第一次读数，帮助采样电容稳定。
    analogRead(LIGHT_PIN);
    const int lightValue = analogRead(LIGHT_PIN);

    analogRead(TEMPERATURE_PIN);
    const int temperatureRaw = analogRead(TEMPERATURE_PIN);
    const float temperatureC =
        temperatureRaw * (ADC_REFERENCE_VOLTAGE / 1023.0) / LM35_VOLTS_PER_C;

    const bool warning =
        lightValue > LIGHT_THRESHOLD ||
        temperatureC > TEMPERATURE_THRESHOLD_C;

    digitalWrite(LED_PIN, warning ? HIGH : LOW);

    Serial.print(F("Light: "));
    Serial.print(lightValue);
    Serial.print(F(", Temperature: "));
    Serial.print(temperatureC, 2);
    Serial.println(F(" C"));

    if (warning) {
      Serial.println(F("WARNING"));
    }
  }
}