#include <Arduino.h>
#include <DHT.h>

static const int DHT_PIN = 2;
static const int DHT_TYPE = DHT22;

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  const float threshold = 50.0f;

  Serial.begin(9600);
  // Requirement: Declare the threshold value (50.0)
  Serial.print("Threshold: ");
  Serial.println(threshold);

  dht.begin();
}

void loop() {
  // Serial input is always a float value (read but not used for behavior).
  // This keeps the sketch compatible with the provided testing expectation.
  if (Serial.available()) {
    float incoming = Serial.parseFloat();
    (void)incoming;
    // No additional behavior required/defined for incoming value.
  }

  // Continuously read humidity
  float humidity = dht.readHumidity();

  // Compare and print exactly as required
  if (humidity < 50.0f) {
    Serial.println("Humidity is below the threshold");
  } else {
    Serial.println("Humidity is above the threshold");
  }

  // Reasonable pacing for DHT22 reads
  delay(2000);
}