#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t PIN_BUTTON = 2; // D2
constexpr uint8_t PIN_TM1637_CLK = 4; // D4
constexpr uint8_t PIN_TM1637_DIO = 5; // D5

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

static volatile uint32_t counter = 0;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_008\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  display.setBrightness(0x0f);  // reasonable default brightness
  display.clear();

  // Initial display
  display.showNumberDec(counter, false, 4);
}

void loop() {
  static uint8_t lastStable = HIGH;
  static uint8_t lastRead = HIGH;
  static unsigned long lastDebounceMs = 0;

  const uint8_t reading = digitalRead(PIN_BUTTON);

  if (reading != lastRead) {
    lastDebounceMs = millis();
    lastRead = reading;
  }

  if ((millis() - lastDebounceMs) > 25) { // debounce interval
    if (reading != lastStable) {
      lastStable = reading;

      // Count on press (active LOW: INPUT_PULLUP)
      if (lastStable == LOW) {
        counter += 1;

        // Display updated value (up to 4 digits)
        display.showNumberDec(counter, false, 4);
      }
    }
  }
}
