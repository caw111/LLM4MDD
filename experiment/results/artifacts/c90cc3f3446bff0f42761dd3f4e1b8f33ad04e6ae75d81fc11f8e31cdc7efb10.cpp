#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <math.h>

// ======= Hardware mapping (authoritative) =======
// LM35 temperature sensor: A1
static const uint8_t PIN_LM35 = A1;

// LED: D13
static const uint8_t PIN_LED = 13;

// Buzzer (tone output): D6
static const uint8_t PIN_BUZZER = 6;

// TM1637: CLK=D4, DIO=D5
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 5;

// Provided digital sensor pins (device names D2/D3)
static const uint8_t PIN_VIBRATION = 2;     // D2
static const uint8_t PIN_DOOR_MAGNET = 3;    // D3

// ======= Requirements / behavior =======
// - Monitor medicine cold-chain transport box using:
//   LM35 (A1), vibration sensor (D2), door magnet (D3)
// - GPS simulated data via Serial input.
// - Use TM1637 to alternately display current temperature and abnormal counter.
// - Record:
///   1) temperature anomalies
//   2) opening events
//   3) severe vibration counts
// - Once temperature exceeds upper limit OR door opened during transport:
//   - immediately alarm and output event log with location via Serial.
// - "异常计数" shown on display is interpreted as total abnormal events
//   (temperature + door + vibration).

// ======= Thresholds / timing (preserved behavior intent) =======
// Temperature upper limit (℃)
static const float TEMP_UPPER_LIMIT_C = 8.0; // adjust as required

// Alarm hold / buzzer behavior
static const uint16_t BUZZER_BURST_MS = 200;   // each loop burst duration
static const uint16_t BUZZER_PAUSE_MS = 50;    // pause between bursts
static const uint16_t BUZZER_BEEP_FREQ = 3000; // Hz

// Vibration detection
static const uint16_t VIBRATION_EDGE_DEBOUNCE_MS = 80;
static const uint32_t VIBRATION_MIN_INTERVAL_MS = 250; // prevent overcounting

// Door open detection
static const uint16_t DOOR_DEBOUNCE_MS = 80;

// "Transport active" state
static const bool TRANSPORT_ACTIVE_ALWAYS = true;

// ======= GPS parsing (simulated via Serial) =======
struct GPSPos {
  bool valid = false;
  double lat = 0.0;
  double lon = 0.0;
  unsigned long lastUpdateMs = 0;
};
static GPSPos g_pos;

// ======= Display =======
static const uint16_t DISPLAY_SWITCH_MS = 1000;
static unsigned long g_lastDisplaySwitchMs = 0;
static bool g_showTemp = true;

// TM1637 display object
static TM1637Display tm(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ======= Counters =======
static uint32_t g_tempAbnormalCount = 0;
static uint32_t g_doorOpenCount = 0;
static uint32_t g_vibrationSevereCount = 0;

#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint32_t totalAbnormalCount();
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void stopAlarm();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void triggerAlarmBurst();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayNumberInt(int value);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayTemperatureC(float tempC);
#line 148 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static float readLM35C();
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool extractDoubleAfterKey(const char* s, const char* key, double &out);
#line 171 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool parseGPSLine(const char* line, GPSPos &pos);
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void serviceGPSSerial();
#line 252 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void logEvent(const char* eventType, float tempC, uint32_t vibCountSnapshot);
#line 284 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool doorOpenedRawTriggered(int doorRaw);
#line 289 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleTempLogic(float tempC);
#line 302 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleDoorLogic(int doorRaw);
#line 330 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleVibrationLogic(int vibRaw);
#line 352 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 372 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_023\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint32_t totalAbnormalCount() {
  return g_tempAbnormalCount + g_doorOpenCount + g_vibrationSevereCount;
}

// ======= State tracking =======
static float g_lastTempC = NAN;
static unsigned long g_lastTempSampleMs = 0;
static const uint16_t TEMP_SAMPLE_INTERVAL_MS = 500;

static bool g_alarmActive = false;

// Door state
static int g_prevDoorRaw = HIGH;
static unsigned long g_lastDoorChangeMs = 0;
static bool g_doorEventArmed = true;

// Vibration edge tracking
static unsigned long g_lastVibEdgeMs = 0;
static unsigned long g_lastVibCountMs = 0;

// ======= Utility =======
static void stopAlarm() {
  g_alarmActive = false;
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}

