#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Hardware mappings (frozen contract)
static constexpr uint8_t PWM_PIN = 5;   // D5 -> io_0 (pwm_count)
static constexpr uint8_t POT_PIN = A0;  // A0 -> io_1 (adc_count)
static constexpr uint8_t LM35_PIN = A1;  // A1 -> io_2 (adc_count)
static constexpr uint8_t BTN_PIN = 2;   // D2 -> io_3 (digital_input, active_high, pullup=false)

// TM1637 (CLK=D4, DIO=D3)
static constexpr uint8_t TM1637_CLK = 4; // D4
static constexpr uint8_t TM1637_DIO = 3; // D3

// ---- Minimal TM1637 implementation (no external libs) ----
// Supports showing integer 0..9999 with no leading zeros.
// invalid_policy="blank" handled by not updating when outside range in this sketch.
class TM1637Number {
public:
  TM1637Number(uint8_t clk, uint8_t dio, uint8_t brightness = 7)
      : _clk(clk), _dio(dio), _brightness(brightness) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, HIGH);
    // Set brightness
    setBrightness(_brightness);
  }

  // invalid_policy="blank": if invalid, blank display
  void showInteger(int32_t value) {
    if (value < 0 || value > 9999) {
      blank();
      return;
    }

    // Convert to digits
    int32_t v = value;
    int digits[4] = {0, 0, 0, 0};
    digits[3] = v % 10; v /= 10;
    digits[2] = v % 10; v /= 10;
    digits[1] = v % 10; v /= 10;
    digits[0] = v % 10;

    // leading_zero=false => blank leading zeros
    uint8_t seg[4];
    for (int i = 0; i < 4; i++) {
      bool leading = true;
      for (int k = 0; k < i; k++) {
        if (digits[k] != 0) { leading = false; break; }
      }
      // If i is before the first non-zero digit, blank it.
      if (i != 3) {
        // Determine if this position is leading
        // (For value 0, keep last digit; others blank)
        bool allPrevZero = true;
        for (int k = 0; k < i; k++) if (digits[k] != 0) { allPrevZero = false; break; }
        if (allPrevZero && digits[i] == 0) {
          seg[i] = 0x00; // blank
        } else {
          seg[i] = digitToSeg(digits[i]);
        }
      } else {
        // last digit always shown
        seg[i] = digitToSeg(digits[i]);
      }
    }

    writeSegments(seg);
  }

  void blank() {
    uint8_t seg[4] = {0x00, 0x00, 0x00, 0x00};
    writeSegments(seg);
  }

private:
  uint8_t _clk, _dio;
  uint8_t _brightness;

  // Segment map for TM1637 (common approach)
  static uint8_t digitToSeg(int d) {
    // 0..9, segments: dp ignored (bit 7)
    static const uint8_t table[10] = {
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
    if (d < 0 || d > 9) return 0x00;
    return table[d];
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(4);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(4);
      b >>= 1;
    }
  }

  void start() {
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, LOW);
    delayMicroseconds(5);
    digitalWrite(_clk, LOW);
  }

  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    delayMicroseconds(5);
  }

  void setBrightness(uint8_t b) {
    // Command: 0x88 | brightness (lower 2-3 bits)
    start();
    writeByte(0x88 | (b & 0x07));
    stop();
  }

  void writeSegments(const uint8_t seg[4]) {
    // Data command: 0x40
    start();
    writeByte(0x40);
    // Addressing per TM1637: 0x00..0x03 auto increment with ACK ignored
    for (int i = 0; i < 4; i++) {
      writeByte(seg[i]);
    }
    stop();

    // Display control: turn on
    start();
    writeByte(0x80 | 0x01);
    stop();
  }
};

// ---- Accepted model implementation (control_period_ms = 10) ----
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Model state (must preserve exact initial values)
static bool manualMode = false;     // initial false
static bool previousPressed = false; // initial false

// Inputs (io_1, io_2, io_3) as f64/bool
static double io_1 = 0.0; // pot adc_count
static double io_2 = 0.0; // lm35 adc_count
static bool io_3 = false; // button

// Outputs: io_0 (pwm_count) Real, io_4 (i32) integer
static double io_0 = 0.0;
static int32_t io_4 = 0;

// Hardware drivers
static TM1637Number tm(TM1637_CLK, TM1637_DIO, 7);

#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readInputs();
#line 179 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickModel();
#line 204 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeOutputs();
#line 220 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_002\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void readInputs() {
  // analog input reads in [0..1023] on UNO
  io_1 = (double)analogRead(POT_PIN); // adc_count
  io_2 = (double)analogRead(LM35_PIN); // adc_count

  // digital_input semantics from contract: active_high=true, pullup=false
  // Model semantics: io_3 true/false reflect electrical level.
  io_3 = (digitalRead(BTN_PIN) == HIGH);
}

static void tickModel() {
  // Update ordering exactly as in provided Accepted model:
  // 1) perform action tick:
  //    first start;
  //    then if (io_3 and not previousPressed) { manualMode := not manualMode; }
  //    then previousPressed := io_3;
  if (io_3 && (!previousPressed)) {
    manualMode = !manualMode;
  }
  previousPressed = io_3;

  // Outputs:
  // io_0 = (manualMode ? io_1 : io_2)
  io_0 = manualMode ? io_1 : io_2;

  // io_4 = (manualMode ? ((io_1>=0.0)?1:0) : ((io_2>=0.0)?1:0))
  // NOTE: since adc_count from analogRead is non-negative, these become 1,
  // but we preserve the exact expression behavior anyway.
  if (manualMode) {
    io_4 = (io_1 >= 0.0) ? 1 : 0;
  } else {
    io_4 = (io_2 >= 0.0) ? 1 : 0;
  }
}

static void writeOutputs() {
  // io_0: pwm_count -> D5 using Arduino analogWrite (0..255)
  // Preserve semantics of "pwm_count" as a scalar mapping; scale is contract scale=1.0, offset=0.0.
  // We must convert f64 range [0..1023] (adc_count) to PWM duty [0..255] for actual hardware.
  // Use deterministic integer mapping: duty = clamp(round(io_0 * 255/1023)).
  double x = io_0;
  if (x < 0.0) x = 0.0;
  if (x > 1023.0) x = 1023.0;
  uint8_t duty = (uint8_t)((x * 255.0) / 1023.0 + 0.5);

  analogWrite(PWM_PIN, duty);

  // io_4 on TM1637: invalid_policy="blank", leading_zero=false, brightness=7
  tm.showInteger(io_4);
}

void setup() {
  // Pin modes
  pinMode(PWM_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT); // pullup disabled per contract (pullup=false)

  // Analog pins are configured implicitly by analogRead; no further setup required.

  // TM1637 init
  tm.begin();

  // Apply initial outputs consistent with model initial state at time 0
  // (Read inputs only at first control tick; otherwise keep deterministic initial output policy)
  analogWrite(PWM_PIN, 0);
  tm.showInteger(0);

  // Start time for 10ms periodic loop
  // (No serial per contract: serial_channels empty)
}

void loop() {
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if ((int32_t)(now - nextTick) >= 0) {
    nextTick = now + CONTROL_PERIOD_MS;

    // Sample/update in one control cycle
    readInputs();
    tickModel();
    writeOutputs();
  }
}
