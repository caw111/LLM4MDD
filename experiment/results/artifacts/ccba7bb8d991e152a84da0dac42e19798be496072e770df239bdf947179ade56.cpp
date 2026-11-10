#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_PIR = 2;      // D2 PIR
static const uint8_t PIN_DOOR = 3;     // D3 门磁
static const uint8_t PIN_TM_CLK = 4;    // D4 TM1637 CLK
static const uint8_t PIN_TM_DIO = 5;    // D5 TM1637 DIO
static const uint8_t PIN_BUZZER = 6;    // D6 蜂鸣器
static const uint8_t PIN_GREEN_LED = 12; // D12 绿色LED
static const uint8_t PIN_RED_LED = 13;   // D13 红色LED
static const uint8_t PIN_RELEASE = 7;   // D7 按钮(人工释放)

static const uint8_t TM_DISPLAY_UPDATE_MS = 200;

TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

enum RoomState : uint8_t {
  IDLE = 0,
  PRENOTICE,      // 到预约开始前1分钟闪绿
  OCCUPIED,       // 有人进入后 occupied，红灯常亮
  OVERTIME        // 预约结束后若仍有人在内蜂鸣并输出 overtime use
};

static RoomState state = IDLE;

// ---- Serial time handling (MM:SS or HH:MM:SS not specified; use HH:MM) ----
// Requirement: "串口可输入预约开始和结束时间；"
//
// We implement a robust parser that accepts:
// - Start: "S=HH:MM" or "START HH:MM"
// - End:   "E=HH:MM" or "END HH:MM"
// Also allows minutes since midnight as integer: "S=123" meaning 02:03.
static bool haveStart = false;
static bool haveEnd = false;
static uint16_t startMin = 0; // minutes since 00:00
static uint16_t endMin = 0;   // minutes since 00:00

// Convert current time. Since no RTC is provided, we derive time from millis since boot.
// This is acceptable for relative demo behavior; if host sends times, system assumes "current time" is since last reset.
static uint32_t bootMs = 0;
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t nowMin();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readPIR();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDoor();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLEDs(bool green, bool red);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerTone(uint16_t freq, uint16_t durationMs);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerStop();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool parseTimeToMinutes(const String &t, uint16_t &outMin);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleSerialLine(const String &line);
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateInRoomState();
#line 236 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayStateForTime(RoomState st);
#line 278 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void enterState(RoomState newState);
#line 309 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 334 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_022\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t nowMin() {
  uint32_t ms = millis();
  uint32_t totalSec = ms / 1000UL;
  uint16_t minutes = (totalSec / 60UL) % (24UL * 60UL);
  return minutes;
}

static uint32_t lastDisplayUpdate = 0;
static uint32_t lastGreenToggle = 0;
static bool greenOn = false;

static bool pirTriggered = false;
static bool doorTriggered = false;
static bool inRoom = false; // occupancy inferred from PIR/door sequence
static uint32_t occStartMs = 0;
static uint32_t lastOvertimeBeepMs = 0;
static uint8_t overtimeBeepCount = 0;

static bool releasePressed = false;
static uint32_t lastReleaseSampleMs = 0;
static uint32_t releaseDebounceMs = 30;

// For preventing repeated overtime messages
static bool overtimeNotified = false;

// ---- Helpers for inputs ----
static bool readPIR() {
  // D2 PIR digital input: HIGH means motion detected (occupied activity).
  return digitalRead(PIN_PIR) == HIGH;
}

static bool readDoor() {
  // D3 door magnetic: common is HIGH when closed? Requirement says "门磁 D3" -> door state.
  // We preserve logic as follows:
  // - doorOpen is HIGH => door open (person entering/exists).
  // - We'll treat any door state change as "door activity"; occupancy uses PIR primary plus last door.
  return digitalRead(PIN_DOOR) == HIGH;
}

static void setLEDs(bool green, bool red) {
  digitalWrite(PIN_GREEN_LED, green ? HIGH : LOW);
  digitalWrite(PIN_RED_LED, red ? HIGH : LOW);
}

static void buzzerTone(uint16_t freq, uint16_t durationMs) {
  tone(PIN_BUZZER, freq, durationMs);
}

