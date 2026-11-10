#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  // Initialize and begin DHT sensor
  dht.begin();
}

void loop() {
  // Continuously read the DHT sensor's humidity value
  float humidity = dht.readHumidity();

  // Print humidity to the serial monitor in the format: "Humidity: {humidity value}"
  Serial.print("Humidity: ");
  Serial.println(humidity, 1);

  delay(2000);
}