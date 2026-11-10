#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

enum LedMode { CYCLING, HOLD, OFF };

const int PIN_LED_R = 2;
const int PIN_LED_G = 3;
const int PIN_LED_B = 4;

// Buttons (active-low assumed due to wiring with pull-ups required)
const int PIN_BTN1 = 5;  // k1
const int PIN_BTN2 = 6;  // k2
const int PIN_BTN3 = 7;  // k3

// RGB LED is common cathode -> HIGH turns a color ON, LOW OFF
const int COLOR_RED = 0;
const int COLOR_GREEN = 1;
const int COLOR_BLUE = 2;

static LedMode mode = CYCLING;
static int currentColor = COLOR_RED;
static unsigned long colorStartMs = 0;

static bool lastB1 = true;
static bool lastB2 = true;
static bool lastB3 = true;
static unsigned long lastDebounceMs1 = 0;
static unsigned long lastDebounceMs2 = 0;
static unsigned long lastDebounceMs3 = 0;
const unsigned long DEBOUNCE_MS = 30;

const unsigned long COLOR_HOLD_MS = 2000;

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setLedRGB(bool r, bool g, bool b);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyColor(int color);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void turnOffLed();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readButtonPressed(int pin);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool pollButtonEdge(int pin, bool &lastState, unsigned long &lastDebounceMs);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtons();
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setupPins();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void startCycling();
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setLedRGB(bool r, bool g, bool b) {
  digitalWrite(PIN_LED_R, r ? HIGH : LOW);
  digitalWrite(PIN_LED_G, g ? HIGH : LOW);
  digitalWrite(PIN_LED_B, b ? HIGH : LOW);
}

void applyColor(int color) {
  switch (color) {
    case COLOR_RED:   setLedRGB(true,  false, false); break;
    case COLOR_GREEN: setLedRGB(false, true,  false); break;
    case COLOR_BLUE:  setLedRGB(false, false, true);  break;
    default:          setLedRGB(false, false, false); break;
  }
}

void turnOffLed() {
  setLedRGB(false, false, false);
}

bool readButtonPressed(int pin) {
  // active-low: pressed when LOW
  return digitalRead(pin) == LOW;
}

bool pollButtonEdge(int pin, bool &lastState, unsigned long &lastDebounceMs) {
  bool state = (digitalRead(pin) == LOW); // pressed?
  unsigned long now = millis();

  // Convert to stable edge detection with debounce on pressed state
  if (state != lastState) {
    if (now - lastDebounceMs >= DEBOUNCE_MS) {
      lastDebounceMs = now;
      bool prevPressed = lastState;
      lastState = state;
      // Return true only on transition into pressed state
      return (!prevPressed && state);
    }
  }
  return false;
}

void handleButtons() {
  // Poll for press edges (immediate response)
  bool b1Pressed = pollButtonEdge(PIN_BTN1, lastB1, lastDebounceMs1);
  bool b2Pressed = pollButtonEdge(PIN_BTN2, lastB2, lastDebounceMs2);
  bool b3Pressed = pollButtonEdge(PIN_BTN3, lastB3, lastDebounceMs3);

  // Priority is not specified; handle in order k1, k2, k3.
  // Behavior matches requirement per button press.
  if (b1Pressed) {
    mode = HOLD;
    applyColor(currentColor); // keep the current color
  }
  if (b2Pressed) {
    if (mode != OFF) {
      mode = CYCLING;
      // Resume cycling from point it was stopped: keep currentColor,
      // restart timing from now so it continues with full 2s display intervals.
      colorStartMs = millis();
      applyColor(currentColor);
    }
  }
  if (b3Pressed) {
    mode = OFF;
    turnOffLed();
  }
}

void setupPins() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  // Buttons with internal pull-ups
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);
}

void startCycling() {
  mode = CYCLING;
  currentColor = COLOR_RED;
  applyColor(currentColor);
  colorStartMs = millis();
}

void setup() {
  setupPins();
  // Initialize debounce states as not-pressed (due to pullups, default HIGH)
  lastB1 = !readButtonPressed(PIN_BTN1);
  lastB2 = !readButtonPressed(PIN_BTN2);
  lastB3 = !readButtonPressed(PIN_BTN3);

  startCycling();
}

void loop() {
  handleButtons();

  if (mode == CYCLING) {
    unsigned long now = millis();
    if (now - colorStartMs >= COLOR_HOLD_MS) {
      // Advance to next color
      colorStartMs = now;
      currentColor = (currentColor + 1) % 3;
      applyColor(currentColor);
    }
  } else if (mode == HOLD) {
    // Keep displaying currentColor (do nothing; button handling maintains it)
  } else if (mode == OFF) {
    // Keep LEDs off (do nothing; button handling maintains it)
  }
}
