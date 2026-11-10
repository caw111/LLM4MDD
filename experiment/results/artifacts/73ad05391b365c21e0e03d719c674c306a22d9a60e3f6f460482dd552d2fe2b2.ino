#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 2;
constexpr uint8_t DIO_PIN = 3;

TM1637Display display(CLK_PIN, DIO_PIN);

void setup() {
  display.setBrightness(7, true);
  display.showNumberDec(1234, false, 4, 0);
}

void loop() {
}