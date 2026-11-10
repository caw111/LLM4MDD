#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin assignments (as specified) --------------------
static const uint8_t PIN_BUTTON = D2;     // mode toggle button
static const uint8_t PIN_PUMP_RELAY = D10;  // pump relay
static const uint8_t PIN_LED = D13;        // status LED
static const uint8_t PIN_BUZZER = D6;      // alarm buzzer (tone output)
static const uint8_t PIN_TM1637_CLK = D4;  // CLK=D4
static const uint8_t PIN_TM1637_DIO = D5;  // DIO=D5

static const uint8_t PIN_SOIL = A0;   // soil moisture sensor
static const uint8_t PIN_RAIN = A1;   // rain sensor
static const uint8_t PIN_WATER = A2;  // water level sensor

// -------------------- TM1637 setup --------------------
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- Thresholds / behavior --------------------
// NOTE: Values are set as practical defaults because the prompt did not provide calibration/thresholds.
// If your circuit uses different calibration, adjust these constants.
static const int SOIL_DRY_THRESHOLD = 450;     // below => dry (depends on sensor)
static const int WATER_SUFFICIENT_THRESHOLD = 450; // above => enough water (depends on sensor)
static const int RAIN_DETECTED_THRESHOLD = 450;    // below or above depends on sensor module; default assumes lower means "wet/rain"

// -------------------- Timing --------------------
static const unsigned long MANUAL_PUMP_SECONDS = 5UL;
static const unsigned long BUZZ_ALARM_TOGGLE_MS = 250UL; // buzzer toggling during alarm
static const unsigned long DISPLAY_UPDATE_MS = 200UL;

// -------------------- State --------------------
enum Mode : uint8_t { AUTO_MODE = 0, MANUAL_MODE = 1 };
static volatile bool buttonEvent = false;

// Button handling (simple edge detect with debounce in loop)
static bool lastButtonStable = HIGH; // because using INPUT_PULLUP
static bool buttonStable = HIGH;
static unsigned long lastDebounceMs = 0;

// Pump control
static bool pumpRunning = false;
static unsigned long pumpStartMs = 0;

// Alarm
static bool alarmActive = false;
static unsigned long lastBuzzToggleMs = 0;
static bool buzzOn = false;

// Display
static unsigned long lastDisplayMs = 0;
static uint8_t displayPage = 0;

// -------------------- Helpers --------------------
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setPump(bool on);
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startAlarm();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stopAlarm();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarm();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isSoilDry();
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isRainDetected();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isWaterSufficient();
#line 128 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 165 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonToggle();
#line 177 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void forceManualPump5s();
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updatePumpStateAuto();
#line 213 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updatePumpStateManual();
#line 224 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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

  // Toggle buzzer output while alarm is active
  unsigned long now = millis();
  if (now - lastBuzzToggleMs >= BUZZ_ALARM_TOGGLE_MS) {
    lastBuzzToggleMs = now;
    buzzOn = !buzzOn;
    if (buzzOn) {
      tone(PIN_BUZZER, 2000); // audible alarm frequency
    } else {
      noTone(PIN_BUZZER);
    }
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
  // Default assumption: analog value lower => dry
  int v = readAnalogSmooth(PIN_SOIL);
  return (v < SOIL_DRY_THRESHOLD);
}

static bool isRainDetected() {
  // Default assumption: rain sensor outputs lower when wet (rain detected)
  int v = readAnalogSmooth(PIN_RAIN);
  return (v < RAIN_DETECTED_THRESHOLD);
}

static bool isWaterSufficient() {
  // Default assumption: water level outputs higher when sufficient
  int v = readAnalogSmooth(PIN_WATER);
  return (v >= WATER_SUFFICIENT_THRESHOLD);
}

static void showNumberOnDisplay(int value, bool dot = false) {
  // TM1637Display uses 4 digits; constrain to 0..9999 for safety.
  if (value < 0) value = 0;
  if (value > 9999) value = 9999;
  // Display as a plain number; dot control if supported
  display.showNumberDecEx(value, dot ? 0x40 : 0x00, true);
}