static void buzzerStop() {
  noTone(PIN_BUZZER);
}

static bool parseTimeToMinutes(const String &t, uint16_t &outMin) {
  // Accept "HH:MM" or "MM" (integer) or "H:MM"
  String s = t;
  s.trim();
  if (s.length() == 0) return false;

  int colon = s.indexOf(':');
  if (colon >= 0) {
    String a = s.substring(0, colon);
    String b = s.substring(colon + 1);
    int h = a.toInt();
    int m = b.toInt();
    if (h < 0 || h > 23 || m < 0 || m > 59) return false;
    outMin = (uint16_t)(h * 60 + m);
    return true;
  } else {
    // integer minutes since midnight
    long v = s.toInt();
    if (v < 0 || v >= 24L * 60L) return false;
    outMin = (uint16_t)v;
    return true;
  }
}

static void handleSerialLine(const String &line) {
  // Examples:
  // START 09:30
  // END 10:15
  // S=09:30
  // E=615   (minutes)
  String s = line;
  s.trim();
  if (s.length() == 0) return;

  // Find key
  // If contains '=', split on first '='
  int eq = s.indexOf('=');
  if (eq >= 0 && eq < (int)s.length() - 1) {
    String key = s.substring(0, eq);
    String val = s.substring(eq + 1);
    key.trim(); val.trim();
    uint16_t minutes;
    if (key.length() > 0 && parseTimeToMinutes(val, minutes)) {
      char k0 = key.charAt(0);
      if (k0 == 'S' || k0 == 's') {
        startMin = minutes;
        haveStart = true;
      } else if (k0 == 'E' || k0 == 'e') {
        endMin = minutes;
        haveEnd = true;
      }
    }
    return;
  }

  // Else use whitespace tokens
  // Try to locate START/END
  String upper = s;
  upper.toUpperCase();
  int idxStart = upper.indexOf("START");
  int idxEnd = upper.indexOf("END");
  if (idxStart >= 0) {
    // Take substring after START
    String val = s.substring(idxStart + 5);
    uint16_t minutes;
    if (parseTimeToMinutes(val, minutes)) {
      startMin = minutes;
      haveStart = true;
    }
    return;
  }
  if (idxEnd >= 0) {
    String val = s.substring(idxEnd + 3);
    uint16_t minutes;
    if (parseTimeToMinutes(val, minutes)) {
      endMin = minutes;
      haveEnd = true;
    }
    return;
  }

  // Also allow "S 09:30" / "E 10:15"
  if (upper.startsWith("S") || upper.startsWith("E")) {
    char k0 = upper.charAt(0);
    String val = s.substring(1);
    uint16_t minutes;
    if (parseTimeToMinutes(val, minutes)) {
      if (k0 == 'S') { startMin = minutes; haveStart = true; }
      else { endMin = minutes; haveEnd = true; }
    }
    return;
  }
}

static void updateInRoomState() {
  // Occupancy detection:
  // - PIR triggered => inRoom true.
  // - If PIR is not triggered but door activity indicates exit,
  //   we allow occupancy to clear when door indicates "open" after being occupied.
  //
  // Because magnet polarity is unknown, we implement:
  // - If PIR has been triggered recently => assume occupied.
  // - If door has been active and PIR not active for a short time => clear.
  static uint32_t lastPIRMs = 0;
  static uint32_t lastDoorActivityMs = 0;

  bool pir = readPIR();
  bool door = readDoor();

  if (pir) {
    pirTriggered = true;
    lastPIRMs = millis();
  }

  // Treat any door state read change as activity:
  // Track last state to identify activity
  static bool lastDoor = false;
  if (door != lastDoor) {
    lastDoorActivityMs = millis();
    lastDoor = door;
  } else {
    // also consider that door being HIGH continuously is activity; keep it light
    // by not updating.
  }
  doorTriggered = true;

  uint32_t now = millis();
  bool pirRecent = (now - lastPIRMs) < 5000UL; // 5s activity window

  // If currently no PIR activity and door had activity in the last 8s, clear.
  // This approximates "someone left through door".
  bool doorRecent = (now - lastDoorActivityMs) < 8000UL;

  if (pirRecent) {
    inRoom = true;
  } else if (inRoom && !pirRecent && doorRecent) {
    // allow a release/door-based clear
    inRoom = false;
    pirTriggered = false;
    doorTriggered = false;
  }
}

