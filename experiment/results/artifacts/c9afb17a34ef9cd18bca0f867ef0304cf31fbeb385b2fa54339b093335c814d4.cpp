#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring from the provided circuit
static const uint8_t BTN1_PIN = 2;   // mega:2 -> btn1:1.l (green), btn1:2.r -> mega:GND
// Bargraph1: 10 LEDs mapped to A1..A10 (mega pins 3..12)
static const uint8_t BARGRAPH_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static const uint8_t DEBOUNCE_MS = 150;

static int progress = 0;                 // number of lit LEDs: 0..10

static bool lastStablePressed = false; // last stable button state
static bool lastRawPressed = false;     // last raw reading state
static uint32_t lastDebounceTime = 0;   // time of last raw change

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBargraph(int litCount);
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBargraph(int litCount) {
  if (litCount < 0) litCount = 0;
  if (litCount > 10) litCount = 10;

  for (int i = 0; i < 10; i++) {
    digitalWrite(BARGRAPH_PINS[i], (i < litCount) ? HIGH : LOW);
  }
}

void setup() {
  // Initialize button pin
  pinMode(BTN1_PIN, INPUT_PULLUP);

  // Initialize bargraph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BARGRAPH_PINS[i], OUTPUT);
    digitalWrite(BARGRAPH_PINS[i], LOW); // all LEDs off on start/reset
  }

  // Ensure initial state
  progress = 0;
  setBargraph(progress);

  // Initialize debounce tracking with current state
  bool currentPressed = (digitalRead(BTN1_PIN) == LOW); // active-low due to INPUT_PULLUP
  lastRawPressed = currentPressed;
  lastStablePressed = currentPressed;
  lastDebounceTime = millis();
}

void loop() {
  // Raw read (button is active-low)
  bool rawPressed = (digitalRead(BTN1_PIN) == LOW);

  // Debounce logic
  if (rawPressed != lastRawPressed) {
    lastDebounceTime = millis();
    lastRawPressed = rawPressed;
  }

  bool stablePressed = lastStablePressed;
  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    stablePressed = rawPressed;
  }

  // Detect a valid press: transition from not pressed to pressed (stable)
  if (!lastStablePressed && stablePressed) {
    progress++;
    if (progress > 10) progress = 0; // cycle 0..10, then reset to 0
    setBargraph(progress);
  }

  lastStablePressed = stablePressed;
}
