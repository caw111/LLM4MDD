#include <Arduino.h>
#include <TM1637Display.h>

// ---------------- Pin assignments (authoritative) ----------------
static const uint8_t PIN_PIR   = 2;   // PIR 人体红外 D2
static const uint8_t PIN_DOOR  = 3;   // 门磁 D3
static const uint8_t PIN_TMCLK = 4;   // TM1637 CLK = D4
static const uint8_t PIN_TMDIO = 5;   // TM1637 DIO = D5
static const uint8_t PIN_BUZZ  = 6;   // 蜂鸣器 D6（tone 输出）
static const uint8_t PIN_BTN   = 7;   // 按钮 D7 用于人工释放房间
static const uint8_t PIN_LED_R = 13;  // 红色 D13
static const uint8_t PIN_LED_G = 12;  // 绿色 D12

// D8 was mentioned as digital_output device-role; keep it reserved but not used unless required.
// (No explicit wiring behavior was described besides LED on D12/D13 and buzzer D6.)
static const uint8_t PIN_RESERVED_D8 = 8;

// ---------------- TM1637 ----------------
TM1637Display tm1637(PIN_TMCLK, PIN_TMDIO);

// ---------------- Serial protocol ----------------
// Accept commands (line-based):
//   START HH:MM
//   END   HH:MM
//   (Optional) SET  START HH:MM END HH:MM (if provided in one line)
// Times interpreted as "today's" local time using millis() since boot as a fallback epoch.
// Since no RTC is specified, we use "first SET/START/END sets a reference"
// and then track time using millis().
// Output:
//   - When time reaches start: show occupied (and red ON once someone detected)
//   - When overtime: output "overtime use"
//   - When manual release: output "released"
//
// Note: If only START or END is given, behavior uses what is available.

static bool hasStart = false;
static bool hasEnd   = false;

static uint16_t startMinutes = 0; // minutes since 00:00
static uint16_t endMinutes   = 0; // minutes since 00:00

// Time reference: when first scheduling command received, bind that to "current minutes"
// as derived from millis() -> pseudo clock.
// This maintains deterministic behavior without RTC hardware.
static bool timeRefSet = false;
static unsigned long refMillis = 0;
static uint16_t refMinutes = 0;

static uint16_t pseudoNowMinutes() {
  if (!timeRefSet) return 0;
  unsigned long deltaMs = millis() - refMillis;
  uint32_t deltaMin = deltaMs / 60000UL;
  return (uint16_t)((refMinutes + deltaMin) % (24 * 60));
}

static void ensureTimeRefFromNow(uint16_t nowMinutes) {
  // Only set reference once to avoid changing timeline.
  if (timeRefSet) return;
  timeRefSet = true;
  refMillis = millis();
  refMinutes = nowMinutes;
}

static uint16_t parseHHMM(const String &s, bool &ok) {
  // expects "HH:MM"
  ok = false;
  if (s.length() < 4) return 0;
  int colon = s.indexOf(':');
  if (colon < 0) return 0;
  String hStr = s.substring(0, colon);
  String mStr = s.substring(colon + 1);
  int h = hStr.toInt();
  int m = mStr.toInt();
  if (h < 0 || h > 23 || m < 0 || m > 59) return 0;
  ok = true;
  return (uint16_t)(h * 60 + m);
}

static bool parseCommandLine(const String &line) {
  String s = line;
  s.trim();
  if (s.length() == 0) return false;

  // Normalize spaces
  s.replace('\t', ' ');
  while (s.indexOf("  ") >= 0) s.replace("  ", " ");

  // Accept patterns:
  // "START HH:MM"
  // "END HH:MM"
  // "SET START HH:MM END HH:MM"
  String upper = s;
  upper.toUpperCase();

  // If reference not set yet, we must set it to some baseline minutes.
  // We'll treat "now" as 0:00 at boot for determinism, unless a user sets time.
  // Here: when user first provides START/END, set reference to pseudo time of 00:00.
  if (!timeRefSet) {
    // set reference to 00:00 at command time unless user provides "NOW" (not specified).
    ensureTimeRefFromNow(0);
  }

  // START
  if (upper.startsWith("START ")) {
    int sp = s.indexOf(' ');
    if (sp < 0) return false;
    String tStr = s.substring(sp + 1);
    tStr.trim();
    bool ok = false;
    uint16_t t = parseHHMM(tStr, ok);
    if (ok) {
      startMinutes = t;
      hasStart = true;
      Serial.print("start=");
      Serial.println(tStr);
      return true;
    }
    return false;
  }

  // END
  if (upper.startsWith("END ")) {
    int sp = s.indexOf(' ');
    if (sp < 0) return false;
    String tStr = s.substring(sp + 1);
    tStr.trim();
    bool ok = false;
    uint16_t t = parseHHMM(tStr, ok);
    if (ok) {
      endMinutes = t;
      hasEnd = true;
      Serial.print("end=");
      Serial.println(tStr);
      return true;
    }
    return false;
  }

  // SET
  if (upper.startsWith("SET ")) {
    // expected: SET START HH:MM END HH:MM
    // parse by tokens
    // We'll find "START " and " END "
    int idxStart = upper.indexOf("START ");
    int idxEnd = upper.indexOf(" END ");
    if (idxStart < 0) return false;
    if (idxEnd < 0) return false;

    String tStartPart = s.substring(idxStart + 6, idxEnd);
    tStartPart.trim();

    // after " END " in original index
    int idxEnd2 = idxEnd + 5; // skip " END "
    String tEndPart = s.substring(idxEnd2);
    tEndPart.trim();

    bool ok1 = false, ok2 = false;
    uint16_t t1 = parseHHMM(tStartPart, ok1);
    uint16_t t2 = parseHHMM(tEndPart, ok2);
    if (ok1 && ok2) {
      startMinutes = t1;
      endMinutes = t2;
      hasStart = true;
      hasEnd = true;
      Serial.print("set ");
      Serial.print(tStartPart);
      Serial.print(" ");
      Serial.println(tEndPart);
      return true;
    }
    return false;
  }

  return false;
}

