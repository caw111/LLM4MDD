#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Pin assignments (per requirements) --------------------
static const uint8_t SOIL_SENSOR_PIN  = A0; // 土壤湿度传感器
static const uint8_t RAIN_SENSOR_PIN  = A1; // 雨滴传感器
static const uint8_t WATER_SENSOR_PIN = A2; // 水位传感器

static const uint8_t PUMP_RELAY_PIN = 10;   // 水泵继电器 D10
static const uint8_t STATUS_LED_PIN = 13;   // 状态 LED D13
static const uint8_t BUZZER_PIN = 6;        // 蜂鸣器 D6

// TM1637: CLK=D4, DIO=D5
static const uint8_t TM1637_CLK_PIN = 4;
static const uint8_t TM1637_DIO_PIN = 5;

// Button D2
static const uint8_t MODE_BUTTON_PIN = 2;    // 自动/手动切换；手动下再次按可强制启动5秒

// -------------------- TM1637 display --------------------
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

// -------------------- Behavior parameters (thresholds/calibration/timers) --------------------
// NOTE: Requirements mention preserving PWM/calibration/thresholds/timers; however these values
// were not explicitly provided in the prompt. We therefore implement typical non-simplifying
// behavior with reasonable defaults and keep all logic paths intact.
//
// You can adjust these values to match your sensor calibration.
static const int SOIL_DRY_THRESHOLD = 550;    // A0: 土壤干燥阈值(读数越高通常越干/或相反；按实际标定)
static const int RAIN_DETECT_THRESHOLD = 500; // A1: 雨滴阈值
static const int WATER_OK_THRESHOLD = 500;    // A2: 水位足够阈值

// Manual forced pump duration
static const unsigned long MANUAL_FORCE_MS = 5000UL;

// Buzzer/Alarm pattern
static const unsigned long ALARM_BEEP_ON_MS  = 200UL;
static const unsigned long ALARM_BEEP_OFF_MS = 200UL;

// -------------------- Mode & state --------------------
enum Mode : uint8_t { AUTO_MODE = 0, MANUAL_MODE = 1 };

static Mode currentMode = AUTO_MODE;
static bool pumpRunning = false;

// Timed control for manual forced start
static bool manualForceActive = false;
static unsigned long manualForceStartMs = 0;

// Alarm control
static bool alarmActive = false;
static unsigned long alarmLastToggleMs = 0;
static bool alarmBuzzerState = false;

// Button debouncing
static bool lastButtonStable = HIGH;
static bool lastButtonReading = HIGH;
static unsigned long lastDebounceMs = 0;
static const unsigned long DEBOUNCE_MS = 40;

// For detecting "press" edges
static bool buttonPressedEvent = false;

// Pump relay output control
static const uint8_t RELAY_ON_LEVEL  = HIGH; // Most relay modules are active-HIGH; adjust if needed.
static const uint8_t RELAY_OFF_LEVEL = LOW;

// -------------------- Helper functions --------------------
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readAnalog(uint8_t pin);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isSoilDry();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isRaining();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isWaterEnough();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setPump(bool on);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAlarm(bool on);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButton();
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonPress();
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarm();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showNumberWithPrefix(char prefix, int value);
#line 195 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showModeText();
#line 205 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readAnalog(uint8_t pin) {
  return analogRead(pin);
}

static bool isSoilDry() {
  int v = readAnalog(SOIL_SENSOR_PIN);
  return (v >= SOIL_DRY_THRESHOLD);
}

static bool isRaining() {
  int v = readAnalog(RAIN_SENSOR_PIN);
  return (v >= RAIN_DETECT_THRESHOLD);
}

static bool isWaterEnough() {
  int v = readAnalog(WATER_SENSOR_PIN);
  return (v >= WATER_OK_THRESHOLD);
}

static void setPump(bool on) {
  pumpRunning = on;
  digitalWrite(PUMP_RELAY_PIN, on ? RELAY_ON_LEVEL : RELAY_OFF_LEVEL);
}

static void setAlarm(bool on) {
  alarmActive = on;
  if (!on) {
    digitalWrite(BUZZER_PIN, LOW);
    alarmBuzzerState = false;
  }
}

// button press detection (edge)
static void updateButton() {
  bool reading = digitalRead(MODE_BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceMs = millis();
    lastButtonReading = reading;
  }

  if ((millis() - lastDebounceMs) > DEBOUNCE_MS) {
    if (reading != lastButtonStable) {
      lastButtonStable = reading;

      // Assuming pull-up wiring: pressed = LOW
      if (lastButtonStable == LOW) {
        buttonPressedEvent = true;
      }
    }
  }
}

static void handleButtonPress() {
  if (!buttonPressedEvent) return;
  buttonPressedEvent = false;

  if (currentMode == AUTO_MODE) {
    currentMode = MANUAL_MODE;
  } else {
    // MANUAL mode: pressing again forces pump start for 5 seconds
    // If alarm is active due to low water, still allow manual force? Requirement:
    // "手动模式下再次按键可强制启动 5 秒。" It does not explicitly override low-water prohibition,
    // but requirement also says "若水位不足则禁止启动并报警" in automatic mode.
    // We'll preserve this prohibition in automatic only; in manual force we start for 5 seconds.
    manualForceActive = true;
    manualForceStartMs = millis();
    setAlarm(false);
    setPump(true);
  }
}

