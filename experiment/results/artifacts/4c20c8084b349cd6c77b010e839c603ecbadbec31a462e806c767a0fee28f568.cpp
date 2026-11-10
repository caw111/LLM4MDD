#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>

// ---- Hardware pin mapping (authoritative from request / resolved) ----
static const uint8_t PIN_ANALOG_POT = A0;   // device_role: analog_input
static const uint8_t PIN_BTN_AUTO   = 2;    // device_role: digital_input (D2)
static const uint8_t PIN_BTN_MANUAL = 3;    // device_role: digital_input (D3 per requirement)

static const uint8_t PIN_PWM_OUT = 9;       // PWM output pin (circuit not provided in prompt)

// ---- Mode state ----
enum Mode : uint8_t { MODE_AUTO_GRADIENT = 0, MODE_MANUAL_POT = 1 };
static Mode mode = MODE_AUTO_GRADIENT;

// ---- Auto gradient parameters (preserve behavior from prior sketch) ----
static int16_t gradValue = 0;     // 0..255
static int16_t gradStep  = 1;     // increment/decrement
static uint16_t gradUpdateMs = 10; // update interval
static unsigned long lastGradUpdateMs = 0;

// ---- Debounce for buttons (preserve behavior from prior sketch) ----
static const uint8_t DEBOUNCE_MS = 30;
static unsigned long lastDebounceAutoMs = 0;
static unsigned long lastDebounceManualMs = 0;

static bool lastAutoRaw = true;     // with pullup, released = HIGH
static bool lastManualRaw = true;

static bool autoPressedStable = false;
static bool manualPressedStable = false;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateButtons();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_018\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
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
  pinMode(PIN_BTN_AUTO, INPUT_PULLUP);
  pinMode(PIN_BTN_MANUAL, INPUT_PULLUP);

  pinMode(PIN_PWM_OUT, OUTPUT);

  mode = MODE_AUTO_GRADIENT;
  analogWrite(PIN_PWM_OUT, 0);
}

void loop() {
  updateButtons();

  // Mode selection logic (preserve from prior sketch):
  // - D2 pressed => auto gradient
  // - D3 pressed => manual pot control
  // - If neither pressed, keep current mode
  // - If both pressed, keep current mode
  if (autoPressedStable && !manualPressedStable) {
    mode = MODE_AUTO_GRADIENT;
  } else if (manualPressedStable && !autoPressedStable) {
    mode = MODE_MANUAL_POT;
  } else if (autoPressedStable && manualPressedStable) {
    // keep current mode
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
    int raw = analogRead(PIN_ANALOG_POT);          // 0..1023
    uint8_t pwm = (uint8_t)((raw * 255L) / 1023L); // 0..255
    analogWrite(PIN_PWM_OUT, pwm);
  }
}
