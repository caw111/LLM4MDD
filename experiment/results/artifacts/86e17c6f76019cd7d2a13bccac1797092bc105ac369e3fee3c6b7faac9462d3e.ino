#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastReadMs = 0;
const unsigned long intervalMs = 5000;

void setup() {
  Serial.begin(9600);
  dht.begin();
  lastReadMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastReadMs >= intervalMs) {
    lastReadMs = now;

    float t = dht.readTemperature(); // Celsius

    // Output only the temperature value (or NAN if read fails) as requested "串口输出"
    Serial.println(t, 2);
  }
}