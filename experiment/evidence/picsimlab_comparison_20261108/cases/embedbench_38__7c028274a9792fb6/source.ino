#include <Arduino.h>
#include <Servo.h>

/*
  Pins (from provided wiring snapshot / frozen contract):
  Pot1: A3 (io_0)  Pot2: A4 (io_1)

  Shift register sr1:
    DS   (DATA) = A0
    SHCP (CLOCK) = A2
    STCP (LATCH) = A1
    OE is tied low (enabled) to sr1:GND (so no OE pin needed)
    MR is tied to VCC (so no MR pin needed)
  Shift register sr2:
    DS, SHCP, STCP are shared (wired via sr1)
    Segment outputs Q0..Q6 to sevseg2 A..G respectively

  Servos:
    servo1 PWM = D2 (io_2)
    servo2 PWM = D3 (io_3)
*/

// ----------- Frozen interface binding -----------
static constexpr uint8_t POT1_PIN = A3;  // io_0
static constexpr uint8_t POT2_PIN = A4;  // io_1

static constexpr uint8_t SERVO1_PIN = 2; // D2
static constexpr uint8_t SERVO2_PIN = 3; // D3

static constexpr uint8_t SR_DATA_PIN  = A0; // A0
static constexpr uint8_t SR_CLOCK_PIN = A2; // A2
static constexpr uint8_t SR_LATCH_PIN = A1; // A1

// ----------- Model configuration (control_period_ms = 5) -----------
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// ----------- Shift595 digit segment mapping (segment_bits [0..6] exactly) -----------
// Contract says:
/// digit selection aligns sr2 with digit 0 (leftmost) and sr1 with digit 1 (rightmost)
/// segment order Q0..Q6 mapped to sevseg A..G respectively:
///  sevseg1: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
/// active_low=true means we output 0 for lit segments, 1 for unlit segments.
//
// Segment bits in order [0..6] correspond to [A,B,C,D,E,F,G].

// Standard common-cathode segment patterns are usually 1=ON.
// We will use those and then invert for active_low.

static uint8_t segmentsCommonCathodeForDigit(uint8_t d) {
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  // Digits 0-9:
  // 0: A B C D E F (no G)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);
    case 1: return (1<<1)|(1<<2);
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);
    case 7: return (1<<0)|(1<<1)|(1<<2);
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);
    default: return 0;
  }
}

// Active-low 595 outputs: output bit=0 means segment ON.
// We'll create 7-bit active-low pattern for Q0..Q6.
static uint8_t activeLow7SegBitsForDigit(uint8_t d) {
  uint8_t cc = segmentsCommonCathodeForDigit(d);     // 1=ON
  uint8_t al = (~cc) & 0x7F;                        // invert bits within A..G
  // Q0..Q6 correspond to bits 0..6 of this 7-bit mask.
  // Return lower 8 bits; we'll place bit0..bit6 to Q0..Q6; Q7S assumed unused.
  return al;
}

