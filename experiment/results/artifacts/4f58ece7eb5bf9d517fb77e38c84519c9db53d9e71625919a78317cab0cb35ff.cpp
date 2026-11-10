#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>

// ---------- Pin mapping (preserve given wiring) ----------
// Button
static const uint8_t BTN_PIN = 2;

// RGB1 (common cathode)
static const uint8_t RGB1_R = 3;
static const uint8_t RGB1_G = 4;
static const uint8_t RGB1_B = 5;

// RGB2 (common cathode)
static const uint8_t RGB2_R = 6;
static const uint8_t RGB2_G = 7;
static const uint8_t RGB2_B = 8;

// Bar graph A1-A7 (pins A1-A5 and A6-A7 as wired)
static const uint8_t BAR_A[7] = {9, 10, 11, 12, 13, 44, 45};

// ---------- Utilities ----------
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setRGB_CommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool onR, bool onG, bool onB);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setBothRGB(bool r1, bool g1, bool b1, bool r2, bool g2, bool b2);
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void applyModeColors(uint8_t mode);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void applyModeBar(uint8_t mode);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isButtonPressed();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleButtonIncrement();
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_86\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline void setRGB_CommonCathode(uint8_t rPin, uint8_t gPin, uint8_t bPin,
                                        bool onR, bool onG, bool onB) {
  // common cathode: HIGH=LED OFF, LOW=LED ON
  digitalWrite(rPin, onR ? LOW : HIGH);
  digitalWrite(gPin, onG ? LOW : HIGH);
  digitalWrite(bPin, onB ? LOW : HIGH);
}

static inline void setBothRGB(bool r1, bool g1, bool b1, bool r2, bool g2, bool b2) {
  setRGB_CommonCathode(RGB1_R, RGB1_G, RGB1_B, r1, g1, b1);
  setRGB_CommonCathode(RGB2_R, RGB2_G, RGB2_B, r2, g2, b2);
}

static void applyModeColors(uint8_t mode) {
  // Mode 0: all components OFF -> both RGB off
  if (mode == 0) {
    setBothRGB(false, false, false, false, false, false);
    return;
  }

  // Color sequence required:
  // (red/blue, green/cyan, yellow/magenta, cyan/yellow, meagenta/cyan, white/yellow, black/white)
  // Interpretation:
  // - for rgb1: first color
  // - for rgb2: second color
  switch (mode) {
    case 1: // red/blue
      setBothRGB(true,  false, false,   false, false, true);
      break;
    case 2: // green/cyan
      setBothRGB(false, true,  false,   false, true,  true);
      break;
    case 3: // yellow/magenta
      setBothRGB(true,  true,  false,   true,  false, true);
      break;
    case 4: // cyan/yellow
      setBothRGB(false, true,  true,    true,  true,  false);
      break;
    case 5: // meagenta/cyan (magenta/cyan)
      setBothRGB(true,  false, true,    false, true,  true);
      break;
    case 6: // white/yellow
      setBothRGB(true,  true,  true,    true,  true,  false);
      break;
    case 7: // black/white
      setBothRGB(false, false, false,   true,  true,  true);
      break;
    default:
      setBothRGB(false, false, false, false, false, false);
      break;
  }
}

static void applyModeBar(uint8_t mode) {
  // Mode 0: none lit.
  // Modes 1-7: light number of LEDs equal to mode.
  for (uint8_t i = 0; i < 7; i++) {
    uint8_t pin = BAR_A[i];
    if (mode >= (i + 1)) {
      digitalWrite(pin, HIGH);  // bar LEDs on for this circuit
    } else {
      digitalWrite(pin, LOW);
    }
  }
}

// ---------- Button logic ----------
static uint8_t mode = 0;

static const unsigned long HOLD_MS = 150;   // valid press requires holding >= 0.15s
static const unsigned long DEBOUNCE_MS = 20; // avoid false triggers

static unsigned long pressStartMs = 0;
static unsigned long lastHandledMs = 0;
static bool btnPrevPressed = false;

static bool isButtonPressed() {
  // mega:2 -> btn1:1.l (one side of pushbutton)
  // mega:GND -> btn1:2.r (other side of pushbutton)
  // Use INPUT_PULLUP: pressed -> connected to GND -> LOW
  return digitalRead(BTN_PIN) == LOW;
}

static void handleButtonIncrement() {
  bool pressed = isButtonPressed();
  unsigned long now = millis();

  if (pressed && !btnPrevPressed) {
    // Button just started being held
    pressStartMs = now;
  }

  if (pressed) {
    // If held long enough, register a valid press once
    if ((now - pressStartMs) >= HOLD_MS && (now - lastHandledMs) >= DEBOUNCE_MS) {
      lastHandledMs = now;

      mode++;
      if (mode > 7) mode = 0;

      // Wait until release to prevent multiple triggers while held
      while (isButtonPressed()) {
        delay(1);
      }
    }
  }

  btnPrevPressed = pressed;
}

void setup() {
  // Button input
  pinMode(BTN_PIN, INPUT_PULLUP);

  // RGB pins output
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Bar graph pins output
  for (uint8_t i = 0; i < 7; i++) {
    pinMode(BAR_A[i], OUTPUT);
  }

  // Initialization: all OFF
  mode = 0;
  applyModeColors(mode);
  applyModeBar(mode);
}

void loop() {
  handleButtonIncrement();

  // Apply current mode outputs (keeps synchronous behavior with mode transitions)
  applyModeColors(mode);
  applyModeBar(mode);

  delay(2);
}
