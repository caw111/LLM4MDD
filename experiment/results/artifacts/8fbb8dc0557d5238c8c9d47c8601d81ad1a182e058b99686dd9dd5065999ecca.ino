#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22

// LCD wiring (per circuit)
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHTPIN, DHTTYPE);

float lastHumidity = NAN;
float lastTemperature = NAN;

void setup() {
  // 2. Begin display and change it to landscape mode (rotation 1)
  tft.begin();
  tft.setRotation(1);

  // 3. Set the text color to white, and size to 2
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  // 4. Initialize and begin the dht22 sensor
  dht.begin();
}

void loop() {
  // 5. Continuously read humidity and temperature value of dht22 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // If sensor returns NaN, do nothing (avoids clearing display on bad reads)
  if (!isnan(humidity) && !isnan(temperature)) {
    // 6. If sensor value is changed, clear and print both fields
    if (humidity != lastHumidity || temperature != lastTemperature) {
      tft.fillScreen(ILI9341_BLACK);

      tft.setCursor(0, 0);
      tft.print("Humidity: ");
      tft.print(humidity);

      tft.setCursor(0, 100);
      tft.print("Temperature: ");
      tft.print(temperature);

      lastHumidity = humidity;
      lastTemperature = temperature;
    }
  }

  // Wait for 100 milliseconds at the end of every loop iteration
  delay(100);
}