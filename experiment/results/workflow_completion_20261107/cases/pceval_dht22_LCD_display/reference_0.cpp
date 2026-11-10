#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>
#include <math.h>

Adafruit_ILI9341 display(10, 9, 8);
DHT sensor(2, DHT22);

bool haveReading = false;
float previousHumidity = 0.0f;
float previousTemperature = 0.0f;

void setup() {
  display.begin();
  display.setRotation(1);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  sensor.begin();
}

void loop() {
  const float humidity = sensor.readHumidity();
  const float temperature = sensor.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    if (!haveReading || humidity != previousHumidity || temperature != previousTemperature) {
      display.fillScreen(ILI9341_BLACK);
      display.setCursor(0, 0);
      display.print("Humidity: ");
      display.print(humidity, 2);
      display.setCursor(0, 100);
      display.print("Temperature: ");
      display.print(temperature, 2);
      previousHumidity = humidity;
      previousTemperature = temperature;
      haveReading = true;
    }
  }

  delay(100);
}
