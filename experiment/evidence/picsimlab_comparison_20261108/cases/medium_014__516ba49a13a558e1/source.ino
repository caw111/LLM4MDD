#include <Arduino.h>

static const uint32_t CONTROL_PERIOD_MS = 5;

static const uint8_t BUZZ_PIN = 6; // D6

// --- Serial framing: line_or_idle, idle_timeout_ms=20, baud=9600 ---
// Parse ASCII tokens separated by whitespace/newline.
// Accept commands of the form: BUZZ=1  (value token can be "1" or "BUZZ 1" depending on input style?)
// Requirement states "命令token为ASCII；commands BUZZ=1". We'll accept:
///   - "BUZZ=1" (exact token)
///   - "BUZZ 1" (two tokens)
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;
static const uint32_t SERIAL_BAUD = 9600;

struct SerialCommandState {
  // Validity holds for a single control step only when a complete valid serial frame was received.
  bool valid;      // io_1_valid
  int32_t buzz;    // io_1
  uint32_t lastCharMs;
  bool sawAny;
  String lineBuf;
};

static SerialCommandState g_rx;

// Model state
static int32_t buzzRemainingMs = 0;

// Output ports (io_0, serial_0_trigger, serial_0_value_0)
static int32_t io_0 = 0;
static bool serial_0_trigger = false;
static int32_t serial_0_value_0 = 0;

// Timing
static uint32_t g_lastTickMs = 0;

// Helpers
static inline void resetOutputs() {
  io_0 = (buzzRemainingMs > 0) ? 1000 : 0;
  serial_0_trigger = (g_rx.valid && g_rx.buzz == 1);
  serial_0_value_0 = (buzzRemainingMs > 0) ? 1000 : 0;
}

static void applyModelTick() {
  // ACCEPTED executable model tick:
  // if buzzRemainingMs > 0 and io_1_valid -> buzzRemainingMs := 0
  // else:
  //   if buzzRemainingMs > 0 -> buzzRemainingMs := 0
  //   if io_1_valid and io_1 == 1 -> buzzRemainingMs := 500
  if (buzzRemainingMs > 0 && g_rx.valid) {
    buzzRemainingMs = 0;
  } else {
    if (buzzRemainingMs > 0) {
      buzzRemainingMs = 0;
    }
    if (g_rx.valid && g_rx.buzz == 1) {
      buzzRemainingMs = 500;
    }
  }

  resetOutputs();
}

static void handleParsedLine(const String &line) {
  // Determine io_1 and validity for this control step.
  // If no valid command recognized -> io_1_valid=false, io_1 unchanged (but model uses only validity+value check).
  // For strict behavior, set buzz=0 when invalid.
  g_rx.valid = false;
  g_rx.buzz = 0;

  // Normalize: remove leading/trailing whitespace
  String s = line;
  s.trim();
  if (s.length() == 0) return;

  // Accept patterns:
  // 1) BUZZ=1
  // 2) BUZZ 1
  // 3) BUZZ:1
  // We'll be conservative: look for "BUZZ" then parse integer following.
  int idx = s.indexOf("BUZZ");
  if (idx < 0) return;

  // Find delimiter after BUZZ
  int after = idx + 4;
  while (after < (int)s.length() && (s[after] == ' ' || s[after] == '\t')) after++;

  // Skip possible separators
  if (after < (int)s.length() && (s[after] == '=' || s[after] == ':' )) after++;

  while (after < (int)s.length() && (s[after] == ' ' || s[after] == '\t')) after++;

  // Parse integer token from after
  int start = after;
  while (after < (int)s.length() && (isDigit(s[after]) || s[after] == '-' )) after++;
  if (start == after) return;

  long val = s.substring(start, after).toInt();
  // Only BUZZ=1 is in commands
  if (val == 1) {
    g_rx.valid = true;
    g_rx.buzz = 1;
  }
}

