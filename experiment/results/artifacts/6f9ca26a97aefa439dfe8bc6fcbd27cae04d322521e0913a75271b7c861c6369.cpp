#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

// -------------------- Pin bindings (authoritative) --------------------
static const uint8_t ULTRASONIC_TRIG_PIN = 8; // D8
static const uint8_t ULTRASONIC_ECHO_PIN = 9; // D9
static const uint8_t TM1637_CLK_PIN       = 4; // D4
static const uint8_t TM1637_DIO_PIN       = 5; // D5

// -------------------- Timing / model contract --------------------
static const uint32_t ULTRASONIC_TIMEOUT_US = 25000UL; // "each wait must have 25000us超时"
static const uint32_t CONTROL_PERIOD_MS = 5;            // frozen controller period (not strict, used for scheduling)
static const uint32_t START_SPACING_MS_MIN = 60;
static const uint32_t START_SPACING_MS_MAX = 160;
static const uint32_t END_TO_END_LATENCY_BUDGET_MS = 250; // used as planning; behavior below measures promptly

// -------------------- TM1637 minimal driver --------------------
// Assumptions: standard TM1637 interface (DIO/CLK). Behavior:
// - "invalid_policy=blank": when value==10000 => display blank.
// - No leading zeros.
class TM1637Number {
public:
  TM1637Number(uint8_t clkPin, uint8_t dioPin)
    : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, HIGH);
    // Typical startup
    setBrightness(7);
    clear();
  }

  void setBrightness(uint8_t b) {
    brightness_ = b & 0x07;
  }

  // Display integer in range [0..9999].
// If blankValue == true => blank display.
  void displayIntOrBlank(int value, bool blankValue) {
    if (blankValue) {
      clear();
      return;
    }
    if (value < 0) {
      clear();
      return;
    }
    if (value > 9999) {
      clear();
      return;
    }

    uint16_t digits[4] = {0, 0, 0, 0};

    // No leading zeros.
    int v = value;
    bool started = false;
    for (int pos = 3; pos >= 0; --pos) { // rightmost to leftmost
      int d = v % 10;
      v /= 10;
      if (!started) {
        if (d == 0 && v != 0) {
          // still leading; show blank (0 bits) to avoid leading zeros
          digits[pos] = 0x0000;
        } else {
          started = true;
          digits[pos] = seg(d);
        }
      } else {
        digits[pos] = seg(d);
      }
    }

    // Special case: value == 0 should show "0" (no leading zero suppression).
    if (value == 0) {
      digits[3] = seg(0);
      digits[0] = digits[1] = digits[2] = 0x0000;
    }

    writeDigits(digits);
  }

private:
  uint8_t clkPin_;
  uint8_t dioPin_;
  uint8_t brightness_ = 7;

  // Segment encoding for TM1637 (typical, no decimal point)
  // a b c d e f g
  uint16_t seg(int d) const {
    switch (d) {
      case 0: return 0x3F; // 0
      case 1: return 0x06; // 1
      case 2: return 0x5B; // 2
      case 3: return 0x4F; // 3
      case 4: return 0x66; // 4
      case 5: return 0x6D; // 5
      case 6: return 0x7D; // 6
      case 7: return 0x07; // 7
      case 8: return 0x7F; // 8
      case 9: return 0x6F; // 9
      default: return 0x00;
    }
  }

  void start() {
    pinMode(dioPin_, OUTPUT);
    digitalWrite(dioPin_, HIGH);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, LOW);
    digitalWrite(clkPin_, LOW);
  }

  void stop() {
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; ++i) {
      digitalWrite(clkPin_, LOW);
      digitalWrite(dioPin_, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(3);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(3);
      b >>= 1;
    }
    digitalWrite(clkPin_, LOW);
    pinMode(dioPin_, INPUT_PULLUP);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    pinMode(dioPin_, OUTPUT);
  }

  void setBrightness(uint8_t b) {
    brightness_ = b & 0x07;
    // Typical command: 0x88 | brightness (6=0x8?); implemented in writeDigits
  }

  void writeCommand(uint8_t cmd) {
    start();
    writeByte(cmd);
    stop();
  }

  void writeDigits(uint16_t digits[4]) {
    // Command for data: 0x40
    // 2nd command: start address 0x00
    // Use brightness: 0x80 | 0x0..0x7
    start();
    writeByte(0x40);
    writeByte(0x00);

    // TM1637 expects little endian digits: D1..D4 mapping varies;
    // Common mapping for libraries: digits[0]=rightmost? We'll use a stable approach:
    // We'll send [0]=digit0 (rightmost) ... then digit3 (leftmost).
    // Our digits[] indexed [0..3] left->right, so convert.
    uint16_t send[4] = { digits[3], digits[2], digits[1], digits[0] };
    for (int i = 0; i < 4; ++i) {
      writeByte(static_cast<uint8_t>(send[i] & 0xFF));
    }
    stop();

    // Display control with brightness and normal mode
    writeCommand(0x88 | brightness_);
  }

  void clear() {
    uint16_t digits[4] = {0, 0, 0, 0};
    writeDigits(digits);
  }
};

// -------------------- Controller model (ACCEPTED executable model) --------------------
static int32_t io_2_lastValidDistCmQuantized = 10000; // initial blank requested out-of-range

