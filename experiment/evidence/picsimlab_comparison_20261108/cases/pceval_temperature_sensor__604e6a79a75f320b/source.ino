#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const float threshold = 30.0f;

void setup() {
  Serial.begin(9600);

  // Declare the threshold value
  Serial.println(threshold);

  dht.begin();
}

void loop() {
  // Continuously read temperature
  float tempC = dht.readTemperature(); // Celsius

  // If reading fails, retry without printing compare result
  if (isnan(tempC)) {
    return;
  }

  // Compare and print result
  if (tempC < threshold) {
    Serial.println("Temperature is below the threshold");
  } else if (tempC > threshold) {
    Serial.println("Temperature is above the threshold");
  } else {
    // Exact equality: not specified; keep deterministic behavior
    Serial.println("Temperature is above the threshold");
  }
}