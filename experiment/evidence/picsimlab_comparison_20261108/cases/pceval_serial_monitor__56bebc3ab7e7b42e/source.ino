#include <Arduino.h>

struct Controller {
  // Model state
  int32_t lastIo0Value = 0;

  // Inputs (wired to serial receiver)
  int32_t io_0 = 0;
  bool io_0_valid = false;
  int32_t io_2 = 0; // elapsed_ms delta (not required for logic, kept for contract)

  // Outputs (wired to serial transmitter)
  bool io_1 = false;              // gate for fixed echo prefix
  bool serial_0_trigger = false; // assert on new valid integer
  int32_t serial_0_value_0 = 0; // echoed integer
};

// Serial framing: line_or_idle with idle_timeout_ms=20
// We implement: read characters, when newline received OR when
// no new characters arrive for idle_timeout_ms, parse buffered content as integer.
static constexpr uint32_t SERIAL_BAUD = 9600;
static constexpr uint32_t IDLE_TIMEOUT_MS = 20;

static Controller ctrl;

// Input parsing buffer
static char rxBuf[32];
static size_t rxLen = 0;
static bool haveFrameCandidate = false;

static uint32_t lastCharTimeMs = 0;

// Timing for control period (10 ms)
static uint32_t lastTickMs = 0;
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static inline void resetRxBuffer() {
  rxLen = 0;
  haveFrameCandidate = false;
  rxBuf[0] = '\0';
}

static bool tryParseIntFromBuffer(int32_t &outVal) {
  if (!haveFrameCandidate) return false;
  if (rxLen == 0) return false;

  // Ensure null-terminated
  rxBuf[(rxLen < sizeof(rxBuf)) ? rxLen : (sizeof(rxBuf) - 1)] = '\0';

  // Use strtol for robust sign handling
  char *endp = nullptr;
  long v = strtol(rxBuf, &endp, 10);
  if (endp == rxBuf) return false; // no digits

  // Clamp to int32_t range defensively (shouldn't be needed for typical inputs)
  if (v < INT32_MIN || v > INT32_MAX) return false;
  outVal = (int32_t)v;
  return true;
}

static void model_tick(int32_t io_0_in, bool io_0_valid_in, int32_t io_2_in) {
  // Preserve exact accepted executable model state updates and outputs
  // Controller.perform action tick:
  // first start; then if io_0_valid then lastIo0Value := io_0;
  // outputs: io_1 = io_0_valid; serial_0_trigger = io_0_valid; serial_0_value_0 = lastIo0Value
  if (io_0_valid_in) {
    ctrl.lastIo0Value = io_0_in;
  }

  ctrl.io_0 = io_0_in;
  ctrl.io_0_valid = io_0_valid_in;
  ctrl.io_2 = io_2_in;

  ctrl.io_1 = ctrl.io_0_valid;
  ctrl.serial_0_trigger = ctrl.io_0_valid;
  ctrl.serial_0_value_0 = ctrl.lastIo0Value;
}

static void output_serial() {
  // serial_text_output (text = "Received: ") gated by io_0_valid (io_1)
  // serial_value (label omitted; prints numeric) gated by serial_0_trigger
  if (ctrl.io_1) {
    Serial.print(F("Received: "));
  }
  if (ctrl.serial_0_trigger) {
    Serial.print(ctrl.serial_0_value_0);
  }
  // No explicit newline specified by contract; preserve behavior as-is (no CR/LF).
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  resetRxBuffer();
  lastCharTimeMs = millis();

  ctrl.lastIo0Value = 0;
  ctrl.io_0_valid = false;
  ctrl.io_1 = false;
  ctrl.serial_0_trigger = false;
  ctrl.serial_0_value_0 = 0;

  lastTickMs = millis();
}

void loop() {
  // Continuous read and buffer accumulation
  while (Serial.available() > 0) {
    int c = Serial.read();
    if (c < 0) break;

    uint32_t nowMs = millis();
    lastCharTimeMs = nowMs;

    // framing: line_or_idle
    if (c == '\r') {
      // ignore CR (typical line endings)
      continue;
    }
    if (c == '\n') {
      // line terminator -> treat as end of frame
      haveFrameCandidate = (rxLen > 0);
      break; // stop collecting further; parse on tick
    }

    // Append character if within buffer
    if (rxLen < (sizeof(rxBuf) - 1)) {
      rxBuf[rxLen++] = (char)c;
      haveFrameCandidate = true;
    } else {
      // buffer overflow: keep last byte off (discard extra); still mark candidate
      haveFrameCandidate = true;
    }
  }

  // Control period tick at 10ms granularity
  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) >= CONTROL_PERIOD_MS) {
    uint32_t dt = now - lastTickMs;
    lastTickMs = now;

    // elapsed_ms semantics: delta_since_previous_step
    int32_t io_2 = (int32_t)dt;

    // Determine io_0_valid and io_0 from buffered content.
    // We assert io_0_valid for this model step only when a complete valid frame arrives.
    int32_t io_0 = ctrl.io_0; // holds previous value if invalid; but assignment is irrelevant unless valid.
    bool io_0_valid = false;

    // Trigger frame finalization on newline or on idle timeout (line_or_idle)
    // If no chars have arrived for IDLE_TIMEOUT_MS and we have candidate, parse.
    bool idleTimedOut = false;
    if (haveFrameCandidate) {
      if ((uint32_t)(now - lastCharTimeMs) >= IDLE_TIMEOUT_MS) {
        idleTimedOut = true;
      }
    }

    // If we broke due to newline, parse regardless of idle timeout.
    // We detect newline consumption by setting haveFrameCandidate and leaving rxBuf; newline char isn't stored.
    // However, after reading newline we may still have not advanced lastCharTimeMs; still fine.
    // We'll parse either on idle timeout OR when Serial.available() was exhausted due to newline break.
    // Since newline break exits inner while with rxBuf already filled, we parse whenever either
    // idleTimedOut or there is a line candidate and the serial buffer is currently empty
    // for a short time. To keep deterministic, only parse on idleTimedOut here.
    // But we also must parse promptly on newline. We'll handle newline by immediate parse:
    // Detect newline by checking whether last received character was '\n' isn't possible now.
    // Instead, we rely on idle timeout as well; this matches line_or_idle contract.

    if (idleTimedOut) {
      // Parse buffered payload
      int32_t parsed = 0;
      if (tryParseIntFromBuffer(parsed)) {
        io_0 = parsed;
        io_0_valid = true;
      }
      // Reset buffer after idle-based frame finalization (even if parse fails)
      resetRxBuffer();
      lastCharTimeMs = now;
    } else {
      // No complete frame for this step
      io_0_valid = false;
    }

    // Execute accepted executable model tick in this exact step ordering
    model_tick(io_0, io_0_valid, io_2);

    // Emit outputs for this step (gated by io_0_valid)
    output_serial();
  }
}