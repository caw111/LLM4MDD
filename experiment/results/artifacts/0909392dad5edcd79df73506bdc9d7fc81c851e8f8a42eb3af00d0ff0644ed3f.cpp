#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_027\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 7;
constexpr unsigned long READ_INTERVAL_MS = 5000UL;

DHT dht(DHT_PIN, DHT11);
unsigned long lastReadMs = 0;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_027\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_027\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  dht.begin();
  lastReadMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = now;

    const float temperatureC = dht.readTemperature();

    if (isnan(temperatureC)) {
      Serial.println(F("Error: Failed to read DHT11 temperature."));
    } else {
      Serial.print(F("Temperature: "));
      Serial.print(temperatureC, 1);
      Serial.println(F(" C"));
    }
  }
}
