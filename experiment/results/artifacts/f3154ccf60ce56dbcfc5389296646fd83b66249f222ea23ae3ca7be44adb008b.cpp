#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===== Pins (per requirements / authoritative mapping) =====
static const uint8_t PIN_BTN_D2 = 2;  // D2: mode switch
static const uint8_t PIN_BTN_D3 = 3;  // D3: parameter adjust
static const uint8_t PIN_LED_PWM = 9;  // D9: PWM LED
static const uint8_t PIN_TM1637_CLK = 4; // D4: TM1637 CLK
static const uint8_t PIN_TM1637_DIO = 5; // D5: TM1637 DIO
static const uint8_t PIN_LED_BUILTIN = 13; // (kept per inventory)

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== Modes =====
enum Mode : uint8_t {
  MODE_OFF = 0,    // 常灭
  MODE_ON = 1,     // 常亮
  MODE_BREATH = 2  // 呼吸灯
};

static Mode currentMode = MODE_OFF;

// ===== Parameters =====
// D3调整当前模式参数：
// 常亮亮度(0..255): brightness
// 呼吸速度(1..10) -> 用于决定呼吸周期(越大越快)
static uint8_t onBrightness = 180; // 初始亮度
static uint8_t breathSpeed = 5;    // 初始呼吸速度等级

// ===== Display behavior =====
// 显示当前模式编号或参数值：
// - 模式切换时显示模式编号(0/1/2)
// - 参数调整时显示参数值(常亮:亮度0-255；呼吸:速度1-10)
// - 不持续闪烁：在参数调整后的显示保持一段时间后回到模式编号
static uint32_t lastChangeMs = 0;
static uint32_t lastParamDisplayMs = 0;
static bool showParamNow = false;

// ===== Button debouncing / press detection =====
struct ButtonState {
  uint8_t pin;
  bool stableLevel;           // stable reading
  bool lastStableLevel;
  uint32_t lastDebounceMs;
  bool pressed;              // edge flag
};

static ButtonState btnMode{PIN_BTN_D2, HIGH, HIGH, 0, false};
static ButtonState btnParam{PIN_BTN_D3, HIGH, HIGH, 0, false};

// Read button: active LOW assumed (pullup)
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonRaw(uint8_t pin);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButton(ButtonState &b);
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void consumePressed(ButtonState &b);
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t breathPeriodMs();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t breathWave(uint8_t x);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyLedByMode();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showNumber(uint8_t value);
#line 162 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 189 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void adjustParam();
#line 208 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void nextMode();
#line 217 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 247 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline bool readButtonRaw(uint8_t pin) {
  return digitalRead(pin);
}

static void updateButton(ButtonState &b) {
  bool raw = readButtonRaw(b.pin);
  if (raw != b.stableLevel) {
    b.lastDebounceMs = millis();
    b.stableLevel = raw;
  }
  // Ensure debounce window (simple, but ensures stableLevel changes don't chatter)
  if ((millis() - b.lastDebounceMs) >= 20) {
    if (b.stableLevel != b.lastStableLevel) {
      b.lastStableLevel = b.stableLevel;
      // pressed when stable level goes LOW (active-low)
      b.pressed = (b.stableLevel == LOW);
    } else {
      // no new edge
      b.pressed = false;
    }
  } else {
    // within debounce window, don't emit events
    b.pressed = false;
  }
}

static void consumePressed(ButtonState &b) {
  b.pressed = false;
}

// ===== Breathing control =====
static uint8_t breathPhase = 0;
static uint32_t breathLastStepMs = 0;

// Breathing period mapping:
// breathSpeed: 1..10 => period(ms) roughly 2000..700 (faster with higher speed)
static uint32_t breathPeriodMs() {
  uint8_t s = breathSpeed;
  if (s < 1) s = 1;
  if (s > 10) s = 10;
  // Map: s=1 => 2000, s=10 => 700
  uint32_t period = 2000UL - (uint32_t)( (s - 1) * (1300UL / 9) );
  if (period < 700) period = 700;
  return period;
}

// Nonlinear-ish breathing using sinusoidal LUT (8-bit)
static const uint8_t breathLUT[256] = {
  // Generated-ish values: 0..255 sinusoid shifted to [0..255]
  // To keep code compact in source, we generate at runtime instead of listing 256 values.
};

static uint8_t breathWave(uint8_t x) {
  // x: 0..255
  // Use integer approximation of sin. Arduino has sin() but float is heavier.
  // We'll still compute with sinf for correctness; this sketch prioritizes behavior over size.
  // x mapped to angle 0..2pi
  float a = (2.0f * PI * (float)x) / 255.0f;
  float s = (sinf(a) + 1.0f) * 0.5f; // 0..1
  int v = (int)(s * 255.0f + 0.5f);
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return (uint8_t)v;
}

