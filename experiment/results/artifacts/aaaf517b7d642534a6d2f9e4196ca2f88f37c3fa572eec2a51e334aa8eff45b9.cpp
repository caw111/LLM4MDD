#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

constexpr uint8_t DHT_PIN = 2;
constexpr float HUMIDITY_THRESHOLD = 50.0f;
constexpr unsigned long READ_INTERVAL_MS = 2000UL;

DHT dht(DHT_PIN, DHT22);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // DHT22 readings must be spaced at least two seconds apart.
  delay(READ_INTERVAL_MS);

  const float humidity = dht.readHumidity();

  // Ignore failed readings rather than printing an incorrect comparison.
  if (isnan(humidity)) {
    return;
  }

  if (humidity > HUMIDITY_THRESHOLD) {
    Serial.println("Humidity is above the threshold");
  } else {
    // Equality is treated as not above the threshold.
    Serial.println("Humidity is below the threshold");
  }
}
