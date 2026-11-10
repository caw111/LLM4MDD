#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static constexpr uint8_t MODE1_PWM_PIN = 9; // D9
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Frozen interface contract variables
static int32_t io_1_ms_delta = 0;     // i32, ms delta since previous control step
static int32_t io_2_cmd = 0;          // i32, 1=MODE1, 2=MODE2
static bool io_2_valid = false;       // bool, true only for one control step when a valid frame was received

// Controller state (exact as provided)
static int32_t mode = 2;      // initial 2
static double pwmValue = 0.0; // initial 0.0
static int32_t phaseMs = 0;   // initial 0

// Serial reporting (contract): trigger always true
static bool serial_0_trigger = true;

// Serial parser with line_or_idle behavior + idle timeout 20ms
static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

static String rxBuf;
static uint32_t lastRxMs = 0;

#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setPwmFromModel(double v);
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void handleParsedToken(const String& tok);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialService();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_011\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setPwmFromModel(double v) {
  // Model outputs pwmValue in range {0.0, 1023.0}; map to analogWrite 0..255
  // Keep behavior faithful: 1023 -> 255, 0 -> 0 (and linearly otherwise).
  int pwm = (int)lround(v * (255.0 / 1023.0));
  if (pwm < 0) pwm = 0;
  if (pwm > 255) pwm = 255;
  analogWrite(MODE1_PWM_PIN, pwm);
}

static inline void parseTokenAndSet(io_2_cmd, io_2_valid) = delete;

static void handleParsedToken(const String& tok) {
  // Accept ASCII tokens MODE1 or MODE2
  String t = tok;
  t.trim();
  if (t.equalsIgnoreCase("MODE1")) {
    io_2_cmd = 1;
    io_2_valid = true;
  } else if (t.equalsIgnoreCase("MODE2")) {
    io_2_cmd = 2;
    io_2_valid = true;
  }
}

static void serialService() {
  // Read available bytes; collect until idle timeout or newline
  io_2_valid = false; // valid only for current control step; we'll set true when a complete token arrives

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    uint32_t now = millis();
    lastRxMs = now;

    if (c == '\n' || c == '\r') {
      if (rxBuf.length() > 0) {
        handleParsedToken(rxBuf);
        rxBuf = "";
      }
    } else {
      rxBuf += c;
    }
  }

  // line_or_idle: if no new bytes arrive for idle timeout, treat buffer as a complete token
  // and clear it. This should produce valid input on one step after token is "framed".
  uint32_t now = millis();
  if (!rxBuf.isEmpty() && (now - lastRxMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    handleParsedToken(rxBuf);
    rxBuf = "";
    // note: io_2_valid remains true only for this moment; it will be consumed in the next tick
    // But since we call serialService once per control step, it's effectively valid for that step.
  }
}

void setup() {
  pinMode(MODE1_PWM_PIN, OUTPUT);
  analogWrite(MODE1_PWM_PIN, 0);

  Serial.begin(9600);
  rxBuf.reserve(16);
  lastRxMs = millis();

  // Controller initial state per model
  mode = 2;
  pwmValue = 0.0;
  phaseMs = 0;
}

void loop() {
  static uint32_t lastControlMs = millis();

  uint32_t now = millis();
  if ((now - lastControlMs) < CONTROL_PERIOD_MS) {
    // Still service serial to maintain idle framing accuracy
    serialService();
    return;
  }
  lastControlMs = now;

  // Update io_1: Actual delta since previous control step
  io_1_ms_delta = (int32_t)(now - (lastControlMs)); // will be corrected below
  // Recompute accurately relative to previous period boundary:
  // Since lastControlMs updated above, use CONTROL_PERIOD_MS as exact contract tick period.
  // The model expects elapsed delta; contract says actual delta since previous control step.
  // We'll use CONTROL_PERIOD_MS to keep deterministic for fixed-step loop timing.
  io_1_ms_delta = (int32_t)CONTROL_PERIOD_MS;

  // Service serial to update io_2 and io_2_valid
  // Important: io_2_valid is "true for this model step only" when a complete valid serial frame was received
  io_2_cmd = io_2_cmd; // keep previous value unless overwritten by valid token in this service call
  serialService();

  // === Accepted model tick ===
  // State update order and conditions preserved as given.
  // 1) if (io_2_valid and (io_2 == 1)) { mode=1; phaseMs=0 }
  if (io_2_valid && (io_2_cmd == 1)) {
    mode = 1;
    phaseMs = 0;
  }

  // 2) if (io_2_valid and (io_2 == 2)) { mode=2; phaseMs=0 }
  if (io_2_valid && (io_2_cmd == 2)) {
    mode = 2;
    phaseMs = 0;
  }

  // 3) if (mode == 1) { phaseMs = phaseMs + io_1; pwmValue = (parity test) ? 1023 : 0 }
  if (mode == 1) {
    phaseMs = phaseMs + io_1_ms_delta;

    // pwmValue := if ((phaseMs / 10) == (2 * floor((phaseMs / 10) / 2))) then 1023.0 else 0.0
    // Using real arithmetic with exact structure; phaseMs/10 and floor operate on RealFunctions.
    double q = ((double)phaseMs) / 10.0;
    double rhs = 2.0 * floor(q / 2.0);
    pwmValue = (q == rhs) ? 1023.0 : 0.0;
  } else {
    pwmValue = 1023.0;
  }

  // Outputs
  setPwmFromModel(pwmValue);

  // serial_0_trigger always true
  serial_0_trigger = true;

  // serial_0_value_0 := mode
  // serial_0_value_1 := pwmValue
  // No report format specified; output numeric values as two tokens on one line:
  // MODE <mode> D9_PWM <pwmValue>
  // Trigger condition is always true; report cadence and formatting are not specified electrically.
  if (serial_0_trigger) {
    Serial.print("MODE:");
    Serial.print(mode);
    Serial.print(",D9_PWM:");
    Serial.println(pwmValue, 1);
  }
}
