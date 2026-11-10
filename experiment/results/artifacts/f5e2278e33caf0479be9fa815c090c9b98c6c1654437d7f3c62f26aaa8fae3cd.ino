#include <Arduino.h>

const int PIN_LDR = A0;     // 光敏电阻
const int PIN_LM35 = A1;    // LM35 温度传感器
const int PIN_LED = 13;     // D13 指示灯

// 设定阈值（按实验监测系统常见需求给出默认值；可在此处按需要修改）
static const int LDR_THRESHOLD = 600;     // 光照阈值（A0 0~1023）
static const float TEMP_THRESHOLD_C = 30.0f; // 温度阈值（摄氏度）

static const unsigned long PERIOD_MS = 1000;

unsigned long lastPrintMs = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(9600);
  // 题目要求 line_or_idle/idle_timeout_ms 不涉及 Arduino 层显式设置，
  // 这里使用默认行为：每次 println 输出一行数据。
}

void loop() {
  unsigned long now = millis();
  if (now - lastPrintMs >= PERIOD_MS) {
    lastPrintMs = now;

    // 读取传感器
    int ldrRaw = analogRead(PIN_LDR);
    int lm35Raw = analogRead(PIN_LM35);

    // 光照值输出（原始 ADC）
    float lightValue = (float)ldrRaw;

    // LM35：典型换算：10mV/°C
    // Uno ADC: 5V reference, 10-bit => 1024 steps
    // 温度(°C) = ADC * (5.0/1024) / 0.010
    float tempC = ((float)lm35Raw * (5.0f / 1024.0f)) / 0.010f;

    // 阈值判断
    bool warn = (ldrRaw > LDR_THRESHOLD) || (tempC > TEMP_THRESHOLD_C);

    // 点亮/熄灭 D13
    digitalWrite(PIN_LED, warn ? HIGH : LOW);

    // 串口输出：每秒一次光照值和温度值；若任一超阈值额外输出 WARNING
    Serial.print("LIGHT=");
    Serial.print(lightValue, 0);
    Serial.print(", TEMP=");
    Serial.print(tempC, 2);

    if (warn) {
      Serial.print(", WARNING");
    }
    Serial.println();
  }
}