// Shift out 8 bits to a single 74HC595 chain (sr1 or sr2), using active_low bits.
// Contract says digit driver is configured with segment_bits [0..6] exactly as provided and
// assumes contiguous mapping for digits, and active_low=true.
// Q7S is wired to the other register DS for cascading; it is not a segment.
// We'll set Q7S (bit7) to 1 (unlit / shift-through inactive) by default.
static void shift595WriteByte(uint8_t value) {
  // Clock/data/latch sequence.
  // We update SR by shifting 8 bits LSB-first (common for Arduino shiftOut style).
  digitalWrite(SR_LATCH_PIN, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
}

static void writeTwoDigitsToCascadedShiftRegisters(uint8_t digit0_leftmost, uint8_t digit1_rightmost) {
  // Contract mapping:
  // - registers: [{register: sr1, digit: 1, display: sevseg1}, {register: sr2, digit: 0, display: sevseg2}]
  // - sr1->sevseg1 corresponds to rightmost digit (digit1)
  // - sr2->sevseg2 corresponds to leftmost digit (digit0)
  //
  // Cascaded wiring:
  // sr1 SHCP/STCP/DATA shared; Q7S from sr1 goes to sr2 DS.
  // Therefore, when shifting two bytes into the cascade, the first byte ends up in the far register (sr2),
  // and the second byte ends up in the near register (sr1) when using standard MSB/LSB ordering.
  //
  // In practice with LSB-first shifting, we simply shift 16 bits total: first the byte for sr2, then sr1,
  // using a bit-serial loop that keeps the order consistent with our shiftOut. We'll implement explicit 16-bit shift:
  uint8_t sr2_byte = activeLow7SegBitsForDigit(digit0_leftmost);   // for sevseg2
  uint8_t sr1_byte = activeLow7SegBitsForDigit(digit1_rightmost);  // for sevseg1

  // Put bit7 (Q7S) to 1 for both bytes.
  sr2_byte |= 0x80;
  sr1_byte |= 0x80;

  // Shift 16 bits into cascade:
  // We'll shift LSB-first per byte, and send sr2_byte first then sr1_byte.
  digitalWrite(SR_LATCH_PIN, LOW);

  auto shiftByteLSBFirst = [&](uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(SR_CLOCK_PIN, LOW);
      digitalWrite(SR_DATA_PIN, (b & (1u << i)) ? HIGH : LOW);
      digitalWrite(SR_CLOCK_PIN, HIGH);
    }
  };

  shiftByteLSBFirst(sr2_byte); // first shifted goes through to sr2 via Q7S chain
  shiftByteLSBFirst(sr1_byte); // second shifted goes to sr1

  digitalWrite(SR_LATCH_PIN, HIGH);
}

// ----------- Servo smoothing behavior -----------
// Contract requires: "servo_angle port updated every control step as the target changes"
// and "move smoothly".
static void updateServoTowards(Servo &sv, float &currentDeg, float targetDeg, float maxStepDegPerPeriod) {
  float diff = targetDeg - currentDeg;
  if (diff > maxStepDegPerPeriod) diff = maxStepDegPerPeriod;
  else if (diff < -maxStepDegPerPeriod) diff = -maxStepDegPerPeriod;
  currentDeg += diff;
  // Convert 0..99 deg to servo.write angle range.
// Many hobby servos treat 0..180 as degrees linearly; use write(currentDeg).
  sv.write((int)lrintf(currentDeg));
}

// ----------- IO model state -----------
static float servo1CurrentDeg = 0.0f;
static float servo2CurrentDeg = 0.0f;

static float io_0 = 0.0f; // pot1 raw adc count (contract type f64, sampled each 5ms)
static float io_1 = 0.0f; // pot2 raw adc count

static float io_2 = 0.0f; // servo1 target
static float io_3 = 0.0f; // servo2 target

static int32_t displayNumber = 0; // io_4 as i32

static uint32_t lastStepMs = 0;

// Frozen contract mappings:
// io_2 = clamp(io_0, 0, 99)
// io_3 = clamp(io_1, 0, 99)
// io_4 = if io_0 in [0,99] then 10*floor(io_0/10) + floor(io_0 - 10*floor(io_0/10)) else 0
// Note: io_4 uses floor on io_0/10 and on remainder. We'll mimic exactly using float->floor.
static float clamp99f(float x) {
  if (x >= 0.0f) {
    if (x <= 99.0f) return x;
    return 99.0f;
  }
  return 0.0f;
}
static int32_t computeDisplayFromIo0(float x) {
  if (x >= 0.0f) {
    if (x <= 99.0f) {
      float q = floorf(x / 10.0f);
      float r = floorf(x - 10.0f * q);
      float val = (2.0f * q) + r; // frozen contract: (2*floor(x/10)) + floor(x - 10*floor(x/10))
      return (int32_t)val;
    }
    return 0;
  }
  return 0;
}

Servo servo1;
Servo servo2;

void setup() {
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization: displays show "00"; both servos at 0 degrees.
  servo1CurrentDeg = 0.0f;
  servo2CurrentDeg = 0.0f;
  servo1.write(0);
  servo2.write(0);

  writeTwoDigitsToCascadedShiftRegisters(0, 0); // digit0 and digit1 both 0 -> "00"
  lastStepMs = millis();
}

