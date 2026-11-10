#include <Arduino.h>

static const uint8_t PWM_PIN = 9;     // D9 PWM LED
static const uint32_t BAUD = 9600;

static uint8_t lastValue = 0;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0);

  Serial.begin(BAUD);
  // framing: line_or_idle, idle_timeout_ms=20
  // We'll implement a "line or idle" parser by accepting complete lines,
  // and also accepting when a gap of ~20ms occurs after digits.
}

static bool parseUint8(const char *s, uint8_t &out) {
  // Accept only base-10 unsigned integers; allow leading/trailing spaces.
  // Return true if at least one digit is present and value <= 255.
  uint32_t val = 0;
  bool seenDigit = false;

  const char *p = s;
  while (*p == ' ' || *p == '\t' || *p == '\r') p++;

  while (*p >= '0' && *p <= '9') {
    seenDigit = true;
    val = val * 10u + (uint32_t)(*p - '0');
    if (val > 255u) return false;
    p++;
  }

  while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') p++;
  return seenDigit && *p == '\0' ? (out = (uint8_t)val, true) : false;
}

void loop() {
  static char buf[8];        // enough for "255" plus terminator
  static uint8_t idx = 0;
  static unsigned long lastByteMs = 0;

  // Idle timeout handling for "line_or_idle"
  const unsigned long now = millis();
  if (idx > 0 && (now - lastByteMs) >= 20) {
    buf[idx] = '\0';
    uint8_t v = 0;
    if (parseUint8(buf, v)) {
      analogWrite(PWM_PIN, v);
      lastValue = v;
      Serial.println((int)v);
    }
    idx = 0;
  }

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteMs = now;

    // Line endings delimit input
    if (c == '\n' || c == '\r') {
      if (idx > 0) {
        buf[idx] = '\0';
        uint8_t v = 0;
        if (parseUint8(buf, v)) {
          analogWrite(PWM_PIN, v);
          lastValue = v;
          Serial.println((int)v);  // integer echo
        }
        idx = 0;
      }
      continue;
    }

    // Accept digits only, but allow leading spaces
    if (c >= '0' && c <= '9') {
      if (idx < (sizeof(buf) - 1)) {
        buf[idx++] = c;
      } else {
        // Overflow: keep buffer capped; additional digits ignored
        // until delimiter/idle flush.
      }
    } else if (c == ' ' || c == '\t') {
      // ignore whitespace inside/around token
      // If buffer is empty, ignore leading whitespace.
      if (idx > 0 && idx < (sizeof(buf) - 1)) {
        // keep a single whitespace is not necessary; ignore
      }
    } else {
      // Ignore any other characters (no behavior specified)
    }
  }
}