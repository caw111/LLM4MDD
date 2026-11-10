#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

// TM1637 minimal driver (no external libs required)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);

    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(10);
  }

  // brightness: 0..7 (per typical TM1637), requirement fixes brightness=7
  void setBrightness(uint8_t brightness) {
    brightness &= 0x07;
    start();
    writeByte(0x88 | brightness); // 0b1000 1000 + brightness
    stop();
  }

  // Display 4 digits (no leading zero behavior is handled by mapping digits directly).
  // If value is 0..1111, this function expects digits already chosen.
  // For this model, we render either 1111 or 0000.
  void displayDigits(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) {
    uint8_t segs[4] = { d0, d1, d2, d3 };

    start();
    writeByte(0x40); // data command
    // order: digit1..digit4 mapped by TM1637 addressing
    // Addresses: 0x00, 0x01, 0x02, 0x03.
    for (uint8_t i = 0; i < 4; i++) {
      writeByte(segs[i]);
    }
    stop();
  }

private:
  uint8_t _clkPin, _dioPin;

  // Segment encodings for digits 0-9 (a,b,c,d,e,f,g,dp) without dp
  // Standard for TM1637: bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
  // These encodings are commonly used and sufficient for numeric display.
  uint8_t seg(uint8_t n) const {
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
    if (n <= 9) return table[n];
    return 0x00;
  }

  void start() {
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(5);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      delayMicroseconds(5);
      // LSB first
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(5);
      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK bit (release)
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT);
    delayMicroseconds(5);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(5);
    pinMode(_dioPin, OUTPUT);
  }

  // helper to map digit value to segment encoding
  uint8_t digitToSeg(uint8_t digit) const {
    return seg(digit);
  }

public:
  // Convenience overload matching the model’s required usage:
  // render numeric value where digits are either 0 or 1.
  void show1111Or0000(bool pressed) {
    if (pressed) {
      uint8_t s1 = digitToSeg(1);
      displayDigits(s1, s1, s1, s1);
    } else {
      uint8_t s0 = digitToSeg(0);
      displayDigits(s0, s0, s0, s0);
    }
  }
};

// Hardware configuration from contract
static const uint8_t PIN_D2 = 2;  // D2 = Arduino digital pin 2
static const uint8_t PIN_D4 = 4;  // CLK = D4
static const uint8_t PIN_D5 = 5;  // DIO = D5

static const uint32_t CONTROL_PERIOD_MS = 5;

TM1637 tm1637(PIN_D4, PIN_D5);

// Model-preserving state (none required beyond combinational mapping),
// but we sample the input and update output in the required control period.
static bool io_0 = false;   // input port
static int32_t io_1 = 0;    // output port (display number in model space: 0 or 1111)

#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_020\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  // io_0: digital_input, active_high=true, pullup=false
  pinMode(PIN_D2, INPUT); // no pullup, active-high

  tm1637.begin();
  tm1637.setBrightness(7); // fixed by contract: brightness=7

  // Initial model output: io_1 = (if io_0 then 1111 else 0)
  // Contract initial for io_0 is false, thus io_1 initial is 0.
  io_0 = false;
  io_1 = 0;
  tm1637.show1111Or0000(false);
}

void loop() {
  static uint32_t lastTick = 0;
  uint32_t now = millis();

  // Fixed 5ms planning period
  if ((now - lastTick) < CONTROL_PERIOD_MS) return;
  lastTick = now;

  // Sample io_0
  // Model semantics: true_level=HIGH, false_level=LOW
  io_0 = (digitalRead(PIN_D2) == HIGH);

  // Accepted executable model:
  // io_1 = (if io_0 then 1111 else 0)
  io_1 = io_0 ? 1111 : 0;

  // Update TM1637 display:
  // requirement: when D2 pressed display 1111, else 0000
  tm1637.show1111Or0000(io_0);
}
