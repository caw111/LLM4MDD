#include <TM1637Display.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;

TM1637Display display(CLK_PIN, DIO_PIN);

void updateDisplay() {
  // Button connects D2 to GND when pressed.
  const bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  static int lastValue = -1;
  const int value = pressed ? 1111 : 0;

  if (value != lastValue) {
    display.showNumberDec(value, true, 4, 0);
    lastValue = value;
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.setBrightness(7, true);
  updateDisplay();
}

void loop() {
  updateDisplay();
}