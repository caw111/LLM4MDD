#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t FAN_PWM_PIN = 5;   // D5 PWM
const uint8_t TEMP_ADC_PIN = A0; // A0 analog input (LM35)

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_003\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline float readLM35C() {
  // Arduino Uno ADC: 10-bit, Vref = 5.0V by default.
  // LM35: 10mV/°C -> 0.01V/°C
  int adc = analogRead(TEMP_ADC_PIN); // 0..1023
  float voltage = (adc * 5.0f) / 1023.0f; // Volts
  float tempC = voltage / 0.01f;         // °C
  return tempC;
}

void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogReference(DEFAULT); // keep default 5V reference
  analogRead(TEMP_ADC_PIN); // warm-up read
}

void loop() {
  float tempC = readLM35C();

  // Requirement: 温度 > 30℃ 时风扇（D5 PWM）开启，否则关闭
  if (tempC > 30.0f) {
    // Enable fan via PWM. Use full scale to preserve "开启" behavior.
    analogWrite(FAN_PWM_PIN, 255);
  } else {
    // Turn off
    analogWrite(FAN_PWM_PIN, 0);
  }

  delay(200);
}
