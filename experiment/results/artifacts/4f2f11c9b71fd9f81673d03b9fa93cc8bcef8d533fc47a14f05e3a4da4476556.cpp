#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 2;   // D2, INPUT_PULLUP (active-low)
const uint8_t LED_PIN = 13;     // D13

static bool running = false;        // false: stopped, LED off
static bool ledState = false;

static bool prevBtn = true;         // with INPUT_PULLUP, not pressed == HIGH
static unsigned long lastToggleMs = 0;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_028\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  // Initial: stopped, D13 off
  running = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  prevBtn = digitalRead(BUTTON_PIN);
  lastToggleMs = millis();
}

void loop() {
  // Edge-triggered toggle: each press (falling edge) toggles running state.
  bool btnNow = digitalRead(BUTTON_PIN); // pressed -> LOW, released -> HIGH

  // Detect falling edge: HIGH (released) -> LOW (pressed)
  if (prevBtn == HIGH && btnNow == LOW) {
    running = !running;

    if (!running) {
      // Stop immediately: LED off; reset timing
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      lastToggleMs = millis();
    } else {
      // Start: from OFF state, start 500ms counting
      ledState = false;
      digitalWrite(LED_PIN, LOW);
      lastToggleMs = millis();
    }
  }

  prevBtn = btnNow;

  if (running) {
    unsigned long now = millis();
    // Toggle every 500ms (tolerance requirement covered by using millis)
    if (now - lastToggleMs >= 500UL) {
      lastToggleMs = now;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    }
  }
}
