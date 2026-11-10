#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ----- Pins (authoritative remap) -----
static const uint8_t PIN_BTN1 = D2;   // browse / confirm
static const uint8_t PIN_BTN2 = D3;   // set hour/minute / browse today status
static const uint8_t PIN_BTN3 = D4;   // set hour/minute / confirm
static const uint8_t PIN_LED  = D13;   // reminder LED
static const uint8_t PIN_BUZZ = D6;    // buzzer (tone output)
static const uint8_t PIN_TM_CLK = D7;  // TM1637 CLK
static const uint8_t PIN_TM_DIO = D5;  // TM1637 DIO

// ----- RTC via Serial -----
static const uint32_t SERIAL_BAUD = 9600;

// Time parsing formats (examples accepted):
// "YYYY-MM-DD HH:MM:SS"
// "YYYY/MM/DD HH:MM:SS"
// "HH:MM:SS"
// If only time is provided (no date), date is retained.
struct DateTime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};

static DateTime rtcNow = {2026, 1, 1, 0, 0, 0};
static unsigned long lastTickMs = 0;

// ----- Reminder schedule (3 times per day) -----
struct ReminderTime {
  int hour;
  int minute;
};

static ReminderTime reminders[3] = {
  {8, 0},
  {12, 0},
  {18, 0}
};

// Status per reminder for "today"
struct DoseStatus {
  bool alreadyTriggeredToday[3];
  bool confirmed[3];
  unsigned long triggeredAtMs[3]; // when alarm started (for 30s confirm window)
  int missedCountToday;            // how many missed today
};

static DoseStatus todayStatus;

// Alarm/UI state
enum UiMode : uint8_t {
  MODE_NORMAL = 0,
  MODE_SET_TIME_H = 1,    // edit hour of a selected reminder
  MODE_SET_TIME_M = 2,    // edit minute of a selected reminder
  MODE_SELECT_REM  = 3,   // select which reminder is being edited
  MODE_WAIT_CONFIRM = 4  // waiting for user confirmation
};

static UiMode uiMode = MODE_NORMAL;

// Which reminder is being edited/selected
static int selectedReminderIndex = 0;
static int confirmReminderIndex = -1;

// Browsing today reminder status
static bool browsing = false;
static unsigned long lastBrowseActionMs = 0;
static int browseIndex = 0; // 0..2

// Timing
static const unsigned long CONFIRM_WINDOW_MS = 30000UL;
static const uint16_t BUZZ_FREQ = 2000; // Hz
static const unsigned long BUZZ_ON_MS = 200;  // intermittent
static const unsigned long BUZZ_OFF_MS = 200;

// TM1637 setup
// Typical: 6 digits. We'll show HHMM in normal mode (and optionally MMDD or time segments).
static const uint8_t TM_MAX_BRIGHTNESS = 7;
static const uint8_t TM_DUMMY = 0;

// Using TM1637Display library
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

// Helpers: time calculations
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isLeapYear(int y);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int daysInMonth(int y, int m);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void advanceOneSecond(DateTime &dt);
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isTimeToTrigger(const DateTime &dt, const ReminderTime &rt);
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showHHMM(int hour, int minute);
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showMM(int minute);
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool tryParseDateTime(const String &line, DateTime &out);
#line 248 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleSerialRTCInput();
#line 272 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void resetTodayStatusIfNeeded();
#line 292 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readBtn(uint8_t pin);
#line 296 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool btnEdge(uint8_t pin, bool &lastState, unsigned long &lastDebounce);
#line 308 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool getButtonPressed(uint8_t pin, bool &prevState, unsigned long &debounceRef);
#line 326 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButtons();
#line 348 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool pressedFalling(bool stable, bool prevStable);
#line 354 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool shouldBuzzNow(unsigned long startMs);
#line 361 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void startReminderAlarm(int idx);
#line 370 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stopReminderAlarm();
#line 377 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void confirmDose(int idx);
#line 391 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleMissedWindowTimeout();
#line 415 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleAlarmPattern();
#line 441 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void triggerDueReminders();
#line 473 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void enterEditMode();
#line 479 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void exitEditMode();
#line 486 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void renderEditDisplay();
#line 497 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleBrowsing();
#line 506 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void announceBrowseStatus(int idx);
#line 522 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 542 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_004\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isLeapYear(int y) {
  if (y % 400 == 0) return true;
  if (y % 100 == 0) return false;
  return (y % 4 == 0);
}

