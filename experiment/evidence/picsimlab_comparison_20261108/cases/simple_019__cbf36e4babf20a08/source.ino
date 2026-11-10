#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t TM1637_CLK = D4; // CLK=D4
static const uint8_t TM1637_DIO = D5; // DIO=D5

TM1637Display display(TM1637_CLK, TM1637_DIO);

static uint16_t mapAdcTo0_9999(uint16_t adc) {
  // Linear mapping: 0..1023 -> 0..9999, floor
  // value = floor(adc * 9999 / 1023)
  return (uint32_t)adc * 9999UL / 1023UL;
}

void setup() {
  // Ensure display is always on and ready
  display.setBrightness(0x0F); // max brightness (always on)
  display.clear();
}

void loop() {
  uint16_t adc = analogRead(A0);          // 0..1023
  uint16_t val = mapAdcTo0_9999(adc);     // 0..9999, floor

  // Display complete decimal number (allow leading zeros/blank behavior controlled by library)
  // Use 4-digit raw numeric display to ensure full 0..9999 coverage.
  display.showNumberDec(val, false, 4, 0);

  // No delay required; response must be <= 50ms after input change.
}