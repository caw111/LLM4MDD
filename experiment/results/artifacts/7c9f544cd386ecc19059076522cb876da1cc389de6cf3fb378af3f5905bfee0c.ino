#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

#define DHTPIN    2
#define DHTTYPE   DHT22

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHTPIN, DHTTYPE);

float lastHumidity = NAN;
float lastTemperature = NAN;

void setup() {
  // 1. Declare Adafruit_ILI9341 object (already done globally)
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
  float h = dht.readHumidity();
  float temp = dht.readTemperature();

  // Only act when the sensor value is changed (and reads are valid)
  bool valid = !isnan(h) && !isnan(temp);
  bool changed = (h != lastHumidity) || (temp != lastTemperature);

  if (valid && changed) {
    // Clear the display with a black background
    tft.fillScreen(ILI9341_BLACK);

    // 6. Print values at required positions and formats
    tft.setCursor(0, 0);
    tft.print("Humidity: ");
    tft.print(h);

    tft.setCursor(0, 100);
    tft.print("Temperature: ");
    tft.print(temp);

    lastHumidity = h;
    lastTemperature = temp;
  }

  // Wait for 100 milliseconds at the end of every loop iteration
  delay(100);
}