static int daysInMonth(int y, int m) {
  static const int dpm[] = {31,28,31,30,31,30,31,31,30,31,30,31};
  if (m == 2) return dpm[m-1] + (isLeapYear(y) ? 1 : 0);
  return dpm[m-1];
}

static void advanceOneSecond(DateTime &dt) {
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

// Compare current time to a reminder time (hour/minute match)
static bool isTimeToTrigger(const DateTime &dt, const ReminderTime &rt) {
  return (dt.hour == rt.hour && dt.minute == rt.minute && dt.second == 0);
}

// Two-digit formatting for display
static void showHHMM(int hour, int minute) {
  int h = constrain(hour, 0, 23);
  int m = constrain(minute, 0, 59);
  // Display expects digits; use setSegments for reliability.
  // We'll use showNumberDecEx style with leading zeros.
  // Show as 4-digit: HHMM
  int value = h * 100 + m;
  display.showNumberDecEx(value, 0b01000000, true, 4, 0); // leftmost colon not used; decimal point mask ignored
}

static void showMM(int minute) {
  int m = constrain(minute, 0, 59);
  int value = m;
  display.showNumberDecEx(value, 0b01000000, true, 2, 2); // show last 2 digits
}

// Serial parsing
static String serialLineBuffer;

static bool tryParseDateTime(const String &line, DateTime &out) {
  // Try parse with sscanf-like patterns using String functions.
  // Accept "YYYY-MM-DD HH:MM:SS" or "YYYY/MM/DD HH:MM:SS"
  // Also accept "HH:MM:SS" (retain date).
  String s = line;
  s.trim();
  if (s.length() == 0) return false;

  // Extract digits in order
  auto getAt = [&](int idx)->int{
    return 0;
  };

  // Determine if it contains year (at least 4 digits at start)
  bool startsWithYear = (s.length() >= 4 && isDigit(s[0]) && isDigit(s[1]) && isDigit(s[2]) && isDigit(s[3]));
  if (startsWithYear) {
    int y = 0, mo = 0, d = 0, hh = 0, mm = 0, ss = 0;
    // Replace separators with spaces/colons to simplify parsing
    // Common cases:
    // YYYY-MM-DD HH:MM:SS
    // YYYY/MM/DD HH:MM:SS
    // We'll parse by manual tokenization: date part and time part separated by space.
    int sp = s.indexOf(' ');
    if (sp < 0) return false;
    String datePart = s.substring(0, sp);
    String timePart = s.substring(sp + 1);

    // Parse datePart: split by '-' or '/'
    int sep1 = datePart.indexOf('-');
    if (sep1 < 0) sep1 = datePart.indexOf('/');
    if (sep1 < 0) return false;
    int sep2 = datePart.indexOf(datePart[sep1], sep1 + 1);
    if (sep2 < 0) return false;

    String ys = datePart.substring(0, sep1);
    String ms = datePart.substring(sep1 + 1, sep2);
    String ds = datePart.substring(sep2 + 1);

    y = ys.toInt();
    mo = ms.toInt();
    d = ds.toInt();

    // Parse timePart: HH:MM:SS
    int c1 = timePart.indexOf(':');
    if (c1 < 0) return false;
    int c2 = timePart.indexOf(':', c1 + 1);
    if (c2 < 0) return false;

    String hhs = timePart.substring(0, c1);
    String mms = timePart.substring(c1 + 1, c2);
    String sss = timePart.substring(c2 + 1);

    hh = hhs.toInt();
    mm = mms.toInt();
    ss = sss.toInt();

    // Basic validation
    if (mo < 1 || mo > 12) return false;
    if (d < 1 || d > daysInMonth(y, mo)) return false;
    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;
    if (ss < 0 || ss > 59) return false;

    out.year = y;
    out.month = mo;
    out.day = d;
    out.hour = hh;
    out.minute = mm;
    out.second = ss;
    return true;
  } else {
    // Parse HH:MM:SS only
    int c1 = s.indexOf(':');
    if (c1 < 0) return false;
    int c2 = s.indexOf(':', c1 + 1);
    if (c2 < 0) return false;

    String hhs = s.substring(0, c1);
    String mms = s.substring(c1 + 1, c2);
    String sss = s.substring(c2 + 1);

    int hh = hhs.toInt();
    int mm = mms.toInt();
    int ss = sss.toInt();

    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;
    if (ss < 0 || ss > 59) return false;

    out = rtcNow; // retain date
    out.hour = hh;
    out.minute = mm;
    out.second = ss;
    return true;
  }
}

static void handleSerialRTCInput() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // line_or_idle with idle timeout: we accept newline or '|' or end when idle
    if (c == '\n' || c == '\r') {
      if (serialLineBuffer.length() > 0) {
        DateTime parsed;
        if (tryParseDateTime(serialLineBuffer, parsed)) {
          rtcNow = parsed;
          lastTickMs = millis();
        }
        serialLineBuffer = "";
      }
    } else {
      // avoid unbounded growth
      if (serialLineBuffer.length() < 80) serialLineBuffer += c;
    }
  }
}

