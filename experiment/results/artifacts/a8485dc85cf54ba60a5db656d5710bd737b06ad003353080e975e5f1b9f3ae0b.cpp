#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// ---- Hardware pin mapping (authoritative from request) ----
static const uint8_t PIN_ANALOG_POT = A0;   // device_role: analog_input
static const uint8_t PIN_BTN_AUTO   = D2;    // device_role: digital_input (D2)
static const uint8_t PIN_BTN_MANUAL = D3;    // (needed by requirement, per wiring note)

// ---- Behavior / configuration ----
// Requirement mentions:
// - Buttons (D2/D3): D2 automatic mode (gradient), D3 manual mode (potentiometer control)
// - Preserve requested PWM, calibration, thresholds, timers and all other behavior.
// However, the prompt does not provide any numeric PWM/calibration/threshold/timer details.
// The sketch below implements the mode switching + functional behavior, while using
// standard Arduino PWM mapping (0..255) from the potentiometer. If you have specific
// thresholds/timers/PWM, provide them and I will adjust exactly.

// ---- Output PWM pin ----
// The prompt says "given circuit" but no circuit/wiring for PWM output is included.
// Use a commonly-used PWM pin (D9). If your circuit uses another pin, tell me and I will remap.
static const uint8_t PIN_PWM_OUT = 9;

// ---- Mode state ----
enum Mode : uint8_t { MODE_AUTO_GRADIENT = 0, MODE_MANUAL_POT = 1 };
static Mode mode = MODE_AUTO_GRADIENT;

// ---- Auto gradient parameters ----
// Again, no exact parameters provided. Use a smooth up/down gradient.
static int16_t gradValue = 0;     // 0..255
static int16_t gradStep  = 1;     // increment/decrement
static uint16_t gradUpdateMs = 10; // update interval (standard default)

// Timing helper
static unsigned long lastGradUpdateMs = 0;

// Debounce for buttons (generic, adjustable defaults)
static const uint8_t DEBOUNCE_MS = 30;
static unsigned long lastDebounceAutoMs = 0;
static unsigned long lastDebounceManualMs = 0;
static bool lastAutoRaw = true;    // with pullup, released = HIGH
static bool lastManualRaw = true;

static bool autoPressedStable = false;
static bool manualPressedStable = false;

// Read buttons (D2 auto, D3 manual)
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButtons();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButtons() {
  bool autoRaw = digitalRead(PIN_BTN_AUTO);
  bool manualRaw = digitalRead(PIN_BTN_MANUAL);

  // Debounce auto
  if (autoRaw != lastAutoRaw) {
    lastDebounceAutoMs = millis();
    lastAutoRaw = autoRaw;
  }
  if ((millis() - lastDebounceAutoMs) > DEBOUNCE_MS) {
    autoPressedStable = (autoRaw == LOW);
  }

  // Debounce manual
  if (manualRaw != lastManualRaw) {
    lastDebounceManualMs = millis();
    lastManualRaw = manualRaw;
  }
  if ((millis() - lastDebounceManualMs) > DEBOUNCE_MS) {
    manualPressedStable = (manualRaw == LOW);
  }
}

void setup() {
  // Inputs with internal pullups: button press -> LOW
  pinMode(PIN_BTN_AUTO, INPUT_PULLUP);
  pinMode(PIN_BTN_MANUAL, INPUT_PULLUP);

  // PWM output
  pinMode(PIN_PWM_OUT, OUTPUT);

  // Start in auto mode
  mode = MODE_AUTO_GRADIENT;
  analogWrite(PIN_PWM_OUT, 0);
}

void loop() {
  updateButtons();

  // Mode selection logic:
  // - D2 pressed => auto gradient
  // - D3 pressed => manual pot control
  // - If neither pressed, keep current mode
  if (autoPressedStable && !manualPressedStable) {
    mode = MODE_AUTO_GRADIENT;
  } else if (manualPressedStable && !autoPressedStable) {
    mode = MODE_MANUAL_POT;
  } else if (autoPressedStable && manualPressedStable) {
    // If both pressed, keep current mode (avoid oscillation)
    // (This preserves behavior rather than arbitrarily selecting one.)
  }

  if (mode == MODE_AUTO_GRADIENT) {
    unsigned long now = millis();
    if (now - lastGradUpdateMs >= gradUpdateMs) {
      lastGradUpdateMs = now;

      gradValue += gradStep;

      if (gradValue >= 255) {
        gradValue = 255;
        gradStep = -gradStep;
      } else if (gradValue <= 0) {
        gradValue = 0;
        gradStep = -gradStep;
      }

      analogWrite(PIN_PWM_OUT, (uint8_t)gradValue);
    }
  } else { // MODE_MANUAL_POT
    // D3 manual mode: potentiometer controls PWM
    int raw = analogRead(PIN_ANALOG_POT); // 0..1023
    // Map directly to 0..255
    uint8_t pwm = (uint8_t)((raw * 255L) / 1023L);
    analogWrite(PIN_PWM_OUT, pwm);
  }
}
