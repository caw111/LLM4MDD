#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr unsigned long REFRESH_INTERVAL_MS = 2000UL;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

unsigned long lastRefreshMs = 0;

void showSensorError() {
  const uint8_t segments[] = {
    SEG_G, SEG_G, SEG_G, SEG_G
  };
  display.setSegments(segments);
}

void setup() {
  dht.begin();
  display.setBrightness(7, true);
  showSensorError();

  // Allow the DHT11 to stabilize before its first reading.
  lastRefreshMs = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - lastRefreshMs < REFRESH_INTERVAL_MS) {
    return;
  }
  lastRefreshMs = now;

  const float temperatureC = dht.readTemperature();

  if (isnan(temperatureC)) {
    showSensorError();
    return;
  }

  const int temperature = static_cast<int>(round(temperatureC));
  display.showNumberDec(temperature, false, 3, 0);

  const uint8_t celsiusSymbol[] = {
    SEG_A | SEG_D | SEG_E | SEG_F
  };
  display.setSegments(celsiusSymbol, 1, 3);
}