// Controller tick: update state only when io_1=true; output depends on io_1 and io_0.
#line 185 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t controller_computeOutput(bool io_1, double io_0_cm);
#line 223 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void startUltrasonicAndMeasureOnce();
#line 266 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t randStartSpacingMs();
#line 277 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 292 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 185 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_007\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t controller_computeOutput(bool io_1, double io_0_cm) {
  // SysML: if io_1 then assign lastValid := (if invalid then 10000 else floor(io_0+0.5))
  if (io_1) {
    int32_t q = 10000;
    if (io_0_cm >= 2.0 && io_0_cm <= 400.0) {
      q = (int32_t)floor(io_0_cm + 0.5);
    } else {
      q = 10000; // blank on invalid
    }
    io_2_lastValidDistCmQuantized = q;
  }

  // SysML out io_2:
  // if io_1 then same expression else hold lastValid
  if (io_1) {
    if (io_0_cm < 2.0 || io_0_cm > 400.0) return 10000;
    return (int32_t)floor(io_0_cm + 0.5);
  } else {
    return io_2_lastValidDistCmQuantized;
  }
}

// -------------------- Ultrasonic sampling (HC-SR04) --------------------
static uint32_t nextStartAtMs = 0;
static bool measuring = false;
static uint32_t measureStartMs = 0;

// Latest IO inputs derived from measurement
static double io_0_ultrasonic_cm = 0.0;
static bool io_1_ultrasonic_valid = false;

// Measurement results tracking for invalid / blank behavior
static bool lastEchoStuckOrInvalid = false;

// We implement measurement in a single-shot blocking sequence that still respects
// the per-wait timeout requirement. Since contract requires display to update within 250ms,
// and timeout is max 25000us + small overhead, blocking is acceptable when sampling spacing
// is between 60..160ms.
static void startUltrasonicAndMeasureOnce() {
  // Trigger pulse: 10us HIGH
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  // Wait for echo:
  // Use pulseIn with timeout in microseconds to satisfy "each wait must have 25000us超时".
  // pulseIn performs: wait for any pulse start then measure pulse width; if none, returns 0.
  // For "each wait must have ...", we approximate by using the same timeout for the pulseIn wait.
  // HC-SR04 will produce a pulse only if echo returns; if echo stuck high, pulseIn likely times out.
  uint32_t duration_us = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, ULTRASONIC_TIMEOUT_US);

  // Determine validity:
  // - invalid if no echo (duration_us==0)
  // - convert to cm: microseconds / 58
  // - then range check 2..400 in controller quantization expression.
  if (duration_us == 0) {
    io_1_ultrasonic_valid = false;
    lastEchoStuckOrInvalid = true;
    // io_0 remains some value; model uses io_0 but invalid flag will drive blank/hold.
    io_0_ultrasonic_cm = 0.0;
    return;
  }

  // Convert
  double cm = (double)duration_us / 58.0;
  io_0_ultrasonic_cm = cm;

  // Validity flag: controller will blank/hold based on io_0 range,
  // but validity flag must distinguish timeout/out-of-range/echo stuck.
  // Contract: io_1=false on timeout/out-of-range/echo stuck.
  // We'll implement: valid iff duration_us!=0 AND cm in [2,400].
  if (cm < 2.0 || cm > 400.0) {
    io_1_ultrasonic_valid = false;
  } else {
    io_1_ultrasonic_valid = true;
  }
  lastEchoStuckOrInvalid = false;
}

static uint32_t randStartSpacingMs() {
  // Deterministic-ish spacing without RNG dependency:
  // Use micros to vary between 60..160ms while not breaking constraints.
  uint32_t x = (uint32_t)(micros() & 0xFFFFFFFFUL);
  uint32_t span = (START_SPACING_MS_MAX - START_SPACING_MS_MIN + 1);
  return START_SPACING_MS_MIN + (x % span);
}

// -------------------- Arduino lifecycle --------------------
TM1637Number display(TM1637_CLK_PIN, TM1637_DIO_PIN);

void setup() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  display.begin();
  display.setBrightness(7);

  // Initial display blank: model initial state requests out-of-display-range 10000 with invalid_policy=blank.
  display.displayIntOrBlank((int)io_2_lastValidDistCmQuantized, true);

  nextStartAtMs = millis() + randStartSpacingMs();
}

void loop() {
  uint32_t nowMs = millis();

  // Periodic scheduling; keep controller update and display update responsive.
  // When it's time, perform one ultrasonic measurement (blocking within its timeout).
  if (nowMs >= nextStartAtMs) {
    // Perform measurement once.
    startUltrasonicAndMeasureOnce();

    // Controller tick as per model: updates only if io_1=true; output blank vs hold_last.
    int32_t out = controller_computeOutput(io_1_ultrasonic_valid, io_0_ultrasonic_cm);

    // TM1637 output: invalid_policy='blank' when requested value is 10000.
    // (The model never outputs other out-of-range values; it outputs 10000 exactly for blank.)
    bool blank = (out == 10000);
    display.displayIntOrBlank((int)out, blank);

    // Schedule next sampling start between 60..160ms.
    nextStartAtMs = nowMs + randStartSpacingMs();
  }

  // Keep loop lightweight; no other behavior defined by model.
  delay(CONTROL_PERIOD_MS);
}
