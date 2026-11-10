#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_015\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LM35_PIN = A2;
const float ADC_REFERENCE_VOLTS = 5.0f;
const unsigned long SAMPLE_INTERVAL_MS = 1000UL;

unsigned long lastSampleMs = 0;
bool firstSample = true;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_015\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_015\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_015\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LM35_PIN, INPUT);
  analogReference(DEFAULT);
  Serial.begin(9600);
  Serial.setTimeout(20);
}

void loop() {
  const unsigned long now = millis();

  if (firstSample || now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    firstSample = false;
    lastSampleMs = now;

    const int raw = analogRead(LM35_PIN);
    const float voltage = raw * (ADC_REFERENCE_VOLTS / 1023.0f);
    const float temperatureC = voltage * 100.0f;  // LM35: 10 mV/°C

    Serial.print(F("Temperature: "));
    Serial.print(temperatureC, 2);
    Serial.println(F(" C"));
  }
}
