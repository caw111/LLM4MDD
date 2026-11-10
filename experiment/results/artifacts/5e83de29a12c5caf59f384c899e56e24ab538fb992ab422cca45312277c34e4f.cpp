#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Authoritative pin assignments from requirements/circuit:
static const uint8_t PIN_TM_CLK = 4;   // D4 (also mentioned as button in legacy; resolved by remap)
static const uint8_t PIN_TM_DIO = 5;   // D5
static const uint8_t PIN_BTN1   = 2;   // D2
static const uint8_t PIN_BTN2   = 3;   // D3
static const uint8_t PIN_BTN3   = 4;   // D4 (if conflict resolved, this may be non-functional)
static const uint8_t PIN_BUZZER = 6;   // D6
static const uint8_t PIN_LED    = 13;  // D13

static const uint32_t SERIAL_BAUD = 9600;

TM1637Display tm(PIN_TM_CLK, PIN_TM_DIO);

// ---------------- RTC via serial input ----------------
struct DateTime {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

static DateTime rtcNow;
static bool rtcValid = false;

static String lineBuf;
static unsigned long lastIdlePollMs = 0;

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool parseDateTime(const String &s, DateTime &out);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint8_t daysInMonth(uint16_t year, uint8_t month);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void tickOneSecond(DateTime &dt);
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool sameYMD(const DateTime &a, const DateTime &b);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void processSerialRTC();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateRTCBySoftwareTick();
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool wasPressedEdge(uint8_t pin);
#line 201 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool button1Pressed();
#line 202 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool button2Pressed();
#line 231 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void outputMissedWarning(int idx);
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool shouldTriggerDose(const DateTime &now, int idx);
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void startAlert(int idx);
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void finishAlertAsConfirmed();
#line 278 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showTimeHHMM(const DateTime &dt);
#line 288 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void browseTodayStatus();
#line 326 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleReminderOutputs();
#line 366 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setupPins();
#line 375 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setupRTCDefaults();
#line 385 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 401 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool parseDateTime(const String &s, DateTime &out) {
  String t = s;
  t.trim();
  if (t.length() < 8) return false;

  int y = -1, m = -1, d = -1, hh = -1, mm = -1, ss = -1;

  // Full format: YYYY-MM-DD HH:MM:SS (or with 'T')
  if (t.indexOf('-') >= 0 && t.indexOf(':') >= 0) {
    String tt = t;
    tt.replace('T', ' ');

    int sp = tt.indexOf(' ');
    if (sp < 0) return false;

    String datePart = tt.substring(0, sp);
    String timePart = tt.substring(sp + 1);

    if (datePart.length() >= 10) {
      int dash1 = datePart.indexOf('-');
      int dash2 = datePart.indexOf('-', dash1 + 1);
      if (dash1 > 0 && dash2 > dash1) {
        y = datePart.substring(0, dash1).toInt();
        m = datePart.substring(dash1 + 1, dash2).toInt();
        d = datePart.substring(dash2 + 1).toInt();
      }
    }

    int c1 = timePart.indexOf(':');
    int c2 = timePart.indexOf(':', c1 + 1);
    if (c1 > 0 && c2 > c1) {
      hh = timePart.substring(0, c1).toInt();
      mm = timePart.substring(c1 + 1, c2).toInt();
      ss = timePart.substring(c2 + 1).toInt();
    }

    if (y >= 0 && m >= 1 && d >= 1 && hh >= 0 && mm >= 0 && ss >= 0) {
      out.year = (uint16_t)y;
      out.month = (uint8_t)m;
      out.day = (uint8_t)d;
      out.hour = (uint8_t)hh;
      out.minute = (uint8_t)mm;
      out.second = (uint8_t)ss;
      return true;
    }
  }

  // Try HH:MM:SS only
  if (t.indexOf(':') >= 0) {
    int c1 = t.indexOf(':');
    int c2 = t.indexOf(':', c1 + 1);
    if (c1 > 0 && c2 > c1) {
      hh = t.substring(0, c1).toInt();
      mm = t.substring(c1 + 1, c2).toInt();
      ss = t.substring(c2 + 1).toInt();
      if (hh >= 0 && hh < 24 && mm >= 0 && mm < 60 && ss >= 0 && ss < 60) {
        out = rtcNow; // keep date if known
        out.hour = (uint8_t)hh;
        out.minute = (uint8_t)mm;
        out.second = (uint8_t)ss;
        return true;
      }
    }
  }

  return false;
}

static uint8_t daysInMonth(uint16_t year, uint8_t month) {
  static const uint8_t dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (month == 2) {
    bool leap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
    return leap ? 29 : 28;
  }
  if (month >= 1 && month <= 12) return dim[month - 1];
  return 30;
}

