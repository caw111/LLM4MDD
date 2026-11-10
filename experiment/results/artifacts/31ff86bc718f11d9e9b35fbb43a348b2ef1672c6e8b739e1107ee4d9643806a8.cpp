#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// HC-SR04 pins (per authoritative pin assignments)
static const uint8_t TRIG_PIN = 8; // D8
static const uint8_t ECHO_PIN = 9; // D9

static const uint32_t SERIAL_BAUD = 9600;

// Requirement: Serial input "READ" => output HC-SR04 distance
// Framing: line_or_idle; idle_timeout_ms=20
// We'll implement a small line/idle parser: accumulate characters until newline
// or until no new characters arrive within idle timeout.
static const uint16_t IDLE_TIMEOUT_MS = 20;

static String cmdBuf;
static unsigned long lastCharTimeMs = 0;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleCommand(const String &cmdRaw);
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_015\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static long readDistanceCm() {
  // Ensure a clean trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure echo pulse width
  // Timeout chosen to avoid hanging; HC-SR04 range is typically up to ~4m
  // 4m round trip time ~ 23 ms, so use 30 ms timeout.
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (durationUs == 0) return -1; // timeout/no echo

  // Speed of sound: 343 m/s => 0.0343 cm/us; distance = duration * 0.0343 / 2
  // distance_cm = durationUs * 0.0343 / 2
  // Using float for accuracy then cast.
  float distanceCm = (durationUs * 0.0343f) * 0.5f;
  return (long)(distanceCm + 0.5f); // round
}

static void handleCommand(const String &cmdRaw) {
  String cmd = cmdRaw;
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd == "READ") {
    long cm = readDistanceCm();
    Serial.println(cm);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(SERIAL_BAUD);
  cmdBuf.reserve(16);
  lastCharTimeMs = millis();
}

void loop() {
  unsigned long now = millis();

  // Read incoming bytes and parse by either newline or idle timeout
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastCharTimeMs = now;

    // Treat newline as command terminator
    if (c == '\n' || c == '\r') {
      if (cmdBuf.length() > 0) {
        handleCommand(cmdBuf);
        cmdBuf = "";
      }
    } else {
      // Avoid buffer blow-up
      if (cmdBuf.length() < 63) cmdBuf += c;
    }
  }

  // Idle timeout framing (line_or_idle)
  if (cmdBuf.length() > 0) {
    if ((now - lastCharTimeMs) >= IDLE_TIMEOUT_MS) {
      handleCommand(cmdBuf);
      cmdBuf = "";
    }
  }
}
