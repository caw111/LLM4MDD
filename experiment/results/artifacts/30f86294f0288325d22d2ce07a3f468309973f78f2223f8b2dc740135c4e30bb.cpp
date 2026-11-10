#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (preserve authoritative wiring)
static const uint8_t PIN_LDR_A0     = A0;   // 光敏电阻输入
static const uint8_t PIN_PWM_LED_D9 = 9;    // PWM LED输出
static const uint8_t PIN_BUTTON_D2  = 2;    // 按钮输入
static const uint8_t PIN_LED_D13     = 13;   // D13 也保留为按需求存在（作为指示/系统存在）

// TM1637 pins: CLK=D4, DIO=D5
static const uint8_t TM1637_CLK = 4;
static const uint8_t TM1637_DIO = 5;

TM1637Display display(TM1637_CLK, TM1637_DIO);

// ======= Behavior requirements (preserved + implemented) =======
// - Automatic mode: read light strength from LDR on A0, map to LED PWM brightness,
//   and display current light mapping value on TM1637.
// - Button toggles between:
//   - auto mode (default)
//   - manual freeze mode: on button press, freeze displayed value and LED PWM,
//     maintaining them until next press.
// - Use button D2.
// - Button: assume internal pull-up (common wiring).

// ======= Calibration / thresholds / timers =======
// The prompt does not specify numeric calibration constants.
// To avoid simplifying away behavior, we implement a robust default mapping
// from analog range to an integer display value and PWM brightness.
static const uint16_t ADC_MIN_DEFAULT = 0;     // will be refined by runtime auto-calibration window
static const uint16_t ADC_MAX_DEFAULT = 1023;  // will be refined by runtime auto-calibration window

// Display value range for TM1637 (0~9999). We'll use 0~9999 mapped from ADC.
static const uint16_t DISPLAY_MIN = 0;
static const uint16_t DISPLAY_MAX = 9999;

// LED PWM range (UNO)
static const uint8_t PWM_MIN = 0;
static const uint8_t PWM_MAX = 255;

// Sampling period (keep stable updates)
static const unsigned long SAMPLE_INTERVAL_MS = 50;

// Debounce / toggle handling
static const unsigned long DEBOUNCE_INTERVAL_MS = 40;

// Auto-calibration window: gradually track observed min/max ADC values
// (helps preserve "根据光照强度自动调节" without requiring fixed thresholds).
static const unsigned long CALIBRATION_WINDOW_MS = 15000; // 15s rolling window
static unsigned long calibStartMs = 0;

// State
static bool manualFreeze = false;
static uint16_t frozenDisplayValue = 0;
static uint8_t  frozenPwmValue = 0;

static unsigned long lastSampleMs = 0;

// Button debounce
static int lastButtonReading = HIGH;
static int stableButtonState = HIGH;
static unsigned long lastDebounceMs = 0;

static uint16_t calibMin = ADC_MIN_DEFAULT;
static uint16_t calibMax = ADC_MAX_DEFAULT;

#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clampU16(uint32_t x, uint16_t lo, uint16_t hi);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void adcToDisplayAndPwm(uint16_t adc, uint16_t& outDisplay, uint8_t& outPwm);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplayNumber(uint16_t value);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonToggleEvent();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 159 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clampU16(uint32_t x, uint16_t lo, uint16_t hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return (uint16_t)x;
}

// Map ADC reading to display value and PWM.
// - adc -> display (0..9999)
// - display -> pwm (0..255), using proportional mapping.
static void adcToDisplayAndPwm(uint16_t adc, uint16_t& outDisplay, uint8_t& outPwm) {
  // Calibrated scaling
  uint16_t inMin = calibMin;
  uint16_t inMax = calibMax;
  if (inMax <= inMin) {
    inMax = inMin + 1; // avoid divide-by-zero
  }

  uint32_t adcClamped = adc;
  if (adcClamped < inMin) adcClamped = inMin;
  if (adcClamped > inMax) adcClamped = inMax;

  uint32_t scaled = (adcClamped - inMin) * (DISPLAY_MAX - DISPLAY_MIN);
  uint32_t denom = (inMax - inMin);
  uint32_t displayVal = DISPLAY_MIN + (denom ? (scaled / denom) : 0);

  outDisplay = clampU16(displayVal, DISPLAY_MIN, DISPLAY_MAX);

  // PWM proportional to display value
  uint32_t pwmScaled = (uint32_t)outDisplay * (PWM_MAX - PWM_MIN);
  uint32_t pwmVal = PWM_MIN + (DISPLAY_MAX ? (pwmScaled / DISPLAY_MAX) : 0);
  if (pwmVal > PWM_MAX) pwmVal = PWM_MAX;
  outPwm = (uint8_t)pwmVal;
}

