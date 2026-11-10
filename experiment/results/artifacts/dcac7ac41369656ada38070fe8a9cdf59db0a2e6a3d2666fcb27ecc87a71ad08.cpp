#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_temperature_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

constexpr uint8_t DHT_PIN = 2;
constexpr float THRESHOLD = 30.0f;
constexpr unsigned long READ_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT22);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_temperature_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_temperature_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_temperature_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(20);
  dht.begin();
  delay(READ_INTERVAL_MS);
}

void loop() {
  const float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    if (temperature > THRESHOLD) {
      Serial.println(F("Temperature is above the threshold"));
    } else {
      Serial.println(F("Temperature is below the threshold"));
    }
  }

  delay(READ_INTERVAL_MS);
}
