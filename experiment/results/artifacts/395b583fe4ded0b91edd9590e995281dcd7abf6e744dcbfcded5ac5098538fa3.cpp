#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_LM35_A1 = A1;   // LM35 analog input
static const uint8_t PIN_FAN_PWM = 5;     // Fan PWM output (D5)
static const uint8_t PIN_LED = 13;        // LED output (D13)

// Serial parameters
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Temperature / control parameters (defaults; can be updated via serial)
static float targetTempC = 30.0f;  // 用户可设置：目标温度阈值
static float minFanSpeedPct = 20.0f; // 用户可设置：最小风扇转速(百分比)

// Fan control constraints
static const float FAN_MIN_CLAMP_PCT = 0.0f;
static const float FAN_MAX_CLAMP_PCT = 100.0f;

// LM35 calibration (as provided wiring uses LM35 A1; typical is 10mV/°C at 5V)
// Assume 5V reference unless otherwise specified.
static const float VREF = 5.0f;
static const float LM35_MV_PER_C = 10.0f;

// Serial parsing buffer
static char rxBuf[96];
static uint8_t rxLen = 0;
static uint32_t lastRxMs = 0;

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float clampf(float x, float lo, float hi);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readTemperatureC();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t fanDutyFromMinPct(float minPct);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t computeFanDuty(float tempC);
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void processCommandLine(const char *line);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pollSerial();
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 211 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static inline float readTemperatureC() {
  // Read ADC and convert to temperature in Celsius
  // Arduino UNO ADC: 10-bit, range 0..1023
  int adc = analogRead(PIN_LM35_A1);
  float v = (adc * VREF) / 1023.0f;              // volts
  float tempC = (v * 1000.0f) / LM35_MV_PER_C;  // (mV)/ (mV/°C)
  return tempC;
}

static inline uint8_t fanDutyFromMinPct(float minPct) {
  // Map min fan speed percentage to PWM duty (0..255)
  minPct = clampf(minPct, FAN_MIN_CLAMP_PCT, FAN_MAX_CLAMP_PCT);
  float duty = (minPct / 100.0f) * 255.0f;
  return (uint8_t)roundf(duty);
}

static inline uint8_t computeFanDuty(float tempC) {
  // Requirement: "系统根据实时温度自动调节风扇"
  // Keep behavior deterministic: as temperature rises above target, increase fan linearly.
  // Below or equal target: keep at minimum fan speed (minFanSpeedPct).
  //
  // If temp exceeds target by 5°C or more, LED on + overheat warning (handled separately).
  //
  // We'll scale overheat margin [0..5] to additional fan over min.
  // Above 5°C, fan saturates at 100%.
  float minDuty = fanDutyFromMinPct(minFanSpeedPct);
  if (tempC <= targetTempC) {
    return (uint8_t)minDuty;
  }

  float delta = tempC - targetTempC; // > 0
  // Map delta in [0..5] to fan in [min..100%]
  float t = clampf(delta / 5.0f, 0.0f, 1.0f);
  float duty = minDuty + t * (255.0f - minDuty);
  return (uint8_t)roundf(duty);
}

