#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring per requirements:
// TM1637: CLK=D4, DIO=D5
// Button: D2 (clear/reset every count)
static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;
static const uint8_t BUTTON_PIN = D2;

TM1637Display display(TM1637_CLK, TM1637_DIO);

static unsigned long lastTickMs = 0;
static unsigned long secondsCount = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize display
  display.setBrightness(0x0f, true); // max brightness, automatic brightness enabled

  // Show initial value
  display.showNumberDec(0, true);
  lastTickMs = millis();
}

void loop() {
  // Button (D2) clears the counter
  // Assumption: active-low due to INPUT_PULLUP
  if (digitalRead(BUTTON_PIN) == LOW) {
    secondsCount = 0;
    display.showNumberDec(secondsCount, true);
    // Debounce / avoid repeated clears while held
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(5);
    }
    lastTickMs = millis();
  }

  // Every second increment
  unsigned long now = millis();
  if (now - lastTickMs >= 1000UL) {
    // Handle potential drift by incrementing once per elapsed second
    unsigned long elapsed = (now - lastTickMs) / 1000UL;
    secondsCount += elapsed;
    lastTickMs += elapsed * 1000UL;

    display.showNumberDec(secondsCount, true);
  }
}
