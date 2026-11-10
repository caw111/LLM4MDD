#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

// ==== Pins (per requirement; remapped wiring is authoritative) ====
static const uint8_t PIN_DHT       = 7;  // D7 (DHT11 data)
static const uint8_t PIN_TRIG      = 8;  // D8 (HC-SR04 TRIG)
static const uint8_t PIN_ECHO      = 9;  // D9 (HC-SR04 ECHO)
static const uint8_t PIN_TM1637_CLK = 4; // D4 (TM1637 CLK)
static const uint8_t PIN_TM1637_DIO = 5; // D5 (TM1637 DIO)
static const uint8_t PIN_BUZZER    = 6;  // D6 (passive buzzer)

// ==== Serial ====
static const uint32_t SERIAL_BAUD = 115200;

// ==== Sampling intervals constraints ====
static const uint32_t DHT_MIN_MS = 2000;
static const uint32_t DHT_MAX_MS = 2200;

static const uint32_t US_MIN_MS  = 60;
static const uint32_t US_MAX_MS  = 180;

static const uint32_t US_ECHO_TIMEOUT_US = 25000UL; // must have 25,000us timeout
static const float    US_CM_DIV = 58.0f; // distance_cm = echo_us / 58

// Valid ranges / thresholds
static const float TEMP_VALID_MIN = -40.0f;
static const float TEMP_VALID_MAX = 80.0f;

static const float DIST_VALID_MIN = 2.0f;
static const float DIST_VALID_MAX = 400.0f;

static const float TEMP_ALARM_THRESHOLD = 32.0f; // strict: temp > 32
static const float DIST_ALARM_THRESHOLD = 15.0f; // strict: dist < 15

// Alarm beeping: 1000Hz, 500ms on / 500ms off, full period 1000ms
static const uint32_t BEEP_PERIOD_MS = 1000;
static const uint32_t BEEP_ON_MS     = 500;
static const uint32_t BEEP_OFF_MS    = 500;
static const uint32_t EDGE_TOLERANCE_MS = 100; // used implicitly by deterministic phase continuity

// Display / paging behavior
static const uint32_t DISPLAY_PAGE_SWITCH_MS = 2000; // alternation every 2000ms
static const uint32_t DISPLAY_QUANT_TOLERANCE_MS = 150; // page switch tolerance 150ms (timing window behavior)

// Serial reporting behavior
static const uint32_t REPORT_EVERY_ALARM_MS = 1000; // sustained every 1000ms (±200ms)
static const uint32_t REPORT_AFTER_CHANGE_MS = 200; // alarm start + reason changes within 200ms

// DHT caching and non-blocking behavior
// - DHT failures => invalid; don't block other devices beyond scheduled operations.
static const uint32_t LOOP_DELAY_MS = 1;

// ===== Minimal TM1637 driver (self-contained) =====
class TM1637 {
public:
  TM1637(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    delay(50);
    _brightness = 7;
    clear();
  }

  void setBrightness(uint8_t b) {
    if (b > 7) b = 7;
    _brightness = b;

    start();
    writeByte(0x40);
    stop();

    start();
    writeByte(0x88 | _brightness);
    stop();
  }

  void clear() {
    uint8_t segs[4] = {0, 0, 0, 0};
    showRaw(segs, true);
  }

  // integer display with leading zero blanking
  void showNumberInt(int32_t num, bool leadingZeroBlank = true) {
    uint8_t segs[4];
    encodeIntegerToSegments(num, segs, leadingZeroBlank);
    showRaw(segs, true);
  }

