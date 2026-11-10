/**
  Read the humidity sensor value 
  03 MAY 2025 by Blind Author
*/

#include "DHT.h"

#define DHTTYPE DHT22

#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();

  Serial.print("Humidity: ");
  Serial.println(humidity);

  delay(1000);
}