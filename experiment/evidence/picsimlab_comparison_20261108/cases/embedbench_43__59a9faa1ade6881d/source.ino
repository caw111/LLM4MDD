#include <Arduino.h>

// --------- Hardware contract (frozen wiring) ---------
// 74HC595 (sr1):
//   DS   = A0
//   SHCP = A2 (clock)
//   STCP = A1 (latch)
//   OE   tied to GND.3 (active enabled when OE low)
//   MR   tied high via sr1:MR->VCC (reset inactive)
// 7-seg COM.1 tied to +5V (always on common)
// 595 outputs Q0..Q6 -> sevseg segments A..G respectively
// Active_low per contract: active_low=true with mapping for digit register.

static constexpr uint8_t SR_DATA_PIN  = A0; // DS
static constexpr uint8_t SR_CLK_PIN   = A2; // SHCP
static constexpr uint8_t SR_LATCH_PIN = A1; // STCP

// Bargraph1 segments A1..A10 -> D2..D11, active_high=true
static constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// Potentiometers:
// pot1: A3
// pot2: A4
// pot3: A5
static constexpr uint8_t POT1_PIN = A3;
static constexpr uint8_t POT2_PIN = A4;
static constexpr uint8_t POT3_PIN = A5;

// --------- 74HC595 segment pattern (active_low) ---------
// Contract says: active_low=true and segment_bits [0..6] correspond to segments [A..G].
// "digit" -> "sevseg1 single digit (0-9)". We'll use standard 7-seg (A..G) where
// 0 lights A,B,C,D,E,F and not G, etc. Active_low: 0 bits mean ON.
static inline uint8_t segPatternActiveLow(uint8_t digit) {
  // Bit positions: 0=A,1=B,2=C,3=D,4=E,5=F,6=G
  // 1=OFF, 0=ON (active_low)
  switch (digit) {
    case 0: return 0b0000001; // A B C D E F on, G off
    case 1: return 0b1001111; // B C on
    case 2: return 0b0010010; // A B D E G on
    case 3: return 0b0000110; // A B C D G on
    case 4: return 0b1001100; // B C F G on
    case 5: return 0b0100100; // A C D F G on
    case 6: return 0b0100000; // A C D E F G on
    case 7: return 0b0001111; // A B C on
    case 8: return 0b0000000; // all on
    case 9: return 0b0000100; // A B C D F G on
    default: return 0b1111111; // blank
  }
}