  void showRaw(uint8_t segs[4], bool autoUpdate = true) {
    (void)autoUpdate;

    start();
    writeByte(0x40); // command
    for (int i = 0; i < 4; i++) writeByte(segs[i] & 0xFF);
    stop();

    start();
    writeByte(0xC0 | 0x00); // display control
    stop();
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness = 7;

  void start() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      delayMicroseconds(3);
      if (b & 0x01) digitalWrite(_dio, HIGH);
      else digitalWrite(_dio, LOW);
      delayMicroseconds(3);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(3);
      b >>= 1;
    }
    // ack
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    delayMicroseconds(3);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(10);
    pinMode(_dio, OUTPUT);
  }

  // Common segment mapping
  // NOTE: Fix initializer error: map must be exactly 10 entries for indices 0..9.
  static uint8_t digitSeg(int d) {
    static const uint8_t map[10] = {
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F  // 9
    };
    if (d < 0 || d > 9) return 0;
    return map[d];
  }

  void encodeIntegerToSegments(int32_t num, uint8_t segs[4], bool leadingZeroBlank) {
    for (int i = 0; i < 4; i++) segs[i] = 0;

    // Representable range on 4-digit display (with optional minus): keep conservative.
    if (num < -999 || num > 9999) {
      // blank
      return;
    }

    bool neg = (num < 0);
    uint32_t x = neg ? (uint32_t)(-num) : (uint32_t)num;

    bool any = false;
    for (int pos = 3; pos >= 0; pos--) {
      uint8_t val = (uint8_t)(x % 10);
      x /= 10;

      if (!any && leadingZeroBlank && val == 0) {
        segs[pos] = 0;
      } else {
        segs[pos] = digitSeg(val);
        any = true;
      }
    }

    if (neg) {
      // Minus sign on leftmost if it isn't blanked away by digits
      segs[0] = 0x40;
    }

    // If num == 0, ensure we show 0 (require integer display; when valid it shouldn't be blank)
    if (num == 0) {
      segs[0] = segs[1] = segs[2] = 0;
      segs[3] = digitSeg(0);
    }
  }
};

// ===== Devices =====
DHT dht(PIN_DHT, DHT11);
TM1637 tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== State: scheduled sampling =====
static uint32_t nextDhtMs = 0;
static uint32_t nextUSMs  = 0;

// ===== State: cached latest valid measurements =====
static bool  dhtValid = false;
static float lastTempC = NAN;
static float latestTempC = NAN;
static bool  dhtNewValidThisCycle = false;

static bool  distValid = false;
static float lastDistCm = NAN;
static float latestDistCm = NAN;
static bool  distNewValidThisCycle = false;

// ===== Display state =====
enum Page : uint8_t { PAGE_TEMP = 0, PAGE_DIST = 1 };
static Page currentPage = PAGE_TEMP;
static uint32_t pageSwitchDueMs = 0;

// ===== Alarm state machine with phase continuity =====
static bool alarmActive = false;      // true when any reason成立 (from cached latest valid)
static bool alarmPhaseOn = false;    // current on/off within beeping phase
static uint32_t alarmStartMs = 0;    // when alarm became active
static uint32_t tonePhaseOffsetMs = 0;

static bool reasonTemp = false; // dhtValid && (temp > 32), strict
static bool reasonDist = false; // distValid && (dist < 15), strict
static bool prevReasonTemp = false;
static bool prevReasonDist = false;

// Serial reporting schedule
static bool pendingAfterChangeReport = false;
static uint32_t afterChangeReportDueMs = 0;

static uint32_t lastSustainedReportMs = 0;
static bool firstAlarmStartReported = false;

// ===== Utility =====
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint32_t randBetween(uint32_t a, uint32_t b);
#line 258 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void scheduleNextDHT(uint32_t nowMs);
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void scheduleNextUS(uint32_t nowMs);
#line 267 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readDHTOnce(uint32_t nowMs);
#line 287 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readUSOnce(uint32_t nowMs);
#line 320 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateReasons();
#line 328 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void startAlarm(uint32_t nowMs);
#line 356 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void stopAlarmImmediate();
#line 362 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void applyBeepFSM(uint32_t nowMs);
#line 385 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void maybeReportAfterChange(uint32_t nowMs);
#line 407 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void maybeReportSustained(uint32_t nowMs);
#line 430 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateSerialReports(uint32_t nowMs);
#line 450 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int quantizeRoundedFloatToInt(float x);
#line 458 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayBlank();
#line 462 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayIntOrBlank(int value, bool valid);
#line 471 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateDisplay(uint32_t nowMs);
#line 503 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 558 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint32_t randBetween(uint32_t a, uint32_t b) {
  if (a > b) { uint32_t t = a; a = b; b = t; }
  uint32_t span = (b - a + 1);
  uint32_t r = (uint32_t)micros();
  return a + (r % span);
}

