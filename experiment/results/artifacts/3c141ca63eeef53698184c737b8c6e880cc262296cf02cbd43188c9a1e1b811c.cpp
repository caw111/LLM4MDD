#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_003\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Authoritative hardware mapping remaps the LM35 input from A1 to A0.
const uint8_t LM35_PIN = A0;
const uint8_t FAN_PIN = 5;

const float ADC_REFERENCE_VOLTS = 5.0f;
const float FAN_THRESHOLD_C = 30.0f;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_003\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_003\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_003\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  analogReference(DEFAULT);
  analogWrite(FAN_PIN, 0);
}

void loop() {
  const int raw = analogRead(LM35_PIN);
  const float voltage = raw * (ADC_REFERENCE_VOLTS / 1023.0f);
  const float temperatureC = voltage * 100.0f;  // LM35: 10 mV/°C.

  analogWrite(FAN_PIN, temperatureC > FAN_THRESHOLD_C ? 255 : 0);
  delay(100);
}
