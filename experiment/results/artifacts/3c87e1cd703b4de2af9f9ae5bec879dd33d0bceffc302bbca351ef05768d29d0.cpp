#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/*
  TM1637 4-digit display (Adafruit-compatible style implementation)
  Pin bindings (frozen contract):
    CLK = D4  (Arduino pin 4)
    DIO = D5  (Arduino pin 5)
*/

static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin)
    : clkPin_(clkPin), dioPin_(dioPin) {}

  void begin() {
    pinMode(clkPin_, OUTPUT);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(clkPin_, HIGH);
    digitalWrite(dioPin_, HIGH);
  }

  void setBrightness(uint8_t brightness) {
    // Contract brightness = 7
    // TM1637 brightness command uses values 0..7 (some libs accept 0..7)
    brightness &= 0x07;
    start();
    writeByte(0x88);      // brightness command
    writeByte(0x80 | brightness);
    stop();
  }

  // Display 4 digits; if leading_zero=false, we still send blanks for leading zeros
  // invalid_policy="blank" => blanking for digits that are not part of representation
  void displayNumber(int32_t num, bool leadingZero, bool invalidBlank) {
    // Represent as unsigned 4-digit value: contract does not specify overflow policy,
    // so we keep exact increment behavior and modulo-4-digit rendering.
    // Most TM1637 sketches render lower 4 digits.
    int32_t val = num;
    bool neg = false;
    if (val < 0) { neg = true; val = -val; }

    uint8_t digits[4] = {0, 0, 0, 0};
    // Extract decimal digits
    for (int i = 3; i >= 0; --i) {
      digits[i] = (uint8_t)(val % 10);
      val /= 10;
    }

    // Leading zero handling: blank leading zeros (except last digit)
    if (!leadingZero) {
      bool allBlank = true;
      for (int i = 0; i < 3; ++i) {
        if (digits[i] != 0) {
          allBlank = false;
        }
        if (allBlank && invalidBlank && digits[i] == 0) {
          digits[i] = 0xFF; // marker for blank
        }
      }
    }

    // Optional minus sign support is not specified; keep behavior conservative:
    // if negative, show '-' at the first position if it exists.
    // This does not contradict contract, which doesn't mention negatives.
    // We'll apply it only if negative and there is room.
    if (neg) {
      // Put minus sign at the first non-blank position (leftmost).
      // If all leading positions blank, place at leftmost.
      bool placed = false;
      for (int i = 0; i < 4; ++i) {
        if (!placed) {
          if (digits[i] == 0xFF) {
            digits[i] = 0xFE; // marker for minus
            placed = true;
          }
        }
      }
      if (!placed) {
        digits[0] = 0xFE;
      }
    }

    // Convert markers to segment codes
    uint8_t seg[4];
    for (int i = 0; i < 4; ++i) {
      seg[i] = digitToSegments(digits[i], invalidBlank);
    }

    // Write to display
    start();
    writeByte(0x40); // data command
    for (uint8_t pos = 0; pos < 4; ++pos) {
      writeByte(seg[pos] & 0xFF);
    }
    stop();

    // Display on
    start();
    writeByte(0x88);
    stop();
  }

private:
  uint8_t clkPin_;
  uint8_t dioPin_;

  // Segment encoding for TM1637 (common for many libs: a b c d e f g dp)
  // 0..9 codes; blank code = 0x00; minus code = 0x40
  uint8_t digitToSegments(uint8_t d, bool invalidBlank) {
    if (d == 0xFF) { // blank marker
      return invalidBlank ? 0x00 : digitToSegments(0, invalidBlank);
    }
    if (d == 0xFE) { // minus marker
      return 0x40;
    }
    static const uint8_t digitMap[10] = {
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
    if (d <= 9) return digitMap[d];
    return invalidBlank ? 0x00 : 0x00;
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
    delayMicroseconds(5);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(clkPin_, LOW);
      if (b & 0x01) digitalWrite(dioPin_, HIGH);
      else digitalWrite(dioPin_, LOW);
      delayMicroseconds(5);
      digitalWrite(clkPin_, HIGH);
      delayMicroseconds(5);
      b >>= 1;
    }

    // Acknowledge
    digitalWrite(clkPin_, LOW);
    pinMode(dioPin_, INPUT);
    delayMicroseconds(5);
    digitalWrite(clkPin_, HIGH);
    delayMicroseconds(5);
    pinMode(dioPin_, OUTPUT);
    digitalWrite(dioPin_, LOW);
  }
};

// --- Generated Accepted executable model implementation ---
// Controller states:
//   accumulatedMs : i32 initial 0
//   displayNumber : i32 initial 0
// Updates (ordered):
//   accumulatedMs = accumulatedMs + io_0
//   if accumulatedMs >= 1000:
//      accumulatedMs = accumulatedMs - 1000
//      displayNumber = displayNumber + 1
// Output:
//   io_1 = displayNumber
static int32_t accumulatedMs = 0;
static int32_t displayNumber = 0;

static TM1637* disp = nullptr;

#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 203 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 192 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_018\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup() {
  disp = new TM1637(TM1637_CLK, TM1637_DIO);
  disp->begin();

  // brightness=7 per contract
  disp->setBrightness(7);

  // initial output io_1 = displayNumber = 0
  disp->displayNumber(displayNumber, /*leading_zero=*/false, /*invalidBlank=*/true);
}

void loop() {
  // control_period_ms = 10
  // Use elapsed time since previous control step as io_0 (delta_since_previous_step).
  static uint32_t lastMs = millis();
  uint32_t now = millis();
  uint32_t elapsed = now - lastMs;
  lastMs = now;

  // io_0 is i32 "delta time since previous control step"
  int32_t io_0 = (int32_t)elapsed;

  // Controller tick with exact update ordering.
  // accumulatedMs := accumulatedMs + io_0
  accumulatedMs = accumulatedMs + io_0;

  // if accumulatedMs >= 1000 then:
  if (accumulatedMs >= 1000) {
    // accumulatedMs := accumulatedMs - 1000
    accumulatedMs = accumulatedMs - 1000;
    // displayNumber := displayNumber + 1
    displayNumber = displayNumber + 1;
  }

  // Output io_1 := displayNumber
  disp->displayNumber(displayNumber, /*leading_zero=*/false, /*invalidBlank=*/true);

  // Keep approximate 10ms control period
  // Do not interfere with elapsed computation (the delta is already measured).
  delay(10);
}
