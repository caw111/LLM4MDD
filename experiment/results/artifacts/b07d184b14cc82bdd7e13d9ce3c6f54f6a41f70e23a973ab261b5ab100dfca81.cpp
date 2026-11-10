#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// TM1637: CLK=D4, DIO=D5
// Button: D2 (clear/reset every count)
static const uint8_t TM1637_CLK = 4;   // D4 on Arduino Uno
static const uint8_t TM1637_DIO = 5;   // D5 on Arduino Uno
static const uint8_t BUTTON_PIN = 2;  // D2 on Arduino Uno

TM1637Display display(TM1637_CLK, TM1637_DIO);

static unsigned long lastTickMs = 0;
static unsigned long secondsCount = 0;

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  display.setBrightness(0x0f, true); // max brightness, automatic brightness enabled
  display.showNumberDec(0, true);

  lastTickMs = millis();
}

void loop() {
  // Button clears the counter (active-low with INPUT_PULLUP)
  if (digitalRead(BUTTON_PIN) == LOW) {
    secondsCount = 0;
    display.showNumberDec(secondsCount, true);

    // Debounce / avoid repeated clears while held
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(5);
    }

    lastTickMs = millis();
  }

  // Every second increment (handles drift by incrementing per elapsed second)
  unsigned long now = millis();
  if (now - lastTickMs >= 1000UL) {
    unsigned long elapsed = (now - lastTickMs) / 1000UL;
    secondsCount += elapsed;
    lastTickMs += elapsed * 1000UL;

    display.showNumberDec(secondsCount, true);
  }
}
