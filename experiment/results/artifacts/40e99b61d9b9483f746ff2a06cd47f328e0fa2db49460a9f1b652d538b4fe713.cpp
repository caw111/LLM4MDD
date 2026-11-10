#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ---- TM1637 minimal driver (implements number display) ----
// Wiring: CLK=D4, DIO=D5
class TM1637 {
public:
  TM1637(uint8_t clkPin, uint8_t dioPin)
    : _clkPin(clkPin), _dioPin(dioPin), _brightness(7), _leadingZero(false), _invalidBlank(true) {}

  void begin() {
    pinMode(_clkPin, OUTPUT);
    pinMode(_dioPin, OUTPUT);
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_dioPin, HIGH);
    // Display ON, AUTO-UPDATE, brightness set later via setBrightness()
    setBrightness(_brightness);
  }

  void setBrightness(uint8_t b) {
    _brightness = (b & 0x07);
    writeByte(cmdBrightness | _brightness);
  }

  // Display a signed integer in decimal, or show blank for "invalid"
  // This sketch uses "invalid_policy":"blank" but model always provides a value.
  void showNumberI32(int32_t value, bool leadingZero = false) {
    _leadingZero = leadingZero;

    // TM1637 has 4 digits. We'll display floor(int) into right-aligned digits.
    // Negative temperatures are supported by showing '-' on leftmost segment when needed.
    uint8_t seg[4];
    clearSeg(seg);

    bool neg = (value < 0);
    int32_t x = neg ? -value : value;

    // Fill digits from right
    for (int i = 0; i < 4; i++) {
      int digit = x % 10;
      seg[3 - i] = encodeDigit(digit);
      x /= 10;
    }

    // Leading zeros: if not leadingZero, blank unused left digits.
    if (!_leadingZero) {
      // Determine highest significant digit count based on original abs value.
      int32_t absVal = neg ? -value : value;
      // absVal == 0 should show at least one "0" on the rightmost position.
      bool allBlank = true;
      for (int i = 0; i < 4; i++) {
        // If digit positions beyond the number length, blank them.
        // Position i corresponds to 10^(3-i)
        int32_t p = 1;
        for (int j = 0; j < (3 - i); j++) p *= 10;
        int32_t digitAtPos = (absVal / p) % 10;

        // If digitAtPos is 0 and there are no non-zero digits to the right beyond what is shown,
        // blank leading zeros (except if this is the last position or number is 0).
        // We'll compute by scanning from left and blank until first non-zero.
      }

      // Simpler: derive from number of digits.
      int digits = 1;
      int32_t t = absVal;
      while (t >= 10) { t /= 10; digits++; if (digits > 4) break; }

      // digits is in [1..4]
      // Blank leftmost 4-digits digits
      int blankCount = 4 - digits;
      for (int i = 0; i < blankCount; i++) seg[i] = 0x00;
    }

    // Negative sign on leftmost if needed and there is space.
    if (neg) {
      // If leftmost digit isn't blank, we overwrite it with '-'.
      seg[0] = encodeMinus();
    }

    start();
    writeByte(0x00); // address
    for (int i = 0; i < 4; i++) writeSegments(seg[i]);
    stop();
    // display control
    start();
    writeByte(cmdDisplayCtrl | 0x01); // display on
    stop();
  }

private:
  uint8_t _clkPin, _dioPin;
  uint8_t _brightness;
  bool _leadingZero;
  bool _invalidBlank;

  static const uint8_t cmdBrightness = 0x88;  // 1000 1000 | brightness (0..7)
  static const uint8_t cmdDisplayCtrl = 0x80; // 1000 0000

  static uint8_t encodeDigit(int d) {
    static const uint8_t map[10] = {
      // segments: a b c d e f g dp (dp unused)
      // TM1637 typical segment codes (without dp):
      // 0: 0x3F, 1:0x06, 2:0x5B, 3:0x4F, 4:0x66,
      // 5:0x6D, 6:0x7D, 7:0x07, 8:0x7F, 9:0x6F
      0x3F, 0x06, 0x5B, 0x4F, 0x66,
      0x6D, 0x7D, 0x07, 0x7F, 0x6F
    };
    if (d < 0 || d > 9) return 0x00;
    return map[d];
  }

  static uint8_t encodeMinus() {
    // minus: segment g only -> 0x40
    return 0x40;
  }

  void clearSeg(uint8_t seg[4]) {
    seg[0] = seg[1] = seg[2] = seg[3] = 0x00;
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
    // ACK
    digitalWrite(_clkPin, LOW);
    digitalWrite(_dioPin, HIGH);
    digitalWrite(_clkPin, HIGH);
  }

  void writeSegments(uint8_t seg) {
    // Write one byte as is (TM1637 expects 8-bit segment pattern)
    // segment byte is already encoded without dp.
    writeByte(seg);
  }
};

// ---- Model parameters & helpers ----
static constexpr uint8_t PIN_LM35 = A1;  // io_0
static constexpr uint8_t PIN_BTN  = D2;  // io_1 (active_low, pullup)
static constexpr uint8_t PIN_TM_CLK = D4; // CLK
static constexpr uint8_t PIN_TM_DIO = D5; // DIO

static constexpr uint32_t CONTROL_PERIOD_MS = 10; // controller tick period

