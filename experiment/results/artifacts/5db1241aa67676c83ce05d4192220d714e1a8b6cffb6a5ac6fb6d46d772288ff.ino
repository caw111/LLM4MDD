#include <Arduino.h>

/// === Frozen controller state & contract ===
static int32_t currentNumber = 0;   // initial 0
static int32_t accumulatedMs  = 0;   // initial 0

/// === Timing contract ===
static const uint32_t CONTROL_PERIOD_MS = 10;
static const uint32_t NUMBER_PERIOD_MS  = 2000;

// Hardware interface pins (preserve wiring from circuit)
static const uint8_t LED1_PIN = 2;   // io_0
static const uint8_t LED2_PIN = 3;   // io_1

// Shift register sr1 (74HC595) pins:
static const uint8_t SR_DATA_PIN  = A0; // DS (blue)
static const uint8_t SR_LATCH_PIN = A1; // STCP (purple)
static const uint8_t SR_CLOCK_PIN = A2; // SHCP (gray)

/// Bargraph segments A1..A10 use direct GPIO D4..D13 in SEGMENT order.
/// The controller outputs io_3_segment_0..9 correspond to A1..A10.
static const uint8_t BAR_A_PINS[10] = {
  4, 5, 6, 7, 8, 9, 10, 11, 12, 13
};

/// Not connected in the circuit (all C1..C10 to GND), but we model them as not-needed.
/// Bargraph outputs are active-high.

/// === Shift register display mapping ===
/// Controller uses "shift595_number" with:
/// - invalid_policy: blank (not applicable since currentNumber is always 0..9)
/// - leading_zero: false (not applicable for single digit; we always show number 0..9)
/// - segment_bits: [0..6] active_low=true
///
/// Wiring indicates:
///   sr1 Q0..Q6 => sevseg1 A..G
///   and sevseg1 COM.1 driven by sr1: MR connected to COM.1 (sr1 MR tied to sevseg1 COM.1)
///
/// We'll implement the exact digit segment patterns for a common arrangement where:
/// - active_low: segment ON means corresponding SR output bit should be LOW.
/// - Active segments for digit N: standard 7-seg A,B,C,D,E,F,G.
///
static uint8_t digitTo74hc595_Q_bits_activeHigh(uint8_t digit) {
  // Returns 7-bit pattern for Q0..Q6 where each bit=1 means the corresponding segment is ON.
  // Segment indices: 0:A, 1:B, 2:C, 3:D, 4:E, 5:F, 6:G.
  // Standard digit segments:
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  bool a=false,b=false,c=false,d=false,e=false,f=false,g=false;

  switch (digit) {
    case 0: a=b=c=d=e=f=true; g=false; break;
    case 1: b=c=true; break;
    case 2: a=b=d=e=true; g=true; break;
    case 3: a=b=c=d=true; g=true; break;
    case 4: b=c=f=true; g=true; break;
    case 5: a=c=d=f=true; g=true; break;
    case 6: a=c=d=e=f=true; g=true; break;
    case 7: a=b=c=true; break;
    case 8: a=b=c=d=e=f=g=true; break;
    case 9: a=b=c=d=f=g=true; break;
    default:
      // invalid: blank => all segments off (all false)
      break;
  }

  uint8_t q = 0;
  if (a) q |= (1u << 0);
  if (b) q |= (1u << 1);
  if (c) q |= (1u << 2);
  if (d) q |= (1u << 3);
  if (e) q |= (1u << 4);
  if (f) q |= (1u << 5);
  if (g) q |= (1u << 6);
  return q;
}

static void shift595_write7seg_activeLow(int32_t digit) {
  // sr1 has 7-seg segment bits mapped to Q0..Q6.
  // active_low=true => segment ON => SR output bit LOW.
  // We'll clock out a byte with bit0..bit6 corresponding to Q0..Q6.
  // Bit7 unused => keep LOW (doesn't matter for sevseg).
  uint8_t segOn_high = 0;
  if (digit >= 0 && digit <= 9) {
    segOn_high = digitTo74hc595_Q_bits_activeHigh((uint8_t)digit);
  } else {
    segOn_high = 0; // invalid => blank
  }

  // Convert to active-low drive: 1->segment ON should become 0 on the SR output bit.
  // So SR bit = NOT(segOn_high). For ON segments => 0, OFF segments => 1.
  uint8_t qbits_activeLow = (uint8_t)(~segOn_high) & 0x7F;

  // Ensure bit7 (Q7) off (set high to avoid accidental drive). We'll set it to 1.
  qbits_activeLow |= (1u << 7);

  // The 74HC595 shifts MSB-first or LSB-first depending on wiring; we must preserve correct behavior.
  // Common Arduino usage is MSB-first; however for correctness with Q0..Q6 mapping,
  // we will shift out starting from Q7 down to Q0.
  // This ensures that bit i in our byte ends up in Q{i} (typical Q0 corresponds to first shifted bit depending on variant).
  // In practice with standard SPI-like shiftOut, shifting MSB-first places bit7 first and bit0 last => Q0 gets last bit.
  // Our byte layout uses bit positions corresponding to Q bits.
  for (int8_t bit = 7; bit >= 0; --bit) {
    uint8_t val = (qbits_activeLow >> bit) & 0x01;
    digitalWrite(SR_DATA_PIN, val ? HIGH : LOW);
    // Pulse clock
    digitalWrite(SR_CLOCK_PIN, HIGH);
    digitalWrite(SR_CLOCK_PIN, LOW);
  }

  // Latch
  digitalWrite(SR_LATCH_PIN, HIGH);
  digitalWrite(SR_LATCH_PIN, LOW);
}