static void applyLedByMode() {
  switch (currentMode) {
    case MODE_OFF:
      analogWrite(PIN_LED_PWM, 0);
      break;
    case MODE_ON:
      analogWrite(PIN_LED_PWM, onBrightness);
      break;
    case MODE_BREATH: {
      // Update phase based on speed and time
      uint32_t now = millis();
      uint32_t period = breathPeriodMs();

      // We want full cycle in period. Let phaseStep = period/255 per increment.
      // Compute step interval; avoid divide-by-zero.
      uint32_t stepInterval = period / 255UL;
      if (stepInterval < 1) stepInterval = 1;

      if ((now - breathLastStepMs) >= stepInterval) {
        breathLastStepMs = now;
        breathPhase++;
      }

      uint8_t wave = breathWave(breathPhase);
      // Scale wave by brightness ceiling: keep breath max at onBrightness for consistency
      uint16_t scaled = (uint16_t)wave * (uint16_t)onBrightness; // 0..onBrightness*255
      uint8_t out = (uint8_t)(scaled / 255U);
      analogWrite(PIN_LED_PWM, out);
      break;
    }
  }
}

// ===== Display =====
static void showNumber(uint8_t value) {
  // Display expects 0..9999. We'll show 0..255 and 1..10 fine.
  // Use 4 digits. We'll clear first to avoid leftover segments.
  display.showNumberDec(value, false, 4, 0);
}

// For "模式切换显示模式编号": show 0/1/2 for current mode.
// For parameter display: show parameter values.
// Keep for a short time after parameter change, then return to mode number.
static const uint32_t PARAM_DISPLAY_HOLD_MS = 1500;

static void updateDisplay() {
  uint32_t now = millis();

  bool shouldShowParam = showParamNow;
  if (showParamNow && (now - lastParamDisplayMs) > PARAM_DISPLAY_HOLD_MS) {
    shouldShowParam = false;
    showParamNow = false;
  }

  if (shouldShowParam) {
    if (currentMode == MODE_ON) {
      showNumber(onBrightness); // 0..255
    } else if (currentMode == MODE_BREATH) {
      showNumber(breathSpeed); // 1..10
    } else {
      // In MODE_OFF, no meaningful parameter; show mode number
      showNumber((uint8_t)currentMode);
    }
  } else {
    showNumber((uint8_t)currentMode);
  }
}

// ===== Parameter adjust behavior =====
// D3用于调整当前模式参数
// - 常亮: 亮度以步进调整，范围0..255循环
// - 呼吸: 速度等级步进，范围1..10循环
static void adjustParam() {
  if (currentMode == MODE_ON) {
    // Step 10 for responsiveness; wrap 0..255
    const uint8_t step = 10;
    if (onBrightness + step <= 255) onBrightness += step;
    else onBrightness = 0; // wrap
  } else if (currentMode == MODE_BREATH) {
    // Step 1, wrap 1..10
    if (breathSpeed < 10) breathSpeed++;
    else breathSpeed = 1;
  } else {
    // MODE_OFF: no parameter; keep OFF
  }

  showParamNow = true;
  lastParamDisplayMs = millis();
}

// Mode switch behavior on D2 press
static void nextMode() {
  // 0->1->2->0
  currentMode = (Mode)(((uint8_t)currentMode + 1) % 3);
  lastChangeMs = millis();

  // when mode changes, don't force parameter display; show mode number
  showParamNow = false;
}

void setup() {
  pinMode(PIN_BTN_D2, INPUT_PULLUP);
  pinMode(PIN_BTN_D3, INPUT_PULLUP);

  pinMode(PIN_LED_PWM, OUTPUT);
  pinMode(PIN_LED_BUILTIN, OUTPUT);

  // Initial states for button debounce
  btnMode.stableLevel = readButtonRaw(btnMode.pin);
  btnMode.lastStableLevel = btnMode.stableLevel;
  btnMode.lastDebounceMs = millis();
  btnMode.pressed = false;

  btnParam.stableLevel = readButtonRaw(btnParam.pin);
  btnParam.lastStableLevel = btnParam.stableLevel;
  btnParam.lastDebounceMs = millis();
  btnParam.pressed = false;

  // TM1637 init
  display.setBrightness(0x0f); // reasonable default (0..7 usually; TM1637 libs differ; 0x0f is accepted by many libs)
  display.clear();

  // Initialize LED
  analogWrite(PIN_LED_PWM, 0);
  digitalWrite(PIN_LED_BUILTIN, LOW);

  // Show initial mode number
  showNumber((uint8_t)currentMode);
}

void loop() {
  // Update button states
  updateButton(btnMode);
  updateButton(btnParam);

  // Handle mode button press
  if (btnMode.pressed) {
    consumePressed(btnMode);
    nextMode();
  }

  // Handle parameter button press
  if (btnParam.pressed) {
    consumePressed(btnParam);
    adjustParam();
  }

  // Apply LED effect by current mode
  applyLedByMode();

  // Optional: builtin LED as activity indicator (preserve wiring role)
  // Keep it synced with PWM output activity: ON when mode != OFF.
  digitalWrite(PIN_LED_BUILTIN, (currentMode != MODE_OFF) ? HIGH : LOW);

  // Update display
  updateDisplay();
}
