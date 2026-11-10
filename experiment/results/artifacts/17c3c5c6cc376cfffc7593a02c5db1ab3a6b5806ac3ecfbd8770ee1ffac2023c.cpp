#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/*
  ACCEPTED executable model + frozen hardware contract implementation.

  Control period: 5 ms (used as loop timing granularity).
  Serial receive: 9600 baud, "line_or_idle" framing, idle_timeout_ms = 20.
  Model:
    displayValue holds last valid (0..31) serial integer.
    LED outputs are active-high on D2..D6:
      io_0..io_4 correspond to LED1..LED5 (LSB..MSB).
    serial_0_trigger asserted for one step only when a complete valid frame was received.
*/

static constexpr uint8_t LED1_PIN = 2; // io_0
static constexpr uint8_t LED2_PIN = 3; // io_1
static constexpr uint8_t LED3_PIN = 4; // io_2
static constexpr uint8_t LED4_PIN = 5; // io_3
static constexpr uint8_t LED5_PIN = 6; // io_4

// Model state
static int32_t displayValue = 0;

// Timing
static const uint32_t CONTROL_PERIOD_MS = 5;
static uint32_t nextTickMs = 0;

// Serial framing (line_or_idle with idle_timeout_ms=20)
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t IDLE_TIMEOUT_MS = 20;

static bool io_5_valid = false;
static int32_t io_5 = 0;

// Buffer for current numeric token
static char rxBuf[16];
static uint8_t rxLen = 0;

// For idle timeout detection
static bool haveRxActivity = false;
static uint32_t lastByteMs = 0;

#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool isDigitChar(char c);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void resetToken();
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool tryParseAndValidate(int32_t &outVal);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(int32_t dv);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateSerialInputs();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 181 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_binary_led\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline bool isDigitChar(char c) {
  return (c >= '0' && c <= '9');
}

static void resetToken() {
  rxLen = 0;
  rxBuf[0] = '\0';
}

static bool tryParseAndValidate(int32_t &outVal) {
  if (rxLen == 0) return false;

  rxBuf[rxLen] = '\0';

  // Only non-negative integers are expected (0..31).
  // If token contains non-digits, reject.
  for (uint8_t i = 0; i < rxLen; i++) {
    if (!isDigitChar(rxBuf[i])) return false;
  }

  long v = strtol(rxBuf, nullptr, 10);
  if (v < 0 || v > 31) return false;

  outVal = (int32_t)v;
  return true;
}

// Apply model outputs (frozen expressions)
static inline void applyOutputs(int32_t dv) {
  // Use integer math as floor(dv/x) for non-negative dv (dv is 0..31).
  // The model expressions:
  // io_0 = (dv - 1*floor(dv/2)) == 1  <=> (dv % 2) == 1
  // io_1 = (floor(dv/2) - 1*floor(floor(dv/2)/2)) == 1  <=> (floor(dv/2) % 2) == 1 => (dv>>1)&1
  // io_2 => (dv>>2)&1, io_3 => (dv>>3)&1, io_4 => (dv>>4)&1
  // But we keep the exact structure by using integer floors.
  int32_t q2 = dv / 2;
  int32_t q4 = dv / 4;
  int32_t q8 = dv / 8;
  int32_t q16 = dv / 16;

  bool io_0 = (dv - 1 * q2) == 1;
  bool io_1 = ((q2 - 1 * (q2 / 2)) == 1);
  bool io_2 = ((q4 - 1 * (q4 / 2)) == 1);
  bool io_3 = ((q8 - 1 * (q8 / 2)) == 1);
  bool io_4 = ((q16 - 1 * (q16 / 2)) == 1);

  // Digital outputs are active-high; model true => LED ON.
  digitalWrite(LED1_PIN, io_0 ? HIGH : LOW);
  digitalWrite(LED2_PIN, io_1 ? HIGH : LOW);
  digitalWrite(LED3_PIN, io_2 ? HIGH : LOW);
  digitalWrite(LED4_PIN, io_3 ? HIGH : LOW);
  digitalWrite(LED5_PIN, io_4 ? HIGH : LOW);
}

// Serial receiver producing io_5_valid for exactly one control step
static void updateSerialInputs() {
  io_5_valid = false;

  uint32_t now = millis();

  // Read all available bytes (line_or_idle tokenization)
  bool gotAny = false;
  while (Serial.available() > 0) {
    int b = Serial.read();
    if (b < 0) break;
    gotAny = true;

    // Line_or_idle: treat newline/carriage return as end-of-token (frame boundary)
    char c = (char)b;
    if (c == '\n' || c == '\r') {
      // If we have a token, attempt validation and trigger if valid.
      int32_t v = 0;
      if (tryParseAndValidate(v)) {
        io_5 = v;
        io_5_valid = true;
      }
      resetToken();
      haveRxActivity = false;
      lastByteMs = 0;
      // Continue consuming any further bytes (still within same step)
      continue;
    }

    // Ignore leading whitespace
    if (rxLen == 0 && (c == ' ' || c == '\t')) {
      continue;
    }

    // Accumulate digits (and reject non-digit by storing and later failing parse)
    if (rxLen < sizeof(rxBuf) - 1) {
      rxBuf[rxLen++] = c;
      rxBuf[rxLen] = '\0';
    }

    haveRxActivity = true;
    lastByteMs = now;
  }

  // If no newline was received, use idle timeout to end frame.
  // idle_timeout_ms = 20: if there was activity and now - lastByteMs >= timeout, finalize.
  if (!io_5_valid && haveRxActivity) {
    if ((now - lastByteMs) >= IDLE_TIMEOUT_MS) {
      int32_t v = 0;
      if (tryParseAndValidate(v)) {
        io_5 = v;
        io_5_valid = true;
      }
      resetToken();
      haveRxActivity = false;
      lastByteMs = 0;
    }
  }

  // If gotAny was true but no frame boundary, io_5_valid remains false for this step.
  (void)gotAny;
}

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);

  // Initial model state displayValue = 0
  // Apply initial outputs.
  applyOutputs(displayValue);

  nextTickMs = millis();
  resetToken();
  haveRxActivity = false;
  lastByteMs = 0;
  io_5 = 0;
  io_5_valid = false;
}

void loop() {
  // Enforce 5 ms control period granularity
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return;
  }
  // catch-up in case of long delays
  while ((int32_t)(now - nextTickMs) >= 0) {
    nextTickMs += CONTROL_PERIOD_MS;
  }

  // === Tick: input sampling + validity ===
  updateSerialInputs();

  // === Model tick update ===
  // perform action tick: if (io_5_valid) displayValue := io_5
  if (io_5_valid) {
    displayValue = io_5;
  }

  // === Outputs ===
  applyOutputs(displayValue);

  // serial_0_trigger is modeled as io_5_valid (one-step pulse),
  // but no explicit pin exists for it in the frozen hardware contract.
  // serial_0_value_0 is displayValue; not exposed as an Arduino output pin.
}
