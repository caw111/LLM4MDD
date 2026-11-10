#include <Arduino.h>

static inline int tm1637DigitsToValue(uint8_t d) { return (int)d; }

// Minimal TM1637 driver (4-digit display capable; we use only the last digit for 0..15)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : _clkPin(clkPin), _dioPin(dioPin) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
  }

  // brightness: 0..7
  void setBrightness(uint8_t brightness) {
    // Command: 0x88 | brightness
    start();
    writeByte(0x88 | (brightness & 0x07));
    stop();
  }

  // Display value on rightmost digit, optional leading zeros control
  // We blank invalid values (policy invalid_policy="blank") but for 0..15 it's always valid.
  void displayOneDigit(int value, bool leadingZero) {
    (void)leadingZero;
    uint8_t seg = encode(value);
    // We'll display as 4-digit with only last digit set.
    uint8_t data[4] = {0, 0, 0, seg};
    writeData(data);
  }

private:
  uint8_t _clkPin, _dioPin;

  // Segment encoding for common cathode
  // bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
  uint8_t encode(int v) const {
    if (v < 0 || v > 15) return 0x00; // blank
    static const uint8_t table[16] = {
      // 0..9, A..F
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F, // 9
      0x77, // A
      0x7C, // b
      0x39, // C
      0x5E, // d
      0x79, // E
      0x71  // F
    };
    return table[v];
  }

  void start() {
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, LOW);
  }

  void stop() {
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(_dioPin, HIGH);
    delayMicroseconds(2);
  }

  void writeByte(uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(_clkPin, LOW);
      digitalWrite(_dioPin, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(2);
      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(2);
      b >>= 1;
    }
    // Acknowledge bit (ignored)
    digitalWrite(_clkPin, LOW);
    pinMode(_dioPin, INPUT);
    digitalWrite(_clkPin, HIGH);
    delayMicroseconds(2);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, LOW);
  }

  void writeData(uint8_t data[4]) {
    // Command 0x40 for fixed address (data write)
    start();
    writeByte(0x40);
    // Data: 4 bytes starting from digit0
    for (int i = 0; i < 4; i++) writeByte(data[i]);
    stop();
  }
};

// -------- Hardware bindings (frozen contract) --------
// Inputs
static constexpr uint8_t PIN_D2 = 2;  // io_0
static constexpr uint8_t PIN_D3 = 3;  // io_1
// Outputs
static constexpr uint8_t PIN_LED = 13; // io_2
static constexpr uint8_t PIN_BUZZ = 6; // io_3 tone_output
// TM1637 pins
static constexpr uint8_t PIN_TM1637_CLK = 4; // D4
static constexpr uint8_t PIN_TM1637_DIO = 5; // D5

static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// -------- Accepted executable model state --------
struct Controller {
  int32_t count = 0;         // initial 0
  bool alarmLocked = false;  // initial false
  bool prevD2Pressed = false;
  bool prevD3Pressed = false;

  // tick: preserves exact ordered updates per model
  void tick(bool io_0, bool io_1) {
    // order: update alarm reset when D3 rising
    if (io_1 && (!prevD3Pressed)) {
      alarmLocked = false;
      count = 0;
    }

    // if not alarmLocked, handle D2 rising for increment with wrap 15->0
    if (!alarmLocked) {
      if (io_0 && (!prevD2Pressed)) {
        if (count == 15) count = 0;
        else count = count + 1;
      }
    }

    // if count==15 and not alarmLocked, lock alarm
    if ((count == 15) && (!alarmLocked)) {
      alarmLocked = true;
    }

    // D3 rising: decrement only if not alarmLocked
    if (io_1 && (!prevD3Pressed)) {
      if (!alarmLocked) {
        if (count == 0) count = 15;
        else count = count - 1;
      }
    }

    // update prev flags at end
    prevD2Pressed = io_0;
    prevD3Pressed = io_1;
  }

  // outputs per model
  bool io_2_ledEven() const {
    // io_2 = (count == (2 * floor(count/2)))
    // for integer count, this is true iff count even.
    int32_t half = (int32_t)floor((double)count / 2.0);
    int32_t evenPart = 2 * half;
    return (count == evenPart);
  }

  int32_t io_3_buzzerHz() const {
    return alarmLocked ? 440 : 0;
  }

  int32_t io_4_displayCount() const {
    return count;
  }
};

// -------- Sound generation --------
// Use tone() / noTone() for simple mapping of frequency and ensure updates follow control steps.
// tone_output: invalid_policy="silence"
static void applyBuzzer(int32_t hz) {
  if (hz <= 0) {
    noTone(PIN_BUZZ);
  } else {
    tone(PIN_BUZZ, (unsigned int)hz);
  }
}

Controller controller;
TM1637 tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

void setup() {
  pinMode(PIN_D2, INPUT); // contract: no explicit pullup override
  pinMode(PIN_D3, INPUT);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZ, OUTPUT);

  tm1637.begin();
  tm1637.setBrightness(7); // brightness=7
  // Initial outputs: model outputs immediately from initial state (count=0, alarmLocked=false)
  digitalWrite(PIN_LED, controller.io_2_ledEven() ? HIGH : LOW);
  applyBuzzer(controller.io_3_buzzerHz());
  tm1637.displayOneDigit(controller.io_4_displayCount(), false);
}

void loop() {
  static uint32_t lastStepMs = millis();

  uint32_t now = millis();
  // Keep control period = 5ms. If late, we still do single tick (no catch-up), matching typical periodic controller.
  if ((now - lastStepMs) >= CONTROL_PERIOD_MS) {
    lastStepMs = now;

    // io_5 elapsed_ms (delta since previous control step) is not used in the model outputs/updates.
    // Read inputs with model_semantics: true=HIGH, false=LOW.
    bool io_0 = (digitalRead(PIN_D2) == HIGH);
    bool io_1 = (digitalRead(PIN_D3) == HIGH);

    controller.tick(io_0, io_1);

    // Apply outputs in the same control step (deterministic synchronous update model)
    digitalWrite(PIN_LED, controller.io_2_ledEven() ? HIGH : LOW);

    applyBuzzer(controller.io_3_buzzerHz());

    int32_t disp = controller.io_4_displayCount();
    // invalid_policy="blank" for out-of-range; display 0..15 only.
    if (disp >= 0 && disp <= 15) tm1637.displayOneDigit((int)disp, false);
    else tm1637.displayOneDigit(-1, false);
  }
}