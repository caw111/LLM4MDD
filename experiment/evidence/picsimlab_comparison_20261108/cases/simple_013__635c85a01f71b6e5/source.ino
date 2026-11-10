#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float tempC = dht.readTemperature(); // temperature in Celsius

  // If reading fails, DHT library returns NaN; do not simplify behavior:
  // output the value (NaN prints as "nan" by Serial).
  Serial.println(tempC);

  delay(1000); // every second
}