#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// TM1637 wiring (preserve requested authoritative pin assignments)
const uint8_t TM1637_CLK_PIN = 4; // D4
const uint8_t TM1637_DIO_PIN = 5; // D5
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

static unsigned long lastRefreshMs = 0;

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  dht.begin();
  display.setBrightness(0x0f); // Max brightness by default
  display.clear();
  lastRefreshMs = 0;
}

void loop() {
  unsigned long now = millis();
  if (now - lastRefreshMs >= 2000UL) { // refresh every 2 seconds
    lastRefreshMs = now;

    float tempC = dht.readTemperature(); // DHT11 temperature in °C

    // If invalid reading, show something deterministic
    if (isnan(tempC)) {
      display.showNumberDecEx(-1, 0b01000000, true); // -1 with decimal point style
    } else {
      // DHT11 typically provides integer values; still display with whole number
      int16_t t = (int16_t)round(tempC);

      // Show temperature as integer on TM1637
      display.showNumberDec(t, false);
    }
  }
}
