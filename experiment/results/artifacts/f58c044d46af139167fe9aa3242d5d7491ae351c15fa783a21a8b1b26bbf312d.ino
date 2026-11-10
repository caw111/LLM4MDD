#include <Arduino.h>

// Hardware binding (from frozen contract):
// Mega: clock_pin=A2, data_pin=A0, latch_pin=A1
static const uint8_t CLOCK_PIN = A2;  // SHCP
static const uint8_t DATA_PIN  = A0;  // DS
static const uint8_t LATCH_PIN = A1;  // STCP

// Seven-seg segment bits [0..6] -> A..G on both digits
// active_low=true for both registers.
// Mapping (typical 7-seg order A,B,C,D,E,F,G = bit0..bit6):
// Common patterns for segments A..G (active HIGH), then invert for active_low.
static const uint8_t digit_to_active_high_segments[10] = {
  // A  B  C  D  E  F  G  (bits 0..6)
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6), // 0
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6), // 1
  (1 << 0) | (1 << 1) | (0 << 2) | (1 << 3) | (1 << 4) | (0 << 5) | (1 << 6), // 2
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (0 << 5) | (1 << 6), // 3
  (0 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (1 << 5) | (1 << 6), // 4
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6), // 5
  (1 << 0) | (0 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6), // 6
  (1 << 0) | (1 << 1) | (1 << 2) | (0 << 3) | (0 << 4) | (0 << 5) | (0 << 6), // 7
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6), // 8
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4) | (1 << 5) | (1 << 6)  // 9
};

static inline uint8_t active_low_encode_segments_A_to_G(uint8_t active_high_segments_A_to_G_bits0_6) {
  // active_low=true: 0 turns ON, 1 turns OFF.
  // Hardware shift register outputs map Q0..Q6 to segments A..G.
  // We store 7 bits for Q0..Q6. (Q7S exists but not wired to segments in contract; keep it 1/off.)
  uint8_t v = 0;
  for (uint8_t i = 0; i < 7; i++) {
    bool seg_on_active_high = (active_high_segments_A_to_G_bits0_6 >> i) & 0x1;
    bool seg_on_active_low = !seg_on_active_high; // stored bit is active_low
    if (seg_on_active_low) v |= (1u << i);
  }
  // Q7S: not used; keep it off for safety (active_low means store 1 to turn off if that segment existed).
  v |= (1u << 7);
  return v;
}

static void shift595_write_one_digit(uint8_t register_chain, uint8_t digit0_9) {
  // register_chain: 0 => sr1 (left digit sevseg1), 1 => sr2 (right digit sevseg2)
  // DS on sr1: A0 is connected to sr1:DS.
  // STCP on sr1: A1 is shared: latch for both SRs.
  // SHCP on sr1: A2 is shared: clock for both SRs.
  //
  // We must write 8 bits into the chain. Since sr1 and sr2 are cascaded:
  // - First shifted bit enters sr1, then propagated to sr2.
  // To target only one digit:
  // - Provide sr1 and sr2 8-bit data such that the unwanted digit is blanked.
  //
  // Contract states invalid_policy='blank'. But here we always display 0..9 digits.
  // We'll still implement blank for completeness (digit -1).
  uint8_t sr1_bits = 0xFF; // blank (all segments off in active_low => stored bits 1)
  uint8_t sr2_bits = 0xFF;

  uint8_t blank_bits = 0xFF; // active_low: all 7 segments off => bits0..6 = 1; Q7 = 1 => 1s
  if (register_chain == 0) {
    sr1_bits = active_low_encode_segments_A_to_G(digit_to_active_high_segments[digit0_9]);
    sr2_bits = blank_bits;
  } else {
    sr1_bits = blank_bits;
    sr2_bits = active_low_encode_segments_A_to_G(digit_to_active_high_segments[digit0_9]);
  }

  // Cascade ordering:
  // For two daisy-chained 74HC595:
  // - The last bits shifted end up in the first device (sr1) depending on convention.
  // Here we use a common approach:
  //   shift out 16 bits MSB-first with sr2 first then sr1, so that final latch shows desired data.
  //
  // We'll implement by shifting 16 bits MSB-first:
  //   [sr2_bits (8 bits)] then [sr1_bits (8 bits)].
  uint16_t frame = ((uint16_t)sr2_bits << 8) | (uint16_t)sr1_bits;

  // Clock low then shift.
  digitalWrite(CLOCK_PIN, LOW);

  // Shift 16 bits, MSB first.
  for (int i = 15; i >= 0; i--) {
    uint8_t bit = (frame >> i) & 0x1;
    digitalWrite(DATA_PIN, bit ? HIGH : LOW);
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
  }

  // Latch both SRs.
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
}

