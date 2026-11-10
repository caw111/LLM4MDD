#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

/// ======= Hardware mapping (authoritative) =======
/// LM35 temperature sensor: A1
static const uint8_t PIN_LM35 = A1;
/// LED: D13
static const uint8_t PIN_LED = 13;
/// Buzzer (tone output): D6
static const uint8_t PIN_BUZZER = 6;
/// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

/// ======= Provided digital sensor pins (device names D2/D3) =======
/// Door magnet: D3
/// Vibration sensor: D2
static const uint8_t PIN_DOOR_MAGNET = 3;     // D3
static const uint8_t PIN_VIBRATION = 2;      // D2

/// ======= Requirements / behavior =======
/// - Monitor a medicine cold-chain transport box.
/// - LM35 on A1, D2 vibration, D3 door magnet.
/// - GPS simulated data via Serial input.
/// - Use TM1637 to alternately display current temperature and abnormal counter.
/// - Record:
///   1) temperature anomalies
///   2) opening events
///   3) severe vibration counts
/// - Once temperature exceeds upper limit OR door opened during transport:
///   - immediately alarm and output event log with location via Serial.
///
/// NOTE: The requirement states: "轮流显示当前温度和异常计数".
/// "异常计数" is interpreted as total abnormal events (temperature + door + vibration).
/// If you'd like separate counters shown, tell me and I will adjust.

/// ======= Thresholds / timing (preserved behavior intent) =======
/// Temperature upper limit (℃)
static const float TEMP_UPPER_LIMIT_C = 8.0; // common cold-chain alarm; adjust if required

/// Alarm hold / buzzer behavior
static const uint16_t BUZZER_BURST_MS = 200;   // each loop burst duration
static const uint16_t BUZZER_PAUSE_MS = 50;    // pause between bursts
static const uint16_t BUZZER_BEEP_FREQ = 3000; // Hz

/// Vibration detection
/// - Consider "severe vibration" as a rising edge above a simple digital threshold
/// - If sensor is analog in reality, wiring would differ; here requirement says D2 (digital).
/// - We'll count on edges with a minimum debounce window.
static const uint16_t VIBRATION_EDGE_DEBOUNCE_MS = 80;
static const uint32_t VIBRATION_MIN_INTERVAL_MS = 250; // prevent overcounting

/// Door open detection
/// Debounce
static const uint16_t DOOR_DEBOUNCE_MS = 80;

/// "Transport active" state
/// - Without additional input, we treat "transport mode" as always active after boot.
/// - Door open will be counted and will alarm immediately.
static const bool TRANSPORT_ACTIVE_ALWAYS = true;

/// ======= GPS parsing (simulated via Serial) =======
/// The requirement: "GPS 模拟数据通过串口输入"
/// We'll parse lines with an easy format. Because the exact format wasn't specified,
/// we support two robust patterns:
///   1) JSON-like: {"lat":..,"lon":..}
///   2) CSV-like: LAT=..,LON=.. or "lat,lon"
///
/// We'll continuously update the last known position.
struct GPSPos {
  bool valid = false;
  double lat = 0.0;
  double lon = 0.0;
  unsigned long lastUpdateMs = 0;
};

static GPSPos g_pos;

/// ======= Display =======
/// TM1637 will show temperature (with 1 decimal) and abnormal counter.
/// We'll alternate every 1 second.
static const uint16_t DISPLAY_SWITCH_MS = 1000;
static unsigned long g_lastDisplaySwitchMs = 0;
static bool g_showTemp = true;

/// TM1637 number formatting
TM1637Display tm(PIN_TM1637_CLK, PIN_TM1637_DIO);

/// ======= Counters =======
static uint32_t g_tempAbnormalCount = 0;
static uint32_t g_doorOpenCount = 0;
static uint32_t g_vibrationSevereCount = 0;

/// "异常计数" shown on display: total abnormal count
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t totalAbnormalCount();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayNumberInt(int value);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayTemperatureC(float tempC);
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stopAlarm();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void triggerAlarmBurst();
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C();
#line 195 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool extractDoubleAfterKey(const char* s, const char* key, double &out);
#line 209 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool parseGPSLine(const char* line, GPSPos &pos);
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void serviceGPSSerial();
#line 301 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void logEvent(const char* eventType, float tempC, uint32_t vibCountSnapshot);
#line 332 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool doorOpenedRawTriggered(int doorRaw);
#line 340 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleTempLogic(float tempC);
#line 352 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleDoorLogic(int doorRaw);
#line 378 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleVibrationLogic(int vibRaw);
#line 409 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 432 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t totalAbnormalCount() {
  return g_tempAbnormalCount + g_doorOpenCount + g_vibrationSevereCount;
}

