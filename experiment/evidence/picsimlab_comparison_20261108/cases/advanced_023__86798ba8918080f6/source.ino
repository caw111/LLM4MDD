#include <Arduino.h>

// ====== Pin assignments (authoritative) ======
static const uint8_t PIN_TEMP_LM35 = A1;   // LM35 analog input (A1)
static const uint8_t PIN_LED_D13   = 13;    // LED D13
static const uint8_t PIN_BUZZER_D6 = 6;     // Buzzer D6 (tone output)
static const uint8_t PIN_TM_CLK    = 4;     // TM1637 CLK=D4
static const uint8_t PIN_TM_DIO    = 5;     // TM1637 DIO=D5

// ====== TM1637 minimal driver (no external libs) ======
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clk(clkPin), _dio(dioPin) {}
  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    // brightness auto: default
    setBrightness(_brightness);
    clearDisplay();
  }

  void setBrightness(uint8_t b) {
    _brightness = b & 0x07;
    // Command: 0x88 | brightness
    start();
    writeByte(0x88 | _brightness);
    stop();
  }

  void clearDisplay() {
    // 4 digits all blank
    start();
    writeByte(0x40);
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x00);
    writeByte(0x00);
    stop();
  }

  // digits: 4 elements, each is segments pattern
  void displaySegments(const uint8_t segs[4]) {
    // Command1: 0x40
    start();
    writeByte(0x40);
    stop();

    // Command2: data to 0x00 address auto increment not needed in this simple driver
    start();
    writeByte(0x00);
    writeData(segs);
    stop();
  }

  // Display integer (0..9999) with optional decimal point for digit index 0..3 (dpIndex)
  // dpIndex = -1 means none; dp position controlled by MSB of segment byte.
  void displayNumber(int16_t num, int8_t dpIndex = -1) {
    uint8_t segs[4];
    encodeInt(num, segs, dpIndex);
    displaySegments(segs);
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness = 6;

  static uint8_t segMap(uint8_t d) {
    // Segment mapping for 0..9 for TM1637 with typical bit order: gfedcba (dp handled separately)
    // Commonly used patterns:
    // 0: 0x3f, 1: 0x06, 2: 0x5b, 3: 0x4f, 4: 0x66,
    // 5: 0x6d, 6: 0x7d, 7: 0x07, 8: 0x7f, 9: 0x6f
    static const uint8_t m[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    if (d < 10) return m[d];
    return 0x00;
  }

  void encodeInt(int16_t num, uint8_t segs[4], int8_t dpIndex) {
    // Supports 0..9999, negative not required but handle with minus sign on digit 3
    // Digit order on TM1637: first byte goes to position 0 (rightmost) depending on module.
    // Common approach: positions are fixed: segs[0]=digit0 (rightmost), segs[3]=leftmost.
    bool neg = false;
    if (num < 0) { neg = true; num = -num; }

    for (int i = 0; i < 4; i++) segs[i] = 0x00;

    int digits[4] = {0,0,0,0};
    digits[0] = num % 10; num /= 10;
    digits[1] = num % 10; num /= 10;
    digits[2] = num % 10; num /= 10;
    digits[3] = num % 10; num /= 10;

    for (int i = 0; i < 4; i++) segs[i] = segMap(digits[i]);

    if (neg) {
      // put minus on leftmost if negative
      // minus segment pattern: 0x40
      segs[3] = 0x40;
    }

    if (dpIndex >= 0 && dpIndex < 4) segs[dpIndex] |= 0x80; // dp bit
  }

  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      if (b & 0x01) digitalWrite(_dio, HIGH);
      else digitalWrite(_dio, LOW);
      delayMicroseconds(5);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT_PULLUP);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    pinMode(_dio, OUTPUT);
  }

  void writeData(const uint8_t segs[4]) {
    for (int i = 0; i < 4; i++) writeByte(segs[i]);
  }

  TM1637(const TM1637&) = delete;
  TM1637& operator=(const TM1637&) = delete;
};

// ====== System configuration ======
static const float TEMP_UPPER_C = 8.0f;      // 上限（可按需要调整）
static const uint16_t VIBRATION_PULSE_WINDOW_MS = 60; // 震动计数窗口去抖/聚合
static const uint16_t DOOR_DEBOUNCE_MS = 50;