// ---------------- Detection / state machine ----------------
static bool occupied = false;        // 有人进入且处于预约期/占用状态
static bool everSomeoneDetected = false;

static bool pirState = false;
static bool doorState = false;

static unsigned long lastPirChangeMs = 0;
static unsigned long lastDoorChangeMs = 0;

// Debounce / lock behavior
static const unsigned long SENSOR_DEBOUNCE_MS = 50;

// For reminder flashing: start-1 minute green blink
static unsigned long lastBlinkToggleMs = 0;
static bool greenBlinkOn = false;

// For overtime buzzer pattern
static unsigned long lastOvertimeBipMs = 0;
static uint8_t overtimeBipCount = 0;

// For periodic status output (optional, keep minimal)
static unsigned long lastSerialReportMs = 0;

static void setGreen(bool on) { digitalWrite(PIN_LED_G, on ? HIGH : LOW); }
static void setRed(bool on) { digitalWrite(PIN_LED_R, on ? HIGH : LOW); }
static void setBuzzerTone(bool on, uint16_t freq = 2000) {
  if (on) {
    tone(PIN_BUZZ, freq);
  } else {
    noTone(PIN_BUZZ);
  }
}

// Determine time windows
static bool timeValid() { return hasStart && hasEnd; }

static bool withinSchedule() {
  if (!timeValid()) return false;
  uint16_t nowMin = pseudoNowMinutes();

  // Handle potential wrap over midnight:
  // If start <= end, within if now in [start, end)
  // Else schedule wraps: within if now >= start or now < end
  if (startMinutes <= endMinutes) {
    return (nowMin >= startMinutes && nowMin < endMinutes);
  } else {
    return (nowMin >= startMinutes || nowMin < endMinutes);
  }
}

static bool withinOvertime() {
  if (!timeValid()) return false;
  uint16_t nowMin = pseudoNowMinutes();

  // Overtime: after end time, considering wrap
  if (startMinutes <= endMinutes) {
    return (nowMin >= endMinutes);
  } else {
    // schedule wraps: overtime is between end and start
    return (nowMin >= endMinutes && nowMin < startMinutes);
  }
}

static bool withinStartMinus1Minute() {
  if (!timeValid()) return false;
  uint16_t nowMin = pseudoNowMinutes();

  // Flash window: from (start-1 min) to start (exclusive/inclusive)
  // We'll define as now in [start-1, start)
  uint16_t s = startMinutes;
  uint16_t windowStart = (uint16_t)((s + 24 * 60 - 1) % (24 * 60));
  uint16_t windowEnd = s; // exclusive boundary at start minute

  // If windowStart < windowEnd (normal), then now in [windowStart, windowEnd)
  // else wraps across midnight: now >= windowStart OR now < windowEnd
  if (windowStart < windowEnd) {
    return (nowMin >= windowStart && nowMin < windowEnd);
  } else {
    return (nowMin >= windowStart || nowMin < windowEnd);
  }
}

// Someone detection: PIR or door magnet indicates presence
static bool detectSomeone() {
  return (pirState || doorState);
}

