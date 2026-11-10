#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t BUTTON_PIN = 2;  // D2
const uint8_t TM_CLK_PIN  = 4;  // D4 (CLK)
const uint8_t TM_DIO_PIN  = 5;  // D5 (DIO)

TM1637Display tmDisplay(TM_CLK_PIN, TM_DIO_PIN);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  tmDisplay.setBrightness(0x0f); // max brightness (library-supported)
  tmDisplay.clear();
}

void loop() {
  // INPUT_PULLUP: pressed -> LOW, not pressed -> HIGH
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    tmDisplay.showNumberDecEx(1111, 0b01000000, true);
  } else {
    tmDisplay.showNumberDecEx(0, 0b01000000, true); // show "0000" when number is 0
  }

  delay(20); // basic debounce to reduce rapid re-triggering
}