static inline void shift595Write7SegActiveLow(uint8_t digit) {
  const uint8_t seg = segPatternActiveLow(digit); // bits 0..6 correspond A..G (active_low)
  // Q0->A, Q1->B, ... Q6->G. Remaining Q7 unused; keep it OFF (HIGH for active_low).
  // Compose 8-bit value where bit0=Q0.
  uint8_t out = (uint8_t)(seg & 0x7F); // Q0..Q6
  // out bit7(Q7) set to 1 (OFF) to be safe:
  out |= 0x80;

  // Shift out MSB first is typical; but hardware contract for "registers_nearest_mcu_first"
  // doesn't give bit order explicitly. For deterministic behavior, we will shift out MSB..LSB
  // so that bit positions map to Q7..Q0 in the order sent by common wiring.
  // The contract states "segment_bits [0..6]" and Q0..Q6 to segments A..G, which aligns with:
  // LSB-most mapping if we shift MSB first. We'll implement MSB-first for standard 74HC595.
  digitalWrite(SR_LATCH_PIN, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_CLK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (out & (1U << i)) ? HIGH : LOW);
    digitalWrite(SR_CLK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
  digitalWrite(SR_LATCH_PIN, LOW);
}

// --------- Model state (ACCEPTED executable model) ---------
// Initial effective requirements conflict with model;
// we must implement the ACCEPTED executable model exactly.
static int32_t displayDigit = 0;     // i32, initial 0
static int32_t digitTickPhase = 0;   // i32, initial 0

// Timing note: frozen model bundle has no digit-change period;
// it only uses the tick action (immediately applied during loop iterations).
// We keep the controller semantics by performing the tick once per main loop cycle,
// but also preserve the required 10ms control_period_ms for sampling consistency.
static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static uint32_t lastControlMs = 0;

// --------- LED bargraph brightness model ---------
// The frozen interface contract includes outputs io_1_segment_0..io_1_segment_9 as bool,
// but does not include PWM. However, requirement prose says pot1 controls brightness
// via time modulation of segment GPIO outputs in SysML.
// We'll implement a 0..255 brightness time modulation across each 10ms control period
// while keeping segment on/off selection exactly: segment i on iff displayDigit >= (i+1),
// and segment off otherwise.
static inline uint8_t potToBrightness255(int raw) {
  // raw expected 0..1023 -> 0..255
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  return (uint8_t)((raw * 255L) / 1023L);
}

// Map pot2 and pot3 to 1s..2s as stated; though the ACCEPTED model doesn't use them,
// we still read them to maintain "state maintenance" and continuous update.
static inline uint32_t potToSeconds1to2(int raw) {
  // 1.0..2.0 seconds; return in microseconds for finer control if needed.
  if (raw < 0) raw = 0;
  if (raw > 1023) raw = 1023;
  // seconds in 1..2 inclusive
  // use 32-bit ms for potential later
  uint32_t ms = 1000UL + (uint32_t)(((uint32_t)raw * 1000UL) / 1023UL);
  return ms;
}

void setup() {
  // SR pins
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  // Bargraph pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Pots as analog inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  lastControlMs = millis();

  // Initial outputs per model initial state (displayDigit=0):
  shift595Write7SegActiveLow((uint8_t)displayDigit);
  // bargraph: digit=0 => 0 segments lit => all off
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
}

void loop() {
  const uint32_t now = millis();
  if (now - lastControlMs < CONTROL_PERIOD_MS) return;
  lastControlMs = now;

  // --------- Sample inputs (io_2, io_3, io_4) ---------
  // ACCEPTED model uses io_3 as the "input Real" with range check [1.0..1023.0]
  // and reads io_3. The interface registry names:
  //  io_2: pot1  (brightness)
  //  io_3: pot2  (speed/base speed)
  //  io_4: pot3  (additional speed)
  // BUT sysml2 model says it compares io_3 in [1..1023]. That's pot2.
  const int rawPot1 = analogRead(POT1_PIN);
  const int rawPot2 = analogRead(POT2_PIN);
  const int rawPot3 = analogRead(POT3_PIN);

  const float io_2 = (float)rawPot1; // adc_count
  const float io_3 = (float)rawPot2; // adc_count
  const float io_4 = (float)rawPot3; // adc_count (unused by ACCEPTED model logic)

  // --------- Execute ACCEPTED executable model tick action ---------
  // perform action tick:
  //   digitTickPhase := 0
  //   displayDigit := (if (io_3 between 1.0 and 1023.0)
  //                      then ((if (digitTickPhase >= 1) then 1 else 0) + displayDigit)
  //                      else displayDigit)
  //   displayDigit := (if displayDigit >= 10 then displayDigit - 10 else displayDigit)
  digitTickPhase = 0;

  if ((io_3 >= 1.0f) && (io_3 <= 1023.0f)) {
    if (digitTickPhase >= 1) {
      displayDigit = displayDigit + 1;
    } else {
      // digitTickPhase is 0 so +0
      displayDigit = displayDigit;
    }
  } else {
    displayDigit = displayDigit;
  }

  if (displayDigit >= 10) {
    displayDigit = displayDigit - 10;
  }

  // Note: With digitTickPhase forced to 0 each tick, displayDigit never changes.
  // Still, we must apply ordered updates exactly as above, hence implemented.

  // --------- Outputs per ACCEPTED model ---------
  // sevseg display via shift595_number:
  shift595Write7SegActiveLow((uint8_t)displayDigit);

  // bargraph segments:
  // output bool io_1_segment_i:
  //   segment_0..segment_8 => displayDigit >= (i+1) (i32 comparisons)
  //   segment_9 => always false
  //
  // brightness: implement pot1-driven time modulation across each 10ms period.
  const uint8_t brightness = potToBrightness255((int)io_2); // 0..255

  // Time modulation strategy:
  // - We'll do a sub-cycle based on remaining time in the 10ms window using micros().
  // - Set segment pins HIGH if "should be lit" AND (micros_in_window < brightness*window/255).
  const uint32_t windowStartUs = micros();
  const uint32_t windowUs = CONTROL_PERIOD_MS * 1000UL;

  // "Continuously update": within the control period, we at least refresh once
  // and time-modulate within it. Keep it simple/deterministic: compute once at end of window.
  // Since we need time modulation, do a short loop with a few steps to approximate PWM.
  // Deterministic: use fixed number of steps.
  const uint8_t litCountMax = 9; // segment_9 always false
  (void)litCountMax;

  // Determine target on/off for each segment (exact selection):
  bool segOn[10];
  for (uint8_t i = 0; i < 9; i++) {
    segOn[i] = (displayDigit >= (int32_t)(i + 1)) ? true : false; // segment_i lights for i+1
  }
  segOn[9] = false;

  // Fixed-step PWM within the control period.
  const uint8_t STEPS = 16; // 16 slices over 10ms
  for (uint8_t s = 0; s < STEPS; s++) {
    // compute time position within window
    uint32_t tUs = (windowUs * (uint32_t)s) / (uint32_t)STEPS;
    bool pwmOn = (tUs < ((uint32_t)brightness * windowUs) / 255UL);

    for (uint8_t i = 0; i < 10; i++) {
      // Output expression requires bool, but brightness modulates them: segment HIGH only when both conditions true.
      digitalWrite(BAR_PINS[i], (segOn[i] && pwmOn) ? HIGH : LOW);
    }
    // small delay to keep modulation across the window
    // (bounded, deterministic using micros)
    uint32_t targetUs = windowStartUs + ((windowUs * (uint32_t)(s + 1)) / (uint32_t)STEPS);
    while ((int32_t)(micros() - targetUs) < 0) {
      // busy wait
    }
  }

  // Keep io_1_segment outputs already set; next control period will update again.
  // io_0 already set through shift register.
  (void)io_4; // intentionally unused by accepted model tick semantics
}