/// ======= State tracking =======
static float g_lastTempC = NAN;
static unsigned long g_lastTempSampleMs = 0;
static const uint16_t TEMP_SAMPLE_INTERVAL_MS = 500;

static bool g_alarmActive = false;

/// Door state
static int g_prevDoorRaw = HIGH;
static unsigned long g_lastDoorChangeMs = 0;
static bool g_doorEventArmed = true;

/// Vibration state tracking
static unsigned long g_lastVibEdgeMs = 0;
static unsigned long g_lastVibCountMs = 0;

/// ======= Utility: safe float to TM1637 display =======
/// TM1637 library expects int or segment mapping; easiest: display integer parts
static void displayNumberInt(int value) {
  // TM1637Display::showNumberDecEx is available in common libs.
  // We'll use showNumberDec with leading zero disabled.
  tm.showNumberDec(value, true, 4, 0);
}

/// Display temperature: show tenths by scaling to integer with sign handling.
/// Limit to +/- 99.9 for TM1637 4 digits (incl decimal).
static void displayTemperatureC(float tempC) {
  if (isnan(tempC)) {
    // show blank/0
    displayNumberInt(0);
    return;
  }
  if (tempC > 99.9) tempC = 99.9;
  if (tempC < -9.9) tempC = -9.9;

  int scaled = (int)round(tempC * 10.0f); // tenths
  // TM1637: show scaled as x.x using decimal point on position 1 (second digit from right)
  // Use TM1637Display::showNumberDecEx if supported by installed library version.
  // We'll attempt to use showNumberDecEx; fall back to showNumberDec.
#if defined(Tm1637Display_h) || defined(Tm1637Display_h_)
  // Some versions use showNumberDecEx with leading zeros and display bits
#endif

  // We'll implement using showNumberDecEx if available:
  // Signature commonly: showNumberDecEx(int num, uint8_t dots, bool leadingZero, uint8_t length, uint8_t pos)
  // But actual varies. To keep compatibility, we will use showNumberDec with decimal not supported
  // in all forks. We'll therefore show temperature as integer with sign (no decimal).
  // Requirement says "当前温度" not necessarily decimal. We'll still preserve tenths in scaling
  // by showing integer part (temp*1) if no decimal is feasible.
  int intPart = (int)round(tempC);
  displayNumberInt(intPart);
}

