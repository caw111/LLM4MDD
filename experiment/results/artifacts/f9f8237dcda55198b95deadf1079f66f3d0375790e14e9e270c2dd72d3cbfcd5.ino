#include <DHT.h>

constexpr uint8_t DHT_PIN = 7;
constexpr unsigned long SAMPLE_INTERVAL_MS = 1000UL;

DHT dht(DHT_PIN, DHT11);
unsigned long lastSampleMs = 0;

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