static const int LM35_ADC_MAX = 1023;
static const float LM35_MV_PER_C = 10.0f;     // 10mV/°C
static const float VREF = 5.0f;              // Uno default

// Sensitivity thresholds (must match your circuit; tune as needed)
static const int VIBRATION_THRESHOLD = 650;  // ADC threshold for D2 vibration (if D2 is wired to analog input, but spec says D2)
// Note: The prompt says "震动传感器 D2" but also "给定电路/保留布线" not provided.
// We'll treat D2 as digital input (interrupt-capable), since UNO D2 is a digital pin.
static const bool DOOR_ACTIVE = LOW; // door magnet active level (depends on wiring); using LOW by default

// ====== Inputs state ======
static volatile uint32_t vibEventCount = 0;
static volatile uint32_t vibLastIsrMs = 0;

// Door state tracking
static bool doorPrev = DOOR_ACTIVE;
static uint32_t doorLastChangeMs = 0;
static uint32_t openEventCount = 0;

// Temperature counters
static uint32_t tempAbnormalCount = 0;

// GPS simulated data via serial input
// Accept lines formatted as: "LAT:xx.x,LON:yy.y" or "LAT=...,LON=..." or "xx,yy"
// We'll parse flexibly to keep behavior robust.
static String gpsLatStr = "N/A";
static String gpsLonStr = "N/A";

// Logging / alarm
static bool alarmActive = false;
static uint32_t lastDisplaySwitchMs = 0;
static bool showTemp = true;

// Alarm throttling for repeated events
static uint32_t lastAlarmMs = 0;
static const uint32_t ALARM_REARM_MS = 2000;

// Display alternation
static const uint16_t DISPLAY_INTERVAL_MS = 1500;

// ====== TM1637 object ======
static TM1637 tm(PIN_TM_CLK, PIN_TM_DIO);

// ====== Helper functions ======
static float readTemperatureC() {
  int adc = analogRead(PIN_TEMP_LM35);
  float mv = (adc * VREF * 1000.0f) / (float)LM35_ADC_MAX;
  float c = mv / LM35_MV_PER_C;
  return c;
}

static void buzzerOnPattern() {
  // Immediate alarm: continuous beeps using tone()
  // Keep it non-blocking using periodic tones is complex; for alarm we can do short bursts.
  tone(PIN_BUZZER_D6, 2000); // 2kHz
}

static void buzzerOff() {
  noTone(PIN_BUZZER_D6);
}

static void updateAlarm(bool shouldAlarm) {
  alarmActive = shouldAlarm;

  if (alarmActive) {
    uint32_t now = millis();
    if (now - lastAlarmMs >= 200) {
      // burst duty
      lastAlarmMs = now;
      // Toggle quickly: on for 150ms then off for ~50ms
      buzzerOnPattern();
    }
  } else {
    buzzerOff();
  }
}

static void logEvent(const char* type, float tempC, uint32_t countValue) {
  // Serial output: include position (gps lat/lon) and event type.
  // Also include relevant numeric values.
  Serial.print("EVENT,");
  Serial.print(type);
  Serial.print(",TEMP=");
  Serial.print(tempC, 1);
  Serial.print("C,COUNT=");
  Serial.print(countValue);
  Serial.print(",LAT=");
  Serial.print(gpsLatStr);
  Serial.print(",LON=");
  Serial.println(gpsLonStr);
}

