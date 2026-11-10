#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const float threshold = 30.0f;

void setup() {
  Serial.begin(9600);
  // Declare the threshold value
  Serial.println(String("Threshold: ") + threshold);
  dht.begin();
}

void loop() {
  // Continuously read temperature and compare against threshold
  float t = dht.readTemperature(); // in °C

  if (isnan(t)) {
    // If sensor read fails, try again on next loop iteration
    return;
  }

  if (t < threshold) {
    Serial.println("Temperature is below the threshold");
  } else if (t > threshold) {
    Serial.println("Temperature is above the threshold");
  } else {
    // Preserve compare behavior: above vs below only
    Serial.println("Temperature is above the threshold");
  }

  // DHT sensors should not be polled too quickly
  delay(2000);
}