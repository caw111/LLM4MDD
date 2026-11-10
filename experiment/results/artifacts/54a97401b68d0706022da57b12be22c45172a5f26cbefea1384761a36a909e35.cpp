#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>

// ==== Pins (per requirement) ====
static const uint8_t PIN_DHT      = 7;  // D7
static const uint8_t PIN_TRIG     = 8;  // D8
static const uint8_t PIN_ECHO     = 9;  // D9
static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5
static const uint8_t PIN_BUZZER   = 6;  // D6 (passive)

// ==== Protocol / behavior constants ====
static const uint32_t SERIAL_BAUD = 115200;

// Sampling intervals constraints
static const uint32_t DHT_MIN_MS = 2000;
static const uint32_t DHT_MAX_MS = 2200;

static const uint32_t US_MIN_MS  = 60;
static const uint32_t US_MAX_MS  = 180;

static const uint32_t US_ECHO_TIMEOUT_US = 25000UL;
static const float    US_CM_DIV = 58.0f; // distance_cm = echo_us / 58

// Valid ranges
static const int16_t TEMP_VALID_MIN = -40; // DHT11 typical min/max; but requirement focuses on invalid-blank & threshold strictness
static const int16_t TEMP_VALID_MAX = 80;

// Distance valid range: 2..400cm
static const float DIST_VALID_MIN = 2.0f;
static const float DIST_VALID_MAX = 400.0f;

// Display / switching
static const uint32_t DISPLAY_PAGE_SWITCH_MS = 2000;  // alternation every 2000ms between temp/distance
static const uint32_t DISPLAY_QUANTIZE_TOLERANCE_MS = 150; // page switch tolerance 150ms (applied implicitly by scheduling windows)

// Quantization tolerance: integer with quantization error <=1
// We'll display integer cast with rounding to nearest integer (gives quant error <=0.5)

// Update response tolerance: "Each time new readout output response tolerance 200ms"
// We'll update outputs immediately upon completing a successful read, but display page changes are time-based.

// Alarm rules
static const float TEMP_ALARM_THRESHOLD = 32.0f;     // alarm when temp > 32 (strict)
static const float DIST_ALARM_THRESHOLD = 15.0f;     // alarm when dist < 15 (strict)

// Alarm beeping: start immediately when any reason成立; 1000Hz tone with 500ms on / 500ms off
static const uint32_t BEEP_PERIOD_MS = 1000;
static const uint32_t BEEP_ON_MS = 500;
static const uint32_t BEEP_OFF_MS = 500;
static const uint32_t EDGE_TOLERANCE_MS = 100; // edge tolerance

// Serial reporting
static const uint32_t REPORT_EVERY_ALARM_MS = 1000; // sustained报警每1000ms within 200ms tolerance
static const uint32_t REPORT_AFTER_CHANGE_MS = 200;  // within 200ms after alarm start and reason changes

// TM1637 minimal driver (no external libs to keep sketch self-contained)
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
    // command: 0x88 | brightness
    start();
    writeByte(0x40);
    stop();
    start();
    writeByte(0x88 | _brightness);
    stop();
  }

  void clear() {
    uint8_t segs[4] = {0,0,0,0};
    showRaw(segs, true);
  }

  // Set digits: d0..d3 already mapped segments, if blank flag true then show 0
  void showNumberInt(int32_t num, bool leadingZeroBlank = true) {
    // We'll display with sign handling and decimal-free.
    uint8_t segs[4];
    encodeIntegerToSegments(num, segs, leadingZeroBlank);
    showRaw(segs, true);
  }

  void showRaw(uint8_t segs[4], bool autoUpdate = true) {
    (void)autoUpdate;
    start();
    writeByte(0x40); // command
    // data
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
    // LSB first
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

  // Segment mapping for TM1637 (a,b,c,d,e,f,g,dp not used here except dp)
  // Common: 0x3F=0, 0x06=1, 0x5B=2, 0x4F=3, 0x66=4, 0x6D=5, 0x7D=6, 0x07=7, 0x7F=8, 0x6F=9
  static uint8_t digitSeg(int d) {
    static const uint8_t map[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x67}; // 9 and 10?? last unused
    if (d < 0 || d > 9) return 0;
    if (d == 9) return 0x6F;
    return map[d];
  }

  void encodeIntegerToSegments(int32_t num, uint8_t segs[4], bool leadingZeroBlank) {
    // Right align
    for (int i = 0; i < 4; i++) segs[i] = 0;
    if (num < -999 || num > 9999) {
      // out of representable; blank
      segs[0]=segs[1]=segs[2]=segs[3]=0;
      return;
    }

    bool neg = (num < 0);
    uint32_t x = neg ? (uint32_t)(-num) : (uint32_t)num;

    // Fill digits
    int pos = 3;
    bool any = false;
    for (; pos >= 0; pos--) {
      uint8_t val = (x % 10);
      x /= 10;

      if (x == 0 && !any && leadingZeroBlank && val == 0) {
        // blank
        segs[pos] = 0;
      } else {
        segs[pos] = digitSeg(val);
        any = true;
      }

      if (x == 0) {
        // remaining leading positions handled by blank logic above; but sign may be placed later.
      }
    }

    if (neg) {
      // Put minus sign in leftmost if blank space; segment for minus: 0x40
      // Find first non-empty? but requirement doesn't mandate exact; we'll set leftmost.
      // Use segs[0]=0x40 always.
      segs[0] = 0x40;
    }

    // If number is 0 and leadingZeroBlank => should be blank or "0"? We'll keep "0" visible for non-blank measurements.
    // Determine: if num==0 then any==false after loop; set segs[3]=digitSeg(0)
    if (num == 0) {
      segs[3] = digitSeg(0);
      segs[0] = segs[1] = segs[2] = 0;
    }
  }
};

