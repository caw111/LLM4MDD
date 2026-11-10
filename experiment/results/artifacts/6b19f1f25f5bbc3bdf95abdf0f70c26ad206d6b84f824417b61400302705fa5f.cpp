#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Minimal TM1637 driver (4-digit) with CLK=D2, DIO=D3
// Implements: display integer 1234 on startup; update behavior matches
// frozen contract: no inputs, outputs remain constant.
class TM1637 {
public:
  TM1637(uint8_t dioPin, uint8_t clkPin)
      : dio(dioPin), clk(clkPin) {}

  void begin() {
    pinMode(dio, OUTPUT);
    pinMode(clk, OUTPUT);
    digitalWrite(dio, HIGH);
    digitalWrite(clk, HIGH);
    // Sync / start condition
    // (Many implementations rely on write commands later.)
  }

  void setBrightness(uint8_t brightness) {
    brightness &= 0x07; // 0..7
    writeByte(0x88 | brightness); // command: display control
  }

  void displayRaw(const uint8_t seg[4]) {
    // Command to set auto-increment addressing starts at 0x40 (data command)
    start();
    writeByte(0x40);
    stop();

    for (uint8_t i = 0; i < 4; i++) {
      start();
      writeByte(0x40 + i);
      writeByte(seg[i] & 0xFF);
      stop();
    }

    // finalize: display control already set by brightness command
  }

  void showNumber(int number, bool leadingZero = false) {
    // Range: 0..9999 (contract does not specify leading-zero behavior beyond config false)
    if (number < 0) number = 0;
    if (number > 9999) number = number % 10000;

    uint8_t digits[4];
    digits[0] = (number / 1000) % 10;
    digits[1] = (number / 100) % 10;
    digits[2] = (number / 10) % 10;
    digits[3] = (number % 10);

    bool started = leadingZero;
    uint8_t segs[4];
    for (uint8_t i = 0; i < 4; i++) {
      uint8_t d = digits[i];
      if (!started && d == 0) {
        // invalid_policy = blank
        segs[i] = 0x00;
      } else {
        started = true;
        segs[i] = segMap[d];
      }
    }

    displayRaw(segs);
  }

private:
  uint8_t dio;
  uint8_t clk;

  // segment mapping: a b c d e f g dp
  // Common mapping for TM1637 (no decimal point used)
  static constexpr uint8_t segMap[10] = {
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

  void start() {
    pinMode(dio, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
    digitalWrite(dio, LOW);
    digitalWrite(clk, LOW);
  }

  void stop() {
    pinMode(dio, OUTPUT);
    digitalWrite(clk, LOW);
    digitalWrite(dio, LOW);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(clk, LOW);
      digitalWrite(dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(clk, HIGH);
    }
    digitalWrite(clk, LOW);
    // ACK: release dio
    pinMode(dio, INPUT);
    digitalWrite(clk, HIGH);
    // Read ACK (ignored)
    digitalWrite(clk, LOW);
    pinMode(dio, OUTPUT);
  }
};

constexpr uint8_t TM1637::segMap[10];

static constexpr uint8_t TM1637_CLK_PIN = D2;
static constexpr uint8_t TM1637_DIO_PIN = D3;

TM1637 tm(TM1637_DIO_PIN, TM1637_CLK_PIN);

// Frozen controller output: io_0 = 1234, constant.
// control_period_ms = 5 -> not strictly required since no inputs/changes,
// but loop() maintains pacing to preserve timing granularity.
static const uint32_t control_period_ms = 5;

uint32_t nextTickMs = 0;

#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_017\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  tm.begin();
  tm.setBrightness(7); // brightness: 7
  // initial requirement: display 1234
  tm.showNumber(1234, false); // leading_zero=false
  nextTickMs = millis() + control_period_ms;
}

void loop() {
  // Maintain control period scheduling granularity (5ms)
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) return;
  nextTickMs += control_period_ms;

  // Controller has no states, no updates, no inputs.
  // Output expression remains constant: io_0 equals 1234.
  tm.setBrightness(7);      // keep hardware display control consistent
  tm.showNumber(1234, false);
}