// Day boundary reset
static int lastRecordedDay = -1;

static void resetTodayStatusIfNeeded() {
  int currentKey = rtcNow.year * 10000 + rtcNow.month * 100 + rtcNow.day;
  static int lastKey = -1;
  if (currentKey != lastKey) {
    lastKey = currentKey;

    for (int i = 0; i < 3; i++) {
      todayStatus.alreadyTriggeredToday[i] = false;
      todayStatus.confirmed[i] = false;
      todayStatus.triggeredAtMs[i] = 0;
    }
    todayStatus.missedCountToday = 0;
  }
}

// Buttons (simple debounced edge detection)
static bool lastBtn1 = HIGH, lastBtn2 = HIGH, lastBtn3 = HIGH;
static unsigned long lastDebounce1 = 0, lastDebounce2 = 0, lastDebounce3 = 0;
static const unsigned long DEBOUNCE_MS = 30;

static bool readBtn(uint8_t pin) {
  return digitalRead(pin);
}

static bool btnEdge(uint8_t pin, bool &lastState, unsigned long &lastDebounce) {
  bool current = readBtn(pin);
  unsigned long now = millis();
  if (current != lastState) {
    lastDebounce = now;
    lastState = current;
  }
  // Edge detect on falling edge (assuming buttons to GND; use INPUT_PULLUP)
  // If lastState updated immediately, we need separate logic; implement robustly:
  return false;
}

static bool getButtonPressed(uint8_t pin, bool &prevState, unsigned long &debounceRef) {
  bool current = readBtn(pin);
  unsigned long now = millis();
  if (current != prevState) {
    debounceRef = now;
    prevState = current;
  }
  if ((now - debounceRef) > DEBOUNCE_MS) {
    // pressed when goes LOW
    // We need "transition" detection: store stable state
  }
  return false;
}

// We'll implement with stable state tracking and event detection.
static bool stableBtn1 = HIGH, stableBtn2 = HIGH, stableBtn3 = HIGH;
static bool prevStableBtn1 = HIGH, prevStableBtn2 = HIGH, prevStableBtn3 = HIGH;