static void triggerAlarmBurst() {
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

// ======= Display helpers =======
static void displayNumberInt(int value) {
  tm.showNumberDec(value, true, 4, 0);
}

static void displayTemperatureC(float tempC) {
  if (isnan(tempC)) {
    displayNumberInt(0);
    return;
  }
  if (tempC > 99.9) tempC = 99.9;
  if (tempC < -9.9) tempC = -9.9;

  // Keep behavior from previous sketch: display integer rounded temperature.
  int intPart = (int)round(tempC);
  displayNumberInt(intPart);
}

// ======= Temperature reading =======
static float readLM35C() {
  int raw = analogRead(PIN_LM35);
  float v = (raw * (5.0f / 1023.0f)); // volts
  float tempC = v * 100.0f;          // 10mV/°C => *100
  return tempC;
}

// ======= GPS parsing =======
static bool extractDoubleAfterKey(const char* s, const char* key, double &out) {
  const char* p = strstr(s, key);
  if (!p) return false;
  p += strlen(key);

  while (*p && (*p == ' ' || *p == '"' || *p == ':' || *p == '=')) p++;

  char* endp = nullptr;
  double val = strtod(p, &endp);
  if (endp == p) return false;

  out = val;
  return true;
}

static bool parseGPSLine(const char* line, GPSPos &pos) {
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

  // Fallback: "lat,lon" or "lat lon"
  char buf[180];
  strncpy(buf, line, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  // Replace brackets/quotes with spaces
  for (size_t i = 0; i < strlen(buf); i++) {
    if (buf[i] == '[' || buf[i] == ']' || buf[i] == '{' || buf[i] == '}' || buf[i] == '"')
      buf[i] = ' ';
  }

  char* endptr = nullptr;
  char* p = buf;

  double a = strtod(p, &endptr);
  if (endptr == p) return false;

  while (*endptr == ' ' || *endptr == ',' || *endptr == '\t') endptr++;
  double b = strtod(endptr, &endptr);
  if (endptr == nullptr) return false;

  if (fabs(a) <= 90.0 && fabs(b) <= 180.0) {
    pos.lat = a;
    pos.lon = b;
    pos.valid = true;
    pos.lastUpdateMs = millis();
    return true;
  }
  return false;
}

static void serviceGPSSerial() {
  static char lineBuf[200];
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
      if (idx < sizeof(lineBuf) - 1) lineBuf[idx++] = c;
    }
  }

  // idle_timeout_ms = 20 (as requirement)
  static unsigned long lastCheckMs = 0;
  unsigned long now = millis();
  if (now - lastCheckMs >= 10) {
    lastCheckMs = now;
    if (idx > 0 && (now - lastCharMs) > 20) {
      lineBuf[idx] = '\0';
      parseGPSLine(lineBuf, g_pos);
      idx = 0;
    }
  }
}

// ======= Event logging =======
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

// ======= Conditions / logic =======
static bool doorOpenedRawTriggered(int doorRaw) {
  // Common door magnet sensor: normally closed (door closed => HIGH), open => LOW.
  return (doorRaw == LOW);
}

static void handleTempLogic(float tempC) {
  if (!TRANSPORT_ACTIVE_ALWAYS) return;

  if (tempC > TEMP_UPPER_LIMIT_C) {
    g_tempAbnormalCount++;

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
  unsigned long now = millis();

  static int prevVibRaw = HIGH;
  if (vibRaw != prevVibRaw) {
    if (now - g_lastVibEdgeMs >= VIBRATION_EDGE_DEBOUNCE_MS) {
      g_lastVibEdgeMs = now;
      prevVibRaw = vibRaw;

      // Severe event: transition to HIGH
      if (vibRaw == HIGH) {
        if (now - g_lastVibCountMs >= VIBRATION_MIN_INTERVAL_MS) {
          g_lastVibCountMs = now;
          g_vibrationSevereCount++;
          // No immediate alarm requirement for vibration (only counts).
        }
      }
    }
  }
}

// ======= Arduino setup/loop =======
void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_DOOR_MAGNET, INPUT_PULLUP); // assume open => LOW
  pinMode(PIN_VIBRATION, INPUT_PULLUP);  // fix compilation error: remove stray ';'

  tm.setBrightness(0x0f);

  Serial.begin(9600);

  g_prevDoorRaw = digitalRead(PIN_DOOR_MAGNET);
  g_lastDoorChangeMs = millis();

  g_lastDisplaySwitchMs = millis();
  g_showTemp = true;

  stopAlarm();
}

void loop() {
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
  }

  if (g_showTemp) {
    displayTemperatureC(g_lastTempC);
  } else {
    uint32_t total = totalAbnormalCount();
    int shown = (int)(total % 10000);
    displayNumberInt(shown);
  }

  // Alarm sustain behavior:
  // Keep alarm active while any condition remains true (temp abnormal or door opened).
  bool tempNowAbnormal = (!isnan(g_lastTempC) && g_lastTempC > TEMP_UPPER_LIMIT_C);
  bool doorNowOpened = doorOpenedRawTriggered(doorRaw);

  if (tempNowAbnormal || doorNowOpened) {
    triggerAlarmBurst();
  } else {
    stopAlarm();
  }
}
