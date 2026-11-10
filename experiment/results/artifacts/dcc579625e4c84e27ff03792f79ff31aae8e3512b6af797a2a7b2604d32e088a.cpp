#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Uno
// D2: Button input (mode toggle)
// D13: LED output (blink / solid / off)

const uint8_t BUTTON_PIN = 2;   // D2
const uint8_t LED_PIN     = 13;  // D13

// Mode: 0=blink, 1=solid, 2=off
volatile uint8_t mode = 0;

const unsigned long BLINK_INTERVAL_MS = 250;
static unsigned long lastToggleMs = 0;

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_016\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  // Initialize blink timing
  lastToggleMs = millis();
}

void loop() {
  // Read button (active-low due to INPUT_PULLUP)
  static bool lastBtn = HIGH;
  static unsigned long lastDebounceMs = 0;
  const unsigned long DEBOUNCE_MS = 30;

  bool btn = digitalRead(BUTTON_PIN);
  if (btn != lastBtn) {
    lastDebounceMs = millis();
    lastBtn = btn;
  }

  // Detect stable press (falling edge after debounce)
  static bool stableBtn = HIGH;
  if ((millis() - lastDebounceMs) > DEBOUNCE_MS) {
    stableBtn = btn;
  }

  static bool prevStableBtn = HIGH;
  if (stableBtn != prevStableBtn) {
    prevStableBtn = stableBtn;

    // Press detected when input transitions to LOW
    if (stableBtn == LOW) {
      mode = (mode + 1) % 3; // D2 toggles mode: blink -> solid -> off -> blink ...
      // Reset timing for smoother transitions
      lastToggleMs = millis();
      if (mode == 0) {
        // ensure LED state matches blink start
        digitalWrite(LED_PIN, HIGH);
      } else if (mode == 1) {
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }

  // Apply LED behavior based on current mode
  switch (mode) {
    case 0: { // blink
      unsigned long now = millis();
      if (now - lastToggleMs >= BLINK_INTERVAL_MS) {
        lastToggleMs = now;
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      }
      break;
    }
    case 1: // solid on
      digitalWrite(LED_PIN, HIGH);
      break;
    case 2: // off
    default:
      digitalWrite(LED_PIN, LOW);
      break;
  }
}