static void updateButtons() {
  unsigned long now = millis();

  auto debounceUpdate = [&](uint8_t pin, bool &rawLast, unsigned long &debT, bool &stable, bool &prevStable) {
    bool raw = readBtn(pin);
    if (raw != rawLast) {
      debT = now;
      rawLast = raw;
    }
    if ((now - debT) > DEBOUNCE_MS) {
      if (stable != raw) {
        prevStable = stable;
        stable = raw;
      }
    }
  };

  debounceUpdate(PIN_BTN1, lastBtn1, lastDebounce1, stableBtn1, prevStableBtn1);
  debounceUpdate(PIN_BTN2, lastBtn2, lastDebounce2, stableBtn2, prevStableBtn2);
  debounceUpdate(PIN_BTN3, lastBtn3, lastDebounce3, stableBtn3, prevStableBtn3);
}

static bool pressedFalling(bool stable, bool prevStable) {
  // Press when stable goes from HIGH to LOW
  return (prevStable == HIGH && stable == LOW);
}

// Reminder behavior / confirmation
static bool shouldBuzzNow(unsigned long startMs) {
  unsigned long elapsed = millis() - startMs;
  unsigned long cycle = BUZZ_ON_MS + BUZZ_OFF_MS;
  unsigned long pos = elapsed % cycle;
  return pos < BUZZ_ON_MS;
}

static void startReminderAlarm(int idx) {
  confirmReminderIndex = idx;
  uiMode = MODE_WAIT_CONFIRM;
  todayStatus.triggeredAtMs[idx] = millis();
  todayStatus.confirmed[idx] = false;
  digitalWrite(PIN_LED, HIGH); // start LED on; then pattern in loop
  tone(PIN_BUZZ, BUZZ_FREQ); // will be controlled below by pattern
}

static void stopReminderAlarm() {
  uiMode = MODE_NORMAL;
  confirmReminderIndex = -1;
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZ);
}

static void confirmDose(int idx) {
  todayStatus.confirmed[idx] = true;
  stopReminderAlarm();
  Serial.print(F("dose confirmed for "));
  Serial.print(idx + 1);
  Serial.print(F(" at "));
  Serial.print(rtcNow.year); Serial.print('-');
  Serial.print(rtcNow.month); Serial.print('-');
  Serial.print(rtcNow.day); Serial.print(' ');
  Serial.print(rtcNow.hour); Serial.print(':');
  Serial.print(rtcNow.minute); Serial.print(':');
  Serial.println(rtcNow.second);
}

static void handleMissedWindowTimeout() {
  if (uiMode != MODE_WAIT_CONFIRM) return;
  int idx = confirmReminderIndex;
  if (idx < 0 || idx > 2) return;

  unsigned long startMs = todayStatus.triggeredAtMs[idx];
  if (startMs == 0) return;

  if (millis() - startMs >= CONFIRM_WINDOW_MS) {
    // Missed
    Serial.print(F("missed dose "));
    Serial.print(idx + 1);
    Serial.print(F(" at "));
    Serial.print(rtcNow.hour); Serial.print(':');
    Serial.print(rtcNow.minute); Serial.print(':');
    Serial.println(rtcNow.second);

    todayStatus.missedCountToday++;

    // Allow further browsing; then stop alarm but keep triggered flag so it won't re-trigger today
    stopReminderAlarm();
  }
}

static void handleAlarmPattern() {
  if (uiMode != MODE_WAIT_CONFIRM) return;
  int idx = confirmReminderIndex;
  if (idx < 0 || idx > 2) return;

  unsigned long startMs = todayStatus.triggeredAtMs[idx];
  if (startMs == 0) return;

  if (todayStatus.confirmed[idx]) {
    stopReminderAlarm();
    return;
  }

  // Buzz intermittent
  if (shouldBuzzNow(startMs)) {
    tone(PIN_BUZZ, BUZZ_FREQ);
  } else {
    noTone(PIN_BUZZ);
  }

  // LED flash in sync with buzzer cadence (same on/off)
  unsigned long elapsed = millis() - startMs;
  unsigned long pos = elapsed % (BUZZ_ON_MS + BUZZ_OFF_MS);
  digitalWrite(PIN_LED, (pos < BUZZ_ON_MS) ? HIGH : LOW);
}