static void tickOneSecond(DateTime &dt) {
  dt.second++;
  if (dt.second >= 60) {
    dt.second = 0;
    dt.minute++;
    if (dt.minute >= 60) {
      dt.minute = 0;
      dt.hour++;
      if (dt.hour >= 24) {
        dt.hour = 0;
        dt.day++;
        if (dt.day > daysInMonth(dt.year, dt.month)) {
          dt.day = 1;
          dt.month++;
          if (dt.month > 12) {
            dt.month = 1;
            dt.year++;
          }
        }
      }
    }
  }
}

static bool sameYMD(const DateTime &a, const DateTime &b) {
  return a.year == b.year && a.month == b.month && a.day == b.day;
}

static void updateTodayStateIfNeeded(); // forward

static void processSerialRTC() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    lastIdlePollMs = millis();
    if (c == '\r') continue;

    if (c == '\n') {
      if (lineBuf.length() > 0) {
        DateTime parsed;
        if (parseDateTime(lineBuf, parsed)) {
          rtcNow = parsed;
          rtcValid = true;
          updateTodayStateIfNeeded();
        }
      }
      lineBuf = "";
    } else {
      if (lineBuf.length() < 64) lineBuf += c;
    }
  }

  // idle-based commit (line_or_idle)
  if (lineBuf.length() > 0) {
    if (millis() - lastIdlePollMs > 20) {
      DateTime parsed;
      if (parseDateTime(lineBuf, parsed)) {
        rtcNow = parsed;
        rtcValid = true;
        updateTodayStateIfNeeded();
      }
      lineBuf = "";
    }
  }
}

static void updateRTCBySoftwareTick() {
  if (!rtcValid) return;

  static unsigned long lastTickMs = 0;
  unsigned long nowMs = millis();
  if (lastTickMs == 0) lastTickMs = nowMs;

  if (nowMs - lastTickMs >= 1000UL) {
    unsigned long steps = (nowMs - lastTickMs) / 1000UL;
    lastTickMs += steps * 1000UL;
    for (unsigned long i = 0; i < steps; i++) tickOneSecond(rtcNow);
  }
}

// ---------------- Buttons ----------------
static bool wasPressedEdge(uint8_t pin) {
  static uint8_t prev[10] = {0};
  uint8_t idx = pin % 10;
  uint8_t cur = digitalRead(pin);
  bool pressed = false;
  if (cur == HIGH && prev[idx] == LOW) pressed = true;
  prev[idx] = cur;
  return pressed;
}

static bool button1Pressed() { return wasPressedEdge(PIN_BTN1); } // D2
static bool button2Pressed() { return wasPressedEdge(PIN_BTN2); } // D3

// ---------------- Medication reminder settings ----------------
struct DoseSetting {
  uint8_t hour;
  uint8_t minute;
  bool enabled;
};

static DoseSetting dose[3] = {
  {8, 0, true},
  {12, 0, true},
  {20, 0, true}
};

// Today's confirmation state
static bool confirmedToday[3] = {false, false, false};
static DateTime confirmedForDate;
static bool confirmedForDateInitialized = false;

// Missed counters
static uint32_t missedCount = 0;

// Reminder active state machine
enum ReminderState : uint8_t { IDLE = 0, ALERTING = 1, WAIT_CONFIRM = 2 };
static ReminderState state = IDLE;
static int activeDoseIdx = -1;
static unsigned long alertStartMs = 0;

static void outputMissedWarning(int idx) {
  Serial.print("missed dose ");
  Serial.println(idx + 1);
}

static void updateTodayStateIfNeeded() {
  if (!rtcValid) return;
  static bool first = true;
  if (first) {
    confirmedForDate = rtcNow;
    confirmedForDateInitialized = true;
    first = false;
    for (int i = 0; i < 3; i++) confirmedToday[i] = false;
    return;
  }
  if (!sameYMD(confirmedForDate, rtcNow)) {
    confirmedForDate = rtcNow;
    confirmedForDateInitialized = true;
    for (int i = 0; i < 3; i++) confirmedToday[i] = false;
    state = IDLE;
    activeDoseIdx = -1;
  }
}

static bool shouldTriggerDose(const DateTime &now, int idx) {
  if (!dose[idx].enabled) return false;
  if (confirmedToday[idx]) return false;
  // trigger at hh:mm:00 only (as in previous sketch behavior)
  if (now.hour == dose[idx].hour && now.minute == dose[idx].minute && now.second == 0) return true;
  return false;
}

static void startAlert(int idx) {
  state = ALERTING;
  activeDoseIdx = idx;
  alertStartMs = millis();
}

