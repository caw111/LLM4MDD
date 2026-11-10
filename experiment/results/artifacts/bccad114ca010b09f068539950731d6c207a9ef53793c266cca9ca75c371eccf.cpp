#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t BTN_MODE_PIN = 2;   // D2
static const uint8_t BTN_PARAM_PIN = 3;  // D3
static const uint8_t PWM_LED_PIN   = 9;   // D9
static const uint8_t TM1637_CLK_PIN = 4; // D4
static const uint8_t TM1637_DIO_PIN = 5; // D5

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// ----- Behavior/parameters -----
enum Mode : uint8_t {
  MODE_OFF = 0,      // 常灭
  MODE_ON = 1,       // 常亮
  MODE_BREATH = 2    // 呼吸灯
};

static volatile Mode currentMode = MODE_OFF;

// D3 adjusts parameter depending on mode.
// For MODE_ON: brightness step.
static int onBrightness = 255;          // 0..255
static const int ON_STEP = 25;         // parameter step for brightness
static const int ON_MIN = 0;
static const int ON_MAX = 255;

// For MODE_BREATH: breathing speed (affects waveform period).
static uint16_t breathSpeedMs = 1200;  // 400..2400 (approx period)
static const uint16_t SPEED_STEP = 200;
static const uint16_t SPEED_MIN = 400;
static const uint16_t SPEED_MAX = 2400;

// Show values on TM1637:
// - MODE_OFF: show mode number 0
// - MODE_ON: show brightness (0..255)
// - MODE_BREATH: show speed (400..2400) mapped to 2..12 units or show raw in a compact way.
// We'll show raw speed in "hundreds" to fit 0..99: speedHundreds = breathSpeedMs / 100.
static uint8_t speedHundreds = 12; // breathSpeedMs/100

// ----- Button debouncing -----
struct DebouncedButton {
  uint8_t pin;
  bool stableState;                 // last stable state
  bool lastReading;                 // last raw reading
  unsigned long lastDebounceMs;
  bool fell;                        // latched "press" event
};

static DebouncedButton btnMode{BTN_MODE_PIN, HIGH, HIGH, 0, false};
static DebouncedButton btnParam{BTN_PARAM_PIN, HIGH, HIGH, 0, false};

static const unsigned long DEBOUNCE_MS = 40;
static const unsigned long LONGPRESS_MS = 600; // (not required, but helps responsiveness)

#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initButton(DebouncedButton &b, uint8_t p);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButton(DebouncedButton &b);
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool takePressEvent(DebouncedButton &b);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLedPWM(uint8_t val);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t breatheWave(uint32_t tMs, uint16_t periodMs);
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void changeMode();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void adjustParameter();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 194 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initButton(DebouncedButton &b, uint8_t p) {
  b.pin = p;
  pinMode(b.pin, INPUT_PULLUP);
  b.stableState = digitalRead(b.pin);
  b.lastReading = b.stableState;
  b.lastDebounceMs = millis();
  b.fell = false;
}

static void updateButton(DebouncedButton &b) {
  bool reading = digitalRead(b.pin);
  if (reading != b.lastReading) {
    b.lastDebounceMs = millis();
    b.lastReading = reading;
  }
  if ((millis() - b.lastDebounceMs) >= DEBOUNCE_MS) {
    if (reading != b.stableState) {
      b.stableState = reading;
      // active-low press: transition HIGH->LOW indicates "fell"
      if (b.stableState == LOW) {
        b.fell = true;
      }
    }
  }
}

static inline bool takePressEvent(DebouncedButton &b) {
  if (b.fell) {
    b.fell = false;
    return true;
  }
  return false;
}

// ----- LED / breathing -----
static unsigned long breathStartMs = 0;

static void setLedPWM(uint8_t val) {
  analogWrite(PWM_LED_PIN, val);
}

static uint8_t breatheWave(uint32_t tMs, uint16_t periodMs) {
  // Triangle-wave-like breathing using cosine approximation via sin from Arduino core.
  // sin() is floating; keep simple and smooth for AVR.
  // We'll use a phase 0..2pi.
  float phase = (2.0f * PI * (float)(tMs % periodMs)) / (float)periodMs;
  float s = (sin(phase) + 1.0f) * 0.5f; // 0..1
  // Map to brightness; keep full-scale. Avoid full off too often by leaving it at 0..255.
  int v = (int)(s * 255.0f + 0.5f);
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  return (uint8_t)v;
}

