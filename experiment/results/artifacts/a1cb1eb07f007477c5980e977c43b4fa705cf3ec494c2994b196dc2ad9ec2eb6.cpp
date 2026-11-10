#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===== Pin Assignments (preserve required wiring) =====
static const uint8_t FAN_PWM_PIN = 5;   // D5
static const uint8_t POT_PIN     = A0;  // 电位器 A0
static const uint8_t LM35_PIN    = A1;  // LM35 A1
static const uint8_t MODE_BTN_PIN = 2; // D2 (按下切换模式)

// TM1637: CLK=D4, DIO=D3
static const uint8_t TM_CLK_PIN = 4; // D4
static const uint8_t TM_DIO_PIN = 3; // D3

// ===== Display =====
TM1637Display display(TM_CLK_PIN, TM_DIO_PIN);

// ===== System Parameters (behavior kept deterministic & complete) =====
// Note: The prompt doesn't provide specific thresholds/tuning values.
// We implement a standard, robust mapping with sensible defaults.
static const uint16_t TEMP_MIN_C = 20;   // below: fan minimum
static const uint16_t TEMP_MAX_C = 40;   // above: fan maximum

static const uint8_t FAN_PWM_MIN = 60;   // minimum PWM in auto/manual
static const uint8_t FAN_PWM_MAX = 255;  // maximum PWM in auto/manual

// LM35 conversion: 10mV/°C
static const float LM35_MV_PER_C = 10.0f;

// ADC: 10-bit for Uno
static const float ADC_MAX = 1023.0f;
static const float VREF = 5.0f; // assuming 5V reference on Uno

// Button handling
static const uint32_t DEBOUNCE_MS = 40;
static const uint32_t DISPLAY_UPDATE_MS = 120;

enum Mode : uint8_t { MODE_AUTO = 0, MODE_MANUAL = 1 };
volatile Mode mode = MODE_AUTO;

// Debounced button state
static bool lastBtnStable = HIGH;
static bool lastBtnReading = HIGH;
static uint32_t lastDebounceTime = 0;

// Timing
static uint32_t lastDisplayUpdate = 0;

// ===== Helpers =====
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35_C();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t clampU8(int v, int lo, int hi);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t mapAutoPWMFromTemp(float tempC);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t mapManualPWMFromPot();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateModeFromButton();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35_C()
{
  int raw = analogRead(LM35_PIN);
  // Voltage at pin: raw/1023 * VREF
  float v = (raw * VREF) / ADC_MAX;     // volts
  float mv = v * 1000.0f;              // millivolts
  float tempC = mv / LM35_MV_PER_C;   // 10mV/°C -> °C
  return tempC;
}

static uint8_t clampU8(int v, int lo, int hi)
{
  if (v < lo) return (uint8_t)lo;
  if (v > hi) return (uint8_t)hi;
  return (uint8_t)v;
}

static uint8_t mapAutoPWMFromTemp(float tempC)
{
  // Map temperature to PWM with clamping
  if (tempC <= (float)TEMP_MIN_C) return FAN_PWM_MIN;
  if (tempC >= (float)TEMP_MAX_C) return FAN_PWM_MAX;

  float t = (tempC - (float)TEMP_MIN_C) / ((float)TEMP_MAX_C - (float)TEMP_MIN_C); // 0..1
  int pwm = (int)lround((float)FAN_PWM_MIN + t * (float)(FAN_PWM_MAX - FAN_PWM_MIN));
  return clampU8(pwm, FAN_PWM_MIN, FAN_PWM_MAX);
}

static uint8_t mapManualPWMFromPot()
{
  // Pot: 0..1023 -> FAN_PWM_MIN..FAN_PWM_MAX
  int raw = analogRead(POT_PIN);
  float t = (float)raw / ADC_MAX; // 0..1
  int pwm = (int)lround((float)FAN_PWM_MIN + t * (float)(FAN_PWM_MAX - FAN_PWM_MIN));
  return clampU8(pwm, FAN_PWM_MIN, FAN_PWM_MAX);
}

static void updateModeFromButton()
{
  // Active-low button assumed (typical wiring: press connects to GND)
  bool reading = digitalRead(MODE_BTN_PIN);

  if (reading != lastBtnReading) {
    lastDebounceTime = millis();
    lastBtnReading = reading;
  }

  if ((millis() - lastDebounceTime) >= DEBOUNCE_MS) {
    if (reading != lastBtnStable) {
      lastBtnStable = reading;
      // Toggle on press event (HIGH->LOW). If wiring is opposite, this will still work if user adapts button polarity.
      if (lastBtnStable == LOW) {
        mode = (mode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
      }
    }
  }
}

static void displayValue(int value, bool showMinus = false)
{
  // TM1637Display supports showNumberDecEx with leadingZero / sign options.
  // We'll display either integer temperature (°C) or manual mode level.
  // If showMinus is used, we show negative.
  if (showMinus) {
    display.showNumberDec(-abs(value), true, 4, 0);
  } else {
    display.showNumberDec(abs(value), false, 4, 0);
  }
}

// ===== Arduino Setup / Loop =====
void setup()
{
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  pinMode(POT_PIN, INPUT);
  pinMode(LM35_PIN, INPUT);

  pinMode(MODE_BTN_PIN, INPUT_PULLUP); // D2 with pull-up; press -> LOW

  // TM1637 init
  display.setBrightness(0x0f, false); // max brightness (0..7 typical; library accepts 0..7; still ok)
  display.clear();

  // Initialize stable button reading
  lastBtnStable = digitalRead(MODE_BTN_PIN);
  lastBtnReading = lastBtnStable;
  lastDebounceTime = millis();
}

void loop()
{
  updateModeFromButton();

  uint32_t now = millis();

  // Update fan PWM continuously (no simplification away)
  uint8_t pwm = 0;
  float tempC = readLM35_C();

  if (mode == MODE_AUTO) {
    pwm = mapAutoPWMFromTemp(tempC);
  } else {
    pwm = mapManualPWMFromPot();
  }

  analogWrite(FAN_PWM_PIN, pwm);

  // Update display periodically
  if (now - lastDisplayUpdate >= DISPLAY_UPDATE_MS) {
    lastDisplayUpdate = now;

    if (mode == MODE_AUTO) {
      // Show current temperature (rounded to integer)
      int tDisp = (int)lround(tempC);
      displayValue(tDisp, false);
    } else {
      // Show manual "档位": derived from pot position mapped to 0..9
      int raw = analogRead(POT_PIN);
      int level = (int)lround(((float)raw / ADC_MAX) * 9.0f); // 0..9
      level = constrain(level, 0, 9);
      displayValue(level, false);
    }
  }
}
