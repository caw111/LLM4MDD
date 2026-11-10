#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdint.h>

static constexpr uint8_t PIN_D2 = 2;
static constexpr uint8_t PIN_D3 = 3;

static constexpr uint8_t PIN_TM1637_CLK = 4; // CLK = D4
static constexpr uint8_t PIN_TM1637_DIO = 5; // DIO = D5

static constexpr uint8_t PIN_BUZZER = 6;    // D6

// ---------------- TM1637 minimal driver (no external deps) ----------------
class TM1637Number {
public:
  TM1637Number(uint8_t clkPin, uint8_t dioPin, uint8_t brightness = 7, bool leadingZero = false)
    : clkPin_(clkPin), dioPin_(dioPin), brightness_(brightness), leadingZero_(leadingZero) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
    clear();
  }

  void clear() {
    // blank all digits
    uint8_t blank = 0x00;
    uint8_t data[4] = {blank, blank, blank, blank};
    writeDigits(data);
  }

  void displayInt(int32_t value) {
    // Model: "range 0..20 inclusive", invalid_policy=blank, leading_zero=false
    if (value < 0 || value > 20) {
      clear();
      return;
    }

    // TM1637 4-digit display; show as right-aligned number with no leading zeros.
    // value 0..20 => 1 or 2 digits.
    uint8_t digits[4] = {0, 0, 0, 0};

    int32_t v = value;
    int ones = v % 10;
    int tens = (v / 10) % 10;

    if (tens == 0) {
      // show single digit (ones) at position 3
      digits[0] = 0x00;
      digits[1] = 0x00;
      digits[2] = 0x00;
      digits[3] = seg(ones);
    } else {
      // show two digits at positions 2 and 3
      digits[0] = 0x00;
      digits[1] = 0x00;
      digits[2] = seg(tens);
      digits[3] = seg(ones);
    }

    writeDigits(digits);
  }

private:
  uint8_t clkPin_;
  uint8_t dioPin_;
  uint8_t brightness_;
  bool leadingZero_;

  static uint8_t seg(int d) {
    // Common 7-seg encoding for TM1637 (segments a,b,c,d,e,f,g,dp with dp off).
    // Standard mapping used widely:
    // 0: 0x3f 1:0x06 2:0x5b 3:0x4f 4:0x66 5:0x6d 6:0x7d 7:0x07 8:0x7f 9:0x6f
    static const uint8_t map[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    if (d < 0 || d > 9) return 0x00;
    return map[d];
  }

  void start() {
    digitalWrite(dioPin_, HIGH);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, LOW);
  }

  void stop() {
    digitalWrite(clkPin_, LOW);
    digitalWrite(dioPin_, LOW);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    digitalWrite(dioPin_, HIGH);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(clkPin_, LOW);
      digitalWrite(dioPin_, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(2);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }
    // ACK
    digitalWrite(clkPin_, LOW);
    pinMode(dioPin_, INPUT_PULLUP);
    delayMicroseconds(2);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(dioPin_, HIGH);
  }

  void writeDigits(const uint8_t digits[4]) {
    // Data command
    start();
    writeByte(0x40); // data command
    stop();

    // Segment data
    start();
    writeByte(0xC0); // address auto-increment
    for (int i = 0; i < 4; i++) {
      writeByte(digits[i]);
    }
    stop();

    // Display control command
    start();
    writeByte(0x88 | (brightness_ & 0x07)); // brightness, display on
    stop();
  }
};

// ---------------- Accepted executable model controller ----------------
struct Controller {
  // Model states (must preserve exact semantics)
  int32_t counter = 0;
  bool prevIncPressed = false;
  bool prevDecPressed = false;

  // Control_period_ms = 5. We'll run tick as fast as possible with 5ms pacing.
  void tick(bool io_0, bool io_1) {
    // updates in order
    if (io_0 && (!prevIncPressed)) {
      int32_t newCounter = (counter >= 20) ? 20 : (counter + 1);
      counter = newCounter;
    }

    if (io_1 && (!prevDecPressed)) {
      int32_t newCounter = (counter <= 0) ? 0 : (counter - 1);
      counter = newCounter;
    }

    prevIncPressed = io_0;
    prevDecPressed = io_1;
  }

  int32_t io_2_buzzerHz() const {
    // io_2 = 1000 when counter==20 or counter==0 else 0
    return (counter == 20 || counter == 0) ? 1000 : 0;
  }

  int32_t io_3_displayVal() const {
    return counter;
  }
};

// ---------------- Main sketch ----------------
static Controller ctrl;
static TM1637Number tm(PIN_TM1637_CLK, PIN_TM1637_DIO, 7, false);

#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBuzzer(int32_t hz);
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 198 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_004\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBuzzer(int32_t hz) {
  // Model output io_2: i32 Hz, invalid_policy=silence
  if (hz <= 0) {
    noTone(PIN_BUZZER);
  } else {
    // Use Arduino tone(). This produces a square wave at requested frequency.
    tone(PIN_BUZZER, static_cast<unsigned int>(hz));
  }
}

void setup() {
  pinMode(PIN_D2, INPUT); // catalog defaults assumed; contract notes active_high and pullup false
  pinMode(PIN_D3, INPUT); // active_high, pullup=false

  pinMode(PIN_BUZZER, OUTPUT);

  tm.begin();
  // TM1637 initial display set to 0 to match counter lower bound.
  tm.displayInt(ctrl.io_3_displayVal());
  applyBuzzer(ctrl.io_2_buzzerHz());
}

void loop() {
  static uint32_t lastMs = 0;
  const uint32_t periodMs = 5;

  uint32_t now = millis();
  if ((now - lastMs) < periodMs) return;
  lastMs = now;

  // Sample inputs io_0, io_1 (active_high semantics)
  bool io_0 = (digitalRead(PIN_D2) == HIGH);
  bool io_1 = (digitalRead(PIN_D3) == HIGH);

  // Tick controller with ordered updates
  ctrl.tick(io_0, io_1);

  // Outputs
  applyBuzzer(ctrl.io_2_buzzerHz());
  tm.displayInt(ctrl.io_3_displayVal());
}