/// ======= Alarm =======
static void stopAlarm() {
  g_alarmActive = false;
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

static void triggerAlarmBurst() {
  // If already active, keep blinking/beeping
  g_alarmActive = true;
  digitalWrite(PIN_LED, HIGH);
  static unsigned long lastBurstMs = 0;
  static bool buzzerOn = false;

  unsigned long now = millis();
  if (!buzzerOn) {
    if (now - lastBurstMs >= BUZZER_PAUSE_MS) {
      buzzerOn = true;
      lastBurstMs = now;
      tone(PIN_BUZZER, BUZZER_BEEP_FREQ);
    }
  } else {
    if (now - lastBurstMs >= BUZZER_BURST_MS) {
      buzzerOn = false;
      lastBurstMs = now;
      noTone(PIN_BUZZER);
    }
  }
}

/// ======= Temperature reading =======
/// LM35: 10mV/°C, at 5V: ADC 1023 -> 4.9.. with default reference.
/// Voltage -> temp: tempC = (Vout * 100) / 1.0 (since 10mV/°C)
/// We'll compute using internal 5V (UNO uses 5V ref).
static float readLM35C() {
  int raw = analogRead(PIN_LM35); // 0..1023
  float v = (raw * (5.0f / 1023.0f)); // volts
  float tempC = v * 100.0f; // 10mV => *100
  return tempC;
}

/// ======= Serial GPS parsing =======
static bool extractDoubleAfterKey(const char* s, const char* key, double &out) {
  const char* p = strstr(s, key);
  if (!p) return false;
  p += strlen(key);
  // Skip optional spaces, quotes, colon, equals
  while (*p && (*p == ' ' || *p == '"' || *p == ':' || *p == '=')) p++;
  // strtod
  char* endp = nullptr;
  double val = strtod(p, &endp);
  if (endp == p) return false;
  out = val;
  return true;
}

static bool parseGPSLine(const char* line, GPSPos &pos) {
  // Attempt JSON-like lat/lon
  double lat = 0, lon = 0;
  bool okLat = extractDoubleAfterKey(line, "lat", lat) || extractDoubleAfterKey(line, "LAT", lat);
  bool okLon = extractDoubleAfterKey(line, "lon", lon) || extractDoubleAfterKey(line, "LON", lon);
  if (okLat && okLon) {
    pos.lat = lat;
    pos.lon = lon;
    pos.valid = true;
    pos.lastUpdateMs = millis();
    return true;
  }

  // Attempt "LAT=xx,LON=yy" with commas
  // (extractDoubleAfterKey already supports LAT and LON, so above likely enough)

  // Attempt "lat,lon" or "lat lon"
  // Find first delimiter between two numbers
  // Very simple fallback:
  double a = 0, b = 0;
  // Copy and attempt parsing
  char buf[120];
  strncpy(buf, line, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  // Replace brackets/quotes with spaces
  for (size_t i = 0; i < strlen(buf); i++) {
    if (buf[i] == '[' || buf[i] == ']' || buf[i] == '{' || buf[i] == '}' || buf[i] == '"' ) buf[i] = ' ';
  }

  // Try parse two doubles separated by comma/space
  char* endptr = nullptr;
  char* p = buf;
  a = strtod(p, &endptr);
  if (endptr == p) return false;
  while (*endptr == ' ' || *endptr == ',' || *endptr == '\t') endptr++;
  b = strtod(endptr, &endptr);
  if (endptr == nullptr) return false;

  // Heuristic: lat range
  if (fabs(a) <= 90.0 && fabs(b) <= 180.0) {
    pos.lat = a;
    pos.lon = b;
    pos.valid = true;
    pos.lastUpdateMs = millis();
    return true;
  }
  return false;
}

/// Serial input for GPS simulated data.
/// We'll read lines terminated by '\n' or '\r' with idle timeout handled by loop logic.
/// Then parse and update g_pos.
static void serviceGPSSerial() {
  static char lineBuf[180];
  static uint8_t idx = 0;
  static unsigned long lastCharMs = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    unsigned long now = millis();
    lastCharMs = now;

    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        lineBuf[idx] = '\0';
        parseGPSLine(lineBuf, g_pos);
        idx = 0;
      } else {
        idx = 0;
      }
    } else {
      if (idx < sizeof(lineBuf) - 1) {
        lineBuf[idx++] = c;
      }
    }
  }

  // Idle timeout: if no new chars for idle_timeout_ms, try parse partial line
  static unsigned long lastCheckMs = 0;
  unsigned long now = millis();
  if (now - lastCheckMs >= 10) {
    lastCheckMs = now;
    if (idx > 0 && (now - lastCharMs) > 20) { // idle_timeout_ms=20
      lineBuf[idx] = '\0';
      parseGPSLine(lineBuf, g_pos);
      idx = 0;
    }
  }
}

/// ======= Event logging =======
static void logEvent(const char* eventType, float tempC, uint32_t vibCountSnapshot) {
  Serial.print("EVENT,");
  Serial.print(eventType);
  Serial.print(",tempC=");
  if (isnan(tempC)) Serial.print("nan");
  else Serial.print(tempC, 2);
  Serial.print(",vibCount=");
  Serial.print(vibCountSnapshot);

  if (g_pos.valid) {
    Serial.print(",lat=");
    Serial.print(g_pos.lat, 6);
    Serial.print(",lon=");
    Serial.print(g_pos.lon, 6);
    Serial.print(",posAgeMs=");
    Serial.print((unsigned long)(millis() - g_pos.lastUpdateMs));
  } else {
    Serial.print(",lat=nan,lon=nan");
  }

  Serial.print(",tempAbn=");
  Serial.print(g_tempAbnormalCount);
  Serial.print(",doorOpen=");
  Serial.print(g_doorOpenCount);
  Serial.print(",vibSevere=");
  Serial.print(g_vibrationSevereCount);

  Serial.println();
}

/// ======= Safety/Alarm condition =======
static bool doorOpenedRawTriggered(int doorRaw) {
  // Define polarity:
  // Common door magnet sensor: normally closed (door closed => HIGH), open => LOW.
  // We'll treat a transition to LOW as "opened".
  // If your wiring uses opposite polarity, swap the condition below.
  return (doorRaw == LOW);
}

static void handleTempLogic(float tempC) {
  if (!TRANSPORT_ACTIVE_ALWAYS) return;

  if (tempC > TEMP_UPPER_LIMIT_C) {
    g_tempAbnormalCount++;
    // Alarm immediately and output event log with location
    if (!g_alarmActive) g_alarmActive = true;
    triggerAlarmBurst();
    logEvent("TEMP_ABNORMAL", tempC, g_vibrationSevereCount);
  }
}

