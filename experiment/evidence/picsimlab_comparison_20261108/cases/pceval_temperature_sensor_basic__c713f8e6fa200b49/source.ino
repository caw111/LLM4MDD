#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperatureC = dht.readTemperature(); // Celsius

  // Print temperature continuously in the required format.
  // If reading fails, DHT library returns NAN; print as-is.
  Serial.print("Current temperature: ");
  Serial.println(temperatureC);

  delay(1000);
}