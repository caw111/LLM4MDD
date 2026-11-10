#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

static const uint8_t BTN1_PIN = 2;
static const uint8_t BTN2_PIN = 3;
static const uint8_t BTN3_PIN = 4;

static const uint8_t BARGRAPH_LED_COUNT = 10;

// Bargraph LED pins as connected in the provided circuit
static const uint8_t BAR_PINS[BARGRAPH_LED_COUNT] = {
  5,  6,  7,  8,  9,
  10, 11, 12, 13, 44
};

static const int STATE_HOLD_MS = 2000;

// Simple debounce / edge-detect
static const uint16_t DEBOUNCE_MS = 40;

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t readButtonStable(uint8_t pin);
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int angleForButton(uint8_t btnIndex);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int ledsForAngle(int angle);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBargraph(int litCount);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyState(int angle);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t readButtonStable(uint8_t pin) {
  // Active-low assumed? In the given wiring, buttons connect pin to GND when pressed.
  // So use INPUT_PULLUP and treat LOW as pressed.
  return (digitalRead(pin) == LOW) ? 1 : 0;
}

int angleForButton(uint8_t btnIndex) {
  // btnIndex: 1->30, 2->60, 3->90
  if (btnIndex == 1) return 30;
  if (btnIndex == 2) return 60;
  return 90;
}

int ledsForAngle(int angle) {
  // Rules: 30 -> first 3 LEDs, 60 -> first 6 LEDs, 90 -> all 10
  if (angle == 30) return 3;
  if (angle == 60) return 6;
  return 10;
}

void setBargraph(int litCount) {
  for (int i = 0; i < (int)BARGRAPH_LED_COUNT; i++) {
    digitalWrite(BAR_PINS[i], (i < litCount) ? HIGH : LOW);
  }
}

void applyState(int angle) {
  int lit = ledsForAngle(angle);
  setBargraph(lit);
  servo1.write(angle);
}

void setup() {
  // Buttons: use internal pullups; wired such that pressed connects to GND (so LOW when pressed)
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Bargraph LED pins
  for (int i = 0; i < (int)BARGRAPH_LED_COUNT; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  servo1.attach(45); // connected to mega:45 -> servo1:PWM
  applyState(0);     // servo at 0 degrees, bargraph off
  delay(20);
}

void loop() {
  static uint8_t lastPressed = 0;
  static uint32_t lastChangeMs = 0;

  uint8_t b1 = readButtonStable(BTN1_PIN);
  uint8_t b2 = readButtonStable(BTN2_PIN);
  uint8_t b3 = readButtonStable(BTN3_PIN);

  uint8_t currentlyPressed = 0;
  uint8_t currentBtnIndex = 0;

  if (b1) { currentlyPressed = 1; currentBtnIndex = 1; }
  else if (b2) { currentlyPressed = 1; currentBtnIndex = 2; }
  else if (b3) { currentlyPressed = 1; currentBtnIndex = 3; }

  uint32_t now = millis();

  // Debounce based on changes in "pressed any"
  if (currentlyPressed != lastPressed) {
    lastChangeMs = now;
    lastPressed = currentlyPressed;
  }

  if (now - lastChangeMs >= DEBOUNCE_MS) {
    // Only act on a stable press edge (transition to pressed)
    static uint8_t prevStablePressed = 0;

    if (currentlyPressed && !prevStablePressed) {
      int angle = angleForButton(currentBtnIndex);
      applyState(angle);

      // Maintain state for at least 2 seconds
      uint32_t start = millis();
      while (millis() - start < (uint32_t)STATE_HOLD_MS) {
        // Keep outputs steady; still allow debouncing without changing state
        // (buttons are ignored during this hold period per required behavior)
        delay(10);
      }
    }

    prevStablePressed = currentlyPressed;
  }

  delay(5);
}