static void finishAlertAsConfirmed() {
  if (activeDoseIdx >= 0 && activeDoseIdx < 3) {
    confirmedToday[activeDoseIdx] = true;
  }
  state = IDLE;
  activeDoseIdx = -1;
}

// ---------------- TM1637 display helpers ----------------
static void showTimeHHMM(const DateTime &dt) {
  uint8_t hh = dt.hour;
  uint8_t mm = dt.minute;
  // TM1637Display::showNumberDecEx with 4 digits (HHMM) and colon control via dots mask
  // Use "colon" as bit 6 (0b01000000) to mimic previous behavior.
  tm.setBrightness(7);
  tm.showNumberDecEx((int)(hh * 100 + mm), 0b01000000, true);
}

// ---------------- Browse today's reminder status ----------------
static void browseTodayStatus() {
  static uint8_t browseIndex = 0;
  static bool browsing = false;

  // Browse/next state:
  // D3 increments reminder status page
  // D2 also allowed to browse (legacy), and can stop browsing outside alert.
  if (button2Pressed()) {
    browsing = true;
    browseIndex = (browseIndex + 1) % 3;
  } else if (button1Pressed()) {
    browsing = true;
  }

  if (!browsing) return;

  int idx = browseIndex;
  uint8_t hh = dose[idx].hour;
  uint8_t mm = dose[idx].minute;
  int data = hh * 100 + mm;

  // Blink every 500ms when not confirmed
  bool blink = ((millis() / 500UL) % 2 == 0);

  tm.setBrightness(7);
  if (confirmedToday[idx]) {
    tm.showNumberDecEx(data, 0b01000000, true);
  } else {
    if (blink) tm.showNumberDecEx(data, 0x00, true);
    else tm.showNumberDecEx(data, 0b01000000, true);
  }

  static unsigned long lastActionMs = 0;
  if (button2Pressed() || button1Pressed()) lastActionMs = millis();
  if (millis() - lastActionMs > 3000UL) browsing = false;
}

// ---------------- Outputs during reminder ----------------
static void handleReminderOutputs() {
  if (state == IDLE) return;
  if (activeDoseIdx < 0) return;

  unsigned long nowMs = millis();
  unsigned long elapsed = nowMs - alertStartMs;

  // Intermittent buzzer and LED blink during alert/confirm window
  const unsigned long cycleMs = 1000;
  const unsigned long onMs = 200;
  unsigned long pos = elapsed % cycleMs;

  bool buzOn = (pos < onMs);
  bool ledOn = buzOn;

  if (state == ALERTING || state == WAIT_CONFIRM) {
    if (buzOn) tone(PIN_BUZZER, 2000);
    else noTone(PIN_BUZZER);

    digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
  }

  // After 30 seconds without confirm -> missed
  if (elapsed >= 30000UL) {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);

    outputMissedWarning(activeDoseIdx);
    missedCount++;

    // prevent re-triggering for the same time
    confirmedToday[activeDoseIdx] = true;
    state = IDLE;
    activeDoseIdx = -1;
  } else {
    state = WAIT_CONFIRM;
  }
}

// ---------------- Setup/loop ----------------
static void setupPins() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);
}

static void setupRTCDefaults() {
  rtcNow.year = 2000;
  rtcNow.month = 1;
  rtcNow.day = 1;
  rtcNow.hour = 0;
  rtcNow.minute = 0;
  rtcNow.second = 0;
  rtcValid = false;
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  setupPins();

  tm.setBrightness(7);
  tm.clear();

  // Avoid taking address of temporary array (fix compiler error)
  uint8_t offSegs[4] = {0, 0, 0, 0};
  tm.setSegments(offSegs);

  setupRTCDefaults();
  updateTodayStateIfNeeded();
}

void loop() {
  processSerialRTC();

  if (rtcValid) {
    updateRTCBySoftwareTick();
    updateTodayStateIfNeeded();

    // Display current time in idle; also during alert (per previous behavior)
    if (state == IDLE) showTimeHHMM(rtcNow);
    else showTimeHHMM(rtcNow);

    // Trigger checks only when idle
    if (state == IDLE) {
      for (int i = 0; i < 3; i++) {
        if (shouldTriggerDose(rtcNow, i)) {
          startAlert(i);
          break;
        }
      }
    }

    // Confirmation: button D2
    if ((state == ALERTING || state == WAIT_CONFIRM) && button1Pressed()) {
      finishAlertAsConfirmed();
    }

    // Browse allowed during idle
    if (state == IDLE) {
      browseTodayStatus();
    }

    if (state != IDLE) {
      handleReminderOutputs();
    }
  } else {
    // RTC not valid yet
    tm.showNumberDecEx(0, 0b01000000, true);
    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);

    if (state == IDLE) {
      browseTodayStatus();
    }
  }
}
