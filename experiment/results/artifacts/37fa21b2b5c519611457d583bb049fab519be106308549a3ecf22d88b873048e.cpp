#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_013\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

constexpr uint8_t DHT_PIN = 7;
constexpr unsigned long SAMPLE_INTERVAL_MS = 1000UL;

DHT dht(DHT_PIN, DHT11);
unsigned long lastSampleMs = 0;

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_013\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_013\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_013\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  dht.begin();
  lastSampleMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;

    const float temperatureC = dht.readTemperature();

    if (isnan(temperatureC)) {
      Serial.println(F("Error: DHT11 temperature read failed"));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(temperatureC, 1);
      Serial.println(F(" C"));
    }
  }
}