static void displayStateForTime(RoomState st) {
  // TM1637: show different 4-digit content using segments.
  // We'll display:
  // - PRENOTICE: "----" with flashing handled by LEDs; show "PING" like "1-0-" is hard; keep numeric:
  //   show remaining seconds until start in MMSS if possible
  // - OCCUPIED: show "OCUP" not possible; show "1010" pattern
  // - OVERTIME: show "HOT!" not possible; show "2222"
  // - IDLE: show "0000"
  uint16_t cur = nowMin();

  auto show4 = [&](uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
    // Write custom 4 digits
    tm1637.setSegments((uint8_t*)nullptr); // no-op placeholder, but TM1637Display requires methods.
  };

  (void)show4;

  if (st == IDLE) {
    tm1637.showNumberDecEx(0, 0b01000000, true, 4); // clear with dots
  } else if (st == PRENOTICE) {
    // Show seconds remaining until start (up to 59 seconds)
    if (haveStart) {
      uint16_t n = cur;
      // minutes difference modulo a day (assume same day)
      int diffMin = (int)startMin - (int)n;
      if (diffMin < 0) diffMin = 0;
      uint16_t remSec = (uint16_t)diffMin * 60;
      // If inside the last minute, we show 60..0
      if (remSec > 999) remSec = 999;
      tm1637.showNumberDecEx(remSec, 0, true, 4);
    } else {
      tm1637.showNumberDecEx(0, 0, true, 4);
    }
  } else if (st == OCCUPIED) {
    // show "8888" as occupied indicator
    tm1637.showNumberDecEx(8888, 0, true, 4);
  } else if (st == OVERTIME) {
    // show "9999"
    tm1637.showNumberDecEx(9999, 0, true, 4);
  }
}

static void enterState(RoomState newState) {
  if (state == newState) return;

  state = newState;

  if (state == IDLE) {
    setLEDs(false, false);
    buzzerStop();
    greenOn = false;
    lastGreenToggle = 0;
    overtimeNotified = false;
    overtimeBeepCount = 0;
    pirTriggered = false;
    doorTriggered = false;
    inRoom = false;
  } else if (state == PRENOTICE) {
    setLEDs(true, false); // will blink in loop
    buzzerStop();
    greenOn = true;
    lastGreenToggle = millis();
  } else if (state == OCCUPIED) {
    setLEDs(false, true);
    buzzerStop();
    occStartMs = millis();
  } else if (state == OVERTIME) {
    // Red stays on; beep while overtime condition remains
    setLEDs(false, true);
    lastOvertimeBeepMs = 0;
  }
}

void setup() {
  pinMode(PIN_PIR, INPUT);       // D2
  pinMode(PIN_DOOR, INPUT);      // D3
  pinMode(PIN_TM_CLK, OUTPUT);
  pinMode(PIN_TM_DIO, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);   // D6 tone output
  pinMode(PIN_GREEN_LED, OUTPUT); // D12
  pinMode(PIN_RED_LED, OUTPUT);   // D13
  pinMode(PIN_RELEASE, INPUT_PULLUP); // D7 button, assume active LOW

  setLEDs(false, false);
  buzzerStop();

  tm1637.setBrightness(7, true);
  tm1637.showNumberDecEx(0, 0, true, 4);

  Serial.begin(9600);

  bootMs = millis();
  enterState(IDLE);

  // Initial occupancy sample
  updateInRoomState();
}

