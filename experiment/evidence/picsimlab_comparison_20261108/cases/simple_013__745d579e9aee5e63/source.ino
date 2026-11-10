#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Read temperature from DHT11 and output once per second
  float t = dht.readTemperature(); // Celsius

  // Output temperature value (including NaN if read fails, as DHT library returns)
  Serial.println(t);

  delay(1000);
}