// ==== Sensors/actuators ====
DHT dht(PIN_DHT, DHT11);
TM1637 tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ==== Timing helpers with "do not block" constraints ====
static uint32_t nextDhtMs = 0;
static uint32_t nextUSMs  = 0;

// "Start within first 200ms" for both sensors: we'll schedule quickly.
static uint32_t programStartMs = 0;

// DHT cache (must preserve last valid reading during cache period; failure must mark invalid)
static bool     dhtValid = false;
static float    lastTempC = NAN;
static float    latestTempC = NAN;
static bool     dhtNewValidThisCycle = false;

// Ultrasonic cache
static bool     distValid = false;
static float    lastDistCm = NAN;
static float    latestDistCm = NAN;
static bool     distNewValidThisCycle = false;

// Display state
enum Page : uint8_t { PAGE_TEMP = 0, PAGE_DIST = 1 };
static Page currentPage = PAGE_TEMP;
static uint32_t pageSwitchDueMs = 0;
static int lastDisplayedValue = 0;

// Alarm beeping state machine (phase continuity on reason changes)
static bool alarmActive = false;
static bool prevAlarmActive = false;
static uint32_t alarmStartMs = 0;     // when alarm first became active
static uint32_t alarmPhaseMs = 0;    // keep phase relative to first start
static bool toneOnPhase = false;

// Reason flags
static bool reasonTemp = false;        // temp > 32 valid strict
static bool reasonDist = false;        // dist < 15 valid strict
static bool prevReasonTemp = false;
static bool prevReasonDist = false;

// Serial reporting tracking
static bool firstAlarmAnnounced = false;
static uint32_t lastAlarmReportMs = 0; // sustained reports every 1000ms
static uint32_t afterChangeReportDueMs = 0;
static bool pendingAfterChangeReport = false;

// ==== Utility random (deterministic enough) ====
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t randBetween(uint32_t a, uint32_t b);
#line 269 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void scheduleNextDHT(uint32_t nowMs);
#line 273 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void scheduleNextUS(uint32_t nowMs);
#line 282 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readDHTOnce(uint32_t nowMs);
#line 304 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readUSOnce(uint32_t nowMs);
#line 339 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateReasons();
#line 345 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyBeepFSM(uint32_t nowMs);
#line 401 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateSerialReports(uint32_t nowMs);
#line 463 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayIntOrBlank(int value, bool valid, bool isDistance);
#line 476 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int quantizeRoundedFloatToInt(float x);
#line 482 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay(uint32_t nowMs);
#line 506 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 548 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t randBetween(uint32_t a, uint32_t b) {
  if (a > b) { uint32_t t=a; a=b; b=t; }
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

// ==== Measurement functions (non-blocking wrt other devices timing) ====
// - DHT read: DHT library may block a bit; but requirement says not block other devices or "model timers".
//   We'll ensure loop remains responsive by not doing long operations elsewhere; and DHT read happens at scheduled times only.
// - US read: must timeout at 25000us.

