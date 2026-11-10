#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t POT_PIN = A0;

// 10 us bus timing keeps each display update well below 50 ms.
TM1637Display display(CLK_PIN, DIO_PIN, 10);

void setup() {
  pinMode(POT_PIN, INPUT);
  analogReference(DEFAULT);
  display.setBrightness(7, true);
  display.showNumberDec(0, false, 4, 0);
}

void loop() {
  const uint16_t adc = analogRead(POT_PIN);
  const uint16_t value =
      static_cast<uint16_t>((static_cast<uint32_t>(adc) * 9999UL) / 1023UL);

  display.showNumberDec(value, false, 4, 0);
  delay(5);
}