void loop() {
  uint32_t now = millis();
  if (now - lastStepMs < CONTROL_PERIOD_MS) return;
  lastStepMs += CONTROL_PERIOD_MS;

  // Sample pots (analog inputs). Scale = 1.0, offset = 0.0
  // Use float adc count as f64-equivalent.
  io_0 = (float)analogRead(POT1_PIN);
  io_1 = (float)analogRead(POT2_PIN);

  // Model outputs io_2, io_3 according to frozen contract (clamp to 0..99).
  io_2 = clamp99f(io_0);
  io_3 = clamp99f(io_1);

  // Smooth servo movement towards targets each control step.
  // Choose max step so movement is continuous and not instantaneous.
  // Step size in degrees per 5ms. Keep modest.
  const float maxStepDegPerPeriod = 1.0f; // 1 deg every 5ms => ~200 deg/s (smooth)
  updateServoTowards(servo1, servo1CurrentDeg, io_2, maxStepDegPerPeriod);
  updateServoTowards(servo2, servo2CurrentDeg, io_3, maxStepDegPerPeriod);

  // Display update:
  // io_4 computed from io_0 (pot1/servo1) only.
  displayNumber = computeDisplayFromIo0(io_0);

  // Need tens/units from io_0 mapping exactly per frozen contract digits used in shift595_number.
  // The frozen contract defines io_4 as an i32 numerical value:
  //   tens = floor(io_0/10) and units = floor(io_0 - 10*floor(io_0/10))
  // but multiplied in a non-standard way in io_4: (2*tens + units).
  //
  // However, the contract for shift595_number outputs digit-wise from io_0/servo1 angle.
  // It explicitly computes:
  //   digit (digit 1) = floor(io_0/10)
  //   digit (digit 0) = floor(io_0 - 10*floor(io_0/10))
  // from the displayed meaning "tens and units digits".
  //
  // Yet in "outputs.io_4", the expression is:
  //   (2*floor(io_0/10)) + floor(io_0 - 10*floor(io_0/10))
  // That corresponds to encoding the two digits into a base-10-ish value only if "2" were "10".
  // To preserve the controller state and ordered updates exactly as specified by the frozen contract,
  // we must use the digit extraction used by the display driver request:
  // - tens digit: floor(io_0 / 10.0)
  // - units digit: floor(io_0 - 10*floor(io_0/10))
  //
  // We'll compute these directly from io_0 (not from displayNumber), then route to digits:
  // leftmost = digit 0 (sr2), rightmost = digit 1 (sr1).
  float q = floorf(io_0 / 10.0f);
  float r = floorf(io_0 - 10.0f * q);

  int tensDigit = (int)q; // floor 0..9
  int unitsDigit = (int)r; // floor remainder 0..9

  // Clamp digits safely.
  if (tensDigit < 0) tensDigit = 0;
  if (tensDigit > 9) tensDigit = 9;
  if (unitsDigit < 0) unitsDigit = 0;
  if (unitsDigit > 9) unitsDigit = 9;

  // Apply digit placement: sr2 digit 0 (leftmost) gets unitsDigit? per contract digit field:
  // digit 0 leftmost on sr2, digit 1 rightmost on sr1.
  // Tens from pot1 should be on one display (sevseg1) and units on the other (sevseg2),
  // with physical positions sevseg1 left of sevseg2 implies:
  // tens -> sevseg1 (left), units -> sevseg2 (right).
  // But frozen contract notes opposite digit0/digit1 placement based on display positions.
  // We'll follow frozen contract's explicit digit placement:
  //   digit0_leftmost -> sr2 (sevseg2)
  //   digit1_rightmost -> sr1 (sevseg1)
  //
  // Therefore leftmost digit on sevseg2 should receive "digit0", which from model semantics is unitsDigit,
  // and rightmost digit on sevseg1 should receive "digit1", which is tensDigit.
  // This preserves the frozen contract's digit field alignment note.
  uint8_t digit0_for_sr2 = (uint8_t)unitsDigit;
  uint8_t digit1_for_sr1 = (uint8_t)tensDigit;

  writeTwoDigitsToCascadedShiftRegisters(digit0_for_sr2, digit1_for_sr1);
}