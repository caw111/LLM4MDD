#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_018\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr unsigned long UPDATE_INTERVAL_MS = 1000UL;

TM1637Display display(CLK_PIN, DIO_PIN);

uint16_t number = 0;
unsigned long lastUpdate = 0;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_018\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_018\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\simple_018\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  display.setBrightness(7, true);
  display.showNumberDec(number, false);
  lastUpdate = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - lastUpdate;

  if (elapsed >= UPDATE_INTERVAL_MS) {
    const unsigned long increments = elapsed / UPDATE_INTERVAL_MS;
    lastUpdate += increments * UPDATE_INTERVAL_MS;
    number = (number + increments) % 10000UL;
    display.showNumberDec(number, false);
  }
}
