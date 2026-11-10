#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint16_t CONTROL_PERIOD_MS = 10;
static constexpr uint32_t SERIAL_BAUD = 9600;
static constexpr uint16_t SERIAL_IDLE_TIMEOUT_MS = 20;

static constexpr uint8_t PWM_PIN = 9;

// Ports
static float io_0 = 0.0f;      // f64
static int32_t io_1 = 0;       // i32
static bool io_1_valid = false; // bool

// Internal model state
static float pwmDuty = 0.0f;   // f64, initial 0.0

// Serial parsing with "line_or_idle" + idle timeout.
// Accepts one integer frame (0..255). Invalid/out-of-range frames produce no valid event.
// "io_1_valid" is true for one control step only when a complete valid frame is received.
static String rxLine;
static bool hasPendingFrame = false;
static int32_t pendingValue = 0;

static bool frameCandidateActive = false;
static unsigned long lastByteTime = 0;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool parsePendingIfComplete(unsigned long nowMs);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setupModel();
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickModel();
#line 121 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool parsePendingIfComplete(unsigned long nowMs) {
  (void)nowMs;

  // If we have a candidate that hasn't produced a frame yet, use idle timeout
  // to decide completion for "line_or_idle".
  if (!frameCandidateActive) return false;

  unsigned long elapsed = nowMs - lastByteTime;
  if (elapsed < SERIAL_IDLE_TIMEOUT_MS) return false;

  // Idle timeout reached => treat whatever we accumulated as a frame.
  frameCandidateActive = false;

  // If empty, it's invalid/no event.
  if (rxLine.length() == 0) {
    rxLine = "";
    return false;
  }

  // Try parse as integer (base 10).
  // If parse fails => invalid frame => no event.
  char *endPtr = nullptr;
  long v = rxLine.toInt(); // Arduino's toInt returns 0 on failure; we must detect failure
  // Detect failure more robustly: manually parse.
  // We'll implement manual parse:
  const char *s = rxLine.c_str();
  bool neg = false;
  size_t i = 0;

  while (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') i++;
  if (s[i] == '-') { neg = true; i++; }

  if (s[i] < '0' || s[i] > '9') {
    // Not starting with digit => failure
    rxLine = "";
    return false;
  }

  long accum = 0;
  bool anyDigit = false;
  while (s[i] >= '0' && s[i] <= '9') {
    anyDigit = true;
    accum = accum * 10 + (s[i] - '0');
    i++;
  }

  if (!anyDigit) {
    rxLine = "";
    return false;
  }

  // Skip trailing whitespace
  while (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') i++;

  // If there are remaining non-ws chars, treat as invalid
  if (s[i] != '\0') {
    rxLine = "";
    return false;
  }

  if (neg) accum = -accum;

  // Validate range 0..255
  if (accum >= 0 && accum <= 255) {
    pendingValue = (int32_t)accum;
    hasPendingFrame = true;
    rxLine = "";
    return true;
  }

  // Out of range => invalid frame => no event
  rxLine = "";
  return false;
}

static void setupModel() {
  pwmDuty = 0.0f;
  io_0 = pwmDuty;
  io_1 = 0;
  io_1_valid = false;
}

// Model tick: ACCEPTED executable model
static void tickModel() {
  // Preserves the exact state update rule:
  // if io_1_valid then pwmDuty := io_1 else no update.
  if (io_1_valid) {
    pwmDuty = (float)io_1;
  }
  // out attribute io_0 : Real = pwmDuty;
  io_0 = pwmDuty;
}

// Apply model output to hardware port
static void applyOutputs() {
  // Map pwmDuty (0..255) to analogWrite duty on D9.
  // Duty is expected to be a "pwm_count" integer-like value.
  // Clamp to [0,255] for safety while preserving intended behavior.
  int pwm = (int)(pwmDuty + 0.5f);
  if (pwm < 0) pwm = 0;
  if (pwm > 255) pwm = 255;

  analogWrite(PWM_PIN, pwm);
}

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  analogWrite(PWM_PIN, 0);

  Serial.begin(SERIAL_BAUD);

  rxLine.reserve(16);
  frameCandidateActive = false;
  lastByteTime = 0;
  hasPendingFrame = false;

  setupModel();
}

void loop() {
  static unsigned long lastTick = 0;

  unsigned long now = millis();

  // Continuously read serial without blocking.
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    unsigned long t = millis();
    lastByteTime = t;

    // Consider newline as a terminator for "line_or_idle".
    if (c == '\n' || c == '\r') {
      // Finalize current candidate immediately.
      frameCandidateActive = false;

      if (rxLine.length() == 0) {
        rxLine = "";
      } else {
        // Parse completed line now, as it's already line-terminated.
        // If valid range 0..255 => set pending.
        const char *s = rxLine.c_str();
        bool neg = false;
        size_t i = 0;

        while (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') i++;
        if (s[i] == '-') { neg = true; i++; }

        if (s[i] < '0' || s[i] > '9') {
          rxLine = "";
        } else {
          long accum = 0;
          bool anyDigit = false;
          while (s[i] >= '0' && s[i] <= '9') {
            anyDigit = true;
            accum = accum * 10 + (s[i] - '0');
            i++;
          }

          if (!anyDigit) {
            rxLine = "";
          } else {
            while (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') i++;
            if (s[i] != '\0') {
              rxLine = "";
            } else {
              if (neg) accum = -accum;
              if (accum >= 0 && accum <= 255) {
                pendingValue = (int32_t)accum;
                hasPendingFrame = true;
              }
              rxLine = "";
            }
          }
        }
      }
    } else {
      // Accumulate characters for the current frame.
      frameCandidateActive = true;
      rxLine += c;
    }
  }

  // Handle idle timeout completion for "line_or_idle".
  parsePendingIfComplete(now);

  // Enforce control period: 10ms.
  if ((now - lastTick) < CONTROL_PERIOD_MS) {
    return;
  }
  lastTick += CONTROL_PERIOD_MS;

  // Update serial valid input for this model step only.
  // "value holds its previous value otherwise" means io_1_valid is true
  // only for the step where the frame arrives, and false otherwise.
  io_1_valid = false;
  if (hasPendingFrame) {
    io_1 = pendingValue;
    io_1_valid = true;
    hasPendingFrame = false;
  }

  // Tick model (ordered updates as specified)
  tickModel();

  // Apply outputs to D9 PWM
  applyOutputs();
}
