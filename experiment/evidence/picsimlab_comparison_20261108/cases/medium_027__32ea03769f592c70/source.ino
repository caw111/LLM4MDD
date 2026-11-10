#include <DHT.h>

// DHT11 on D7
constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  static unsigned long lastReadMs = 0;
  const unsigned long intervalMs = 5000;

  unsigned long now = millis();
  if (now - lastReadMs >= intervalMs) {
    lastReadMs = now;

    float tempC = dht.readTemperature(); // Celsius
    if (isnan(tempC)) {
      Serial.println("NaN");
    } else {
      Serial.println(tempC);
    }
  }
}