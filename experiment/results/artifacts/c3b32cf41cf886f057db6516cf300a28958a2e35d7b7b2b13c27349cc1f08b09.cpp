#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

TM1637Display display(4, 5); // CLK=D4, DIO=D5

// Pin assignments
const uint8_t PIN_SENSOR_D2 = 2;   // D2:光电计数传感器，记录产品通过数
const uint8_t PIN_STOP_BTN_D3 = 3; // D3:人工停线按钮
const uint8_t PIN_MODE_BTN_D7 = 7;   // D7:切换显示目标、实际和停机次数

const uint8_t PIN_BUZZER_D6 = 6;     // D6:蜂鸣器
const uint8_t PIN_LED_D13 = 13;      // D13:LED

// -------------------- User-configurable timing/production parameters --------------------
// 目标产量：每分钟目标产量（达成率=实际累计产量 / 目标产量 * 100%）
static const uint16_t TARGET_PER_MIN = 60; // change if needed

// 连续 30 秒无产品通过或人工停线，则报警并记录停机事件
static const uint32_t NO_PRODUCT_ALARM_MS = 30000UL;

// -------------------- Display mode --------------------
enum DisplayMode : uint8_t {
  SHOW_TARGET = 0,
  SHOW_ACTUAL = 1,
  SHOW_STOPS  = 2
};

volatile bool sensorPulse = false;

// State for production counting
volatile uint32_t totalCount = 0;        // 累计产量
volatile uint32_t stopEvents = 0;       // 停机事件次数（记录）
volatile bool manualStopActive = false;

// Time tracking
unsigned long lastProductMs = 0;   // 最后一次“产品通过”时刻
unsigned long lastStopMs = 0;     // 最后一次“人工停线”触发时刻（用于“无产品通过或人工停线”的判定）
unsigned long programStartMs = 0;

// Alarm handling
bool alarmActive = false;
unsigned long alarmLastBeepMs = 0;
static const uint16_t ALARM_BEEP_ON_MS = 200;
static const uint16_t ALARM_BEEP_OFF_MS = 200;

// Button debouncing
static const uint16_t DEBOUNCE_MS = 50;
unsigned long lastStopBtnSampleMs = 0;
bool lastStopBtnLevel = HIGH;

unsigned long lastModeBtnSampleMs = 0;
bool lastModeBtnLevel = HIGH;

DisplayMode displayMode = SHOW_ACTUAL;

// TM1637 digits helpers
// Display limited to 4 digits; values over range will clamp.
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clamp4Digits(uint32_t v);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showNumber(uint16_t value);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerOff();
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerBeep();
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleAlarm();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearAlarmIfConditionCleared();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void sensorISR();
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_030\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t clamp4Digits(uint32_t v) {
  if (v > 9999UL) return 9999;
  return (uint16_t)v;
}

static void showNumber(uint16_t value) {
  display.clear();
  display.setBrightness(7);
  display.showNumberDec(value, false); // no leading zeros
}

// Beeper control
static void buzzerOff() {
  digitalWrite(PIN_LED_D13, LOW);
  noTone(PIN_BUZZER_D6);
}

static void buzzerBeep() {
  // Keep tone running continuously while desired window; we use gating in loop.
  digitalWrite(PIN_LED_D13, HIGH);
  tone(PIN_BUZZER_D6, 2000);
}

static void handleAlarm() {
  if (!alarmActive) {
    alarmActive = true;
    // Record stop event when alarm triggers due to 30s no product or manual stop
    stopEvents++;
    // Also log once on entering alarm
    Serial.print(F("[ALARM] 30s no product/manual-stop detected. stopEvents="));
    Serial.println(stopEvents);
  }

  // Beep pattern while alarmActive
  unsigned long now = millis();
  if (now - alarmLastBeepMs >= (alarmLastBeepMs == 0 ? 0 : 0)) {
    // no-op
  }

  // Use a simple on/off cycle based on elapsed time since last toggle.
  static bool beepOn = false;
  static unsigned long lastToggleMs = 0;

  if (lastToggleMs == 0) lastToggleMs = now;

  unsigned long elapsed = now - lastToggleMs;
  if (!beepOn) {
    if (elapsed >= ALARM_BEEP_OFF_MS) {
      beepOn = true;
      lastToggleMs = now;
      alarmLastBeepMs = now;
      buzzerBeep();
    }
  } else {
    if (elapsed >= ALARM_BEEP_ON_MS) {
      beepOn = false;
      lastToggleMs = now;
      buzzerOff();
    }
  }
}

