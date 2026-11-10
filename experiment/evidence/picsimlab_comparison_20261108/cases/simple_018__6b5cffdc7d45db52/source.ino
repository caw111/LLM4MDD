#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t TM1637_CLK_PIN = 4; // D4
const uint8_t TM1637_DIO_PIN = 5; // D5

TM1637Display tm1637(TM1637_CLK_PIN, TM1637_DIO_PIN);

void setup() {
  // Optionally set brightness (kept default behavior if not needed).
  // tm1637.setBrightness(0x0f);
}

void loop() {
  static uint16_t value = 0;
  static uint32_t lastUpdateMs = 0;

  uint32_t now = millis();
  if (now - lastUpdateMs >= 1000) {
    lastUpdateMs = now;
    tm1637.showNumberDec(value, false); // each second increment, show up to 4 digits
    value++;
    // Let it overflow naturally for 16-bit; TM1637 will display lower 4 digits.
  }
}