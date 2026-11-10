#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 four-digit display
// Requirement wiring (authoritative pin assignments):
// CLK -> D2, DIO -> D3
// NOTE: Provided configuration JSON remapped pins to D4/D5; however the user requirement
// explicitly states D2/D3. We preserve the user-requested wiring.
static const uint8_t TM1637_CLK = 2;  // D2
static const uint8_t TM1637_DIO = 3;  // D3

TM1637Display display(TM1637_CLK, TM1637_DIO);

void setup() {
  display.setBrightness(0x0f); // maximum brightness (typical safe default)
  display.clear();
  display.showNumberDec(1234, false);
}

void loop() {
  // Continuously keep displaying 1234 as required.
  display.showNumberDec(1234, false);
}