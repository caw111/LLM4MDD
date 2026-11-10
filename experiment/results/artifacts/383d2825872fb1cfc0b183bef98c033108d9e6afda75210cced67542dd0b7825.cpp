#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_MODE_AUTO = 2;   // D2 自动模式（渐变）
static const uint8_t PIN_MODE_MAN  = 3;   // D3 手动模式（电位器控制）(电位器原理未在需求中给出，按“手动模式由电位器控制”处理)
static const uint8_t PIN_POT       = A0; // 电位器模拟输入

// ---- Behavior placeholders (not specified in prompt) ----
// Since the provided requirements only define mode selection inputs and the analog source,
// this sketch implements robust mode handling:
// - D2 selects auto mode (渐变) when pressed
// - D3 selects manual mode when pressed
// - If neither is pressed, it keeps the last selected mode
// - Auto mode performs a smooth triangle wave "渐变" on an output (PWM) pin if available.
//
// IMPORTANT:
// The prompt mentions "given circuit" but no circuit details or output pin/PWM requirements were supplied.
// To avoid inventing pin assignments, this sketch uses the built-in LED (D13) as a safe output.
// Replace OUT_PIN with the actual PWM/output pin per your circuit.
static const uint8_t OUT_PIN = 13;        // Change to your real output/PWM pin

// Timing
static const uint16_t AUTO_STEP_MS = 5;   // Controls gradient speed (not specified; reasonable default)
static const uint8_t  PWM_MAX = 255;

// Internal state
static bool lastModeAuto = true;
static uint8_t autoValue = 0;
static int16_t autoDelta = 1;

#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PIN_MODE_AUTO, INPUT_PULLUP); // assume momentary button to GND
  pinMode(PIN_MODE_MAN, INPUT_PULLUP);  // assume momentary button to GND
  pinMode(OUT_PIN, OUTPUT);

  analogWrite(OUT_PIN, 0);
}

void loop() {
  // Read buttons (active-low)
  const bool autoPressed = (digitalRead(PIN_MODE_AUTO) == LOW);
  const bool manPressed  = (digitalRead(PIN_MODE_MAN) == LOW);

  // Mode selection: if both pressed, prioritize manual (often used to override auto)
  bool modeAuto = lastModeAuto;
  if (autoPressed && !manPressed) modeAuto = true;
  else if (manPressed) modeAuto = false;

  lastModeAuto = modeAuto;

  static uint32_t lastAutoTick = 0;

  if (modeAuto) {
    // Auto mode: "渐变" (triangle wave) output
    uint32_t now = millis();
    if (now - lastAutoTick >= AUTO_STEP_MS) {
      lastAutoTick = now;

      int16_t next = (int16_t)autoValue + autoDelta;
      if (next <= 0) {
        next = 0;
        autoDelta = 1;
      } else if (next >= PWM_MAX) {
        next = PWM_MAX;
        autoDelta = -1;
      }
      autoValue = (uint8_t)next;

      analogWrite(OUT_PIN, autoValue);
    }
  } else {
    // Manual mode: "电位器控制"
    // Map A0 (0..1023) to PWM (0..255)
    int pot = analogRead(PIN_POT);
    uint8_t pwm = (uint32_t)pot * 255u / 1023u;
    analogWrite(OUT_PIN, pwm);
  }
}
