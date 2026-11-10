#include <Arduino.h>
#include <TM1637Display.h>

// ===== Pin assignments (per requirements) =====
static const uint8_t PIN_LED      = 13; // D13
static const uint8_t PIN_BUZZER   = 6;  // D6
static const uint8_t PIN_VALVE    = 10; // D10 (水流电磁阀)
static const uint8_t PIN_IR        = 2;  // D2 (红外接近传感器)

static const uint8_t PIN_TRIG      = 8;  // D8 (HC-SR04 Trig)
static const uint8_t PIN_ECHO      = 9;  // D9 (HC-SR04 Echo)

static const uint8_t PIN_TM1637_CLK = 4; // D4 (TM1637 CLK)
static const uint8_t PIN_TM1637_DIO = 5; // D5 (TM1637 DIO)

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ===== Water system behavior parameters (preserve behavior) =====
// NOTE: Thresholds/timers/calibration are not provided in the prompt.
// To avoid changing intended behavior, we implement a robust default set that
// is commonly used for such systems and can be tuned in code:
// - Valve opens for a fixed duration after a valid IR detection.
// - If tank level is below a low threshold, valve is inhibited and alarm is sounded.
// - Display shows remaining level percent and today's usage count.
// - Serial can query statistics.
//
// If you have specific values, update the constants below accordingly.

static const uint16_t BAUD = 9600;

// Fixed valve open time (milliseconds)
static const uint32_t VALVE_ON_TIME_MS = 3000UL; // 默认：3秒

// Minimum interval between consecutive counts when IR is held near
static const uint32_t MIN_COUNT_INTERVAL_MS = 10000UL; // 默认：10秒防抖/限速

// Tank low level threshold (percent)
static const uint8_t LOW_LEVEL_PERCENT = 15; // 默认：低于15%禁止出水并报警

// Alarm behavior
static const uint32_t ALARM_ON_MS  = 200;  // 默认蜂鸣器开
static const uint32_t ALARM_OFF_MS = 200;  // 默认蜂鸣器关
static const uint8_t  ALARM_BEEP_CYCLES = 0; // 0 means continuous toggle based on timings

// HC-SR04 -> level percent calibration
// Provide a typical ultrasonic mapping; adjust as needed.
// We estimate distance in cm, then map from MAX to MIN distances into percent.
static const float ULTRA_MAX_DISTANCE_CM = 30.0f; // when tank is nearly empty
static const float ULTRA_MIN_DISTANCE_CM = 5.0f;  // when tank is full

// How often to update level measurement
static const uint32_t LEVEL_UPDATE_INTERVAL_MS = 800UL;

// Echo timeout (ms) to prevent blocking
static const uint32_t ECHO_TIMEOUT_MS = 35UL; // enough for ~6m, but we only need tank range

// ===== Runtime state =====
static volatile uint32_t todayUseCount = 0;

static uint32_t lastLevelUpdateMs = 0;
static uint8_t currentLevelPercent = 0;
static bool lowLevel = false;

static bool valveActive = false;
static uint32_t valveStartMs = 0;

static bool irWasHigh = false;
static uint32_t lastCountMs = 0;

// Alarm timing
static uint32_t lastAlarmToggleMs = 0;
static bool alarmState = false;

// Serial input buffer
static String serialLine;
static uint32_t lastDisplayUpdateMs = 0;

// ===== Utility: Ultrasonic distance (cm) =====
static float measureDistanceCm() {
  // Trigger pulse
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Measure echo with timeout
  unsigned long duration = pulseIn(PIN_ECHO, HIGH, (unsigned long)ECHO_TIMEOUT_MS * 1000UL);
  if (duration == 0) {
    // Timeout: return max distance (assume low level)
    return ULTRA_MAX_DISTANCE_CM;
  }

  // HC-SR04: distance(cm) = duration(us) * 0.0343 / 2
  float dist = (duration * 0.0343f) * 0.5f;
  if (dist < 0.0f) dist = 0.0f;
  return dist;
}

// ===== Utility: Map distance to level percent =====
static uint8_t distanceToPercent(float distCm) {
  if (distCm >= ULTRA_MAX_DISTANCE_CM) return 0;
  if (distCm <= ULTRA_MIN_DISTANCE_CM) return 100;

  float span = (ULTRA_MAX_DISTANCE_CM - ULTRA_MIN_DISTANCE_CM);
  float p = (ULTRA_MAX_DISTANCE_CM - distCm) * 100.0f / span;
  if (p < 0) p = 0;
  if (p > 100) p = 100;
  return (uint8_t)(p + 0.5f);
}