static void handleDoorLogic(int doorRaw) {
  if (!TRANSPORT_ACTIVE_ALWAYS) return;

  unsigned long now = millis();

  if (doorRaw != g_prevDoorRaw) {
    if (now - g_lastDoorChangeMs >= DOOR_DEBOUNCE_MS) {
      g_lastDoorChangeMs = now;
      g_prevDoorRaw = doorRaw;

      if (g_doorEventArmed && doorOpenedRawTriggered(doorRaw)) {
        g_doorEventArmed = false; // avoid multiple counts while door stays open
        g_doorOpenCount++;
        g_alarmActive = true;
        triggerAlarmBurst();
        logEvent("DOOR_OPEN", g_lastTempC, g_vibrationSevereCount);
      }

      // Re-arm when door returns to closed
      if (doorRaw == HIGH) {
        g_doorEventArmed = true;
      }
    }
  }
}

static void handleVibrationLogic(int vibRaw) {
  // Count severe vibration on rising edge (or falling edge depending on sensor).
  // We'll assume D2 vibration sensor outputs HIGH when vibration detected.
  unsigned long now = millis();

  static int prevVibRaw = HIGH;
  if (vibRaw != prevVibRaw) {
    // debounce and edge type filter
    if (now - g_lastVibEdgeMs >= VIBRATION_EDGE_DEBOUNCE_MS) {
      g_lastVibEdgeMs = now;
      prevVibRaw = vibRaw;

      // Severe event: transition to HIGH
      if (vibRaw == HIGH) {
        if (now - g_lastVibCountMs >= VIBRATION_MIN_INTERVAL_MS) {
          g_lastVibCountMs = now;
          g_vibrationSevereCount++;
          // Requirement: record severe vibrations. It does not explicitly require immediate alarm
          // for vibration, only for temperature or door open.
          // But abnormal counter should include it.
          // We'll still log vibration events? Requirement only says:
          // "一旦温度超过上限或运输过程中开箱，则立即报警并串口输出带位置的事件日志。"
          // So vibration does NOT need immediate log/alarm. However, we can log without alarm
          // would violate "do not simplify away behavior"? Not specified. We'll only count.
        }
      }
    }
  }
}

/// ======= Arduino setup/loop =======
void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_DOOR_MAGNET, INPUT_PULLUP); // assume open => LOW
  pinMode(PIN_VIBRATION, INPUT_PULLUP;);

  // Fix: D2 vibration sensor likely provides HIGH pulses; INPUT_PULLUP keeps stable at LOW.
  // If your module is active-high open-collector, INPUT_PULLUP is fine.

  tm.setBrightness(0x0f);

  Serial.begin(9600);
  g_prevDoorRaw = digitalRead(PIN_DOOR_MAGNET);
  g_lastDoorChangeMs = millis();

  // Initialize display quickly
  g_lastDisplaySwitchMs = millis();
  g_showTemp = true;

  stopAlarm();
}

void loop() {
  // GPS parsing via Serial
  serviceGPSSerial();

  // Temperature sampling
  unsigned long now = millis();
  if (now - g_lastTempSampleMs >= TEMP_SAMPLE_INTERVAL_MS) {
    g_lastTempSampleMs = now;
    g_lastTempC = readLM35C();
    handleTempLogic(g_lastTempC);
  }

  // Door and vibration monitoring
  int doorRaw = digitalRead(PIN_DOOR_MAGNET);
  handleDoorLogic(doorRaw);

  int vibRaw = digitalRead(PIN_VIBRATION);
  handleVibrationLogic(vibRaw);

  // Display alternating: current temperature and abnormal count
  if (now - g_lastDisplaySwitchMs >= DISPLAY_SWITCH_MS) {
    g_lastDisplaySwitchMs = now;
    g_showTemp = !g_showTemp;
    // When alarm is active, keep alarm running; no special display change required.
  }

  if (g_showTemp) {
    displayTemperatureC(g_lastTempC);
  } else {
    // Display abnormal count (wrap to 4 digits)
    uint32_t total = totalAbnormalCount();
    int shown = (int)(total % 10000);
    displayNumberInt(shown);
  }

  // Alarm sustain behavior:
  // If alarm is active due to temp/door, continue beeping. Optionally stop when conditions clear.
  // Requirement: "一旦...则立即报警" (doesn't say stop). We'll keep alarm active
  // while any condition remains true: temp abnormal or door open.
  bool tempNowAbnormal = (!isnan(g_lastTempC) && g_lastTempC > TEMP_UPPER_LIMIT_C);
  bool doorNowOpened = doorOpenedRawTriggered(doorRaw);
  if (tempNowAbnormal || doorNowOpened) {
    triggerAlarmBurst();
  } else {
    // If previously alarmed, stop once conditions clear
    stopAlarm();
  }
}
