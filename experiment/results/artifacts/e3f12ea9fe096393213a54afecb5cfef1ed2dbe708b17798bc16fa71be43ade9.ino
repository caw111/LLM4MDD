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
  float humidity = dht.readHumidity();

  if (!isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.println(humidity, 1);
  } else {
    Serial.println("Error: Failed to read humidity");
  }

  delay(READ_INTERVAL_MS);
}