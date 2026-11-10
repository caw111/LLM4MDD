#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdlib.h>
#include <ctype.h>

static const uint8_t PIN_LM35_A1  = A1;  // LM35 analog input (A1)
static const uint8_t PIN_FAN_PWM   = 5;   // Fan PWM output (D5)
static const uint8_t PIN_LED      = 13;  // LED output (D13)

// Serial parameters
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Temperature / control parameters (defaults; can be updated via serial)
static float targetTempC   = 30.0f;  // 用户可设置：目标温度阈值
static float minFanSpeedPct = 20.0f; // 用户可设置：最小风扇转速(百分比)

// Fan control constraints
static const float FAN_MIN_CLAMP_PCT = 0.0f;
static const float FAN_MAX_CLAMP_PCT = 100.0f;

// LM35 calibration (typical is 10mV/°C at 5V)
// Assume 5V reference unless otherwise specified.
static const float VREF = 5.0f;
static const float LM35_MV_PER_C = 10.0f;

// Serial parsing buffer
static char rxBuf[96];
static uint8_t rxLen = 0;
static uint32_t lastRxMs = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static float clampf(float x, float lo, float hi);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static float readTemperatureC();
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static uint8_t fanDutyFromMinPct(float minPct);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static uint8_t computeFanDuty(float tempC);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static bool parseFloatFromPtr(const char *s, float &outVal);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void processCommandLine(const char *line);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static void pollSerial();
#line 177 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
void setup();
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\complex_018\\repeat_1\\direct_feedback\\attempt_2\\sketch\\sketch.ino"
static inline float clampf(float x, float lo, float hi) {
  if (x < lo) return lo;
  if (x > hi) return hi;
  return x;
}

static inline float readTemperatureC() {
  int adc = analogRead(PIN_LM35_A1);                 // 0..1023
  float v = (adc * VREF) / 1023.0f;                 // volts
  float tempC = (v * 1000.0f) / LM35_MV_PER_C;     // (mV)/(mV/°C)
  return tempC;
}

static inline uint8_t fanDutyFromMinPct(float minPct) {
  minPct = clampf(minPct, FAN_MIN_CLAMP_PCT, FAN_MAX_CLAMP_PCT);
  float duty = (minPct / 100.0f) * 255.0f;
  return (uint8_t)roundf(duty);
}

static inline uint8_t computeFanDuty(float tempC) {
  float minDuty = fanDutyFromMinPct(minFanSpeedPct);
  if (tempC <= targetTempC) {
    return (uint8_t)minDuty;
  }

  float delta = tempC - targetTempC; // > 0
  float t = clampf(delta / 5.0f, 0.0f, 1.0f); // map [0..5] => [0..1]
  float duty = minDuty + t * (255.0f - minDuty);
  return (uint8_t)roundf(duty);
}

// Use strtod (double) to avoid missing strtof on some AVR toolchains
static bool parseFloatFromPtr(const char *s, float &outVal) {
  if (!s) return false;
  char *endp = nullptr;
  outVal = (float)strtod(s, &endp);
  if (endp == s) return false;
  return true;
}

static void processCommandLine(const char *line) {
  if (!line || !*line) return;

  // Make a lowercase copy for simple parsing
  char lower[96];
  size_t n = strnlen(line, sizeof(lower) - 1);
  for (size_t i = 0; i < n; i++) lower[i] = (char)tolower((unsigned char)line[i]);
  lower[n] = '\0';

  // TARGET=...
  char *p = strstr(lower, "target=");
  if (p) {
    float v;
    if (parseFloatFromPtr(p + 7, v)) targetTempC = v;
    return;
  }

  // MINFAN=...
  p = strstr(lower, "minfan=");
  if (p) {
    float v;
    if (parseFloatFromPtr(p + 7, v)) minFanSpeedPct = v;
    return;
  }

  // "set target" / "set min"
  p = strstr(lower, "set ");
  if (p) {
    char *q = strstr(p + 4, "target");
    if (q) {
      char *r = q + 6;
      while (*r && (*r == ' ' || *r == ':' || *r == '=')) r++;
      float v;
      if (parseFloatFromPtr(r, v)) targetTempC = v;
      return;
    }

    q = strstr(p + 4, "min");
    if (q) {
      char *r = q + 3;
      while (*r && (*r == ' ' || *r == ':' || *r == '=')) r++;
      float v;
      if (parseFloatFromPtr(r, v)) minFanSpeedPct = v;
      return;
    }
  }

  // Direct patterns "target:" / "minfan:" / "min:"
  p = strstr(lower, "target:");
  if (p) {
    float v;
    if (parseFloatFromPtr(p + 7, v)) targetTempC = v;
    return;
  }

  p = strstr(lower, "minfan:");
  if (p) {
    float v;
    if (parseFloatFromPtr(p + 7, v)) minFanSpeedPct = v;
    return;
  }

  p = strstr(lower, "min:");
  if (p) {
    float v;
    if (parseFloatFromPtr(p + 4, v)) minFanSpeedPct = v;
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

  // Idle timeout-based commit
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

  analogReference(DEFAULT);

  lastRxMs = millis();
}

void loop() {
  pollSerial();

  float tempC = readTemperatureC();

  // Update fan according to temperature
  uint8_t duty = computeFanDuty(tempC);
  analogWrite(PIN_FAN_PWM, duty);

  // Overheat: if temperature exceeds target + 5°C -> LED on + overheat warning
  bool overheat = (tempC > (targetTempC + 5.0f));

  if (overheat) {
    digitalWrite(PIN_LED, HIGH);

    static uint32_t lastWarnMs = 0;
    uint32_t now = millis();
    if (now - lastWarnMs >= 500) { // ~2 Hz
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

  delay(50);
}