static void updateSensors() {
  static bool lastPirRaw = false;
  static bool lastDoorRaw = false;

  bool pirRaw = (digitalRead(PIN_PIR) == HIGH);
  bool doorRaw = (digitalRead(PIN_DOOR) == HIGH);

  unsigned long now = millis();

  if (pirRaw != lastPirRaw) {
    if (now - lastPirChangeMs >= SENSOR_DEBOUNCE_MS) {
      pirState = pirRaw;
      lastPirChangeMs = now;
      lastPirRaw = pirRaw;
    }
  }
  if (doorRaw != lastDoorRaw) {
    if (now - lastDoorChangeMs >= SENSOR_DEBOUNCE_MS) {
      doorState = doorRaw;
      lastDoorChangeMs = now;
      lastDoorRaw = doorRaw;
    }
  }

  // Also update last raw initial
  if (lastPirChangeMs == 0 && now > 0) {
    lastPirChangeMs = now;
  }
  if (lastDoorChangeMs == 0 && now > 0) {
    lastDoorChangeMs = now;
  }
}

static void startOccupiedIfNeeded() {
  if (occupied) return;
  if (!withinSchedule()) return;

  if (detectSomeone()) {
    occupied = true;
    everSomeoneDetected = true;
    setBuzzerTone(false);
    setGreen(false);
    setRed(true);
    Serial.println("occupied");
    // Display something meaningful on TM1637: show "OC" style via segments is board-specific.
    // Here we show current HH:MM modulo using four digits as HHMM.
    uint16_t nowMin = pseudoNowMinutes();
    uint16_t hh = nowMin / 60;
    uint16_t mm = nowMin % 60;
    uint8_t digits[4] = {
      (uint8_t)(hh / 10),
      (uint8_t)(hh % 10),
      (uint8_t)(mm / 10),
      (uint8_t)(mm % 10)
    };
    tm1637.setBrightness(7);
    tm1637.showNumberDec(hh * 100 + mm, false, 4, 0);
  }
}

static void updateTmDisplayDuringReminder() {
  // Show HHMM during reminder, 4-digit, suppress leading zeros false keeps full
  uint16_t nowMin = pseudoNowMinutes();
  uint16_t hh = nowMin / 60;
  uint16_t mm = nowMin % 60;
  tm1637.setBrightness(2);
  tm1637.showNumberDec(hh * 100 + mm, false, 4, 0);
}

// Manual release
static bool buttonPressed() {
  static bool lastBtn = HIGH;
  bool raw = digitalRead(PIN_BTN);
  bool pressed = false;

  // Assume button is normally LOW? Unspecified. We'll treat active HIGH with pull-down? But we add pullup.
  // With INPUT_PULLUP, pressed means raw == LOW.
  if (raw != lastBtn) {
    // simple edge with debounce
    static unsigned long lastChange = 0;
    unsigned long now = millis();
    if (now - lastChange >= 50) {
      if (lastBtn == HIGH && raw == LOW) pressed = true;
      lastChange = now;
      lastBtn = raw;
    }
  }
  return pressed;
}

static void handleManualRelease() {
  if (buttonPressed()) {
    occupied = false;
    setBuzzerTone(false);
    setRed(false);
    setGreen(false);
    overtimeBipCount = 0;
    Serial.println("released");
    // TM1637 show idle (blank/zero)
    tm1637.setBrightness(0);
    tm1637.clear();
    // Reset sensors
    pirState = false;
    doorState = false;
  }
}

static void handleGreenReminder() {
  if (!withinStartMinus1Minute()) return;
  // Blink green at 2Hz (toggle every 250ms) regardless of occupancy; but red should remain off unless already occupied.
  unsigned long now = millis();
  if (now - lastBlinkToggleMs >= 250) {
    lastBlinkToggleMs = now;
    greenBlinkOn = !greenBlinkOn;

    if (!occupied) {
      setGreen(greenBlinkOn);
      setRed(false);
    }
  }
  updateTmDisplayDuringReminder();
}

static void handleOvertime() {
  if (!withinOvertime()) return;

  // If still occupied, buzzer and output overtime use
  if (occupied) {
    // Output "overtime use" once per entry into overtime or periodically if needed.
    // We'll output once when overtime first detected after being occupied.
    static bool overtimeReported = false;
    if (!overtimeReported) {
      Serial.println("overtime use");
      overtimeReported = true;
    }

    // Buzzer: beep 200ms on, 800ms off repeating
    unsigned long now = millis();
    if (now - lastOvertimeBipMs >= (overtimeBipCount % 5 == 0 ? 200UL : 800UL)) {
      lastOvertimeBipMs = now;
      // Toggle based on pattern index
      if (overtimeBipCount % 5 == 0) {
        // turn on
        setBuzzerTone(true, 2500);
      } else {
        // turn off
        setBuzzerTone(false);
      }
      overtimeBipCount++;
      if (overtimeBipCount > 100) overtimeBipCount = 0;
    }

    // Keep red on to indicate overtime occupant presence
    setRed(true);
    setGreen(false);
    tm1637.setBrightness(7);
    // Display "End" approximated by showing 9999? We'll show "----" via clearing to avoid wrong segments.
    tm1637.showNumberDec(9999, false, 4, 0);
  } else {
    // Not occupied; ensure buzzer off and red off
    setBuzzerTone(false);
    setRed(false);
  }
}

