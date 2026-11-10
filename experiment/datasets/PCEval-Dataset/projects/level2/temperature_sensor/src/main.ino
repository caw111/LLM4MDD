/**
  Read temperature sensor value and compare with the input value
  03 MAY 2025 by Blind Author
*/

#include "DHT.h"

#define DHTTYPE DHT22

#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

float threshold = 30.0;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
    float temperature = dht.readTemperature();

    if (temperature < threshold) {
        Serial.println("Temperature is below the threshold");
    } else {
        Serial.println("Temperature is above the threshold");
    }

    delay(1000);
}