static void clearAlarmIfConditionCleared() {
  if (!alarmActive) return;

  // Alarm should clear when products have passed recently or manual stop has been handled recently.
  unsigned long now = millis();
  bool productRecent = (now - lastProductMs) < NO_PRODUCT_ALARM_MS;
  bool stopRecent    = (now - lastStopMs) < NO_PRODUCT_ALARM_MS;

  if (productRecent || stopRecent) {
    alarmActive = false;
    buzzerOff();
    // Log alarm cleared for visibility
    Serial.println(F("[ALARM] Cleared."));
  }
}

// -------------------- Interrupt for sensor D2 --------------------
void sensorISR() {
  // Count product passes. Assume sensor output triggers a pulse.
  totalCount++;
  sensorPulse = true;
}

void setup() {
  pinMode(PIN_SENSOR_D2, INPUT_PULLUP);       // D2 input (photocount sensor)
  pinMode(PIN_STOP_BTN_D3, INPUT_PULLUP);    // D3 manual stop button
  pinMode(PIN_MODE_BTN_D7, INPUT_PULLUP);    // D7 mode switch button

  pinMode(PIN_BUZZER_D6, OUTPUT);
  pinMode(PIN_LED_D13, OUTPUT);

  digitalWrite(PIN_LED_D13, LOW);
  noTone(PIN_BUZZER_D6);

  // TM1637 initial display
  display.setBrightness(7);
  display.clear();
  showNumber(0);

  // Serial
  Serial.begin(9600);
  // Keep line_or_idle framing and idle_timeout_ms=20 is handled by environment requirements; Arduino uses standard.

  // Attach interrupt on D2 (rising edge assumed; if sensor is active-low, hardware mapping assumed already)
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_D2), sensorISR, RISING);

  programStartMs = millis();
  lastProductMs = programStartMs;
  lastStopMs = 0;

  // Initial button state
  lastStopBtnLevel = digitalRead(PIN_STOP_BTN_D3);
  lastModeBtnLevel = digitalRead(PIN_MODE_BTN_D7);

  // Initial log
  Serial.println(F("[BOOT] Production cycle monitor started."));
  Serial.print(F("[CFG] TARGET_PER_MIN="));
  Serial.println(TARGET_PER_MIN);
}

