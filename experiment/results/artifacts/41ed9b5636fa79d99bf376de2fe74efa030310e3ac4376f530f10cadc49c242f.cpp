#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
#include <DHT.h>

constexpr uint8_t DHT_PIN = 2;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long READ_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT_TYPE);

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_humidity_sensor_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(READ_INTERVAL_MS);
}

void loop() {
  float humidity = dht.readHumidity();

  if (!isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.println(humidity, 1);
  } else {
    Serial.println("Error: Failed to read humidity");
  }

  delay(READ_INTERVAL_MS);
}
