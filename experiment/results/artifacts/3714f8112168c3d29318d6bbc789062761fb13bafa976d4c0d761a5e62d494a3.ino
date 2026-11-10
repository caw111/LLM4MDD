#include <Arduino.h>

// ----- Frozen interface contract -----
static const uint32_t CONTROL_PERIOD_MS = 5;   // control period (not strictly necessary for correctness)
static const uint32_t SERIAL_BAUD = 9600;       // UART baud
static const uint32_t IDLE_TIMEOUT_MS = 20;    // framing: line_or_idle

// Hardware binding
static const uint8_t LED_PIN = 13;              // D13, active_high=true

// ----- Model state (ACCEPTED executable model) -----
static bool ledOn = false;        // attribute ledOn : Boolean := false;
static bool prevLedOn = false;   // attribute prevLedOn : Boolean := false;

// ----- Input ports (io_1, io_1_valid) -----
static int32_t io_1 = 0;         // serial command mapped integer: OFF=1, ON=2
static bool io_1_valid = false; // true for this model step only when a complete valid serial frame was received

// ----- Serial receive handling (line_or_idle, idle_timeout_ms=20) -----
// We treat a "complete valid frame" as either a line terminated by '\n'/'\r',
// OR when the input stream has been idle for >= IDLE_TIMEOUT_MS.
static String rxBuffer;

// Track last received time to implement idle_timeout_ms
static uint32_t lastByteTimeMs = 0;

// Helper: map trimmed token to model integer command
static bool parseCommand(const String &token, int32_t &outCmd) {
  // token is expected to be "ON" or "OFF" (case-sensitive per requirement; keep strict)
  if (token == "ON") {
    outCmd = 2;
    return true;
  }
  if (token == "OFF") {
    outCmd = 1;
    return true;
  }
  return false;
}

// Helper: trim whitespace in-place (simple)
static String trimCopy(const String &s) {
  int start = 0;
  int end = (int)s.length() - 1;
  while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) start++;
  while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r' || s[end] == '\n')) end--;
  if (start > end) return String("");
  return s.substring(start, end + 1);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // active_high=true, initial false => LOW

  Serial.begin(SERIAL_BAUD);
  rxBuffer.reserve(16);
  lastByteTimeMs = millis();
}

static void controllerTick() {
  // ACCEPTED executable model "tick"
  // first start; then if valid & ON/OFF set ledOn; else no change
  // then assign prevLedOn := ledOn;
  // out io_0 := ledOn (bound to LED)
  if (io_1_valid && io_1 == 2) {
    ledOn = true;
  } else if (io_1_valid && io_1 == 1) {
    ledOn = false;
  }
  prevLedOn = ledOn;

  // Output io_0 -> io_0 is ledOn, model semantics true->HIGH, false->LOW
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

void loop() {
  static uint32_t nextTickMs = 0;

  // Run at 5ms control period (scheduler granularity)
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    // Not time yet; but still need to accumulate serial bytes to not miss them.
    // We'll read serial opportunistically.
    while (Serial.available() > 0) {
      char c = (char)Serial.read();
      lastByteTimeMs = millis();
      rxBuffer += c;
    }
    return;
  }
  nextTickMs = now + CONTROL_PERIOD_MS;

  // ----- Update serial inputs -----
  // io_1_valid is a pulse for "this model step only" when a complete valid frame is received.
  io_1_valid = false;

  // Read all available bytes into buffer
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastByteTimeMs = millis();
    rxBuffer += c;

    // If newline/CR ends the token, we can finalize immediately.
    if (c == '\n' || c == '\r') {
      // finalize from entire buffer
      String token = trimCopy(rxBuffer);
      rxBuffer = ""; // clear buffer after frame termination

      int32_t cmd = 0;
      if (parseCommand(token, cmd)) {
        io_1 = cmd;
        io_1_valid = true;
      }
      // else: invalid frame ignored (io_1_valid stays false)
      break; // only one frame per tick; strict with controller semantics pulse per step
    }
  }

  // If no newline ended, try idle_timeout framing
  // If buffer has content and we've been idle long enough, finalize it.
  if (!io_1_valid && rxBuffer.length() > 0) {
    uint32_t idleFor = now - lastByteTimeMs;
    if (idleFor >= IDLE_TIMEOUT_MS) {
      String token = trimCopy(rxBuffer);
      rxBuffer = "";

      int32_t cmd = 0;
      if (parseCommand(token, cmd)) {
        io_1 = cmd;
        io_1_valid = true;
      }
    }
  }

  // ----- Controller tick -----
  controllerTick();
}