#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// --- TM1637 minimal driver (single integer display, no leading zeros) ---
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clk(clkPin), _dio(dioPin) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(100);
  }

  void setBrightness(uint8_t brightness) {
    // brightness: 0..7 (implementation: 0x88 | br)
    writeByte(0x88 | (brightness & 0x07));
  }

  void showNumber(int32_t num, bool leadingZero = false) {
    // As per contract: show remaining seconds as a single integer without leading zeros.
    // If num is negative, show '-' with current simple mapping by blanking higher digits.
    uint8_t segments[4] = {0, 0, 0, 0};
    bool neg = (num < 0);
    uint32_t x = neg ? (uint32_t)(-num) : (uint32_t)num;

    // Prepare up to 4 digits, no leading zeros: blank digits above most significant.
    bool started = false;
    for (int i = 0; i < 4; i++) {
      uint32_t digit = x % 10;
      x /= 10;

      if (digit != 0 || started || (i == 0) || leadingZero) {
        started = true;
        segments[3 - i] = digitToSegment(digit);
      } else {
        segments[3 - i] = 0x00; // blank
      }
    }

    // If number is 0 and started never became true, above logic ensures i==0 sets it.
    // Handle negative sign on leftmost possible place.
    if (neg) {
      // place '-' on most significant non-blank digit; simplistic: set digit[0] to '-'
      // with blanking others if needed; keep other segments as computed.
      // If leftmost segment currently blank, put '-' there; else overwrite leftmost.
      segments[0] = 0x40; // '-' segment
    }

    // Write display data starting at address 0
    start();
    writeByte(0x40);     // command: fixed address mode
    stop();

    start();
    writeByte(0xC0);     // command: auto increment
    // no, for simplicity use fixed: Many libraries just use 4 sequential writes in auto increment after 0x40.
    // We'll follow a common approach: fixed address (0x40) then data bytes.
    stop();

    start();
    writeByte(0x40);
    for (int i = 0; i < 4; i++) {
      writeByte(segments[i]);
    }
    stop();

    // Optional: return to normal
  }

private:
  uint8_t _clk, _dio;

  // Segment map: a,b,c,d,e,f,g,dp (typical TM1637 order)
  uint8_t digitToSegment(uint8_t digit) {
    // 0..9
    static const uint8_t map[10] = {
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F  // 9
    };
    if (digit < 10) return map[digit];
    return 0x00;
  }

  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ack bit
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    delayMicroseconds(5);
    digitalWrite(_clk, HIGH);
    delayMicroseconds(5);
    pinMode(_dio, OUTPUT);
  }
};

// ---- Arduino sketch implementing the accepted model with control_period_ms=5ms ----

static const uint8_t CLK_PIN = D4; // CLK=D4
static const uint8_t DIO_PIN = D5; // DIO=D5
static const uint8_t BTN_ADD_PIN = D2;  // D2
static const uint8_t BTN_START_PIN = D3; // D3
static const uint8_t BUZZER_PIN = D6;    // D6

// Model I/O bindings:
// io_0: add button (active high), io_1: start button (active high)
static bool io_0 = false;
static bool io_1 = false;

// Controller state (exactly as model)
static int32_t countdownSeconds = 0;
static bool started = false;
static bool prevAddPressed = false;
static bool prevStartPressed = false;
static bool beepActive = false;
static int32_t beepCounterMs = 0;

// outputs
static int32_t io_2 = 0; // buzzer frequency in Hz
static int32_t io_3 = 0; // display remaining seconds

static TM1637 display(CLK_PIN, DIO_PIN);

// control period scheduling granularity: 5ms
static const uint32_t CONTROL_PERIOD_MS = 5;
static uint32_t lastTickMs = 0;

// SysML/model tick uses decrement of countdown by 1 each tick while started and countdownSeconds > 0.
// In the provided model, "countdownSeconds - 1" executes on every tick, not per second.
// Therefore we must preserve that behavior deterministically: decrement each 5ms tick.
// (The model also sets beepCounterMs := 2000 and decrements by 5ms per tick, yielding 2000ms duration.)
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readInputs();
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 190 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickModel();
#line 241 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 263 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_019\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void readInputs() {
  // Electrical polarity/pullup not specified; use model_semantics active_high true_level=HIGH.
  io_0 = (digitalRead(BTN_ADD_PIN) == HIGH);
  io_1 = (digitalRead(BTN_START_PIN) == HIGH);
}

static inline void applyOutputs() {
  // io_2: if beepActive then 1000 else 0 (Hz). buzzer frequency selection assumed in SysML.
  io_2 = beepActive ? 1000 : 0;
  if (io_2 > 0) {
    tone(BUZZER_PIN, (unsigned int)io_2);
  } else {
    noTone(BUZZER_PIN);
  }

  // io_3: countdownSeconds shown; invalid_policy=blank hides the value when invalid per model rules.
  // No invalid rule specified in model: always valid integer. So always show countdownSeconds.
  io_3 = countdownSeconds;
  // TM1637 contract: leading_zero false, invalid_policy blank.
  // We'll show integer directly; blanking not required because countdownSeconds always integer.
  display.showNumber(io_3, false);
}

static inline void tickModel() {
  // --- updates in the exact order described ---
  // 1) if io_0 && !prevAddPressed => countdownSeconds := started ? countdownSeconds + 10 : countdownSeconds
  if (io_0 && !prevAddPressed) {
    if (started) {
      countdownSeconds = countdownSeconds + 10;
    } else {
      countdownSeconds = countdownSeconds; // unchanged
    }
  }

  // 2) if io_1 && !prevStartPressed =>
  //    started := true
  //    countdownSeconds := (countdownSeconds == 0 ? 10 : countdownSeconds)
  //    beepActive := false
  //    beepCounterMs := 0
  if (io_1 && !prevStartPressed) {
    started = true;
    if (countdownSeconds == 0) countdownSeconds = 10;
    beepActive = false;
    beepCounterMs = 0;
  }

  // 3) if started:
  if (started) {
    // if countdownSeconds > 0 => countdownSeconds := countdownSeconds - 1
    if (countdownSeconds > 0) {
      countdownSeconds = countdownSeconds - 1;
    }
    // if countdownSeconds == 0 && !beepActive => beepActive true and beepCounterMs := 2000
    if ((countdownSeconds == 0) && (!beepActive)) {
      beepActive = true;
      beepCounterMs = 2000;
    }
  }

  // 4) if beepActive:
  if (beepActive) {
    beepCounterMs = beepCounterMs - (int32_t)CONTROL_PERIOD_MS; // -5
    if (beepCounterMs <= 0) {
      beepActive = false;
      beepCounterMs = 0;
    }
  }

  // 5) prevAddPressed := io_0
  prevAddPressed = io_0;
  // 6) prevStartPressed := io_1
  prevStartPressed = io_1;
}

void setup() {
  pinMode(BTN_ADD_PIN, INPUT);
  pinMode(BTN_START_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  display.begin();
  display.setBrightness(7); // brightness=7

  // initialize outputs/state
  countdownSeconds = 0;
  started = false;
  prevAddPressed = false;
  prevStartPressed = false;
  beepActive = false;
  beepCounterMs = 0;

  // initial render
  applyOutputs();

  lastTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  while ((uint32_t)(now - lastTickMs) >= CONTROL_PERIOD_MS) {
    lastTickMs += CONTROL_PERIOD_MS;

    readInputs();
    tickModel();
    applyOutputs();
  }
}
