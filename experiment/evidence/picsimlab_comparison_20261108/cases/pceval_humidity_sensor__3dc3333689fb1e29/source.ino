#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

const float threshold = 50.0f;

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Declare the threshold value
  Serial.println(threshold);
}

void loop() {
  float humidity = dht.readHumidity();

  // If reading fails, keep behavior safe: don't compare invalid values.
  if (isnan(humidity)) {
    delay(2000);
    return;
  }

  if (humidity < threshold) {
    Serial.println("Humidity is below the threshold");
  } else if (humidity > threshold) {
    Serial.println("Humidity is above the threshold");
  } else {
    // If exactly equal to threshold, no specific string was required.
    // Preserve expected behavior with an above/below check as specified.
    Serial.println("Humidity is above the threshold");
  }

  delay(2000);
}