static void triggerDueReminders() {
  resetTodayStatusIfNeeded();

  if (uiMode == MODE_WAIT_CONFIRM) return; // waiting; don't trigger others during confirm window

  for (int i = 0; i < 3; i++) {
    if (!todayStatus.alreadyTriggeredToday[i]) {
      if (isTimeToTrigger(rtcNow, reminders[i])) {
        todayStatus.alreadyTriggeredToday[i] = true;
        startReminderAlarm(i);
      }
    }
  }
}

// Editing schedule mode (use buttons; preserve behavior with browsing capability)
// Mapping:
// - In MODE_NORMAL: If hold BTN2+BTN3 for quick entry? Not specified.
// We'll implement: Press BTN3 to enter edit/select mode, BTN2 to cycle reminder, BTN1 confirm.
// In wait-confirm mode: BTN1 confirms.
// In edit mode:
//   - BTN2 increments hour/minute depending on submode (MODE_SET_TIME_H/M)
//   - BTN3 toggles between hour/minute
//   - BTN1 exits to normal (and shows current time).
//
// Also "按钮可浏览今日提醒状态":
// In normal mode, pressing BTN2 briefly toggles browsing; BTN3 cycles browse index; BTN1 exits browsing.
// While browsing, display shows: 1) reminder number and its time (HHMM) alternation.
// We'll show HHMM for selected reminder time, and for status display we keep showing time; missed/confirmed printed to serial.

static bool editMode = false;

static void enterEditMode() {
  uiMode = MODE_SELECT_REM;
  editMode = true;
  selectedReminderIndex = 0;
}

static void exitEditMode() {
  editMode = false;
  uiMode = MODE_NORMAL;
  browsing = false;
  digitalWrite(PIN_LED, LOW);
}

static void renderEditDisplay() {
  if (uiMode == MODE_SELECT_REM) {
    // Show selected reminder hour/min as HHMM
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  } else if (uiMode == MODE_SET_TIME_H) {
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  } else if (uiMode == MODE_SET_TIME_M) {
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  }
}

static void handleBrowsing() {
  if (!browsing) return;

  // Display reminder time for browseIndex
  if (browseIndex < 0) browseIndex = 0;
  if (browseIndex > 2) browseIndex = 2;
  showHHMM(reminders[browseIndex].hour, reminders[browseIndex].minute);
}

static void announceBrowseStatus(int idx) {
  bool triggered = todayStatus.alreadyTriggeredToday[idx];
  bool confirmed = todayStatus.confirmed[idx];
  Serial.print(F("today reminder "));
  Serial.print(idx + 1);
  Serial.print(F(" scheduled "));
  Serial.print(reminders[idx].hour); Serial.print(':');
  if (reminders[idx].minute < 10) Serial.print('0');
  Serial.print(reminders[idx].minute);
  Serial.print(F(" status="));
  if (!triggered) Serial.print(F("pending"));
  else if (confirmed) Serial.print(F("confirmed"));
  else Serial.print(F("missed_or_waiting"));
  Serial.println();
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZ, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZ);

  display.setBrightness(TM_MAX_BRIGHTNESS);
  display.clear();

  Serial.begin(SERIAL_BAUD);

  // Initialize today status
  resetTodayStatusIfNeeded();
  lastTickMs = millis();
}

