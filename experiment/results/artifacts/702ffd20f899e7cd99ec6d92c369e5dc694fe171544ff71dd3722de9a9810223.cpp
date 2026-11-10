#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 3;   // D3 button input
const uint8_t LED_PIN     = 10; // D10 LED output

static bool ledState = false;
static bool lastButtonRaw = HIGH;
static unsigned long lastDebounceTime = 0;
static const unsigned long debounceDelayMs = 30;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // button to GND: pressed = LOW
  pinMode(LED_PIN, OUTPUT);

  ledState = false;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void loop() {
  bool buttonRaw = digitalRead(BUTTON_PIN);

  if (buttonRaw != lastButtonRaw) {
    lastDebounceTime = millis();
    lastButtonRaw = buttonRaw;
  }

  if (millis() - lastDebounceTime >= debounceDelayMs) {
    // detect falling edge (HIGH -> LOW) meaning button press
    static bool stablePrev = HIGH;
    if (stablePrev == HIGH && buttonRaw == LOW) {
      ledState = !ledState; // toggle once per press
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
    stablePrev = buttonRaw;
  }
}