static void pollSerial() {
  // io_1_valid should be true only when a complete valid serial frame was received.
  // So we clear it at start of each control tick; then set it upon parsing.
  // But the idle timeout defines when a frame is complete; thus we may parse at any time.
  // We'll implement:
  //  - Accumulate characters into g_rx.lineBuf
  //  - Update lastCharMs when char arrives
  //  - When idle timeout elapses with buffer non-empty, parse and set g_rx.valid accordingly for the upcoming tick.
  // Since loop() runs faster than 5ms usually, parsing can happen mid-tick.
  // To keep io_1_valid meaningful for the model tick, we maintain:
  //  - g_rx.valid indicates "a complete frame parsed since last tick".
  // This matches the "value holds its previous value otherwise": we will explicitly clear it each tick.
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // If framing is line_or_idle: '\n' or '\r' should complete the frame too.
    if (c == '\n' || c == '\r') {
      if (g_rx.lineBuf.length() > 0) {
        handleParsedLine(g_rx.lineBuf);
      } else {
        // nothing
      }
      g_rx.lineBuf = "";
      g_rx.sawAny = false;
      g_rx.lastCharMs = millis();
      // After parse, valid might be set. It will last until cleared by next tick.
    } else {
      g_rx.lineBuf += c;
      g_rx.sawAny = true;
      g_rx.lastCharMs = millis();
    }
  }

  // idle timeout parse
  if (g_rx.sawAny && g_rx.lineBuf.length() > 0) {
    uint32_t now = millis();
    if (now - g_rx.lastCharMs >= SERIAL_IDLE_TIMEOUT_MS) {
      handleParsedLine(g_rx.lineBuf);
      g_rx.lineBuf = "";
      g_rx.sawAny = false;
      // lastCharMs not important after reset
    }
  }
}

static void emitSerialValueTrigger() {
  // Contract: serial_0_value_0 / serial_0_trigger are outputs.
  // Not explicitly requiring an output format, but states:
  // "输出数值以同一baud的序列通道在触发时上报。"
  // We'll transmit only when serial_0_trigger==true, reporting BUZZ_FREQUENCY_HZ as integer.
  // Format: "BUZZ_FREQUENCY_HZ=<value>\n"
  if (serial_0_trigger) {
    Serial.print(F("BUZZ_FREQUENCY_HZ="));
    Serial.println(serial_0_value_0);
  }
}

static void applyOutputs() {
  // io_0 controls tone frequency (Hz) on tone_output mapped to D6.
  // Hardware contract: "invalid_policy":"silence" -> if io_0 == 0, stop tone.
  // We'll use Arduino tone()/noTone(). When io_0>0, use tone(D6, io_0).
  if (io_0 > 0) {
    tone(BUZZ_PIN, (unsigned int)io_0);
  } else {
    noTone(BUZZ_PIN);
  }
}

void setup() {
  pinMode(BUZZ_PIN, OUTPUT);
  noTone(BUZZ_PIN);

  Serial.begin(SERIAL_BAUD);
  // Arduino UNO uses default line endings; we implement our own framing in pollSerial().
  g_rx.valid = false;
  g_rx.buzz = 0;
  g_rx.lastCharMs = millis();
  g_rx.sawAny = false;
  g_rx.lineBuf.reserve(32);

  g_lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  // Wait until control period boundary
  if (now - g_lastTickMs < CONTROL_PERIOD_MS) {
    // Still poll serial to reduce latency
    pollSerial();
    return;
  }
  // Advance tick
  g_lastTickMs += CONTROL_PERIOD_MS;

  // Clear io_1_valid for this model step (matches "True for this model step only when a complete valid serial frame was received; value holds its previous value otherwise").
  // We interpret "holds previous value otherwise" at the hardware-input port level; model uses io_1_valid only for this tick.
  // However, clearing each tick ensures validity is pulse-like for each parsed frame, consistent with "complete valid serial frame was received".
  // Also keep g_rx.buzz value (not required when invalid), but set to 0 to avoid accidental matches.
  g_rx.valid = false;
  g_rx.buzz = 0;

  // Poll serial right before model tick so that a frame finishing near boundary is captured.
  pollSerial();
  // If a frame was parsed during pollSerial(), g_rx.valid may now be set.

  // Use ACCEPTED model tick
  applyModelTick();

  // Output phase: apply buzzer output and report values on trigger
  applyOutputs();
  emitSerialValueTrigger();

  // loop continues
}