#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_17\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t RED_LED_PIN = 2;
constexpr uint8_t RGB_RED_PIN = 3;
constexpr uint8_t RGB_GREEN_PIN = 4;
constexpr uint8_t RGB_BLUE_PIN = 5;
constexpr uint8_t BLUE_LED_PIN = 6;

constexpr unsigned long STATE_DURATION_MS = 2000UL;

bool redState = true;
unsigned long stateStartedAt = 0;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_17\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(bool showRed);
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_17\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_17\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_17\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(bool showRed) {
  // All LEDs are active HIGH; the RGB LED has a common cathode.
  digitalWrite(RED_LED_PIN, showRed ? HIGH : LOW);
  digitalWrite(BLUE_LED_PIN, showRed ? LOW : HIGH);
  digitalWrite(RGB_RED_PIN, showRed ? HIGH : LOW);
  digitalWrite(RGB_GREEN_PIN, LOW);
  digitalWrite(RGB_BLUE_PIN, showRed ? LOW : HIGH);
}

void setup() {
  // Preload output levels so initialization starts in the red state.
  applyState(true);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  // Wiring is preserved: pin 6 controls led2. Its supplied circuit
  // attribute is red; a physically blue LED is needed to emit blue light.
  stateStartedAt = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - stateStartedAt >= STATE_DURATION_MS) {
    redState = !redState;
    applyState(redState);
    stateStartedAt = now;
  }
}
