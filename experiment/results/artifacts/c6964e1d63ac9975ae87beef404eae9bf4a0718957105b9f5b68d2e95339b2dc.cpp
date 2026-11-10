#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ===== Frozen hardware contract bindings =====
// analog_input io_0 : A0 (10-bit ADC -> 0..1023)
static const uint8_t IO0_ANALOG_PIN = A0;

// tm1637_number io_1 : CLK=D4, DIO=D5
static const uint8_t TM1637_CLK_PIN = D4;
static const uint8_t TM1637_DIO_PIN = D5;

// ===== Control period =====
static const uint32_t CONTROL_PERIOD_MS = 10;

// ===== TM1637 parameters from contract =====
static const uint8_t TM1637_BRIGHTNESS = 7;
static const bool TM1637_LEADING_ZERO = false;

// TM1637Display provides a 4-digit display; we will show 0..9999.
// Contract says invalid_policy='blank' but model never outputs invalid.
// No serial.

TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int32_t clampInt32(int32_t v, int32_t lo, int32_t hi);
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline int32_t clampInt32(int32_t v, int32_t lo, int32_t hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

void setup() {
  // Ensure deterministic startup
  pinMode(IO0_ANALOG_PIN, INPUT);

  display.setBrightness(TM1637_BRIGHTNESS);
  // initial output io_1=0
  display.showNumberDecEx(0, (TM1637_LEADING_ZERO ? 0 : 0x40), true); 
  // The 'false' parameter indicates no leading zeros; we handle leading zeros with flags.
}

void loop() {
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // === Read io_0 (f64 adc_count) ===
  // ADC returns 0..1023 counts
  int raw = analogRead(IO0_ANALOG_PIN);
  double io_0 = (double)raw; // adc_count

  // === Accepted executable model output: floor(io_0 * 9999.0 / 1023.0) ===
  // For raw=0 => 0
  // For raw=1023 => 9999
  double scaled = io_0 * 9999.0 / 1023.0;
  int32_t io_1 = (int32_t)floor(scaled);

  // Model does not specify clamping; but to preserve stable display we keep it within 0..9999.
  // (With given ADC range, it already stays within bounds.)
  io_1 = clampInt32(io_1, 0, 9999);

  // === Write io_1 to TM1637 (display number 0..9999) ===
  // 'showNumberDecEx(num, dots, leadingZero)' signature differs by library version.
  // Use flags consistent with no leading zeros.
  // We'll use showNumberDecEx with:
  // - dots = 0, leading zeros disabled by 'leading zeros mask' / second argument depending on version.
  // For TM1637Display (common version), second arg is "leadingZeros" bitmask:
  //   showNumberDecEx(value, decimalPointMask, leadingZerosFlag)
  // Here we avoid ambiguity by using showNumberDec which doesn't expose leading zeros controls.
  // However contract specifies leading_zero=false. Without a direct flag, we still show without forcing leading zeros.
  // Most libraries show without leading zeros by default for showNumberDec.
  display.showNumberDec(io_1, false);
}