// ===== Display =====
// We'll multiplex: show level percent (0-100) then usage count (0-9999)
// using a simple time-sliced display to fit 4 digits.
// Format:
// - Level percent: "Lxx" not possible reliably on 7-seg; we show "xx%" as "xx" with DP.
// - Usage count: show lower 4 digits of todayUseCount.
static void updateDisplay() {
  const uint32_t now = millis();
  if (now - lastDisplayUpdateMs < 250) return;
  lastDisplayUpdateMs = now;

  static bool showLevel = true;
  // Toggle view every ~250ms
  showLevel = !showLevel;

  if (showLevel) {
    // Show level percent on 4 digits: e.g., "  0".."100"
    // Use display.showNumberDecEx with leading zeros suppressed.
    // Put percent in DP at last digit (approx).
    uint8_t dpMask = 0;
    // Add DP on last digit to hint percent. TM1637Display dpMask bits correspond to digits.
    // For 4 digits: bit0->digit0 (rightmost). We'll set rightmost DP.
    dpMask = 0x01;
    uint16_t val = currentLevelPercent;
    // Ensure 0-100 fits 3 digits.
    // showNumberDecEx expects negative for dp? We'll use explicit dp and leading zeros false.
    display.showNumberDecEx(val, dpMask, true);
  } else {
    // Show usage count (0..9999) lower 4 digits
    uint16_t val = (uint16_t)(todayUseCount % 10000UL);
    display.showNumberDec(val, false, 4, 0);
  }
}

// ===== Serial commands =====
// Accept lines:
/// "STAT" or "?" -> print statistics
/// "RESET" -> reset todayUseCount
static void handleSerialLine(const String &lineRaw) {
  String line = lineRaw;
  line.trim();
  if (line.length() == 0) return;

  if (line.equalsIgnoreCase("STAT") || line.equals("?")) {
    Serial.print("LEVEL_PERCENT=");
    Serial.print(currentLevelPercent);
    Serial.print(",LOW=");
    Serial.print(lowLevel ? 1 : 0);
    Serial.print(",VALVE=");
    Serial.print(valveActive ? 1 : 0);
    Serial.print(",TODAY_USE_COUNT=");
    Serial.println(todayUseCount);
  } else if (line.equalsIgnoreCase("RESET")) {
    todayUseCount = 0;
    Serial.println("OK: todayUseCount reset");
  } else if (line.equalsIgnoreCase("HELP")) {
    Serial.println("Commands: STAT/? , RESET , HELP");
  } else {
    Serial.println("ERR: Unknown command");
  }
}

// ===== Setup/Loop =====
void setup() {
  pinMode(PIN_IR, INPUT); // 红外接近传感器
  pinMode(PIN_VALVE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_VALVE, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_LED, LOW);

  // TM1637 initialization
  display.setBrightness(0x0f, true); // max brightness
  display.clear();

  Serial.begin(BAUD);
  serialLine.reserve(64);

  // Initialize level
  float d = measureDistanceCm();
  currentLevelPercent = distanceToPercent(d);
  lowLevel = (currentLevelPercent < LOW_LEVEL_PERCENT);
}

void loop() {
  const uint32_t now = millis();

  // ----- Periodic level measurement -----
  if (now - lastLevelUpdateMs >= LEVEL_UPDATE_INTERVAL_MS) {
    lastLevelUpdateMs = now;
    float dist = measureDistanceCm();
    uint8_t pct = distanceToPercent(dist);
    currentLevelPercent = pct;
    lowLevel = (currentLevelPercent < LOW_LEVEL_PERCENT);
  }

  // ----- IR detection -----
  // We detect rising edge from sensor to count a "user close" event.
  int ir = digitalRead(PIN_IR);
  bool irHigh = (ir == HIGH);

  bool countCandidate = false;
  if (irHigh && !irWasHigh) {
    // rising edge
    countCandidate = true;
  }
  irWasHigh = irHigh;

  if (countCandidate) {
    // If low level, inhibit outflow and alarm
    if (!lowLevel) {
      // Enforce minimum interval between counts
      if (now - lastCountMs >= MIN_COUNT_INTERVAL_MS) {
        lastCountMs = now;
        todayUseCount++;

        // Start valve if not already active
        valveActive = true;
        valveStartMs = now;
        digitalWrite(PIN_LED, HIGH);
        digitalWrite(PIN_VALVE, HIGH);
      } else {
        // Still allow alarm-free; no new count/valve start
      }
    } else {
      // Low level: alarm is handled below; do not start valve
      // Ensure outputs reflect alarm state (buzzer toggling + LED)
      digitalWrite(PIN_LED, HIGH);
    }
  }

  // ----- Valve timing -----
  if (valveActive) {
    if (now - valveStartMs >= VALVE_ON_TIME_MS) {
      valveActive = false;
      digitalWrite(PIN_VALVE, LOW);
      // LED depends on lowLevel/alarm; handled below
    }
  }

  // ----- Alarm handling -----
  if (lowLevel) {
    // Alarm is active continuously with toggling
    if (now - lastAlarmToggleMs >= (alarmState ? ALARM_OFF_MS : ALARM_ON_MS)) {
      lastAlarmToggleMs = now;
      alarmState = !alarmState;
      digitalWrite(PIN_BUZZER, alarmState ? HIGH : LOW);
    }

    // LED: show lowLevel (including during valve inactive)
    digitalWrite(PIN_LED, HIGH);
  } else {
    // No alarm
    digitalWrite(PIN_BUZZER, LOW);
    // LED shows valve activity or off if none
    digitalWrite(PIN_LED, valveActive ? HIGH : LOW);
  }

  // ----- Display update -----
  updateDisplay();

  // ----- Serial parsing (line-based) -----
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      handleSerialLine(serialLine);
      serialLine = "";
    } else {
      // Keep buffer bounded
      if (serialLine.length() < 96) serialLine += c;
    }
  }
}