// Alarm logic for auto-mode low water
static void updateAlarm() {
  if (!alarmActive) return;

  unsigned long now = millis();
  if (alarmBuzzerState) {
    if (now - alarmLastToggleMs >= ALARM_BEEP_OFF_MS) {
      alarmBuzzerState = false;
      alarmLastToggleMs = now;
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    if (now - alarmLastToggleMs >= ALARM_BEEP_ON_MS) {
      alarmBuzzerState = true;
      alarmLastToggleMs = now;
      digitalWrite(BUZZER_PIN, HIGH);
    }
  }
}

// Display multiplexing: loop displays soil, water, and current mode
// TM1637: use 4-digit display
// We'll show:
/// - "Sxxx" for soil (0-999 truncated -> show last 3 digits) : S + 3 digits
/// - "Wxxx" for water
/// - "A---" / "M---" for mode (use A for AUTO, H for MANUAL)
static void showNumberWithPrefix(char prefix, int value) {
  int v = value;
  if (v < 0) v = 0;
  if (v > 999) v = 999;

  // TM1637 supports setting segments via "showNumberDecEx" etc; but prefix letter not reliable without custom mapping.
  // We'll instead encode prefix via decimal point dots on first digit when available.
  // Use 4 digits: digit0 = prefix-like indicator using dot, digits1-3 = value.
  // Digit0 will show blank " " by clearing it (by writing 0 with dp off is not ideal). We'll use 0 with DP on to indicate prefix.
  // However TM1637Display provides showNumberDecEx with leading zeros control and dp positions.
/// We'll encode as:
/// - First digit: 0 with DP set ON to indicate prefix
/// - Last 3 digits: value
  uint8_t dots = 0;
  // dots bit0 corresponds to digit0 dp, etc. For TM1637Display, showNumberDecEx accepts dp mask.
  // We'll set dp on digit0.
  dots = 0b0001;

  // Map "S" and "W" to different dp patterns for distinguishability:
  // S: dp digit0, W: dp digit1
  if (prefix == 'W') dots = 0b0010;

  // Value displayed in 3 least-significant digits; put leading digit as 0.
  int dispVal = v; // showNumberDecEx will place digits right-aligned.
  // 4 digits width
  display.showNumberDecEx(dispVal, dots, true);
}

static void showModeText() {
  // Use 4 digits:
  // AUTO: show "0" with dp pattern digit0 to represent A
  // MANUAL: show "0" with dp pattern digit1 to represent M
  // Since letters not reliably supported, use dp indication.
  uint8_t dots = (currentMode == AUTO_MODE) ? 0b0001 : 0b0010;
  display.showNumberDecEx(0, dots, true);
}

// -------------------- Setup & loop --------------------
void setup() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);

  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);

  digitalWrite(STATUS_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  setPump(false);

  display.setBrightness(0x0f); // max brightness

  // Start with AUTO mode
  currentMode = AUTO_MODE;
  manualForceActive = false;
  alarmActive = false;

  // Display initial
  display.clear();
}

void loop() {
  updateButton();
  handleButtonPress();

  // Read sensors
  bool soilDry = isSoilDry();
  bool raining = isRaining();
  bool waterEnough = isWaterEnough();

  // LED state: indicate mode/pump
  // AUTO: LED indicates pump (on when pump running)
  // MANUAL: LED steady ON when in manual; blink when pump running.
  if (currentMode == AUTO_MODE) {
    digitalWrite(STATUS_LED_PIN, pumpRunning ? HIGH : LOW);
  } else {
    if (pumpRunning) {
      // blink
      digitalWrite(STATUS_LED_PIN, (millis() / 250UL) % 2 ? HIGH : LOW);
    } else {
      digitalWrite(STATUS_LED_PIN, HIGH);
    }
  }

  // Control logic
  if (currentMode == AUTO_MODE) {
    // Stop any manual force
    manualForceActive = false;

    // Alarm: if water insufficient, prohibit pump and alarm
    if (!waterEnough) {
      setPump(false);
      if (!alarmActive) {
        alarmLastToggleMs = millis();
        alarmBuzzerState = false;
      }
      setAlarm(true);
    } else {
      // Water enough: clear alarm
      setAlarm(false);

      // Only start pump if soil dry AND not raining AND water sufficient
      if (soilDry && !raining && waterEnough) {
        if (!pumpRunning) setPump(true);
      } else {
        if (pumpRunning) setPump(false);
      }
    }
  } else {
    // MANUAL mode
    // If manual force active, ensure 5-second duration
    if (manualForceActive) {
      unsigned long now = millis();
      if (now - manualForceStartMs >= MANUAL_FORCE_MS) {
        manualForceActive = false;
        setPump(false);
      } else {
        // Keep pump running
        if (!pumpRunning) setPump(true);
      }
    } else {
      // In manual mode without forced start: do not auto-start pump.
      // Requirement: "按钮用于切换自动/手动模式，手动模式下再次按键可强制启动5秒。"
      // So pump should remain off unless forced.
      if (pumpRunning) setPump(false);
    }

    // Manual mode alarm behavior:
    // Requirement specifies low-water prohibition & alarm for automatic mode.
    // We'll keep alarm off in manual unless a force start triggers it (not required).
    setAlarm(false);
  }

  // Update alarm buzzer
  updateAlarm();

  // Display multiplexing across soil, water, and mode
  static uint8_t displayStep = 0;
  static unsigned long lastDispMs = 0;
  const unsigned long DISP_PERIOD_MS = 500;

  unsigned long now = millis();
  if (now - lastDispMs >= DISP_PERIOD_MS) {
    lastDispMs = now;
    displayStep = (displayStep + 1) % 3;
  }

  int soilVal = readAnalog(SOIL_SENSOR_PIN);
  int waterVal = readAnalog(WATER_SENSOR_PIN);

  if (displayStep == 0) {
    // Soil humidity
    // Show last 3 digits; use dp encoding
    showNumberWithPrefix('S', soilVal);
  } else if (displayStep == 1) {
    // Water level
    showNumberWithPrefix('W', waterVal);
  } else {
    // Current mode
    showModeText();
  }
}
