#include <Arduino.h>

static const uint8_t LM35_PIN = A2;

// LM35: 10 mV/°C => 0.01 V per °C. With ADC reference 5.0V:
// ADC volts = (reading/1023)*5.0, so temperature(°C) = volts/0.01
// => temperature = reading * (5.0 / 1023) / 0.01 = reading * (500.0 / 1023)
static const float ADC_REF_VOLTS = 5.0f;
static const float LM35_MV_PER_C = 10.0f; // 10 mV per °C

void setup() {
  Serial.begin(9600);
}

void loop() {
  int adc = analogRead(LM35_PIN); // 0..1023
  float volts = (adc * (ADC_REF_VOLTS / 1023.0f));
  float tempC = (volts * 1000.0f) / LM35_MV_PER_C; // mV to °C

  Serial.println(tempC, 2); // Output Celsius temperature as text with 2 decimals
  delay(200); // small pacing to avoid flooding; preserves continuous behavior
}