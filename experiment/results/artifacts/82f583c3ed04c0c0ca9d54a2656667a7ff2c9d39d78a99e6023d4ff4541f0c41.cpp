#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t FAN_PWM_PIN = 5;   // D5 PWM
const uint8_t TEMP_SENSOR_PIN = A0; // A1 温度传感器 (LM35) - mapped to A0 per provided config

// LM35: 10mV/°C, with 5V ADC reference
// ADC reading: value(0..1023) => voltage = value * (5.0/1023)
// Temperature (°C) = voltage / 0.01
const float ADC_REF_V = 5.0f;
const float LM35_SENS_V_PER_C = 0.01f; // 10mV/°C

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);

  // Ensure fan starts OFF
  analogWrite(FAN_PWM_PIN, 0);
}

void loop() {
  int adc = analogRead(TEMP_SENSOR_PIN);

  float voltage = (adc * ADC_REF_V) / 1023.0f;
  float tempC = voltage / LM35_SENS_V_PER_C;

  if (tempC > 30.0f) {
    // Fan ON (full speed PWM). Requirement: "风扇（D5 PWM）开启"
    analogWrite(FAN_PWM_PIN, 255);
  } else {
    // Fan OFF
    analogWrite(FAN_PWM_PIN, 0);
  }

  // Small delay to avoid excessive ADC reads
  delay(100);
}
