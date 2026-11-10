#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

// TM1637 minimal driver (4-digit, numeric)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
    // allow oscillator
    setBrightness(_brightness);
    clearDisplay();
  }

  void setBrightness(uint8_t b) {
    _brightness = b & 0x07;
    // command done by display update, but set internal too
  }

  void clearDisplay() {
    uint8_t segs[4] = { 0, 0, 0, 0 };
    showRaw(segs, false);
  }

  // Show signed/unsigned integer without leading zeros; 0 displayed as "0"
  // blank policy: suppress leading zeros beyond most-significant digit.
  void showNumber(int32_t value) {
    bool neg = value < 0;
    uint32_t v = neg ? (uint32_t)(-value) : (uint32_t)value;

    uint8_t digits[4];
    for (int i = 0; i < 4; i++) digits[i] = 0;

    // Extract decimal digits
    digits[3] = v % 10; v /= 10;
    digits[2] = v % 10; v /= 10;
    digits[1] = v % 10; v /= 10;
    digits[0] = v % 10; v /= 10;

    // Determine how many leading digits to blank
    bool started = false;
    uint8_t segs[4];
    for (int i = 0; i < 4; i++) {
      uint8_t d = digits[i];
      if (!started) {
        if (d == 0 && i != 3) {
          segs[i] = 0x00; // blank
        } else {
          started = true;
          segs[i] = segForDigit(d);
        }
      } else {
        segs[i] = segForDigit(d);
      }
    }

    if (!started) {
      // value was 0: ensure it shows "0"
      segs[3] = segForDigit(0);
      segs[0] = segs[1] = segs[2] = 0x00;
    }

    // If negative, display minus sign on the first non-blank digit position if room
    if (neg) {
      // Put '-' on the leftmost non-blank position, else on last digit
      int putAt = 0;
      while (putAt < 4 && segs[putAt] == 0x00) putAt++;
      if (putAt < 4) {
        segs[putAt] = segMinus();
      } else {
        segs[3] = segMinus();
      }
    }

    showRaw(segs, false);
  }

private:
  uint8_t _clkPin;
  uint8_t _dioPin;
  uint8_t _brightness = 7;

  static uint8_t segForDigit(uint8_t d) {
    // segments: a b c d e f g dp (dp not used)
    // Common approach mapping:
    // 0: 0x3F, 1: 0x06, 2: 0x5B, 3: 0x4F, 4: 0x66, 5: 0x6D, 6: 0x7D, 7: 0x07, 8: 0x7F, 9: 0x6F
    static const uint8_t table[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
    if (d < 10) return table[d];
    return 0x00;
  }

  static uint8_t segMinus() {
    // minus sign: segment g only -> 0x40
    return 0x40;
  }

  void start() {
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, LOW);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(_clkPin, HIGH);
    }
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, HIGH); // ACK
    digitalWrite(_clkPin, HIGH);
  }

  void showRaw(uint8_t segs[4], bool dots) {
    // Address command for display: 0x40
    // Auto-increment addresses
    start();
    writeByte(0x40);
    stop();

    start();
    writeByte(0xC0 | (uint8_t)(_brightness & 0x07)); // brightness command
    stop();

    // Data write: start with 0x00
    start();
    writeByte(0x42 | (dots ? 0x01 : 0x00)); // base address (0x42 typical)
    for (int i = 0; i < 4; i++) {
      uint8_t v = segs[i] & 0x7F;
      // dp bit is bit7, if dots used; not required here
      if (dots) v |= 0x80;
      writeByte(v);
    }
    stop();
  }

  uint8_t segMinusCached() { return segMinus(); }
};

static TM1637 tm1637(4, 5); // clk=D4 dio=D5

// Contract: control_period_ms=5; implement periodic sampling and update
const uint32_t CONTROL_PERIOD_MS = 5;

// Inputs
const uint8_t PIN_BUTTON = 2; // D2
const uint8_t PIN_POT = A1;   // A1 (io_0)
const uint8_t PIN_A0 = A0;    // A0 (io_3 placeholder, sampled but unused in model)

static uint32_t lastTick = 0;

// Model state: none (stateless), but preserve "ordered updates" by doing in control step
// io_2 = (if io_1 then 8888 else floor(io_0))
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t floorRealToInt(double x);
#line 175 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_010\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t floorRealToInt(double x) {
  // In Arduino AVR, double==float; still implement correct floor for non-negative ADC values.
  // For robustness with potential negative, handle explicitly.
  int32_t i = (int32_t)x; // trunc toward zero
  if (x < 0 && (double)i != x) i -= 1;
  return i;
}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  tm1637.begin();
  tm1637.setBrightness(7); // brightness=7
  lastTick = millis();
}

void loop() {
  uint32_t now = millis();
  if ((uint32_t)(now - lastTick) < CONTROL_PERIOD_MS) return;
  lastTick += CONTROL_PERIOD_MS;

  // Sample inputs (each step corresponds to control period updates)
  // io_0: potentiometer analog_input on A1, scale=1, offset=0, unit adc_count
  int adc1 = analogRead(PIN_POT); // 0..1023
  double io_0 = (double)adc1;

  // io_1: digital_input active_high with INPUT_PULLUP; true when pressed -> HIGH
  bool io_1 = (digitalRead(PIN_BUTTON) == HIGH);

  // io_3: analog input on A0 placeholder (unused in model output)
  int adc0 = analogRead(PIN_A0);
  double io_3 = (double)adc0;
  (void)io_3;

  // Compute output (controller)
  int32_t io_2;
  if (io_1) {
    io_2 = 8888;
  } else {
    io_2 = floorRealToInt(io_0);
  }

  // Output io_2 to TM1637 number display
  tm1637.showNumber(io_2);
}
