#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin assignments (as specified by requirement) --------------------
// NOTE: Use Arduino numeric pin numbers because D2/D4/D5/D6/D10/D13 style aliases may not exist.
static const uint8_t PIN_BUTTON      = 2;   // D2
static const uint8_t PIN_PUMP_RELAY  = 10;  // D10
static const uint8_t PIN_LED          = 13;  // D13
static const uint8_t PIN_BUZZER       = 6;   // D6 (tone output)
static const uint8_t PIN_TM1637_CLK   = 4;   // D4
static const uint8_t PIN_TM1637_DIO   = 5;   // D5

static const uint8_t PIN_SOIL   = A0; // soil moisture sensor
static const uint8_t PIN_RAIN   = A1; // rain sensor
static const uint8_t PIN_WATER  = A2; // water level sensor

// -------------------- TM1637 setup --------------------
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- Thresholds / behavior --------------------
// Values are defaults because prompt did not provide calibration/thresholds.
static const int SOIL_DRY_THRESHOLD = 450;          // below => dry
static const int WATER_SUFFICIENT_THRESHOLD = 450;  // above => enough water
static const int RAIN_DETECTED_THRESHOLD = 450;     // below => "rain detected" (depends on module)

// -------------------- Timing --------------------
static const unsigned long MANUAL_PUMP_SECONDS = 5UL;
static const unsigned long BUZZ_ALARM_TOGGLE_MS = 250UL; // buzzer toggling during alarm
static const unsigned long DISPLAY_UPDATE_MS = 200UL;

// -------------------- State --------------------
enum Mode : uint8_t { AUTO_MODE = 0, MANUAL_MODE = 1 };
static Mode currentMode = AUTO_MODE;

static bool pumpRunning = false;
static unsigned long pumpStartMs = 0;

static bool alarmActive = false;
static unsigned long lastBuzzToggleMs = 0;
static bool buzzOn = false;

static unsigned long lastDisplayMs = 0;
static uint8_t displayPage = 0;

// -------------------- Button handling (debounce + edge) --------------------
static bool lastButtonStable = HIGH; // INPUT_PULLUP -> idle HIGH
static unsigned long lastDebounceMs = 0;

#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setPump(bool on);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void startAlarm();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void stopAlarm();
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateAlarm();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isSoilDry();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isRainDetected();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isWaterSufficient();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplay();
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void forceManualPump5s();
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updatePumpStateAuto();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updatePumpStateManual();
#line 177 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 198 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline void setPump(bool on) {
  pumpRunning = on;
  digitalWrite(PIN_PUMP_RELAY, on ? HIGH : LOW);
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static inline void startAlarm() {
  alarmActive = true;
}

static inline void stopAlarm() {
  alarmActive = false;
  noTone(PIN_BUZZER);
  buzzOn = false;
  digitalWrite(PIN_LED, pumpRunning ? HIGH : LOW);
}

static void updateAlarm() {
  if (!alarmActive) {
    if (buzzOn) {
      buzzOn = false;
      noTone(PIN_BUZZER);
    }
    return;
  }

  unsigned long now = millis();
  if (now - lastBuzzToggleMs >= BUZZ_ALARM_TOGGLE_MS) {
    lastBuzzToggleMs = now;
    buzzOn = !buzzOn;
    if (buzzOn) tone(PIN_BUZZER, 2000);
    else noTone(PIN_BUZZER);
  }
}

static int readAnalogSmooth(uint8_t pin, uint8_t samples = 8) {
  long sum = 0;
  for (uint8_t i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(200);
  }
  return (int)(sum / samples);
}

static bool isSoilDry() {
  int v = readAnalogSmooth(PIN_SOIL);
  return (v < SOIL_DRY_THRESHOLD);
}

static bool isRainDetected() {
  int v = readAnalogSmooth(PIN_RAIN);
  // Default assumption: rain sensor outputs lower when wet/rain detected.
  return (v < RAIN_DETECTED_THRESHOLD);
}

static bool isWaterSufficient() {
  int v = readAnalogSmooth(PIN_WATER);
  return (v >= WATER_SUFFICIENT_THRESHOLD);
}

static void showNumberOnDisplay(int value, bool dot = false) {
  if (value < 0) value = 0;
  if (value > 9999) value = 9999;
  display.showNumberDecEx(value, dot ? 0x40 : 0x00, true);
}

static void updateDisplay() {
  unsigned long now = millis();
  if (now - lastDisplayMs < DISPLAY_UPDATE_MS) return;
  lastDisplayMs = now;

  // Pages: 0 soil, 1 water, 2 mode/status
  displayPage = (displayPage + 1) % 3;

  if (displayPage == 0) {
    int soilVal = readAnalogSmooth(PIN_SOIL);
    showNumberOnDisplay(soilVal, false);
  } else if (displayPage == 1) {
    int waterVal = readAnalogSmooth(PIN_WATER);
    showNumberOnDisplay(waterVal, false);
  } else {
    uint8_t statusBits = 0;
    if (pumpRunning) statusBits |= 0x01;
    if (alarmActive) statusBits |= 0x02;

    int modeVal = (currentMode == AUTO_MODE) ? 1000 : 2000;
    int val = modeVal + statusBits; // 1000..1003 or 2000..2003
    showNumberOnDisplay(val, pumpRunning);
  }
}

static void forceManualPump5s() {
  pumpStartMs = millis();
  setPump(true);
  stopAlarm();
}

static void updatePumpStateAuto() {
  bool dry = isSoilDry();
  bool rain = isRainDetected();
  bool waterOk = isWaterSufficient();

  if (dry && !rain) {
    if (waterOk) {
      stopAlarm();
      if (!pumpRunning) setPump(true);
    } else {
      // Water insufficient => forbid start and alarm
      if (pumpRunning) setPump(false);
      startAlarm();
    }
  } else {
    // Conditions not met => stop pump; alarm off
    if (pumpRunning) setPump(false);
    stopAlarm();
  }
}

static void updatePumpStateManual() {
  // In manual mode: pump only ends by 5s timer; auto conditions should not control it.
  if (!pumpRunning) return;
  unsigned long now = millis();
  if (now - pumpStartMs >= (MANUAL_PUMP_SECONDS * 1000UL)) {
    setPump(false);
  }
}

// -------------------- Arduino required functions --------------------
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pinMode(PIN_PUMP_RELAY, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_PUMP_RELAY, LOW);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  display.setBrightness(0x0f, true);
  display.clear();

  lastButtonStable = digitalRead(PIN_BUTTON);
  lastDebounceMs = millis();

  stopAlarm();
  setPump(false);
}

void loop() {
  // ---- Button debounce ----
  bool reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonStable) {
    lastDebounceMs = millis();
    lastButtonStable = reading;
  }

  static bool prevStablePressed = false;
  bool stablePressed = (lastButtonStable == LOW);

  if (stablePressed && !prevStablePressed) {
    // Press edge
    if (currentMode == AUTO_MODE) {
      // First press toggles to manual; do not force pump.
      currentMode = MANUAL_MODE;
      stopAlarm();
      setPump(false);
    } else {
      // Manual mode: press again forces pump 5 seconds.
      stopAlarm();
      forceManualPump5s();
    }
  }
  prevStablePressed = stablePressed;

  // ---- Update pump behavior per mode ----
  if (currentMode == AUTO_MODE) updatePumpStateAuto();
  else updatePumpStateManual();

  // ---- Update alarm (buzzer toggling when alarmActive) ----
  updateAlarm();

  // ---- Update display (cyclic) ----
  updateDisplay();
}
