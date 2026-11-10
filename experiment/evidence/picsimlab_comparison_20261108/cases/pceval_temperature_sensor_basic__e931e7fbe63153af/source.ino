#include <DHT.h>

constexpr uint8_t DHT_PIN = 2;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long READ_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(READ_INTERVAL_MS);
}

void loop() {
  const float temperature = dht.readTemperature();  // Celsius

  if (!isnan(temperature)) {
    Serial.print(F("Current temperature: "));
    Serial.println(temperature);
  } else {
    Serial.println(F("Error: Failed to read DHT22 temperature."));
  }

  delay(READ_INTERVAL_MS);
}