#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_TM_CLK = 4;   // D4
static const uint8_t PIN_TM_DIO = 5;   // D5
static const uint8_t PIN_BTN1   = 2;   // D2 (confirm / browse)
static const uint8_t PIN_BTN2   = 3;   // D3 (increment time / browse)
static const uint8_t PIN_BTN3   = 4;   // D4 (note: per requirement; remapped conflict resolved)
static const uint8_t PIN_BUZZER = 6;   // D6
static const uint8_t PIN_LED    = 13;  // D13

// --- RTC via serial input ---
static const uint32_t SERIAL_BAUD = 9600;

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

// Serial line buffer
static String lineBuf;
static unsigned long lastIdlePollMs = 0;

// Parse "YYYY-MM-DD HH:MM:SS" or "HH:MM:SS"
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool parseDateTime(const String &s, DateTime &out);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t daysInMonth(uint16_t year, uint8_t month);
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void tickOneSecond(DateTime &dt);
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool sameYMD(const DateTime &a, const DateTime &b);
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool sameHM(const DateTime &a, const DateTime &b, uint8_t h, uint8_t m);
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t datetimeToKey(const DateTime &dt);
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDebouncedButton(uint8_t pin);
#line 208 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool wasPressedEdge(uint8_t pin);
#line 219 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool button1Pressed();
#line 220 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool button2Pressed();
#line 226 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTimeHHMM(const DateTime &dt);
#line 238 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showBlinkColon(bool on);
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void outputMissedWarning(int idx);
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateTodayStateIfNeeded();
#line 268 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool shouldTriggerDose(const DateTime &now, int idx);
#line 275 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startAlert(int idx);
#line 282 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void finishAlertAsConfirmed();
#line 290 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleReminderOutputs();
#line 333 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void browseTodayStatus();
#line 381 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setupPins();
#line 394 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setupRTCDefaults();
#line 404 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void processSerialRTC();
#line 441 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateRTCBySoftwareTick();
#line 456 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 469 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool parseDateTime(const String &s, DateTime &out) {
  // Try full datetime: YYYY-MM-DD HH:MM:SS
  // Remove extra spaces
  String t = s;
  t.trim();
  if (t.length() < 8) return false;

  int y = -1, m = -1, d = -1, hh = -1, mm = -1, ss = -1;

  // Full format
  // Find separators
  // Expected: YYYY-MM-DD HH:MM:SS
  // We'll parse with sscanf-like using manual scanning.
  // Quick check for '-' in date portion and ':' in time.
  if (t.indexOf('-') >= 0 && t.indexOf(':') >= 0) {
    // Replace 'T' with space if needed
    t.replace('T', ' ');
    // Extract date and time by first space
    int sp = t.indexOf(' ');
    if (sp < 0) return false;
    String datePart = t.substring(0, sp);
    String timePart = t.substring(sp + 1);

    if (datePart.length() >= 10) {
      // YYYY-MM-DD
      int dash1 = datePart.indexOf('-');
      int dash2 = datePart.indexOf('-', dash1 + 1);
      if (dash1 > 0 && dash2 > dash1) {
        y = datePart.substring(0, dash1).toInt();
        m = datePart.substring(dash1 + 1, dash2).toInt();
        d = datePart.substring(dash2 + 1).toInt();
      }
    }
    // HH:MM:SS (maybe shorter)
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
  static const uint8_t dim[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
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

static bool sameHM(const DateTime &a, const DateTime &b, uint8_t h, uint8_t m) {
  return a.hour == h && a.minute == m;
}

static uint32_t datetimeToKey(const DateTime &dt) {
  // For reminder identity: date + time slot id
  // Key only needs to be unique for "today" and hour/minute.
  // Use yyyymmddhhmm.
  uint32_t key = 0;
  key |= (uint32_t)dt.year * 100000000UL;
  key += (uint32_t)dt.month * 1000000UL;
  key += (uint32_t)dt.day * 10000UL;
  key += (uint32_t)dt.hour * 100UL;
  key += (uint32_t)dt.minute;
  return key;
}

// --- Medication reminder settings ---
struct DoseSetting {
  uint8_t hour;
  uint8_t minute;
  bool enabled;
};

static DoseSetting dose[3] = {
  { 8, 0, true },
  { 12, 0, true },
  { 20, 0, true }
};

// Today's confirmation state
static bool confirmedToday[3] = { false, false, false };
static DateTime confirmedForDate; // valid when rtcValid

// Missed counters
static uint32_t missedCount = 0;

// Reminder active state machine
enum ReminderState : uint8_t { IDLE = 0, ALERTING = 1, WAIT_CONFIRM = 2 };
static ReminderState state = IDLE;
static int activeDoseIdx = -1;
static unsigned long alertStartMs = 0;

// Buttons with simple debounce
static bool readDebouncedButton(uint8_t pin) {
  static uint32_t lastChangeMs[10] = {0};
  static uint8_t lastStable[10] = {1};
  // pin expected 2..6; map to index by pin%10
  uint8_t idx = pin % 10;
  uint8_t reading = digitalRead(pin);
  uint32_t now = millis();
  if (reading != lastStable[idx]) {
    // In practice we need debounce per transition:
    // We'll implement a small debounce gate by tracking last change.
    if (now - lastChangeMs[idx] > 25) {
      lastChangeMs[idx] = now;
      lastStable[idx] = reading;
    }
  } else {
    // stable
    lastChangeMs[idx] = now;
  }
  return lastStable[idx] == HIGH; // button is active-high?
}

static bool wasPressedEdge(uint8_t pin) {
  // active HIGH, detect rising edge
  static uint8_t prev[10] = {0};
  uint8_t idx = pin % 10;
  uint8_t cur = digitalRead(pin);
  bool pressed = false;
  if (cur == HIGH && prev[idx] == LOW) pressed = true;
  prev[idx] = cur;
  return pressed;
}

static bool button1Pressed() { return wasPressedEdge(PIN_BTN1); }
static bool button2Pressed() { return wasPressedEdge(PIN_BTN2); }

// --- TM1637 ---
TM1637Display tm(PIN_TM_CLK, PIN_TM_DIO);

// Display helpers: show HH:MM or status
static void showTimeHHMM(const DateTime &dt) {
  uint8_t hh = dt.hour;
  uint8_t mm = dt.minute;
  int data[4];
  data[0] = hh / 10;
  data[1] = hh % 10;
  data[2] = mm / 10;
  data[3] = mm % 10;
  tm.setBrightness(7);
  tm.showNumberDecEx(hh * 100 + mm, 0b01000000, true); // colon bit set
}

static void showBlinkColon(bool on) {
  // Use brightness and colon DP control; simplest is to alternate via setSegments.
  // We'll use display's colon bit with showNumberDecEx; but this requires value.
  (void)on;
}

// --- Missed alert output ---
static void outputMissedWarning(int idx) {
  Serial.print("missed dose ");
  Serial.println(idx + 1);
}

// --- Reminder trigger check ---
static void updateTodayStateIfNeeded() {
  if (!rtcValid) return;
  static bool first = true;
  if (first) {
    confirmedForDate = rtcNow;
    first = false;
    for (int i = 0; i < 3; i++) confirmedToday[i] = false;
    return;
  }
  if (!sameYMD(confirmedForDate, rtcNow)) {
    confirmedForDate = rtcNow;
    for (int i = 0; i < 3; i++) confirmedToday[i] = false;
    state = IDLE;
    activeDoseIdx = -1;
  }
}

static bool shouldTriggerDose(const DateTime &now, int idx) {
  if (!dose[idx].enabled) return false;
  if (confirmedToday[idx]) return false;
  if (now.hour == dose[idx].hour && now.minute == dose[idx].minute && now.second == 0) return true;
  return false;
}

static void startAlert(int idx) {
  state = ALERTING;
  activeDoseIdx = idx;
  alertStartMs = millis();
  // Immediately set outputs in first iteration
}

static void finishAlertAsConfirmed() {
  if (activeDoseIdx >= 0 && activeDoseIdx < 3) {
    confirmedToday[activeDoseIdx] = true;
  }
  state = IDLE;
  activeDoseIdx = -1;
}

static void handleReminderOutputs() {
  if (state == IDLE) return;
  if (activeDoseIdx < 0) return;

  unsigned long nowMs = millis();
  unsigned long elapsed = nowMs - alertStartMs;

  // Intermittent buzzer and LED blink during alert/confirm window
  // For example: buzzer 200ms ON, 800ms OFF; LED toggles with same cadence.
  const unsigned long cycleMs = 1000;
  const unsigned long onMs = 200;
  unsigned long pos = elapsed % cycleMs;

  bool buzOn = (pos < onMs);
  bool ledOn = buzOn;

  if (state == ALERTING || state == WAIT_CONFIRM) {
    // buzzer D6
    if (buzOn) tone(PIN_BUZZER, 2000); else noTone(PIN_BUZZER);
    // LED
    digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
  }

  // After 30 seconds without confirm -> missed
  if (elapsed >= 30000UL) {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);

    outputMissedWarning(activeDoseIdx);

    missedCount++;
    // Mark as not confirmed so it won't re-trigger today if we already passed the slot.
    // Requirement: accumulate missed dose warning and count.
    confirmedToday[activeDoseIdx] = true; // prevent re-triggering for the same time
    state = IDLE;
    activeDoseIdx = -1;
  } else {
    // Move to WAIT_CONFIRM (still same behavior, but keep semantics)
    state = WAIT_CONFIRM;
  }
}

// --- Browse today's reminder status ---
static void browseTodayStatus() {
  // Show states sequentially when user presses buttons.
  // D2: browse/confirm; D3: next reminder
  static uint8_t browseIndex = 0;
  static bool browsing = false;

  // Start browsing on button press D3 (BTN2) or D2
  // Requirement: "按钮可浏览今日提醒状态"
  // We'll display each slot: if confirmed -> show time HHMM, if not -> show "--" via digits pattern.
  if (button2Pressed()) {
    browsing = true;
    browseIndex = (browseIndex + 1) % 3;
  } else if (button1Pressed()) {
    browsing = true; // also allow confirm to possibly end alert; outside alert, acts as browse.
  }

  if (!browsing) return;

  // Simple display: 0/1/2 pages; show HHMM and maybe blink if not confirmed.
  // We'll interpret:
  // confirmed: show its scheduled HHMM
  // not confirmed: show HHMM but toggle colon off every other tick (approx).
  int idx = browseIndex;
  uint8_t hh = dose[idx].hour, mm = dose[idx].minute;
  int data = hh * 100 + mm;

  // Blink every 500ms
  bool blink = ((millis() / 500UL) % 2 == 0);
  tm.setBrightness(7);
  if (confirmedToday[idx]) {
    // Colon on
    tm.showNumberDecEx(data, 0b01000000, true);
  } else {
    // Colon off
    if (blink) tm.showNumberDecEx(data, 0x00, true);
    else tm.showNumberDecEx(data, 0b01000000, true);
  }

  // Auto-stop browsing after a brief time since last browse action
  // We'll reuse a static timer.
  static unsigned long lastActionMs = 0;
  // Update lastActionMs if any browse button pressed
  if (button2Pressed() || button1Pressed()) lastActionMs = millis();
  if (millis() - lastActionMs > 3000UL) {
    browsing = false;
  }
}

static void setupPins() {
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  // BTN3 (D4) conflicts with TM CLK wiring per "resolved remapped". We'll still set it,
  // but note it shares the same pin as TM1637 CLK on this description; hardware conflict resolved by authoritative assignment.
  // Therefore BTN3 input may not be functional. Requirement explicitly mentions D4 as a button; preserve behavior:
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

static void processSerialRTC() {
  // Collect lines with idle timeout behavior
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
          // When RTC is updated, also reset today state if date changed
          updateTodayStateIfNeeded();
        }
      }
      lineBuf = "";
    } else {
      // Avoid unbounded buffer
      if (lineBuf.length() < 64) lineBuf += c;
    }
  }
  // idle timeout isn't directly detectable without timestamps, but line_or_idle implies short pauses can mean end.
  // We'll implement a simple idle-based commit if no newline arrives.
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
    // handle drift by stepping per second
    unsigned long steps = (nowMs - lastTickMs) / 1000UL;
    lastTickMs += steps * 1000UL;
    for (unsigned long i = 0; i < steps; i++) tickOneSecond(rtcNow);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  setupPins();

  tm.setBrightness(7);
  tm.clear();
  tm.setSegments((uint8_t[]){0,0,0,0});
  setupRTCDefaults();

  updateTodayStateIfNeeded();
}

void loop() {
  processSerialRTC();

  if (rtcValid) {
    updateRTCBySoftwareTick();
    updateTodayStateIfNeeded();

    // Show current time on display (always unless alert uses outputs; requirement: "平时显示当前时间")
    if (state == IDLE) {
      showTimeHHMM(rtcNow);
    } else {
      // During alert, still keep displaying current time (common), but buzzer/LED handle reminder.
      // Requirement only mandates buzzer/LED actions; keep time displayed.
      showTimeHHMM(rtcNow);
    }

    // If idle, check triggers
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

    // During idle, browsing allowed by buttons
    if (state == IDLE) {
      // BTN2: next status, BTN1: also browse
      browseTodayStatus();
    }

    // Handle outputs in reminder states
    if (state != IDLE) {
      handleReminderOutputs();
    }
  } else {
    // RTC not valid yet: display something indicating time unknown
    // Keep stable: show 00:00
    tm.showNumberDecEx(0, 0b01000000, true);
    // Ensure outputs off
    noTone(PIN_BUZZER);
    digitalWrite(PIN_LED, LOW);

    // Allow browsing even without rtc: show scheduled times with confirmedToday defaults
    if (state == IDLE) {
      browseTodayStatus();
    }
  }
}
