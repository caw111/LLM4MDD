#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr unsigned long UPDATE_INTERVAL_MS = 1000UL;

TM1637Display display(CLK_PIN, DIO_PIN);

uint16_t number = 0;
unsigned long lastUpdate = 0;

void setup() {
  display.setBrightness(7, true);
  display.showNumberDec(number, false);
  lastUpdate = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - lastUpdate;

  if (elapsed >= UPDATE_INTERVAL_MS) {
    const unsigned long increments = elapsed / UPDATE_INTERVAL_MS;
    lastUpdate += increments * UPDATE_INTERVAL_MS;
    number = (number + increments) % 10000UL;
    display.showNumberDec(number, false);
  }
}