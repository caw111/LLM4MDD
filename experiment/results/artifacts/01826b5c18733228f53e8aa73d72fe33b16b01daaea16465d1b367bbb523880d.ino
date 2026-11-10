#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

constexpr uint8_t TFT_RST = 8;
constexpr uint8_t TFT_DC  = 9;
constexpr uint8_t TFT_CS  = 10;
constexpr uint8_t DHT_PIN = 2;

// Uno hardware SPI: MOSI = 11, MISO = 12, SCK = 13.
Adafruit_ILI9341 display(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHT_PIN, DHT22);

float previousHumidity = 0.0f;
float previousTemperature = 0.0f;
bool valuesDisplayed = false;

void setup() {
  display.begin();
  display.setRotation(1);
  display.fillScreen(ILI9341_BLACK);
  display.setTextColor(ILI9341_WHITE);
  display.setTextSize(2);
  display.setTextWrap(false);

  dht.begin();
}

void loop() {
  // The DHT library caches readings to respect the DHT22 sampling interval.
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature(); // Celsius

  if (!isnan(humidity) && !isnan(temperature)) {
    if (!valuesDisplayed ||
        humidity != previousHumidity ||
        temperature != previousTemperature) {
      display.fillScreen(ILI9341_BLACK);

      display.setCursor(0, 0);
      display.print(F("Humidity: "));
      display.print(humidity);

      display.setCursor(0, 100);
      display.print(F("Temperature: "));
      display.print(temperature);

      previousHumidity = humidity;
      previousTemperature = temperature;
      valuesDisplayed = true;
    }
  }

  delay(100);
}