void loop() {
  unsigned long now = millis();

  // Clear alarm if condition has been met
  clearAlarmIfConditionCleared();

  // Detect sensor pulse effect time for alarm logic
  // (We update lastProductMs when at least one pulse has been counted since last check)
  static uint32_t lastSeenTotal = 0;
  if (totalCount != lastSeenTotal) {
    lastSeenTotal = totalCount;
    lastProductMs = now; // update on any product pass
    // If currently alarming, a new product should eventually clear alarm via clearAlarmIfConditionCleared()
  }

  // Manual stop button D3 (active low with pullup assumed)
  bool stopBtnLevel = digitalRead(PIN_STOP_BTN_D3);
  if (now - lastStopBtnSampleMs >= DEBOUNCE_MS) {
    lastStopBtnSampleMs = now;
    if (stopBtnLevel != lastStopBtnLevel) {
      lastStopBtnLevel = stopBtnLevel;
      if (stopBtnLevel == LOW) {
        // Manual stop event triggered
        manualStopActive = true;
        lastStopMs = now;

        Serial.print(F("[STOP] Manual stop triggered. stopEvents="));
        Serial.println(stopEvents);

        // While manual stop is active, we consider it as a condition that also counts as "人工停线"
        // For the 30s no-product or manual-stop alarm rule:
        // - update lastStopMs so alarm won't immediately trigger due to "no product through or stop".
        // - also we will not create a new stopEvents here unless alarm triggers (as per requirement).
      } else {
        manualStopActive = false;
      }
    }
  }

  // Mode switch button D7: cycle among target/actual/stops
  bool modeBtnLevel = digitalRead(PIN_MODE_BTN_D7);
  if (now - lastModeBtnSampleMs >= DEBOUNCE_MS) {
    lastModeBtnSampleMs = now;
    if (modeBtnLevel != lastModeBtnLevel) {
      lastModeBtnLevel = modeBtnLevel;
      if (modeBtnLevel == LOW) {
        // next mode
        displayMode = (DisplayMode)((displayMode + 1) % 3);
        Serial.print(F("[MODE] "));
        if (displayMode == SHOW_TARGET) Serial.println(F("SHOW_TARGET"));
        else if (displayMode == SHOW_ACTUAL) Serial.println(F("SHOW_ACTUAL"));
        else Serial.println(F("SHOW_STOPS"));
      }
    }
  }

  // Alarm trigger condition:
  // If continuous 30 seconds无产品通过 or 人工停线 -> 报警并记录停机事件
  // Implement as: if no product pulses AND no manual stop within last 30s.
  // (i.e., both conditions expired)
  if (!alarmActive) {
    bool noProductFor = (now - lastProductMs) >= NO_PRODUCT_ALARM_MS;
    bool noStopFor = (lastStopMs == 0) ? true : (now - lastStopMs) >= NO_PRODUCT_ALARM_MS;

    if (noProductFor && noStopFor) {
      alarmActive = true;
      stopEvents++;
      Serial.print(F("[ALARM] 30s无产品通过/无人工停线. stopEvents="));
      Serial.println(stopEvents);
    }
  }

  // While alarmActive, drive buzzer
  if (alarmActive) {
    handleAlarm();
  } else {
    buzzerOff();
  }

  // Compute reach rate based on target per minute and accumulated actual count.
  // "实时显示累计产量和达成率" - on serial, also show on each second.
  // For TM1637: show based on displayMode.
  static unsigned long lastSerialMs = 0;
  if (now - lastSerialMs >= 1000UL) {
    lastSerialMs = now;

    uint32_t countSnapshot = totalCount;
    float minutesElapsed = (now - programStartMs) / 60000.0f;
    if (minutesElapsed < 0.001f) minutesElapsed = 0.001f;

    float targetForElapsed = TARGET_PER_MIN * minutesElapsed;
    float rate = (targetForElapsed > 0.0f) ? (countSnapshot / targetForElapsed) * 100.0f : 0.0f;
    if (rate < 0) rate = 0;

    Serial.print(F("[STAT] elapsed_min="));
    Serial.print(minutesElapsed, 2);
    Serial.print(F(", totalCount="));
    Serial.print(countSnapshot);
    Serial.print(F(", target_for_elapsed="));
    Serial.print(targetForElapsed, 1);
    Serial.print(F(", reach_rate%="));
    Serial.println(rate, 1);

    // Also note stopEvents occasionally
    Serial.print(F("[STAT] stopEvents="));
    Serial.println(stopEvents);
  }

  // Update TM1637 display according to mode.
  // Note: TM1637 is 4 digits. Rate is not requested to be shown here; requirement says button switches
  // between target, actual and stop count. We'll follow that exactly.
  uint32_t targetShown = (uint32_t)TARGET_PER_MIN;
  uint32_t actualShown = totalCount;
  uint32_t stopsShown = stopEvents;

  if (displayMode == SHOW_TARGET) {
    showNumber(clamp4Digits(targetShown));
  } else if (displayMode == SHOW_ACTUAL) {
    showNumber(clamp4Digits(actualShown));
  } else {
    showNumber(clamp4Digits(stopsShown));
  }

  // Prevent overly tight loop; small delay for stability
  delay(5);
}
