#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_5\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LED_PIN = 3;
constexpr uint8_t BUTTON_PIN = 2;
constexpr unsigned long DEBOUNCE_MS = 30;

bool ledOn = true;
int lastRawButtonState = HIGH;
int stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_5\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_5\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_5\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  digitalWrite(LED_PIN, HIGH);
  pinMode(LED_PIN, OUTPUT);

  // Button connects pin 2 to ground when pressed.
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  const unsigned long now = millis();
  const int rawButtonState = digitalRead(BUTTON_PIN);

  if (rawButtonState != lastRawButtonState) {
    lastRawButtonState = rawButtonState;
    lastButtonChangeMs = now;
  }

  if (rawButtonState != stableButtonState &&
      now - lastButtonChangeMs >= DEBOUNCE_MS) {
    stableButtonState = rawButtonState;

    // Toggle only on a debounced press, not on release or while held.
    if (stableButtonState == LOW) {
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }
}