void loop() {
  // Keep RTC running (after serial sets it)
  handleSerialRTCInput();

  unsigned long nowMs = millis();
  if (nowMs - lastTickMs >= 1000UL) {
    // Increment by whole seconds to avoid drift
    unsigned long steps = (nowMs - lastTickMs) / 1000UL;
    lastTickMs += steps * 1000UL;
    for (unsigned long i = 0; i < steps; i++) advanceOneSecond(rtcNow);
    resetTodayStatusIfNeeded();
  }

  // Update buttons (debounce + stable edge)
  updateButtons();
  bool b1Pressed = pressedFalling(stableBtn1, prevStableBtn1);
  bool b2Pressed = pressedFalling(stableBtn2, prevStableBtn2);
  bool b3Pressed = pressedFalling(stableBtn3, prevStableBtn3);

  // Normal reminder trigger check (time==HH:MM:00)
  triggerDueReminders();

  // Alarm pattern + timeout handling
  handleAlarmPattern();
  handleMissedWindowTimeout();

  // If waiting for confirm: BTN1 confirms
  if (uiMode == MODE_WAIT_CONFIRM) {
    if (b1Pressed) {
      int idx = confirmReminderIndex;
      if (idx >= 0 && idx < 3) {
        todayStatus.confirmed[idx] = true;
        confirmDose(idx);
      } else {
        stopReminderAlarm();
      }
    }
    // During wait-confirm, ignore edit/browse buttons
    showHHMM(rtcNow.hour, rtcNow.minute);
    return;
  }

  // Browsing today status (only in normal mode)
  if (!editMode) {
    if (b2Pressed) {
      // Toggle browsing
      browsing = !browsing;
      if (browsing) {
        browseIndex = 0;
        announceBrowseStatus(browseIndex);
      } else {
        Serial.println(F("browse exit"));
      }
      lastBrowseActionMs = nowMs;
    }

    if (b3Pressed && browsing) {
      browseIndex++;
      if (browseIndex > 2) browseIndex = 0;
      announceBrowseStatus(browseIndex);
      lastBrowseActionMs = nowMs;
    }

    if (b1Pressed && browsing) {
      browsing = false;
      Serial.println(F("browse exit"));
    }

    if (browsing) {
      handleBrowsing();
      delay(10);
      return;
    }
  }

  // Enter edit mode
  if (!editMode) {
    if (b3Pressed && !b2Pressed && !b1Pressed) {
      // Use BTN3 press in normal mode to enter editing/select mode
      enterEditMode();
      uiMode = MODE_SELECT_REM;
      browsing = false;
      Serial.println(F("enter edit mode"));
    }
  }

  // Handle edit mode
  if (editMode) {
    if (uiMode == MODE_SELECT_REM) {
      // BTN2 cycles which reminder (0..2)
      if (b2Pressed) {
        selectedReminderIndex++;
        if (selectedReminderIndex > 2) selectedReminderIndex = 0;
        Serial.print(F("select reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      // BTN3 toggles edit field: hour -> minute
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_H;
        Serial.print(F("edit hour for reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      // BTN1 exits edit mode
      if (b1Pressed) {
        exitEditMode();
        renderEditDisplay();
      }
    } else if (uiMode == MODE_SET_TIME_H) {
      // BTN2 increments hour
      if (b2Pressed) {
        reminders[selectedReminderIndex].hour++;
        if (reminders[selectedReminderIndex].hour > 23) reminders[selectedReminderIndex].hour = 0;
        renderEditDisplay();
      }
      // BTN3 switches to minute edit
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_M;
        Serial.print(F("edit minute for reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      // BTN1 exits edit mode
      if (b1Pressed) {
        exitEditMode();
      }
    } else if (uiMode == MODE_SET_TIME_M) {
      // BTN2 increments minute
      if (b2Pressed) {
        reminders[selectedReminderIndex].minute++;
        if (reminders[selectedReminderIndex].minute > 59) reminders[selectedReminderIndex].minute = 0;
        renderEditDisplay();
      }
      // BTN3 goes back to hour edit
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_H;
        renderEditDisplay();
      }
      // BTN1 exits edit mode
      if (b1Pressed) {
        exitEditMode();
      }
    }
    delay(10);
    return;
  }

  // Normal display: show current time on TM1637
  showHHMM(rtcNow.hour, rtcNow.minute);

  delay(5);
}