static void scheduleNextDHT(uint32_t nowMs) {
  nextDhtMs = nowMs + randBetween(DHT_MIN_MS, DHT_MAX_MS);
}

static void scheduleNextUS(uint32_t nowMs) {
  nextUSMs = nowMs + randBetween(US_MIN_MS, US_MAX_MS);
}

// ===== Measurement reads =====
static void readDHTOnce(uint32_t nowMs) {
  dhtNewValidThisCycle = false;

  float t = dht.readTemperature(); // Celsius
  if (!isnan(t)) {
    if (t >= TEMP_VALID_MIN && t <= TEMP_VALID_MAX) {
      dhtValid = true;
      lastTempC = t;     // cache last valid
      latestTempC = t;  // latest for this read cycle
      dhtNewValidThisCycle = true;
    } else {
      dhtValid = false;
    }
  } else {
    dhtValid = false;
  }

  scheduleNextDHT(nowMs);
}

static void readUSOnce(uint32_t nowMs) {
  distNewValidThisCycle = false;

  // Trigger pulse
  pinMode(PIN_TRIG, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Echo with 25,000us timeout
  pinMode(PIN_ECHO, INPUT);
  unsigned long echo_us = pulseIn(PIN_ECHO, HIGH, US_ECHO_TIMEOUT_US);

  if (echo_us == 0) {
    distValid = false;
  } else {
    float cm = ((float)echo_us) / US_CM_DIV; // echo_us / 58
    if (cm >= DIST_VALID_MIN && cm <= DIST_VALID_MAX) {
      distValid = true;
      lastDistCm = cm;     // cache last valid
      latestDistCm = cm;  // latest for this read cycle
      distNewValidThisCycle = true;
    } else {
      distValid = false;
    }
  }

  scheduleNextUS(nowMs);
}

// ===== Reasons update =====
static void updateReasons() {
  // Must compare original measurement before integer display.
  // Strict comparisons: valid temperature > 32; valid distance < 15.
  reasonTemp = (dhtValid && (lastTempC > TEMP_ALARM_THRESHOLD));
  reasonDist = (distValid && (lastDistCm < DIST_ALARM_THRESHOLD));
}

// ===== Buzzer control (phase continuity, reason change does not reset phase) =====
static void startAlarm(uint32_t nowMs) {
  alarmActive = true;
  alarmStartMs = nowMs;
  tonePhaseOffsetMs = 0;
  alarmPhaseOn = true;

  // Serial: alarm start + reasons
  Serial.print("ALARM_START: ");
  bool first = true;
  if (reasonTemp) {
    Serial.print("ALARM_TEMP");
    first = false;
  }
  if (reasonDist) {
    if (!first) Serial.print(",");
    Serial.print("ALARM_DISTANCE");
  }
  Serial.println();

  // Schedule after-change report within 200ms from alarm start
  pendingAfterChangeReport = true;
  afterChangeReportDueMs = nowMs + REPORT_AFTER_CHANGE_MS;

  // Mark for sustained reporting
  firstAlarmStartReported = true;
  lastSustainedReportMs = nowMs; // allow immediate within tolerance
}

static void stopAlarmImmediate() {
  alarmActive = false;
  alarmPhaseOn = false;
  noTone(PIN_BUZZER);
}

static void applyBeepFSM(uint32_t nowMs) {
  if (!alarmActive) return;

  // Determine current phase using continuous time since alarmStartMs.
  uint32_t elapsed = nowMs - alarmStartMs;
  uint32_t mod = elapsed % BEEP_PERIOD_MS;

  bool shouldOn = (mod < BEEP_ON_MS);
  // Keep phase continuity on reason changes: we never reset alarmStartMs here.
  if (shouldOn) {
    if (!alarmPhaseOn) {
      alarmPhaseOn = true;
      tone(PIN_BUZZER, 1000); // 1000Hz
    }
  } else {
    if (alarmPhaseOn) {
      alarmPhaseOn = false;
      noTone(PIN_BUZZER);
    }
  }
}

// ===== Serial reporting =====
static void maybeReportAfterChange(uint32_t nowMs) {
  if (!alarmActive) return;
  if (!pendingAfterChangeReport) return;

  // Must be within 200ms tolerance: we schedule exactly due time.
  if ((int32_t)(nowMs - afterChangeReportDueMs) >= 0) {
    pendingAfterChangeReport = false;

    Serial.print("ALARM_REASONS:");
    bool first = true;
    if (reasonTemp) {
      Serial.print(first ? "ALARM_TEMP" : ",ALARM_TEMP");
      first = false;
    }
    if (reasonDist) {
      Serial.print(first ? "ALARM_DISTANCE" : ",ALARM_DISTANCE");
      first = false;
    }
    Serial.println();
  }
}

static void maybeReportSustained(uint32_t nowMs) {
  if (!alarmActive) return;

  if (lastSustainedReportMs == 0) lastSustainedReportMs = nowMs;

  // report every 1000ms with tolerance 200ms -> we just use >=1000ms
  if ((int32_t)(nowMs - lastSustainedReportMs) >= (int32_t)REPORT_EVERY_ALARM_MS) {
    lastSustainedReportMs = nowMs;

    Serial.print("ALARM_REASONS_PERIODIC:");
    bool first = true;
    if (reasonTemp) {
      Serial.print(first ? "ALARM_TEMP" : ",ALARM_TEMP");
      first = false;
    }
    if (reasonDist) {
      Serial.print(first ? "ALARM_DISTANCE" : ",ALARM_DISTANCE");
      first = false;
    }
    Serial.println();
  }
}

static void updateSerialReports(uint32_t nowMs) {
  // Detect reason changes while alarm is active; schedule within 200ms.
  if (alarmActive) {
    bool changed = (reasonTemp != prevReasonTemp) || (reasonDist != prevReasonDist);
    if (changed) {
      pendingAfterChangeReport = true;
      afterChangeReportDueMs = nowMs + REPORT_AFTER_CHANGE_MS;

      // Do not print immediately; due within 200ms.
    }
    maybeReportAfterChange(nowMs);
    maybeReportSustained(nowMs);
  }

  // Update prev after scheduling decisions
  prevReasonTemp = reasonTemp;
  prevReasonDist = reasonDist;
}

// ===== Display update =====
static int quantizeRoundedFloatToInt(float x) {
  // integer quantization; error tolerance 1
  // requirement: display integer; quantization error <= 1.
  // rounding to nearest integer meets this.
  if (isnan(x)) return 0;
  return (int)lround(x);
}

static void displayBlank() {
  tm1637.clear();
}

static void displayIntOrBlank(int value, bool valid) {
  if (!valid) {
    displayBlank();
    return;
  }
  tm1637.setBrightness(7);
  tm1637.showNumberInt(value, true);
}

static void updateDisplay(uint32_t nowMs) {
  // Requirement: initial temp invalid => blank.
  // Switch pages every 2000ms. "page切换容差150ms" -> we allow switch when due
  // and use stable cadence; this makes switch occur at exact time (within tolerance).
  if ((int32_t)(nowMs - pageSwitchDueMs) >= 0) {
    currentPage = (currentPage == PAGE_TEMP) ? PAGE_DIST : PAGE_TEMP;

    // keep stable 2000ms cadence
    // If we missed by more than one period due to delays, advance appropriately.
    uint32_t elapsed = nowMs - pageSwitchDueMs;
    uint32_t steps = elapsed / DISPLAY_PAGE_SWITCH_MS + 1;
    pageSwitchDueMs += steps * DISPLAY_PAGE_SWITCH_MS;
    // Correct currentPage by parity of steps
    if (steps % 2 == 1) {
      currentPage = (currentPage == PAGE_TEMP) ? PAGE_DIST : PAGE_TEMP;
      // Then next toggle will happen again at correct cadence; but for minimal behavior, we just keep one toggle.
      // However steps>1 is unlikely due to non-blocking loop; clamp behavior:
      currentPage = (currentPage == PAGE_TEMP) ? PAGE_DIST : PAGE_TEMP; // revert to single toggle effect
    }
  }

  if (currentPage == PAGE_TEMP) {
    bool valid = dhtValid;
    int disp = valid ? quantizeRoundedFloatToInt(lastTempC) : 0;
    displayIntOrBlank(disp, valid);
  } else {
    bool valid = distValid;
    int disp = valid ? quantizeRoundedFloatToInt(lastDistCm) : 0;
    displayIntOrBlank(disp, valid);
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  // DHT
  dht.begin();

  // Ultrasonic
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // Buzzer (passive)
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  // TM1637
  tm1637.begin();
  tm1637.setBrightness(7);
  tm1637.clear();

  // Initial invalid temperature => blank already by dhtValid false.
  dhtValid = false;
  distValid = false;

  // Schedule first sampling within first 200ms
  uint32_t start = millis();

  // "DHT采样开始200ms内": choose 200ms exactly but allow within 200..2200 constraints for subsequent.
  scheduleNextDHT(start);
  nextDhtMs = start + 200; // explicit within first 200ms

  // "距离采样开始间隔60..180ms": choose within 200ms window
  scheduleNextUS(start);
  nextUSMs = start + 120;

  // Display: initial page shows temperature; then alternates every 2000ms
  currentPage = PAGE_TEMP;
  pageSwitchDueMs = start + DISPLAY_PAGE_SWITCH_MS;

  // Alarm/serial init
  alarmActive = false;
  alarmPhaseOn = false;
  alarmStartMs = 0;
  tonePhaseOffsetMs = 0;

  reasonTemp = reasonDist = false;
  prevReasonTemp = prevReasonDist = false;

  pendingAfterChangeReport = false;
  afterChangeReportDueMs = 0;

  lastSustainedReportMs = 0;
  firstAlarmStartReported = false;
}

void loop() {
  uint32_t nowMs = millis();

  // Sample DHT at its scheduled time
  if ((int32_t)(nowMs - nextDhtMs) >= 0) {
    readDHTOnce(nowMs);
  }

  // Sample ultrasonic at its scheduled time
  if ((int32_t)(nowMs - nextUSMs) >= 0) {
    readUSOnce(nowMs);
  }

  // Update reasons based on cached valid readings
  updateReasons();

  // Start/stop alarm:
  bool nowAlarm = (reasonTemp || reasonDist);

  if (nowAlarm && !alarmActive) {
    startAlarm(nowMs);
  } else if (!nowAlarm && alarmActive) {
    // Requirement: "所有原因消失后200ms内静音"
    // We'll delay actual stop by 200ms without blocking other tasks:
    // implement via a one-shot timer using afterChangeReportDueMs? No: must be mute after 200ms.
    // We'll schedule stop using a static stopDue.
    static bool stopScheduled = false;
    static uint32_t stopDueMs = 0;
    stopScheduled = true;
    stopDueMs = nowMs + 200;

    // We'll keep alarmActive true until stopDueMs, to preserve "edge" and phase continuity.
    // But tone will be turned off exactly at stopDueMs below.
    // Need to ensure beeping doesn't continue after stop due; applyBeepFSM only if alarmActiveTone.
    // We'll model by turning off immediately when due:
    // Temporarily keep alarmActive true until mute time; however serial periodic should not claim reasons not成立.
    // Serial rules require only report when alarmActive (which implies reasons founded). If reasons disappear, no periodic should be sent.
    // So, stop alarmActive now but keep phase continuity for 200ms? Requirement says mute within 200ms; serial shouldn't report.
    // Therefore, set alarmActive false now but mute at stopDue.
    // We will handle with a separate static mute scheduler.
    alarmActive = false;
    alarmPhaseOn = false;
    noTone(PIN_BUZZER); // immediate mute for safety within 200ms; requirement says within 200ms, so OK.
    // Stop any pending after-change reports
    pendingAfterChangeReport = false;
    // Ensure prev reasons updated
    prevReasonTemp = reasonTemp;
    prevReasonDist = reasonDist;
  }

  // Beeping state machine only when alarmActive is true
  // (If reasons disappear, alarmActive becomes false so tone stops immediately; still satisfies "within 200ms".)
  if (alarmActive) {
    applyBeepFSM(nowMs);
    updateSerialReports(nowMs);
  }

  // Display
  updateDisplay(nowMs);

  // Small delay to reduce CPU churn; must not block other devices or timers.
  delay(LOOP_DELAY_MS);
}
