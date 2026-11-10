#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_008\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr unsigned long DEBOUNCE_MS = 30;

TM1637Display display(CLK_PIN, DIO_PIN);

unsigned long count = 0;
unsigned long lastChangeTime = 0;
uint8_t lastRawState = HIGH;
uint8_t stableState = HIGH;

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_008\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_008\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\medium_008\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Wire the button between D2 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  display.setBrightness(7);
  display.showNumberDec(0, false);

  lastRawState = digitalRead(BUTTON_PIN);
  lastChangeTime = millis();
}

void loop() {
  const unsigned long now = millis();
  const uint8_t rawState = digitalRead(BUTTON_PIN);

  if (rawState != lastRawState) {
    lastRawState = rawState;
    lastChangeTime = now;
  }

  if (rawState != stableState &&
      now - lastChangeTime >= DEBOUNCE_MS) {
    stableState = rawState;

    // Count only the press edge; holding the button does not repeat.
    if (stableState == LOW) {
      ++count;
      // The four-digit display shows the lowest four digits.
      display.showNumberDec(static_cast<int>(count % 10000UL), false);
    }
  }
}
