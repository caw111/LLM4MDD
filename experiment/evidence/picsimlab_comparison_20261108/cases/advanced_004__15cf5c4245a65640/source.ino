#include <Arduino.h>
#include <TM1637Display.h>

// ----- Pin mapping -----
// Authoritative pin assignments per requirement:
// CLK=D7, DIO=D5, buttons D2/D3/D4, LED D13, buzzer D6.
// Arduino Uno uses numeric pins for compilation.
static const uint8_t PIN_BTN1   = 2;   // D2
static const uint8_t PIN_BTN2   = 3;   // D3
static const uint8_t PIN_BTN3   = 4;   // D4
static const uint8_t PIN_LED    = 13;  // D13
static const uint8_t PIN_BUZZ   = 6;   // D6 (tone output)
static const uint8_t PIN_TM_CLK = 7;  // D7
static const uint8_t PIN_TM_DIO = 5;  // D5

// ----- RTC via Serial -----
static const uint32_t SERIAL_BAUD = 9600;

// Time parsing formats (examples accepted):
// "YYYY-MM-DD HH:MM:SS"
// "YYYY/MM/DD HH:MM:SS"
// "HH:MM:SS"  (date retained)

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
  MODE_SET_TIME_H = 1,    // edit hour of a selected reminder (kept for compatibility)
  MODE_SET_TIME_M = 2,    // edit minute of a selected reminder (kept for compatibility)
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
static const uint8_t TM_MAX_BRIGHTNESS = 7;
static const uint8_t TM_DUMMY = 0;

TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

// Helpers: time calculations
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

// Two-digit formatting for display: show HHMM as 4 digits
static void showHHMM(int hour, int minute) {
  int h = constrain(hour, 0, 23);
  int m = constrain(minute, 0, 59);
  int value = h * 100 + m;

  // Using decimal point mask 0b01000000 and showing 4 digits.
  // TM1637Display.showNumberDecEx(number, dots, leadingZero, length, pos)
  display.showNumberDecEx(value, 0b01000000, true, 4, 0);
}

// Show last 2 digits (used originally; kept though not used in final behavior)
static void showMM(int minute) {
  int m = constrain(minute, 0, 59);
  int value = m;
  display.showNumberDecEx(value, 0b01000000, true, 2, 2);
}

// Serial parsing
static String serialLineBuffer;

