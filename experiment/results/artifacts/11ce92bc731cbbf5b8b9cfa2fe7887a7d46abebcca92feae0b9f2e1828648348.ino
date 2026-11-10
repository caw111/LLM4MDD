#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t POT_PIN = A1;  // Authoritative remapping of A0
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

void setup() {
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button connects D2 to GND.
  display.setBrightness(7, true);
  display.clear();
}

void loop() {
  const bool buttonPressed = (digitalRead(BUTTON_PIN) == LOW);
  const int value = buttonPressed ? 8888 : analogRead(POT_PIN);

  display.showNumberDec(value, false);
  delay(10);
}