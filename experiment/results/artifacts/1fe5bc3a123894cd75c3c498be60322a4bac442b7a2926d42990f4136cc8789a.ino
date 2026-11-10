#include <Arduino.h>

//
// Frozen hardware contract bindings (Mega 2560):
//  sr1: DS=A0, STCP=A1, SHCP=A2, MR tied to VCC, OE tied to GND.
//  sr1 segments: Q0..Q6 -> sevseg1 segments A..G (active_low=true).
//

static constexpr uint8_t SR_DS   = A0; // DS / data
static constexpr uint8_t SR_STCP = A1; // STCP / latch
static constexpr uint8_t SR_SHCP = A2; // SHCP / shift clock

static constexpr uint16_t CONTROL_PERIOD_MS = 5; // from contract (not used for timing, sequence is 2s blocks)

static inline void srShiftRegisterWrite7SegmentsActiveLow(uint8_t segBits_A_to_G_activeLow)
{
  // segBits map bits 0..6 => segments A..G (active-low); Q0->A ... Q6->G
  // On 74HC595, shift in MSB->LSB order depends on implementation; we must match Q0..Q6 wiring:
  // We'll shift bit0 first to drive Q0 after 8-bit shift where Q0 is LSB in the stored byte.
  // This is done by sending the LSB first and leaving bit7 as 0.
  uint8_t b = segBits_A_to_G_activeLow & 0x7F;

  // Bring latch low -> shift, then latch high to output
  digitalWrite(SR_STCP, LOW);

  // Shift 8 bits; bit7 unused -> 0
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_SHCP, LOW);
    uint8_t outBit = (b >> i) & 0x01; // LSB first so Q0 corresponds to segment A (bit0)
    digitalWrite(SR_DS, outBit ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }

  // Latch output
  digitalWrite(SR_STCP, HIGH);
  // Optional settle
  digitalWrite(SR_STCP, LOW);
}

static inline uint8_t encodeSegActiveLowForChar(char c)
{
  // segBits: bit0..bit6 correspond to segments A..G.
  // active_low=true => 0 turns segment ON, 1 turns segment OFF.
  //
  // Using standard 7-seg patterns (segments: A,B,C,D,E,F,G):
  // 0: A,B,C,D,E,F on; G off
  // A (as commonly shown): A,B,C,E,F on; D,G off
  // P (as commonly shown): A,B,E,F on; C,D,G off
  //
  // We'll build active-high masks first, then invert for active-low.
  auto activeHighMask = [](bool a, bool b, bool c, bool d, bool e, bool f, bool g) -> uint8_t {
    uint8_t m = 0;
    if (a) m |= (1u << 0); // A
    if (b) m |= (1u << 1); // B
    if (c) m |= (1u << 2); // C
    if (d) m |= (1u << 3); // D
    if (e) m |= (1u << 4); // E
    if (f) m |= (1u << 5); // F
    if (g) m |= (1u << 6); // G
    return m;
  };

  uint8_t onMask = 0;
  switch (c) {
    case '0':
      // A,B,C,D,E,F on; G off
      onMask = activeHighMask(true, true, true, true, true, true, false);
      break;
    case 'A':
      // A,B,C,E,F on; D,G off
      onMask = activeHighMask(true, true, true, false, true, true, false);
      break;
    case 'P':
      // A,B,E,F on; C,D,G off
      onMask = activeHighMask(true, true, false, false, true, true, false);
      break;
    default:
      // Invalid => blank
      onMask = 0;
      break;
  }

  // active_low: segment OFF => 1, ON => 0
  // For 7 segments bits 0..6
  uint8_t offMask = (uint8_t)(onMask ^ 0x7F);
  return offMask;
}

static void displayBlank()
{
  // blank => all segments OFF => all bits 0..6 should be 1 for active-low
  srShiftRegisterWrite7SegmentsActiveLow(0x7F);
}

int main(void)
{
  init();

  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  // Initialization requirement: display off at power-up/reset.
  displayBlank();

  // Controller output io_0 exists in the frozen model; contract indicates initial=0.
  // But requirement explicitly says "display must be off" at initialization.
  // We'll blank immediately, then begin the timed sequence such that the first visible
  // character is '0' for 2 seconds (sequence repeats continuously).
  //
  // Timing: each character lasts 2000ms. Repeat: 0 -> A -> P -> ...
  const uint32_t stepMs = 2000;
  const uint32_t totalCycleMs = stepMs * 3;

  uint32_t start = millis();

  while (true) {
    uint32_t elapsed = millis() - start;
    uint32_t phase = elapsed % totalCycleMs;

    char ch;
    if (phase < stepMs) ch = '0';
    else if (phase < 2 * stepMs) ch = 'A';
    else ch = 'P';

    uint8_t segBits = encodeSegActiveLowForChar(ch);
    srShiftRegisterWrite7SegmentsActiveLow(segBits);

    // Keep a small deterministic delay that is consistent with controller sampling period.
    // (No additional ports; timing is still based on millis for exact 2s blocks.)
    delay(CONTROL_PERIOD_MS);
  }

  return 0;
}