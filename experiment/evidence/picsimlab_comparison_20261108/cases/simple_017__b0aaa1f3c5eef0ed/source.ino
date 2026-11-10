#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 4-digit display
// CLK -> D2, DIO -> D3
static const uint8_t TM1637_CLK_PIN = 2;
static const uint8_t TM1637_DIO_PIN = 3;

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

void setup() {
  // Set brightness (0..7). Requirement doesn't specify; choose a sensible default.
  display.setBrightness(5);

  // Display 1234
  display.showNumberDec(1234, true); // leading zero suppression enabled (doesn't affect 1234)
}

void loop() {
  // Keep displaying 1234 continuously as required
  display.showNumberDec(1234, true);
}