/// === Bargraph update ===
static void updateBargraphSegments(int32_t n) {
  // Controller outputs:
  // io_3_segment_k is true iff n == (k+1) for k=0..8, and io_3_segment_9 true iff n==10 (never occurs).
  // The requirement text says: "segments equal to current number (e.g., 3 -> A1-A3)".
  // However the frozen model explicitly uses equality to exact values:
  // - io_3_segment_0 is true for currentNumber == 1
  // - io_3_segment_1 is true for currentNumber == 2
  // ...
  // - io_3_segment_8 is true for currentNumber == 9
  // - io_3_segment_9 is true for currentNumber == 10
  // With currentNumber cycling 0..9, this means:
  // - n==0 => all off
  // - n==k (1..9) => only segment A(k) ON (single bar), not first N segments.
  // We must follow the frozen model outputs exactly.
  for (int i = 0; i < 10; ++i) {
    int32_t segNumber = i + 1; // io_3_segment_i corresponds to currentNumber == (i+1), except i=9 => 10
    bool on = (n == segNumber);
    digitalWrite(BAR_A_PINS[i], on ? HIGH : LOW);
  }
}

/// === Parity LEDs update (follow frozen model) ===
static void updateParityLeds(int32_t n) {
  // Model uses:
  // io_0 (led1) = (n == 2*floor(n/2))  => even
  // io_1 (led2) = (n != 2*floor(n/2)) => odd
  int32_t evenValue = 2 * (n / 2); // floor for positive integers equals truncation
  bool led1_on = (n == evenValue);
  bool led2_on = !led1_on;
  digitalWrite(LED1_PIN, led1_on ? HIGH : LOW);
  digitalWrite(LED2_PIN, led2_on ? HIGH : LOW);
}

/// === Apply all outputs "simultaneously" at number change boundaries ===
static void applyOutputsForCurrentNumber(int32_t n) {
  updateParityLeds(n);
  shift595_write7seg_activeLow(n);
  updateBargraphSegments(n);
}

void setup() {
  // Pins init
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);

  for (int i = 0; i < 10; ++i) {
    pinMode(BAR_A_PINS[i], OUTPUT);
  }

  // Hardware defaults:
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  // Initial outputs per contract:
  // - io_2=currentNumber=0 => show 0
  // - bargraph fully off (n=0 => all off per frozen model)
  // - both LEDs off (n=0 even => led1 ON per parity model would turn it ON)
  // But Frozen model outputs define led1 true for even numbers including 0.
  // Contract text says both LEDs off initially; however the "Accepted executable model" is the oracle.
  // Therefore we follow model outputs precisely:
  applyOutputsForCurrentNumber(currentNumber);
}

void loop() {
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();

  // Enforce control period of 10ms (input io_4 is elapsed_ms delta since previous control step)
  if (lastTickMs == 0) lastTickMs = now;

  uint32_t elapsed = now - lastTickMs;
  if (elapsed < CONTROL_PERIOD_MS) return;

  lastTickMs += elapsed; // keep stable long-term

  // io_4 = actual delta since previous control step (ms)
  int32_t io_4 = (int32_t)elapsed;

  // --- Controller tick (exact frozen model behavior) ---
  // tick:
  // if (accumulatedMs >= 2000) { accumulatedMs -= 2000; currentNumber = (currentNumber==9)?0:currentNumber+1; }
  // accumulatedMs = accumulatedMs + io_4;
  bool numberChanged = false;
  if (accumulatedMs >= (int32_t)NUMBER_PERIOD_MS) {
    accumulatedMs -= (int32_t)NUMBER_PERIOD_MS;
    if (currentNumber == 9) currentNumber = 0;
    else currentNumber = currentNumber + 1;
    numberChanged = true;
  }
  accumulatedMs = accumulatedMs + io_4;

  // Outputs are functions of currentNumber, but requirement says update simultaneously at boundary.
  // Since only number changes at boundary, update only then to preserve simultaneous semantics.
  if (numberChanged) {
    applyOutputsForCurrentNumber(currentNumber);
  }

  // Otherwise no-op: outputs remain as per currentNumber (which is unchanged).
}