static void processCommandLine(const char *line) {
  // Supported commands:
  // - "SET TARGET <value>" or "TARGET=<value>"
  // - "SET MIN <value>" or "MINFAN=<value>"
  // - Also accept "target:xx" / "minfan:xx" case-insensitively (basic).
  //
  // Output is not explicitly required, but serial warning/output for overheat is required.
  // We'll keep responses minimal: only overheat warning is printed from main loop.
  //
  // Examples:
  // "TARGET=35.5"
  // "MINFAN=25"
  // "SET TARGET 36"
  // "SET MIN 20"
  if (!line || !*line) return;

  // Make a lowercase copy for simple parsing
  char lower[96];
  size_t n = strnlen(line, sizeof(lower) - 1);
  for (size_t i = 0; i < n; i++) lower[i] = (char)tolower((unsigned char)line[i]);
  lower[n] = '\0';

  // Try "target" assignments
  char *p = nullptr;

  // TARGET=...
  p = strstr(lower, "target=");
  if (p) {
    float v = strtof(p + 7, nullptr);
    if (isfinite(v)) targetTempC = v;
    return;
  }

  // MINFAN=...
  p = strstr(lower, "minfan=");
  if (p) {
    float v = strtof(p + 7, nullptr);
    if (isfinite(v)) minFanSpeedPct = v;
    return;
  }

  // "set target" / "set min"
  p = strstr(lower, "set ");
  if (p) {
    // Find "target" token
    char *q = strstr(p + 4, "target");
    if (q) {
      // Value after target
      char *r = q + 6;
      // skip separators/spaces/':'/'='
      while (*r && (*r == ' ' || *r == ':' || *r == '=')) r++;
      float v = strtof(r, nullptr);
      if (isfinite(v)) targetTempC = v;
      return;
    }
    q = strstr(p + 4, "min");
    if (q) {
      // Ensure it's "minfan" or "min"
      // Value after min
      char *r = q + 3;
      while (*r && (*r == ' ' || *r == ':' || *r == '=')) r++;
      float v = strtof(r, nullptr);
      if (isfinite(v)) minFanSpeedPct = v;
      return;
    }
  }

  // Direct patterns "target:" or "minfan:" / "min:"
  p = strstr(lower, "target:");
  if (p) {
    float v = strtof(p + 7, nullptr);
    if (isfinite(v)) targetTempC = v;
    return;
  }
  p = strstr(lower, "minfan:");
  if (p) {
    float v = strtof(p + 7, nullptr);
    if (isfinite(v)) minFanSpeedPct = v;
    return;
  }
  p = strstr(lower, "min:");
  if (p) {
    float v = strtof(p + 4, nullptr);
    if (isfinite(v)) minFanSpeedPct = v;
    return;
  }
}

static void pollSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    uint32_t now = millis();
    lastRxMs = now;

    // Accept newline and carriage return as terminators
    if (c == '\n' || c == '\r') {
      rxBuf[rxLen] = '\0';
      if (rxLen > 0) processCommandLine(rxBuf);
      rxLen = 0;
      return;
    }

    // Basic line idle handling: if buffer overflows, reset
    if (rxLen >= sizeof(rxBuf) - 1) {
      rxLen = 0;
    }

    // Ignore leading spaces
    if (rxLen == 0 && (c == ' ' || c == '\t')) continue;

    rxBuf[rxLen++] = c;
  }

  // Idle timeout-based commit (since requirement: line_or_idle with idle_timeout_ms=20)
  if (rxLen > 0) {
    uint32_t now = millis();
    if ((now - lastRxMs) >= SERIAL_IDLE_TIMEOUT_MS) {
      rxBuf[rxLen] = '\0';
      processCommandLine(rxBuf);
      rxLen = 0;
    }
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_FAN_PWM, OUTPUT);
  analogWrite(PIN_FAN_PWM, 0);

  // Ensure analog input is ready
  analogReference(DEFAULT);

  lastRxMs = millis();
}

void loop() {
  pollSerial();

  float tempC = readTemperatureC();

  // Update fan according to temperature (min speed baseline, increasing above target)
  uint8_t duty = computeFanDuty(tempC);
  analogWrite(PIN_FAN_PWM, duty);

  // Overheat condition: if temperature exceeds target + 5°C -> LED on and output overheat warning
  bool overheat = (tempC > (targetTempC + 5.0f));

  if (overheat) {
    digitalWrite(PIN_LED, HIGH);

    // Output overheat warning over serial.
    // Requirement doesn't specify rate; to avoid spamming, emit at ~2 Hz.
    static uint32_t lastWarnMs = 0;
    uint32_t now = millis();
    if (now - lastWarnMs >= 500) {
      lastWarnMs = now;
      Serial.print("overheat ");
      Serial.print(tempC, 2);
      Serial.print("C target=");
      Serial.print(targetTempC, 2);
      Serial.print("C minFan=");
      Serial.print(minFanSpeedPct, 1);
      Serial.print("% pwm=");
      Serial.println(duty);
    }
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  delay(50); // modest loop delay for stable behavior and serial handling
}
