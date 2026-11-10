#include <Arduino.h>

// TM1637 minimal implementation (no external deps)
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin) : clk(clkPin), dio(dioPin) {}

  void begin() {
    pinMode(clk, OUTPUT);
    pinMode(dio, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dio, HIGH);
  }

  void setBrightness(uint8_t brightness) {
    // Brightness: 0..7
    sendByte(0x88 | (brightness & 0x07), 0);
  }

  void displayInt(int value, bool leadingZero = false) {
    // Modes:
    // - blank policy unspecified; we just render as integer with up to 4 digits.
    // - leadingZero false means no extra leading zeros.
    // This sketch follows model: io_2 is either 0, clamp(floor(io_0),0..1023), or 1234.
    // Therefore values are in {0..1023} or 1234.
    int v = value;

    // Convert to four digits.
    uint8_t digits[4] = {0, 0, 0, 0};
    bool neg = false;
    if (v < 0) { neg = true; v = -v; }

    // Clip to 4 digits range for safety
    if (v > 9999) v = v;

    digits[0] = (v / 1000) % 10;
    digits[1] = (v / 100)  % 10;
    digits[2] = (v / 10)   % 10;
    digits[3] = (v)         % 10;

    // If not leadingZero, blank leading zeros (except keep at least one digit visible).
    uint8_t posBlanks = 0;
    if (!leadingZero) {
      for (int i = 0; i < 3; i++) {
        if (digits[i] == 0) posBlanks++;
        else break;
      }
      if (v == 0) posBlanks = 3; // show single '0'
    }

    uint8_t data[4];
    for (int i = 0; i < 4; i++) {
      data[i] = encodeDigit(digits[i]);
      if (!leadingZero) {
        if (posBlanks > 0 && i < posBlanks) data[i] = 0x00; // blank
      }
    }

    // write to display:
    start();
    writeByte(0x40); // address
    for (int i = 0; i < 4; i++) {
      writeByte(data[i]);
    }
    stop();
  }

private:
  uint8_t clk;
  uint8_t dio;

  static uint8_t encodeDigit(uint8_t d) {
    // segments: a b c d e f g (common setting for common modules; this is standard)
    // 0x00 blank handled by caller.
    static const uint8_t seg[10] = {
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
    if (d < 10) return seg[d];
    return 0x00;
  }

  void start() {
    digitalWrite(dio, HIGH);
    digitalWrite(clk, HIGH);
    delayMicroseconds(3);
    digitalWrite(dio, LOW);
    delayMicroseconds(3);
    digitalWrite(clk, LOW);
    delayMicroseconds(3);
  }

  void stop() {
    digitalWrite(clk, LOW);
    digitalWrite(dio, LOW);
    delayMicroseconds(3);
    digitalWrite(clk, HIGH);
    delayMicroseconds(3);
    digitalWrite(dio, HIGH);
    delayMicroseconds(3);
  }

  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(clk, LOW);
      delayMicroseconds(3);
      digitalWrite(dio, (b & 0x01) ? HIGH : LOW);
      delayMicroseconds(3);
      b >>= 1;
      digitalWrite(clk, HIGH);
      delayMicroseconds(3);
    }
    // ACK
    digitalWrite(clk, LOW);
    delayMicroseconds(3);
    pinMode(dio, INPUT);
    digitalWrite(clk, HIGH);
    delayMicroseconds(50);
    pinMode(dio, OUTPUT);
  }

  void sendByte(uint8_t b, uint8_t /*ack*/) {
    start();
    writeByte(b);
    stop();
  }
};

// ===== Accepted executable model (code-gen) =====
static const uint16_t CONTROL_PERIOD_MS = 5;

// Hardware bindings from Frozen interface contract:
// io_0: analog_input f64 adc_count at A0
// io_1: digital_input bool at D2 with pullup=true and model semantics: true=HIGH
// io_2: tm1637_number brightness 7 at clk=D4 dio=D5, leading_zero=false, invalid_policy=blank

// Controller state (preserve exact initial values)
static int32_t mode = 0;                 // i32 initial 0
static bool previousPressed = false;    // bool initial false

// Inputs
static double io_0 = 0.0;  // f64 adc_count
static bool io_1 = true;   // initial true (pullup => idle HIGH)

// Output
static int32_t io_2 = 0;   // i32 initial 0

// Button pin
static const uint8_t PIN_BUTTON = 2; // D2
static const uint8_t PIN_POT = A0;    // A0

// TM1637
static TM1637 tm(4, 5); // D4/D5

static inline double readIo0_f64() {
  // Arduino ADC: 0..1023. Model says clamp 0..1023 then floor(io_0).
  // scale=1 offset=0 -> adc_count directly.
  int raw = analogRead(PIN_POT);
  return (double)raw;
}

static inline bool readIo1_bool() {
  // Model semantics: true_level = HIGH, false_level = LOW
  // hardware binding says pullup=true and device is digital_input at D2.
  // So idle (not pressed) is HIGH, pressed (active low) is LOW.
  // The frozen contract explicitly states: do not invert further; model treats HIGH as true.
  // We therefore read raw pin level directly.
  return digitalRead(PIN_BUTTON) == HIGH;
}

static inline int32_t computeClampAndFloor(double x) {
  // floor(clamp(x, 0..1023)) where clamp uses:
  // if x < 0 => 0
  // else if x > 1023 => 1023
  // else floor(x)
  if (x < 0.0) return 0;
  if (x > 1023.0) return 1023;
  // floor for positive/any real; io_0 is from ADC, but keep exact semantics
  return (int32_t)floor(x);
}

static void controllerTick() {
  // Updates (ordered, exactly as model bundle)
  // Update 1:
  // if (io_1 and not previousPressed) then mode := (mode == 2 ? 0 : mode + 1)
  if (io_1 && (!previousPressed)) {
    if (mode == 2) mode = 0;
    else mode = mode + 1;
  }

  // Update 2:
  // previousPressed := io_1
  previousPressed = io_1;

  // Output expression:
  // io_2 =
  //   if mode==0 then 0
  //   else if mode==1 then (clamp&floor(io_0))
  //   else 1234   (mode==2)
  if (mode == 0) {
    io_2 = 0;
  } else {
    if (mode == 1) {
      io_2 = computeClampAndFloor(io_0);
    } else {
      io_2 = 1234;
    }
  }
}

unsigned long lastTickMs = 0;

void setup() {
  // Button input with pullup (matches frozen contract)
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // ADC
  analogReference(DEFAULT);

  // TM1637 init
  tm.begin();
  tm.setBrightness(7);

  // Ensure initial sampled inputs align with contract initial values
  // (contract sets io_1 initial=true; read pin once to stabilize)
  io_1 = readIo1_bool();
  io_0 = readIo0_f64();

  // Output initial 0 (contract initial i32 0)
  io_2 = 0;
  tm.displayInt(io_2, false);

  lastTickMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastTickMs >= CONTROL_PERIOD_MS) {
    lastTickMs += CONTROL_PERIOD_MS;

    // Sample inputs (at control rate 5ms)
    io_0 = readIo0_f64();
    io_1 = readIo1_bool();

    // Execute controller tick (updates then output)
    controllerTick();

    // Drive output (TM1637 leading_zero=false, invalid_policy=blank)
    tm.displayInt(io_2, false);
  }
}