static void updateDisplay() {
  unsigned long now = millis();
  if (now - lastDisplayMs < DISPLAY_UPDATE_MS) return;
  lastDisplayMs = now;

  // Pages:
  // 0: Soil moisture
  // 1: Water level
  // 2: Mode indicator (AUTO/MANUAL) + pump/alarm status in last digits
  displayPage = (displayPage + 1) % 3;

  if (displayPage == 0) {
    int soilVal = readAnalogSmooth(PIN_SOIL);
    // Show raw analog reading
    showNumberOnDisplay(soilVal, false);
  } else if (displayPage == 1) {
    int waterVal = readAnalogSmooth(PIN_WATER);
    showNumberOnDisplay(waterVal, false);
  } else {
    // Mode / status encoding on display:
    // AUTO_MODE => 1000 + (pumpRunning?1:0) + (alarmActive?2:0)
    // MANUAL_MODE => 2000 + (pumpRunning?1:0) + (alarmActive?2:0)
    // last digit dot used as separator (dot toggled for pumpRunning)
    int base = (alarmActive ? 0 : 0); // keep simple
    uint8_t statusBits = 0;
    if (pumpRunning) statusBits |= 0x01;
    if (alarmActive) statusBits |= 0x02;

    int modeVal = (currentMode == AUTO_MODE) ? 1000 : 2000;
    int val = modeVal + statusBits; // 1000/1001/1002/1003 or 2000/...
    showNumberOnDisplay(val, pumpRunning);
  }
}

// -------------------- Mode / button logic --------------------
static Mode currentMode = AUTO_MODE;

static void handleButtonToggle() {
  // Button toggles between auto/manual.
  currentMode = (currentMode == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;

  // Requirement:
  // - Auto mode: pump only under conditions.
  // - Manual mode: pressing button again can force pump 5 seconds.
  //
  // If mode switched into MANUAL, do NOT start pump automatically unless button press is "again"
  // (handled by separate edge logic: first press only toggles mode).
}

static void forceManualPump5s() {
  pumpStartMs = millis();
  setPump(true);
  stopAlarm();
}

static void updatePumpStateAuto() {
  // Automatic mode:
  // only open pump when soil is dry, no rain, and water level sufficient.
  // if water level insufficient:禁止启动并报警
  if (pumpRunning) {
    // In auto mode, requirement only says "开 启水泵" when conditions met.
    // It doesn't specify a stop duration; implement continuous operation while conditions remain true.
    // If conditions fail, turn off pump.
  }

  bool dry = isSoilDry();
  bool rain = isRainDetected();
  bool waterOk = isWaterSufficient();

  if (dry && !rain) {
    if (waterOk) {
      stopAlarm();
      if (!pumpRunning) setPump(true);
    } else {
      // Water insufficient: forbid start and alarm
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
  // In manual mode, pump starts only when forced by button again for 5 seconds.
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

  // Ensure known state
  digitalWrite(PIN_PUMP_RELAY, LOW);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  display.setBrightness(0x0f, true); // max brightness
  display.clear();

  // Initialize button debounce states
  lastButtonStable = digitalRead(PIN_BUTTON);
  buttonStable = lastButtonStable;
  lastDebounceMs = millis();

  stopAlarm();
  setPump(false);
}

// Debounce & interpret button presses.
// - Button toggles auto/manual each press.
// - In manual mode, pressing button again forces pump 5 seconds.
void loop() {
  // ---- Read analog periodically in control logic (with smoothing inside helpers)
  // ---- Button debounce
  bool reading = digitalRead(PIN_BUTTON);

  if (reading != lastButtonStable) {
    lastDebounceMs = millis();
    lastButtonStable = reading;
  }

  // When reading has been stable for debounce period and is pressed (LOW)
  static bool prevStablePressed = false;
  bool stablePressed = (lastButtonStable == LOW);

  if (stablePressed && !prevStablePressed) {
    // Rising edge of "pressed"
    if (currentMode == AUTO_MODE) {
      // Toggle to manual on press (first press)
      currentMode = MANUAL_MODE;
      // In manual, requirement says "again press can force pump 5 seconds"
      // so do not force pump here.
      stopAlarm();
      // Ensure auto behavior stops immediately
      setPump(false);
    } else {
      // In manual mode: pressing again forces 5s
      stopAlarm();
      forceManualPump5s();
    }
  }
  prevStablePressed = stablePressed;

  // ---- Update mode behavior
  if (currentMode == AUTO_MODE) {
    // If auto: if currently pumping, apply auto rules to stop/start appropriately
    updatePumpStateAuto();
  } else {
    // Manual: pump only ends by 5s timer; auto conditions should not control it.
    updatePumpStateManual();
  }

  // ---- Alarm update
  updateAlarm();

  // ---- Display update
  updateDisplay();
}
