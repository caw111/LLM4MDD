#include <Arduino.h>

static constexpr uint8_t PIN_POT1 = A3; // io_0
static constexpr uint8_t PIN_POT2 = A4; // io_1
static constexpr uint8_t PIN_POT3 = A5; // io_2

// io_3_segment_0..9 mapped to D2..D11 in segment order (A1..A10)
static constexpr uint8_t SEG_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static constexpr uint32_t CONTROL_PERIOD_MS = 5;

static uint32_t nextUpdateMs = 0;

static uint16_t readAdcAvg5(uint8_t pin) {
  uint32_t sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    delayMicroseconds(200);
  }
  return (uint16_t)(sum / 5);
}

// Map ADC count to integer 0..9 (inclusive) without oversimplifying:
// clamp to [0, 1023] then scale with rounding to nearest integer.
static int mapAdcTo0_9(uint16_t adc) {
  if (adc > 1023) adc = 1023;
  // rounded scaling: (adc*9 + 511)/1023
  return (int)((adc * 9UL + 511UL) / 1023UL);
}

// Map ADC count to integer 0..100 (inclusive) with rounding and clamping.
static int mapAdcTo0_100(uint16_t adc) {
  if (adc > 1023) adc = 1023;
  // rounded scaling: (adc*100 + 511)/1023
  return (int)((adc * 100UL + 511UL) / 1023UL);
}

static void setAllSegments(bool v) {
  for (int i = 0; i < 10; i++) digitalWrite(SEG_PINS[i], v ? HIGH : LOW);
}

void setup() {
  // Inputs (potentiometers)
  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_POT3, INPUT);

  // Outputs (bargraph segments)
  for (int i = 0; i < 10; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
    digitalWrite(SEG_PINS[i], LOW); // initialization: all LEDs off
  }

  nextUpdateMs = millis();
}

void loop() {
  // Maintain 5ms control period
  uint32_t now = millis();
  if ((int32_t)(now - nextUpdateMs) < 0) return;
  nextUpdateMs = now + CONTROL_PERIOD_MS;

  // Read potentiometers (sample_period_ms = 5 is effectively satisfied by control period)
  uint16_t adc0 = readAdcAvg5(PIN_POT1); // pot1 lower bound
  uint16_t adc1 = readAdcAvg5(PIN_POT2); // pot2 upper bound
  uint16_t adc2 = readAdcAvg5(PIN_POT3); // pot3 fill percentage

  int lower = mapAdcTo0_9(adc0);
  int upper = mapAdcTo0_9(adc1);
  int fillPct = mapAdcTo0_100(adc2); // 0..100

  // Determine number of lit LEDs inclusively within the active range.
  // Requirement: "based on fill percentage" and direction handling.
  // Active length inclusive = |upper-lower| + 1 LEDs.
  int len = (lower <= upper) ? (upper - lower + 1) : (lower - upper + 1);

  // Fill percentage dictates how many LEDs between bounds are lit.
  // Use rounding to nearest integer of len*fillPct/100.
  // Clamp to [0, len].
  int litCount = (int)((len * fillPct + 50) / 100); // +50 for nearest
  if (litCount < 0) litCount = 0;
  if (litCount > len) litCount = len;

  // Direction handling: if lower <= upper, light from lower->upper inclusive.
  // else light from upper->lower inclusive.
  bool lit[10] = {false, false, false, false, false, false, false, false, false, false};

  if (litCount > 0) {
    int start = (lower <= upper) ? lower : upper;
    int end   = (lower <= upper) ? upper : lower;

    // Lit segments are sequential from start to start+litCount-1 within [start..end] inclusive.
    // start and end are in 0..9 corresponding to segment index (A1..A10).
    for (int i = 0; i < litCount; i++) {
      int pos = start + i; // sequential in that direction (start <= end when ordered this way)
      if (pos >= 0 && pos <= 9) lit[pos] = true;
      // pos is guaranteed within active segment range because litCount <= len.
      (void)end;
    }
  }

  // Update outputs (exact ordered mapping and boolean levels)
  for (int seg = 0; seg < 10; seg++) {
    digitalWrite(SEG_PINS[seg], lit[seg] ? HIGH : LOW);
  }
}