static bool tryParseDateTime(const String &line, DateTime &out) {
  String s = line;
  s.trim();
  if (s.length() == 0) return false;

  auto isDigitCh = [&](char c) -> bool { return (c >= '0' && c <= '9'); };

  bool startsWithYear = (s.length() >= 4 && isDigitCh(s[0]) && isDigitCh(s[1]) && isDigitCh(s[2]) && isDigitCh(s[3]));
  auto daysInMonthLocal = [&](int y, int mo) -> int { return daysInMonth(y, mo); };

  if (startsWithYear) {
    int y = 0, mo = 0, d = 0, hh = 0, mm = 0, ss = 0;

    int sp = s.indexOf(' ');
    if (sp < 0) return false;

    String datePart = s.substring(0, sp);
    String timePart = s.substring(sp + 1);

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

    if (mo < 1 || mo > 12) return false;
    if (d < 1 || d > daysInMonthLocal(y, mo)) return false;
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

// Buttons (simple debounced stable-state edge detection)
static bool lastBtn1 = HIGH, lastBtn2 = HIGH, lastBtn3 = HIGH;
static unsigned long lastDebounce1 = 0, lastDebounce2 = 0, lastDebounce3 = 0;
static const unsigned long DEBOUNCE_MS = 30;

static bool stableBtn1 = HIGH, stableBtn2 = HIGH, stableBtn3 = HIGH;
static bool prevStableBtn1 = HIGH, prevStableBtn2 = HIGH, prevStableBtn3 = HIGH;

static bool readBtn(uint8_t pin) {
  return digitalRead(pin);
}

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
  tone(PIN_BUZZ, BUZZ_FREQ);
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
  Serial.print(rtcNow.second);
  Serial.println();
}

static void handleMissedWindowTimeout() {
  if (uiMode != MODE_WAIT_CONFIRM) return;
  int idx = confirmReminderIndex;
  if (idx < 0 || idx > 2) return;

  unsigned long startMs = todayStatus.triggeredAtMs[idx];
  if (startMs == 0) return;

  if (millis() - startMs >= CONFIRM_WINDOW_MS) {
    Serial.print(F("missed dose "));
    Serial.print(idx + 1);
    Serial.print(F(" at "));
    Serial.print(rtcNow.hour); Serial.print(':');
    Serial.print(rtcNow.minute); Serial.print(':');
    Serial.print(rtcNow.second);
    Serial.println();

    todayStatus.missedCountToday++;

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

  if (shouldBuzzNow(startMs)) {
    tone(PIN_BUZZ, BUZZ_FREQ);
  } else {
    noTone(PIN_BUZZ);
  }

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

// Editing schedule mode + browsing status
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
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  } else if (uiMode == MODE_SET_TIME_H) {
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  } else if (uiMode == MODE_SET_TIME_M) {
    showHHMM(reminders[selectedReminderIndex].hour, reminders[selectedReminderIndex].minute);
  }
}

static void handleBrowsing() {
  if (!browsing) return;
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

  resetTodayStatusIfNeeded();
  lastTickMs = millis();
}

void loop() {
  handleSerialRTCInput();

  unsigned long nowMs = millis();
  if (nowMs - lastTickMs >= 1000UL) {
    unsigned long steps = (nowMs - lastTickMs) / 1000UL;
    lastTickMs += steps * 1000UL;
    for (unsigned long i = 0; i < steps; i++) advanceOneSecond(rtcNow);
    resetTodayStatusIfNeeded();
  }

  updateButtons();
  bool b1Pressed = pressedFalling(stableBtn1, prevStableBtn1);
  bool b2Pressed = pressedFalling(stableBtn2, prevStableBtn2);
  bool b3Pressed = pressedFalling(stableBtn3, prevStableBtn3);

  triggerDueReminders();
  handleAlarmPattern();
  handleMissedWindowTimeout();

  // Confirm current reminder
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
    showHHMM(rtcNow.hour, rtcNow.minute);
    return;
  }

  // Browsing today reminders (normal mode only)
  if (!editMode) {
    if (b2Pressed) {
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

  // Enter edit mode (BTN3 press in normal mode)
  if (!editMode) {
    if (b3Pressed && !b2Pressed && !b1Pressed) {
      enterEditMode();
      uiMode = MODE_SELECT_REM;
      browsing = false;
      Serial.println(F("enter edit mode"));
    }
  }

  // Edit mode handling
  if (editMode) {
    if (uiMode == MODE_SELECT_REM) {
      if (b2Pressed) {
        selectedReminderIndex++;
        if (selectedReminderIndex > 2) selectedReminderIndex = 0;
        Serial.print(F("select reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_H;
        Serial.print(F("edit hour for reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      if (b1Pressed) {
        exitEditMode();
        renderEditDisplay();
      }
    } else if (uiMode == MODE_SET_TIME_H) {
      if (b2Pressed) {
        reminders[selectedReminderIndex].hour++;
        if (reminders[selectedReminderIndex].hour > 23) reminders[selectedReminderIndex].hour = 0;
        renderEditDisplay();
      }
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_M;
        Serial.print(F("edit minute for reminder "));
        Serial.println(selectedReminderIndex + 1);
        renderEditDisplay();
      }
      if (b1Pressed) {
        exitEditMode();
      }
    } else if (uiMode == MODE_SET_TIME_M) {
      if (b2Pressed) {
        reminders[selectedReminderIndex].minute++;
        if (reminders[selectedReminderIndex].minute > 59) reminders[selectedReminderIndex].minute = 0;
        renderEditDisplay();
      }
      if (b3Pressed) {
        uiMode = MODE_SET_TIME_H;
        renderEditDisplay();
      }
      if (b1Pressed) {
        exitEditMode();
      }
    }
    delay(10);
    return;
  }

  // Normal display: current time HHMM
  showHHMM(rtcNow.hour, rtcNow.minute);
  delay(5);
}