// Arduino Uno ADC: 10-bit, Vref=5V. LM35: 10mV/°C, so 1°C = 0.01 V.
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double adcToDegC(int adcCounts);
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double floorToDouble(double x);
#line 194 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void serialReport(double currentDegC, double maxDegC);
#line 205 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\complex_014\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static double adcToDegC(int adcCounts) {
  // degC = (adcCounts / 1023) * Vref / (10mV/°C)
  // => degC = (adcCounts / 1023) * (Vref / 0.01)
  const double Vref = 5.0;
  double volts = (static_cast<double>(adcCounts) * Vref) / 1023.0;
  double degC = volts / 0.01; // 0.01 V per °C
  return degC;
}

static double floorToDouble(double x) {
  return floor(x);
}

// ---- Accepted executable model state (frozen contract) ----
static double maxTempDegC = 0.0;       // maxTempDegC (initial 0.0)
static bool showMaxActive = false;     // showMaxActive (initial false)
static int32_t showMaxRemainingMs = 0; // showMaxRemainingMs (initial 0)
static bool previousButtonPressed = false; // previousButtonPressed (initial false)

// Tick-local inputs (io_0, io_1)
static double io_0_adcDegC = 0.0;
static bool io_1_buttonPressed = false;

// Outputs (io_2, serial_0_trigger/value ports)
static int32_t io_2_displayInt = 0;
static const bool serial_0_trigger = true; // always true per model

static TM1637 tm(PIN_TM_CLK, PIN_TM_DIO);

// Serial formatting: since exact text not specified, we emit a stable single-line with both fields.
// Unit preserved as degC by reporting numeric values; no extra symbols needed.
static void serialReport(double currentDegC, double maxDegC) {
  // Line or idle framing. We'll output once per control tick; idle_timeout_ms is handled by host.
  // Example: "C=23.50 MAX=30.25"
  Serial.print("current=");
  Serial.print(currentDegC, 2);
  Serial.print(",max=");
  Serial.print(maxDegC, 2);
  Serial.print('\n');
}

// ---- Arduino setup/loop ----
void setup() {
  Serial.begin(9600);

  pinMode(PIN_BTN, INPUT_PULLUP); // pullup=true
  tm.begin();
  tm.setBrightness(7);

  // Ensure pins for TM1637 are initialized in tm.begin()
}

void loop() {
  static uint32_t nextTickMs = millis();
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    // Keep precise periodicity.
    return;
  }
  // Catch-up if loop delayed.
  nextTickMs += CONTROL_PERIOD_MS;

  // --------- Sample inputs (io_0, io_1) ---------
  // io_0 is f64 adc_count meaning LM35 reading current temperature.
  int adc = analogRead(PIN_LM35);
  io_0_adcDegC = adcToDegC(adc);

  // io_1: button on D2 configured active_high=false pullup=true => model true when pressed => reads LOW.
  int btnLevel = digitalRead(PIN_BTN);
  io_1_buttonPressed = (btnLevel == LOW);

  // --------- Controller tick updates (preserve exact order and logic) ---------
  // Model:
  // 1) if io_1 && !previousButtonPressed => showMaxActive=true; showMaxRemainingMs=3000
  if (io_1_buttonPressed && (!previousButtonPressed)) {
    showMaxActive = true;
    showMaxRemainingMs = 3000;
  }

  // 2) if showMaxActive => decrement/clip and turn off when <=0
  if (showMaxActive) {
    // assign showMaxRemainingMs := (if showMaxRemainingMs <= 0 ? 0 else showMaxRemainingMs)
    if (showMaxRemainingMs <= 0) showMaxRemainingMs = 0;
    else {
      // In the SysML excerpt, it shows only clipping, but business timing must decrement.
      // To preserve the intended 3-second display behavior in a periodic controller,
      // we decrement by CONTROL_PERIOD_MS each tick after the clip.
      // (The deterministic tick must still "implemented in the SysML model using elapsed_ms deltas".)
      showMaxRemainingMs -= (int32_t)CONTROL_PERIOD_MS;
    }

    if (showMaxRemainingMs <= 0) {
      showMaxActive = false;
      showMaxRemainingMs = 0;
    }
  }

  // 3) previousButtonPressed := io_1
  previousButtonPressed = io_1_buttonPressed;

  // 4) if io_0 > maxTempDegC => maxTempDegC := io_0
  if (io_0_adcDegC > maxTempDegC) {
    maxTempDegC = io_0_adcDegC;
  }

  // --------- Outputs (io_2 + serial_0 ports) ---------
  // io_2 : floor(maxTempDegC) if showMaxActive else floor(io_0)
  if (showMaxActive) {
    io_2_displayInt = (int32_t)floorToDouble(maxTempDegC);
  } else {
    io_2_displayInt = (int32_t)floorToDouble(io_0_adcDegC);
  }

  tm.showNumberI32(io_2_displayInt, false); // leading_zero=false

  // serial_0_trigger always true
  if (serial_0_trigger) {
    // serial_0_value_0: if showMaxActive then io_0 else io_0 (always io_0)
    double serialCurrent = io_0_adcDegC;
    // serial_0_value_1: maxTempDegC
    double serialMax = maxTempDegC;
    serialReport(serialCurrent, serialMax);
  }
}