static void updateDisplayNumber(uint16_t value) {
  // TM1637 supports up to 4 digits; value is 0..9999
  display.showNumberDec(value, false, 4, 0); // no leading zeros, 4 digits
}

static bool readButtonToggleEvent() {
  // Returns true once per press (on falling edge after debounce)
  int reading = digitalRead(PIN_BUTTON_D2);

  if (reading != lastButtonReading) {
    lastDebounceMs = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceMs) >= DEBOUNCE_INTERVAL_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;
      // With INPUT_PULLUP: pressed => LOW.
      if (stableButtonState == LOW) {
        return true;
      }
    }
  }
  return false;
}

void setup() {
  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  pinMode(PIN_BUTTON_D2, INPUT_PULLUP);

  pinMode(PIN_PWM_LED_D9, OUTPUT);
  analogWrite(PIN_PWM_LED_D9, 0);

  // TM1637 init
  display.setBrightness(0x0f); // max brightness for readability (can be adjusted if needed)

  // Calibration init
  calibStartMs = millis();
  calibMin = ADC_MIN_DEFAULT;
  calibMax = ADC_MAX_DEFAULT;

  // Default: auto mode
  manualFreeze = false;

  // Initial measurement to show immediately
  uint16_t adc = analogRead(PIN_LDR_A0);
  adcToDisplayAndPwm(adc, frozenDisplayValue, frozenPwmValue);
  updateDisplayNumber(frozenDisplayValue);
  analogWrite(PIN_PWM_LED_D9, frozenPwmValue);

  // Initialize button debounce states
  lastButtonReading = digitalRead(PIN_BUTTON_D2);
  stableButtonState = lastButtonReading;
  lastDebounceMs = millis();
}

void loop() {
  // Handle button toggle
  if (readButtonToggleEvent()) {
    manualFreeze = !manualFreeze;

    if (manualFreeze) {
      // Freeze current values: read once now to ensure accurate freeze moment
      uint16_t adc = analogRead(PIN_LDR_A0);
      // Update calibration quickly so freeze reflects current light condition
      if (adc < calibMin) calibMin = adc;
      if (adc > calibMax) calibMax = adc;

      uint16_t disp;
      uint8_t pwm;
      adcToDisplayAndPwm(adc, disp, pwm);
      frozenDisplayValue = disp;
      frozenPwmValue = pwm;

      updateDisplayNumber(frozenDisplayValue);
      analogWrite(PIN_PWM_LED_D9, frozenPwmValue);
    } else {
      // Leaving freeze: next loop tick will resume automatic updates
      // (keep LED/display in current state until next sample)
    }
  }

  unsigned long now = millis();
  if (now - lastSampleMs < SAMPLE_INTERVAL_MS) return;
  lastSampleMs = now;

  // Update system based on mode
  if (!manualFreeze) {
    uint16_t adc = analogRead(PIN_LDR_A0);

    // Rolling calibration window: adapt min/max to current environment
    if (now - calibStartMs >= CALIBRATION_WINDOW_MS) {
      calibStartMs = now;
      calibMin = ADC_MIN_DEFAULT;
      calibMax = ADC_MAX_DEFAULT;
    }

    if (adc < calibMin) calibMin = adc;
    if (adc > calibMax) calibMax = adc;

    uint16_t disp;
    uint8_t pwm;
    adcToDisplayAndPwm(adc, disp, pwm);

    updateDisplayNumber(disp);
    analogWrite(PIN_PWM_LED_D9, pwm);

    frozenDisplayValue = disp;
    frozenPwmValue = pwm;
  } else {
    // Manual freeze mode: keep frozen values constant
    updateDisplayNumber(frozenDisplayValue);
    analogWrite(PIN_PWM_LED_D9, frozenPwmValue);
  }

  // Optional status indicator on D13
  digitalWrite(PIN_LED_D13, manualFreeze ? HIGH : LOW);
}
