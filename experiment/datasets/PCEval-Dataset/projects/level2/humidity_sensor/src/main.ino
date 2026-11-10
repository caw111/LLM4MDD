/**
  Read humidity sensor value and compare with the input value
  03 MAY 2025 by Blind Author
*/

#include "DHT.h"

#define DHTTYPE DHT22

#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

float threshold = 50.0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
    float humidity = dht.readHumidity();

    if (humidity < threshold) {
        Serial.println("Humidity is below the threshold");
    } else {
        Serial.println("Humidity is above the threshold");
    }

    delay(1000);
}