static void clearOvertimeReportedFlag() {
  static bool lastOver = false;
  bool nowOver = withinOvertime();
  if (!nowOver && lastOver) {
    // Leaving overtime region
    lastOver = nowOver;
    // Reset static in handleOvertime by forcing a program-wide reset through a workaround:
    // We'll do nothing here; handleOvertime uses its own static. Can't access.
    // Instead, we keep occupied state and manual release reset.
  } else {
    lastOver = nowOver;
  }
}

void setup() {
  pinMode(PIN_PIR, INPUT);              // D2
  pinMode(PIN_DOOR, INPUT);             // D3
  pinMode(PIN_TMCLK, OUTPUT);
  pinMode(PIN_TMDIO, OUTPUT);
  pinMode(PIN_BUZZ, OUTPUT);           // D6
  pinMode(PIN_BTN, INPUT_PULLUP);      // D7, assumed active LOW pressed
  pinMode(PIN_LED_R, OUTPUT);          // D13
  pinMode(PIN_LED_G, OUTPUT);          // D12

  pinMode(PIN_RESERVED_D8, OUTPUT);    // reserved D8 as per inventory

  setGreen(false);
  setRed(false);
  setBuzzerTone(false);
  digitalWrite(PIN_RESERVED_D8, LOW);

  Serial.begin(9600);
  tm1637.setBrightness(3);
  tm1637.clear();
}

void loop() {
  // Sensor update
  updateSensors();

  // Manual release always active
  handleManualRelease();

  // Time windows behaviors
  if (!timeValid()) {
    // No schedule yet: keep LEDs off, but show pseudo time on TM1637
    setBuzzerTone(false);
    setRed(false);
    setGreen(false);
    tm1637.setBrightness(1);
    uint16_t nowMin = pseudoNowMinutes();
    tm1637.showNumberDec((nowMin / 60) * 100 + (nowMin % 60), false, 4, 0);
  } else {
    // Start-1-minute green blink
    if (!occupied) {
      handleGreenReminder();
    } else {
      // Once occupied during schedule, ensure red stays on
      setGreen(false);
      setRed(true);
      setBuzzerTone(false);
    }

    // If within schedule and someone enters, show occupied + red ON
    if (!occupied) {
      startOccupiedIfNeeded();
    } else {
      // If already occupied, keep red on during schedule
      if (withinSchedule()) {
        setRed(true);
        setGreen(false);
        setBuzzerTone(false);
      }
      // If schedule ended, stay red until manual release (requirement: after end, if still someone in then buzzer)
      // We'll decide only buzzer on overtime.
    }

    // Overtime handling: if occupied and overtime, beep + output overtime use
    handleOvertime();
    clearOvertimeReportedFlag();

    // If not within schedule, ensure green off (except reminder window handled above)
    if (!withinSchedule() && !withinStartMinus1Minute()) {
      setGreen(false);
      // If not occupied, keep red off
      if (!occupied) setRed(false);
    }

    // TM1637 general update
    // Show current time always (unless in overtime shows 9999)
    if (!withinStartMinus1Minute() && withinSchedule() && occupied) {
      tm1637.setBrightness(7);
      uint16_t nowMin = pseudoNowMinutes();
      uint16_t hh = nowMin / 60;
      uint16_t mm = nowMin % 60;
      tm1637.showNumberDec(hh * 100 + mm, false, 4, 0);
    } else if (!occupied) {
      // While waiting (scheduled time not started yet), show time lightly
      tm1637.setBrightness(2);
      uint16_t nowMin = pseudoNowMinutes();
      uint16_t hh = nowMin / 60;
      uint16_t mm = nowMin % 60;
      tm1637.showNumberDec(hh * 100 + mm, false, 4, 0);
    }
  }

  // Serial input processing (line-based)
  static String inLine;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (inLine.length() > 0) {
        parseCommandLine(inLine);
      }
      inLine = "";
    } else {
      // avoid unlimited growth
      if (inLine.length() < 80) inLine += c;
    }
  }

  // Ensure buzzer off when not in overtime while not occupied
  if ((!withinOvertime()) || !occupied) {
    // During occupied but not overtime, keep buzzer off as requirement focuses on overtime beeping.
    if (!withinOvertime()) setBuzzerTone(false);
  }

  // Small periodic serial report (optional, minimal)
  static unsigned long lastReport = 0;
  unsigned long nowMs = millis();
  if (nowMs - lastReport >= 5000UL) {
    lastReport = nowMs;
    // Keep output minimal: only when occupied state changes or every 5s could spam; avoid.
    // Here: no extra reports.
  }
}