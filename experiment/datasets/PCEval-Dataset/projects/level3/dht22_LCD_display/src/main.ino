/**
  Show dht22 sensor value in ili9431 lcd display
  06 MAY 2025 by Blind Author
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "DHT.h"

#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8

#define DHTPIN 2
#define DHTTYPE DHT22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

DHT dht(DHTPIN, DHTTYPE);

bool pressed = true;

float humidity = 0;
float temperature = 0;

void setup() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);

    dht.begin();
}

void loop() {

    if (dht.readHumidity() != humidity || dht.readTemperature() != temperature) {
        humidity = dht.readHumidity();
        temperature = dht.readTemperature();

        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.print("Humidity: ");
        tft.println(humidity);
        tft.setCursor(0, 100);
        tft.print("Temperature: ");
        tft.println(temperature);
    }
    delay(100);
} 