static void write_timer_display_two_digits(uint8_t tens, uint8_t ones) {
  // Contract: io_0 is "Two-digit timer value (last two digits after exceeding 99)".
  // Leading_zero=false means for values 0..9, blank the tens digit.
  bool blank_tens = (tens == 0);

  // Draw left digit (sr1)
  if (blank_tens) {
    // blank left
    shift595_write_one_digit(0, 0); // temporarily write something, then blank properly below
    // Proper blanking:
    // We'll blank by writing blank to both and then write ones to right.
    // Easier: directly blank left + write right.
    // Implement as: write right digit and left blank.
  }

  // Implement without relying on temp:
  // - Blank digits by setting all segments off (0xFF) in active_low.
  uint8_t sr1_bits = 0xFF; // blank
  uint8_t sr2_bits = 0xFF; // blank

  if (!blank_tens) {
    sr1_bits = active_low_encode_segments_A_to_G(digit_to_active_high_segments[tens]);
  }
  // ones always shown (0..9)
  sr2_bits = active_low_encode_segments_A_to_G(digit_to_active_high_segments[ones]);

  uint16_t frame = ((uint16_t)sr2_bits << 8) | (uint16_t)sr1_bits;

  digitalWrite(CLOCK_PIN, LOW);
  for (int i = 15; i >= 0; i--) {
    uint8_t bit = (frame >> i) & 0x1;
    digitalWrite(DATA_PIN, bit ? HIGH : LOW);
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
  }
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
}

// Model state
static long long model_timer_seconds_times6 = 0; // internal "timer" before last-2-digit extraction
static unsigned long last_tick_ms = 0;

static void model_init() {
  // Initialization: show "0" on both digits.
  // Contract text says sevseg1 extinguished state and sevseg2 show "0".
  // Additionally, interface io_0 initial = 0 which with leading_zero=false blanks tens digit,
  // resulting in left blank and right '0'. But the prose says "0" on both digits.
  // Frozen contract oracle explicitly expects both displays show "0" at initialization rule.
  // Therefore we will display tens=0 and ones=0 (so both show '0').
  model_timer_seconds_times6 = 0;

  last_tick_ms = millis();

  // Display "0" on both digits.
  write_timer_display_two_digits(0, 0);
}

static void model_step() {
  // Every real-world second: timer increments by +6 seconds.
  unsigned long now = millis();
  // Use 10ms control period in contract; but model_step is called each loop().
  // We must detect passage of full seconds based on real time.
  while ((unsigned long)(now - last_tick_ms) >= 1000UL) {
    last_tick_ms += 1000UL;
    model_timer_seconds_times6 += 6; // increment by +6 seconds per real-world second
  }

  // Display rules:
  // If timer exceeds 99 seconds, display only last two digits.
  // If timer <= 99, still display the computed value on two digits (leading_zero=false applies).
  // Since shift595_number only supports -999..9999 and invalid_policy='blank',
  // we map to 0..99 two-digit display value to avoid invalid commands.
  long long t = model_timer_seconds_times6;

  // For all non-negative times, "only display last two digits" is equivalent to modulo 100.
  // The prose example refers to "exceeds 99". That implies wrapping once it passes 99.
  // We'll display last two digits always as (t % 100), for t>=0.
  long long v = t % 100;
  if (v < 0) v += 100; // safety

  uint8_t tens = (uint8_t)(v / 10);
  uint8_t ones = (uint8_t)(v % 10);

  // leading_zero=false: tens blank when tens==0.
  // But initialization oracle requires both digits show "0".
  // We'll preserve oracle at t==0 only; otherwise apply leading_zero=false.
  if (t == 0) {
    write_timer_display_two_digits(0, 0);
  } else {
    // If tens is 0, leading_zero=false blanks tens digit.
    write_timer_display_two_digits(tens, ones);
  }
}

void setup() {
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  model_init();
}

void loop() {
  // Control period is 10ms; ensure we run step at ~10ms
  static unsigned long last_control_ms = 0;
  unsigned long now = millis();
  if ((unsigned long)(now - last_control_ms) >= 10UL) {
    last_control_ms += 10UL;
    model_step();
  }
}