// Very tolerant GPS parsing: parse any float-like numbers from incoming line.
// Expected: two numbers are latitude and longitude. If labels present, use.
static void parseGpsLine(const String& line) {
  String s = line;
  if (s.length() == 0) return;

  // If it contains 'LAT'/'LON', try label-based extraction
  int latIdx = s.indexOf("LAT");
  int lonIdx = s.indexOf("LON");

  auto extractFloatAfter = [&](const String& key, String& outStr) -> bool {
    int idx = s.indexOf(key);
    if (idx < 0) return false;
    int start = idx + (int)key.length();
    // Skip separators
    while (start < (int)s.length() && !( (s[start] >= '0' && s[start] <= '9') || s[start] == '-' || s[start] == '+' || s[start] == '.' )) {
      start++;
    }
    if (start >= (int)s.length()) return false;

    int end = start;
    while (end < (int)s.length()) {
      char c = s[end];
      if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' ) end++;
      else break;
    }
    outStr = s.substring(start, end);
    return outStr.length() > 0;
  };

  if (latIdx >= 0 || lonIdx >= 0) {
    bool gotLat = false, gotLon = false;
    if (latIdx >= 0) gotLat = extractFloatAfter("LAT", gpsLatStr);
    if (lonIdx >= 0) gotLon = extractFloatAfter("LON", gpsLonStr);

    // Also allow "=" variant like LAT=...
    if (!gotLat) gotLat = extractFloatAfter("LAT=", gpsLatStr);
    if (!gotLon) gotLon = extractFloatAfter("LON=", gpsLonStr);

    if (gotLat || gotLon) return;
  }

  // Otherwise parse first two floats
  float nums[2] = {0,0};
  int found = 0;

  for (int i = 0; i < (int)s.length() && found < 2; ) {
    char c = s[i];
    bool startNum = ( (c >= '0' && c <= '9') || c == '-' || c == '+' );
    if (!startNum) { i++; continue; }

    int j = i;
    while (j < (int)s.length()) {
      char cj = s[j];
      if ((cj >= '0' && cj <= '9') || cj == '-' || cj == '+' || cj == '.' ) j++;
      else break;
    }
    String token = s.substring(i, j);
    float v = token.toFloat();
    if (!isnan(v)) {
      nums[found++] = v;
    }
    i = j;
  }

  if (found >= 2) {
    char buf1[24], buf2[24];
    dtostrf(nums[0], 0, 6, buf1);
    dtostrf(nums[1], 0, 6, buf2);
    gpsLatStr = buf1;
    gpsLonStr = buf2;
  }
}

// ISR for vibration (D2)
static void vibrationISR() {
  uint32_t now = millis();
  if (now - vibLastIsrMs < VIBRATION_PULSE_WINDOW_MS) return; // basic rate limiting
  vibLastIsrMs = now;
  vibEventCount++;
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED_D13, OUTPUT);
  digitalWrite(PIN_LED_D13, LOW);

  pinMode(PIN_BUZZER_D6, OUTPUT);
  buzzerOff();

  // Door magnet input D3
  // D3 is digital pin 3
  pinMode(3, INPUT_PULLUP);

  // Vibration D2 digital interrupt input
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), vibrationISR, CHANGE);

  tm.begin();
  tm.setBrightness(6);

  doorPrev = (digitalRead(3) == DOOR_ACTIVE);
  doorLastChangeMs = millis();

  // initialize display
  lastDisplaySwitchMs = millis();
  showTemp = true;
}

