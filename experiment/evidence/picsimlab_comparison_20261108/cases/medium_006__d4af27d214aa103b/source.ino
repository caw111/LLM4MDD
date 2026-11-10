#include <Arduino.h>
#include <TM1637Display.h>

// TM1637 (CLK=D4, DIO=D5) + A0 potentiometer
static const uint8_t TM1637_CLK = 4;  // D4
static const uint8_t TM1637_DIO = 5;  // D5
static const uint8_t POT_PIN    = A0;

static TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

void setup() {
  tm1637.setBrightness(0x0f);
  analogReference(DEFAULT);
}

void loop() {
  int raw = analogRead(POT_PIN); // 0..1023

  long mapped = (long)raw * 9999L / 1023L; // 0..9999
  if (mapped < 0) mapped = 0;
  if (mapped > 9999) mapped = 9999;

  tm1637.showNumberDec((int)mapped, false, 4, 0); // 4 digits, no leading zeros
}