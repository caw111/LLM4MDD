#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int PIN_LDR  = A0; // 光敏电阻
const int PIN_LM35 = A1; // LM35
const int PIN_LED  = 13; // D13

// ===== Thresholds (settable experiment parameters) =====
const float LIGHT_THRESHOLD = 600.0f; // 光照值阈值(以A0的ADC值范围0~1023表示)
const float TEMP_THRESHOLD  = 30.0f;  // 温度阈值(单位：摄氏度)

// ===== LM35 configuration =====
const float VREF = 5.0f; // Arduino UNO 默认参考电压
// UNO ADC: 10-bit => 1023 max
// LM35: 10 mV / °C => 0.01 V / °C
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float lm35AdcToC(int adc);
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float lm35AdcToC(int adc) {
  float voltage = (adc * (VREF / 1023.0f));
  float tempC = voltage / 0.01f;
  return tempC;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(9600);
}

void loop() {
  static unsigned long lastPrintMs = 0;

  unsigned long now = millis();
  if (now - lastPrintMs >= 1000UL) { // 每秒输出一次
    lastPrintMs = now;

    int lightAdc = analogRead(PIN_LDR);   // 0~1023
    int tempAdc  = analogRead(PIN_LM35);  // 0~1023
    float tempC  = lm35AdcToC(tempAdc);

    // 如果任一值超过阈值，则额外输出 WARNING，并点亮D13 LED
    bool warn = (lightAdc > LIGHT_THRESHOLD) || (tempC > TEMP_THRESHOLD);

    if (warn) digitalWrite(PIN_LED, HIGH);
    else digitalWrite(PIN_LED, LOW);

    // 基于需求：每秒通过串口输出一次光照值和温度值；
    // 如任一值超过阈值，则额外输出 WARNING
    Serial.print("LIGHT=");
    Serial.print(lightAdc);
    Serial.print(", TEMP=");
    Serial.print(tempC, 2);
    if (warn) {
      Serial.print(" WARNING");
    }
    Serial.println();
  }
}