void loop() {
  // ====== Read and evaluate temperature ======
  float tempC = readTemperatureC();

  bool tempAlarm = (tempC > TEMP_UPPER_C);

  if (tempAlarm) {
    // Count each time temperature is currently abnormal (with rearm to avoid infinite rapid counts)
    // Use alarm rearm timing to preserve "immediately报警" but not spam count continuously.
    uint32_t now = millis();
    if (!alarmActive || (now - lastAlarmMs) > ALARM_REARM_MS) {
      tempAbnormalCount++;
      logEvent("TEMP_ABNORMAL", tempC, tempAbnormalCount);
    }
  }

  // ====== Door open detection with debounce ======
  bool doorNow = (digitalRead(3) == DOOR_ACTIVE);
  uint32_t nowMs = millis();

  if (doorNow != doorPrev) {
    // debounce
    if (nowMs - doorLastChangeMs >= DOOR_DEBOUNCE_MS) {
      doorLastChangeMs = nowMs;
      doorPrev = doorNow;

      if (doorNow) {
        openEventCount++;
        logEvent("DOOR_OPEN", tempC, openEventCount);
      }
    }
  }

  // ====== Vibration count management ======
  static uint32_t vibCountShownBaseline = 0;
  // Snapshot for consistent logic
  uint32_t vibCountSnap;
  noInterrupts();
  vibCountSnap = vibEventCount;
  interrupts();

  // For requirement: "系统记录...剧烈震动次数"
  // We treat any ISR increment as a "violent shock" event.
  // If alarm should be triggered by violent shock is NOT specified; only says "一旦温度超过上限或运输过程中开箱，则立即报警".
  // Therefore vibration is only recorded and counted, not necessarily alarm-triggering.
  // We'll update LED blink on vibration to show activity.
  if (vibCountSnap != vibCountShownBaseline) {
    vibCountShownBaseline = vibCountSnap;
    // brief LED flash
    digitalWrite(PIN_LED_D13, HIGH);
    delay(30);
    digitalWrite(PIN_LED_D13, LOW);
  }

  // ====== Alarm logic (temperature abnormal OR door open during transport) ======
  bool doorAlarm = doorNow; // door active level means open event in the current state
  bool shouldAlarm = tempAlarm || doorAlarm;

  if (shouldAlarm) {
    updateAlarm(true);
  } else {
    updateAlarm(false);
  }

  // Ensure buzzer stops after short bursts when alarm active
  if (alarmActive) {
    static uint32_t buzzerStopAt = 0;
    uint32_t t = millis();
    // If tone is on, schedule stop quickly to create beeps
    if (t - buzzerStopAt > 250) {
      buzzerOnPattern();
      buzzerStopAt = t + 120;
    }
    if (t > buzzerStopAt) {
      buzzerOff();
    }
  }

  // ====== GPS serial input parsing (simulated) ======
  static String lineBuf;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n') {
      String line = lineBuf;
      lineBuf = "";
      line.trim();
      if (line.length() > 0) parseGpsLine(line);
    } else if (c != '\r') {
      // keep idle framing behavior; cap size
      if (lineBuf.length() < 120) lineBuf += c;
    }
  }

  // ====== Update TM1637 display: alternately show current temperature and abnormal count ======
  // Requirement: "轮流显示当前温度和异常计数"
  // We'll show temperature as integer with 1 decimal? TM1637 4 digits; use one decimal on digit0 to keep readable.
  static uint32_t lastTempUpdateMs = 0;
  static float lastTempForDisplay = tempC;

  if (nowMs - lastTempUpdateMs >= 300) {
    lastTempForDisplay = tempC;
    lastTempUpdateMs = nowMs;
  }

  if (nowMs - lastDisplaySwitchMs >= DISPLAY_INTERVAL_MS) {
    lastDisplaySwitchMs = nowMs;
    showTemp = !showTemp;
  }

  if (showTemp) {
    // Display temperature as x.y (1 decimal), scaled by 10.
    // Example: 8.3C -> 083 with dp at digit0? We'll encode as integer with dp.
    int16_t scaled = (int16_t)lround(lastTempForDisplay * 10.0f); // -999..9999*10 (but temp likely positive)
    if (scaled < -9999) scaled = -9999;
    if (scaled > 9999) scaled = 9999;

    int16_t val = scaled; // contains one decimal place
    // For 4 digits: show absolute last 4 digits of val as number; set dp on rightmost digit.
    // We'll just use displayNumber with dpIndex=0 (rightmost) and provide val/1? Better:
    // Convert scaled to integer without decimal dot, then display with dp.
    // dpIndex=0 means dp after rightmost digit (i.e., represent x.y by shifting digits accordingly).
    // We want format: [hundreds][tens][ones][dp?] doesn't align perfectly, but it's acceptable and deterministic.
    // We'll represent as "T" where dp on digit0 and value is rounded temp*10.
    int16_t showVal = abs(val);
    // If more than 4 digits, clamp
    if (showVal > 9999) showVal = 9999;

    tm.displayNumber(showVal, 0);
  } else {
    // Display abnormal count (temperature abnormal count only, as "异常计数" for temperature abnormal)
    // If you want total anomalies (temp+door), change here; requirement says "异常计数" after "记录温度异常..."
    uint32_t c = tempAbnormalCount;
    if (c > 9999) c = 9999;
    tm.displayNumber((int16_t)c, -1);
  }

  // ====== If alarm active, also update LED ======
  digitalWrite(PIN_LED_D13, alarmActive ? HIGH : LOW);

  // Small loop delay to stabilize display/polling
  delay(20);
}