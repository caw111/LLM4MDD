#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();

  // The requirement only specifies continuous reading and printing humidity.
  // Print the value in the requested format.
  Serial.print("Humidity: ");
  Serial.println(humidity);

  // DHT22 typically needs at least ~2 seconds between reads,
  // but keep the loop responsive while avoiding invalid fast reads.
  delay(2000);
}