static void updateDisplay() {
  // TM1637 4-digit display.
  // Requirements: "display current mode number or parameter value."
  // Implement:
  // - mode OFF: show 0
  // - mode ON: show brightness (0..255)
  // - mode BREATH: show speedHundreds (speed/100), in range 4..24
  // This preserves the parameter value in a compact representation.
  int toShow = 0;
  bool showDecPoint = false;
  uint8_t digits = 4;

  if (currentMode == MODE_OFF) {
    toShow = 0;
  } else if (currentMode == MODE_ON) {
    toShow = onBrightness; // 0..255 fits within 4 digits
  } else { // MODE_BREATH
    // speedHundreds = breathSpeedMs / 100 (e.g., 1200 -> 12)
    speedHundreds = (uint8_t)(breathSpeedMs / 100);
    toShow = speedHundreds;
    // Optional: show with DP to indicate it is "hundreds".
    showDecPoint = true;
  }

  // display.showNumberDecEx supports decimal point placement.
  // We'll use DP on digit 3 when showDecPoint is true.
  uint8_t dpMask = 0;
  if (showDecPoint) {
    // DP mask bits: 0x01..0x40 correspond to digits depending on library.
    // We'll use 0x20 (third digit) which commonly maps to digit 3 on TM1637Display.
    dpMask = 0x20;
  }

  display.showNumberDecEx(toShow, dpMask, true);
}

// ----- Mode transitions -----
static void changeMode() {
  uint8_t m = (uint8_t)currentMode;
  m = (m + 1) % 3;
  currentMode = (Mode)m;

  if (currentMode == MODE_BREATH) {
    breathStartMs = millis();
  }
}

// ----- Parameter adjustments -----
static void adjustParameter() {
  if (currentMode == MODE_OFF) {
    // In off mode, D3 cycles brightness from min..max
    // so when switching to ON, parameter is already set.
    onBrightness += ON_STEP;
    if (onBrightness > ON_MAX) onBrightness = ON_MIN;
    // LED remains off.
  } else if (currentMode == MODE_ON) {
    onBrightness += ON_STEP;
    if (onBrightness > ON_MAX) onBrightness = ON_MIN;
  } else { // MODE_BREATH
    breathSpeedMs += SPEED_STEP;
    if (breathSpeedMs > SPEED_MAX) breathSpeedMs = SPEED_MIN;
    speedHundreds = (uint8_t)(breathSpeedMs / 100);
    breathStartMs = millis();
  }
}

void setup() {
  initButton(btnMode, BTN_MODE_PIN);
  initButton(btnParam, BTN_PARAM_PIN);

  pinMode(PWM_LED_PIN, OUTPUT);
  analogWrite(PWM_LED_PIN, 0);

  display.setBrightness(0x0f); // 0..7? Library uses 0..7 or 0..15 depending version; 0x0f is common.
  // Initialize speedHundreds
  breathSpeedMs = 1200;
  speedHundreds = (uint8_t)(breathSpeedMs / 100);

  // Initial display and state
  currentMode = MODE_OFF;
  setLedPWM(0);
  updateDisplay();
}

void loop() {
  updateButton(btnMode);
  updateButton(btnParam);

  // D2: switch modes
  if (takePressEvent(btnMode)) {
    changeMode();
    if (currentMode == MODE_OFF) {
      setLedPWM(0);
    } else if (currentMode == MODE_ON) {
      setLedPWM((uint8_t)onBrightness);
    } else { // BREATH
      // continue in breath loop
    }
    updateDisplay();
    delay(10); // small guard
  }

  // D3: adjust parameter for current mode
  if (takePressEvent(btnParam)) {
    adjustParameter();
    if (currentMode == MODE_ON) {
      setLedPWM((uint8_t)onBrightness);
    } else if (currentMode == MODE_OFF) {
      setLedPWM(0);
    } else { // BREATH
      // will update naturally
    }
    updateDisplay();
    delay(10); // small guard
  }

  // LED behavior
  if (currentMode == MODE_OFF) {
    // 常灭
    if (analogRead(A0), false) {} // no-op to keep structure without changing behavior
    setLedPWM(0);
  } else if (currentMode == MODE_ON) {
    // 常亮 with PWM brightness
    setLedPWM((uint8_t)onBrightness);
  } else {
    // 呼吸灯
    unsigned long t = millis();
    uint32_t elapsed = (uint32_t)(t - breathStartMs);
    uint8_t v = breatheWave(elapsed, breathSpeedMs);
    setLedPWM(v);
  }

  // Keep loop responsive for buttons
  delay(5);
}