void loop() {
  // --- Serial receive (idle framing, parse lines) ---
  static String lineBuf;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (lineBuf.length() > 0) {
        handleSerialLine(lineBuf);
        lineBuf = "";
      }
    } else {
      // protect size
      if (lineBuf.length() < 64) lineBuf += c;
    }
  }

  // --- Button D7: artificial release ---
  bool rel = digitalRead(PIN_RELEASE) == LOW;
  if (rel) {
    if (!releasePressed) {
      releasePressed = true;
      // Manual release: force IDLE regardless of sensors and times.
      enterState(IDLE);
      Serial.println(F("manual release"));
    }
  } else {
    releasePressed = false;
  }

  // --- Update occupancy inference ---
  updateInRoomState();

  // --- Determine time windows ---
  bool canSchedule = haveStart && haveEnd;
  uint16_t cur = nowMin();

  bool inPreMinute = false;
  if (canSchedule) {
    // Check if current time is within [start-1min, start)
    // Use minutes granularity; assume same day schedule without wrap.
    int diffStart = (int)startMin - (int)cur;
    if (diffStart > 0 && diffStart <= 1) {
      inPreMinute = true;
    }
  }

  bool pastStart = canSchedule ? ((int)cur - (int)startMin >= 0) : false;
  bool withinSchedule = canSchedule ? ((int)cur - (int)startMin >= 0 && (int)cur - (int)endMin < 0) : false;
  bool afterEnd = canSchedule ? ((int)cur - (int)endMin >= 0) : false;

  // --- State machine ---
  if (state == IDLE) {
    if (canSchedule) {
      if (inPreMinute) enterState(PRENOTICE);
      else {
        // stay idle
        if (inRoom && pastStart) {
          // someone already in room at/after start (edge case)
          enterState(OCCUPIED);
          Serial.println(F("occupied"));
        }
      }
    }
  } else if (state == PRENOTICE) {
    // Blink green until someone enters or start time arrives
    // "到预约开始前 1 分钟闪烁绿灯提醒"
    if (inRoom) {
      enterState(OCCUPIED);
      Serial.println(F("occupied"));
    } else {
      if (!canSchedule) {
        enterState(IDLE);
      } else {
        if (!inPreMinute && !pastStart) {
          // if time moved out somehow, return idle
          // (usually won't happen)
        }
        if (!withinSchedule && afterEnd) {
          // schedule ended without occupancy
          enterState(IDLE);
        }
      }
    }
    // also if start time reached and still no occupancy, keep green blinking briefly or stop
    if (canSchedule && (int)cur - (int)startMin >= 0 && !inRoom) {
      // if start reached but no one entered, stop after moment and go idle
      // (Requirement focuses on last minute; we keep PRENOTICE only within last minute.)
      enterState(IDLE);
    }
  } else if (state == OCCUPIED) {
    // "有人进入后显示 occupied 并点亮红灯"
    // Stay occupied while inRoom remains true
    if (!inRoom) {
      // Room becomes empty, go idle (even if schedule still ongoing)
      enterState(IDLE);
    } else {
      // If overtime
      if (afterEnd) {
        enterState(OVERTIME);
        Serial.println(F("overtime use"));
      }
    }
  } else if (state == OVERTIME) {
    // "预约结束后若仍有人在内则蜂鸣提示并输出 overtime use。"
    if (!inRoom) {
      enterState(IDLE);
    } else {
      if (!overtimeNotified) {
        Serial.println(F("overtime use"));
        overtimeNotified = true;
      }
      // beep prompt while overtime persists
      uint32_t now = millis();
      if (now - lastOvertimeBeepMs >= 900UL) {
        lastOvertimeBeepMs = now;
        buzzerTone(2000, 200);
        overtimeBeepCount++;
      }
      // Keep red on; green off
      setLEDs(false, true);
    }
  }

  // --- LED behavior in PRENOTICE ---
  if (state == PRENOTICE) {
    uint32_t now = millis();
    if (now - lastGreenToggle >= 250UL) { // blink period
      lastGreenToggle = now;
      greenOn = !greenOn;
      digitalWrite(PIN_GREEN_LED, greenOn ? HIGH : LOW);
      digitalWrite(PIN_RED_LED, LOW);
    }
  } else if (state == OCCUPIED) {
    setLEDs(false, true);
  } else if (state == OVERTIME) {
    setLEDs(false, true);
  } else {
    // IDLE
    setLEDs(false, false);
    buzzerStop();
  }

  // --- Display updates ---
  if (millis() - lastDisplayUpdate >= TM_DISPLAY_UPDATE_MS) {
    lastDisplayUpdate = millis();
    displayStateForTime(state);
  }

  // --- Prevent buzzer lingering ---
  // tone() is already non-blocking with duration; but ensure off in non-overtime
  if (state != OVERTIME) buzzerStop();
}