static void readDHTOnce(uint32_t nowMs) {
  dhtNewValidThisCycle = false;
  float t = dht.readTemperature(); // Celsius by default
  if (!isnan(t)) {
    // DHT11 can only output integer-ish values; accept range for validity
    // Requirement: initial invalid temperature -> blank; failure must be invalid and cannot block.
    if (t >= TEMP_VALID_MIN && t <= TEMP_VALID_MAX) {
      dhtValid = true;
      lastTempC = t;
      latestTempC = t;
      dhtNewValidThisCycle = true;
      // cache behavior: keep last valid reading during cache period. We already update only on success.
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

  // Trigger
  pinMode(PIN_TRIG, OUTPUT);
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Echo with timeout 25,000us
  pinMode(PIN_ECHO, INPUT);
  // Use pulseIn but with timeout; pulseIn blocks up to timeout. We'll cap at 25ms.
  unsigned long echo_us = pulseIn(PIN_ECHO, HIGH, US_ECHO_TIMEOUT_US);

  // pulseIn returns 0 on timeout
  if (echo_us == 0) {
    distValid = false;
  } else {
    float cm = (float)echo_us / US_CM_DIV; // echo_us / 58
    // Valid range 2..400
    if (cm >= DIST_VALID_MIN && cm <= DIST_VALID_MAX) {
      distValid = true;
      lastDistCm = cm;
      latestDistCm = cm;
      distNewValidThisCycle = true;
    } else {
      distValid = false;
    }
  }

  scheduleNextUS(nowMs);
}

static void updateReasons() {
  // Only valid measurements can trigger reasons.
  reasonTemp = (dhtValid && (lastTempC > TEMP_ALARM_THRESHOLD));   // strict >
  reasonDist = (distValid && (lastDistCm < DIST_ALARM_THRESHOLD)); // strict <
}

static void applyBeepFSM(uint32_t nowMs) {
  // Any reason成立 => alarmActive
  bool nowAlarm = (reasonTemp || reasonDist);

  if (nowAlarm && !alarmActive) {
    // Alarm start event
    alarmActive = true;
    alarmStartMs = nowMs;
    alarmPhaseMs = 0;
    toneOnPhase = true;

    // Serial: alarm start
    Serial.print("ALARM_START: ");
    bool first = true;
    if (reasonTemp) { Serial.print("ALARM_TEMP"); first = false; }
    if (reasonDist) {
      if (!first) Serial.print(",");
      Serial.print("ALARM_DISTANCE");
    }
    Serial.println();

    // Schedule after-change report within 200ms from start
    pendingAfterChangeReport = true;
    afterChangeReportDueMs = nowMs + REPORT_AFTER_CHANGE_MS;
    firstAlarmAnnounced = true;

    lastAlarmReportMs = nowMs; // allow sustained report timing tolerance
  } else if (!nowAlarm && alarmActive) {
    alarmActive = false;
    // Stop tone immediately but still require "after all reasons disappear within 200ms mute"
    // We'll mute immediately and also ensure within 200ms: schedule? We'll just stop now.
    noTone(PIN_BUZZER);
  }

  // Phase continuity: if already alarmActive, do not reset on reason changes.
  if (alarmActive) {
    uint32_t elapsed = nowMs - alarmStartMs;
    // Maintain phase; compute tone on/off based on 500/500 in 1000ms period.
    uint32_t mod = elapsed % BEEP_PERIOD_MS;
    bool shouldOn = (mod < BEEP_ON_MS);

    // Edge tolerance (100ms): since we use deterministic mod, edges align to start; noTone/ tone changes occur near boundary.
    if (shouldOn) {
      if (!toneOnPhase) {
        toneOnPhase = true;
        tone(PIN_BUZZER, 1000); // 1000Hz
      }
    } else {
      if (toneOnPhase) {
        toneOnPhase = false;
        noTone(PIN_BUZZER);
      }
    }
  }
}

static void updateSerialReports(uint32_t nowMs) {
  bool nowAlarm = alarmActive;

  // Detect reason changes while alarm is active (or becoming active handled elsewhere)
  bool changedReasons = false;
  if (reasonTemp != prevReasonTemp || reasonDist != prevReasonDist) changedReasons = true;

  if (changedReasons && nowAlarm) {
    // reasons changed during active alarm -> report after 200ms (within tolerance)
    pendingAfterChangeReport = true;
    afterChangeReportDueMs = nowMs + REPORT_AFTER_CHANGE_MS;

    // Serial: only "alarm start" / "reason changes after 200ms" / sustained every 1000ms
    // Do not print now; due within 200ms.
  }

  // After-change due
  if (pendingAfterChangeReport && nowAlarm) {
    // Report when due; tolerate up to 200ms (we schedule exactly at +200ms, so it's within tolerance).
    if ((int32_t)(nowMs - afterChangeReportDueMs) >= 0) {
      pendingAfterChangeReport = false;

      // Report all current reasons that are成立
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

  // Sustained reports every 1000ms with tolerance 200ms: we'll emit when ~1000ms elapsed from last report
  if (nowAlarm) {
    if (lastAlarmReportMs == 0) lastAlarmReportMs = nowMs;
    if ((int32_t)(nowMs - lastAlarmReportMs) >= (int32_t)REPORT_EVERY_ALARM_MS) {
      // emit report
      lastAlarmReportMs = nowMs;

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

  prevReasonTemp = reasonTemp;
  prevReasonDist = reasonDist;
}

static void displayIntOrBlank(int value, bool valid, bool isDistance) {
  // "所选测量无效时空白"
  if (!valid) {
    tm1637.clear();
    return;
  }
  // display integer
  tm1637.setBrightness(7);

  // quantization tolerance 1: use rounding to nearest integer
  tm1637.showNumberInt(value, true);
}

static int quantizeRoundedFloatToInt(float x) {
  if (isnan(x)) return 0;
  // rounding to nearest integer
  return (int)lround(x);
}

static void updateDisplay(uint32_t nowMs) {
  // Default: first show temperature; then every 2000ms alternate temperature and distance.
  // "display integer; quantization error tolerance 1; page switch tolerance 150ms"
  // We'll switch when due, and due time aligned to start; any read output within 200ms will reflect on current page.
  if ((int32_t)(nowMs - pageSwitchDueMs) >= 0) {
    // flip page
    currentPage = (currentPage == PAGE_TEMP) ? PAGE_DIST : PAGE_TEMP;
    pageSwitchDueMs += DISPLAY_PAGE_SWITCH_MS; // keep stable cadence
  }

  if (currentPage == PAGE_TEMP) {
    // initial invalid temperature must be blank (dhtValid false => blank)
    int disp = 0;
    bool valid = dhtValid;
    if (valid) disp = quantizeRoundedFloatToInt(lastTempC);
    displayIntOrBlank(disp, valid, false);
  } else {
    int disp = 0;
    bool valid = distValid;
    if (valid) disp = quantizeRoundedFloatToInt(lastDistCm);
    displayIntOrBlank(disp, valid, true);
  }
}

void setup() {
  programStartMs = millis();
  Serial.begin(SERIAL_BAUD);
  delay(20);

  // DHT
  dht.begin();

  // Ultrasonic
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // Buzzer
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  // TM1637
  tm1637.begin();
  tm1637.setBrightness(7);
  tm1637.clear();

  // Schedule first sampling within first 200ms
  uint32_t start = millis();
  scheduleNextDHT(start + 200); // within 200ms initial requirement
  nextUSMs = start + 120;       // within 200ms initial requirement

  pageSwitchDueMs = start + DISPLAY_PAGE_SWITCH_MS; // default first shows temperature; switch after 2000ms
  currentPage = PAGE_TEMP;

  // Initial validity: temperature invalid => blank; distance invalid => blank
  dhtValid = false;
  distValid = false;

  prevReasonTemp = false;
  prevReasonDist = false;

  alarmActive = false;
  pendingAfterChangeReport = false;
  lastAlarmReportMs = 0;
}

void loop() {
  uint32_t nowMs = millis();

  // Scheduled DHT sampling
  // "DHT采样开始间隔2000..2200ms" and "失败必须标为无效，不能阻塞其他设备或模型计时"
  if ((int32_t)(nowMs - nextDhtMs) >= 0) {
    readDHTOnce(nowMs);
  }

  // Scheduled ultrasonic sampling
  // "距离采样开始间隔60..180ms" with echo timeout 25000us
  if ((int32_t)(nowMs - nextUSMs) >= 0) {
    readUSOnce(nowMs);
  }

  // Update reasons based on latest cached valid readings
  updateReasons();

  // Apply beep FSM with phase continuity across reason changes
  applyBeepFSM(nowMs);

  // Serial reporting rules:
  // - alarm start
  // - after change within 200ms
  // - sustained every 1000ms (tolerance 200ms)
  // Also report only current成立 reasons
  if (alarmActive || (reasonTemp || reasonDist)) {
    updateSerialReports(nowMs);
  }

  // Display logic (page switching every 2000ms)
  updateDisplay(nowMs);

  // Ensure non-blocking: short idle
  delay(1);
}
