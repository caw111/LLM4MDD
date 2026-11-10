#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// RGB LED (common cathode)
static const uint8_t RGB_R = 2;  // mega:2 -> rgb1:R (red)
static const uint8_t RGB_G = 3;  // mega:3 -> rgb1:G (green)
static const uint8_t RGB_B = 4;  // mega:4 -> rgb1:B (blue)

// Push button (k1)
static const uint8_t K1_PIN = 5;   // mega:5 -> k1:1.l (green)

// Debounce
static const unsigned long DEBOUNCE_MS = 30;

static uint8_t stateIndex = 0; // 0=off, 1=red, 2=green, 3=blue
static bool lastStableLevel = HIGH;
static bool lastReading = HIGH;
static unsigned long lastChangeMs = 0;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLedOff();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showState(uint8_t idx);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_2\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLedOff() {
  // Common cathode: LED on = drive corresponding pin HIGH
  digitalWrite(RGB_R, LOW);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, LOW);
}

static void showState(uint8_t idx) {
  setLedOff();
  switch (idx) {
    case 1: // red
      digitalWrite(RGB_R, HIGH);
      break;
    case 2: // green
      digitalWrite(RGB_G, HIGH);
      break;
    case 3: // blue
      digitalWrite(RGB_B, HIGH);
      break;
    case 0: // off
    default:
      // already off
      break;
  }
}

void setup() {
  // RGB LED pins
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  // Button pin
  pinMode(K1_PIN, INPUT_PULLUP);

  // Initial: LED off
  stateIndex = 0;
  showState(stateIndex);

  // Initialize debounce state tracking
  lastStableLevel = digitalRead(K1_PIN);
  lastReading = lastStableLevel;
  lastChangeMs = millis();
}

void loop() {
  bool reading = digitalRead(K1_PIN);

  if (reading != lastReading) {
    lastChangeMs = millis();
    lastReading = reading;
  }

  if ((millis() - lastChangeMs) >= DEBOUNCE_MS) {
    if (reading != lastStableLevel) {
      lastStableLevel = reading;

      // Detect press: with INPUT_PULLUP, pressed typically pulls LOW.
      if (lastStableLevel == LOW) {
        // Cycle: red -> green -> blue -> off -> repeat
        // Our stateIndex mapping: 0=off, 1=red, 2=green, 3=blue
        stateIndex = (stateIndex + 1) % 4;
        showState(stateIndex);
      }
    }
  }
}
