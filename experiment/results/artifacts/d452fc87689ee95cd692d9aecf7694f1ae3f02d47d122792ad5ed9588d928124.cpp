#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <TM1637Display.h>

static const uint8_t BTN_PIN  = D2;
static const uint8_t TM_CLK   = D4;
static const uint8_t TM_DIO   = D5;

TM1637Display display(TM_CLK, TM_DIO);

volatile uint32_t countValue = 0;

static bool lastBtnStableState = HIGH;
static bool lastBtnRawState = HIGH;
static unsigned long lastDebounceTimeMs = 0;
static const unsigned long debounceMs = 30;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  display.setBrightness(0x0f);
  display.clear();

  lastBtnRawState = digitalRead(BTN_PIN);
  lastBtnStableState = lastBtnRawState;
}

void loop() {
  int raw = digitalRead(BTN_PIN);

  if (raw != lastBtnRawState) {
    lastDebounceTimeMs = millis();
    lastBtnRawState = raw;
  }

  if ((millis() - lastDebounceTimeMs) >= debounceMs) {
    if (raw != lastBtnStableState) {
      lastBtnStableState = raw;

      // Active LOW button: count on press (transition to LOW)
      if (lastBtnStableState == LOW) {
        countValue++;

        // Display least significant 4 digits (TM1637 4-digit)
        uint16_t shown = (uint16_t)(countValue % 10000UL);
        display.showNumberDec(shown, false, 4, 0);
      }
    }
  }
}
