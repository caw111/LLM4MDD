#include <Arduino.h>
#include <TM1637Display.h>

const uint8_t BUTTON_PIN = D2;   // digital input, active while pressed
const uint8_t TM_CLK_PIN  = D4;  // TM1637 CLK
const uint8_t TM_DIO_PIN  = D5;  // TM1637 DIO

TM1637Display tmDisplay(TM_CLK_PIN, TM_DIO_PIN);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize display (brightness, if supported by library)
  tmDisplay.setBrightness(0x0f); // max brightness
  tmDisplay.clear();
}

void loop() {
  // With INPUT_PULLUP: pressed -> LOW, not pressed -> HIGH
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (pressed) {
    tmDisplay.showNumberDecEx(1111, 0b01000000, true); // 1111, no leading zeros
  } else {
    tmDisplay.showNumberDecEx(0000, 0b01000000, true); // 0000
  }

  // Small delay to reduce rapid re-triggering due to button bounce
  delay(20);
}