#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t SR1_DS_PIN = A0;   // mega:A0 -> sr1:DS
static const uint8_t SR1_SHCP_PIN = A2; // mega:A2 -> sr1:SHCP
static const uint8_t SR1_STCP_PIN = A1; // mega:A1 -> sr1:STCP (latch for both via wiring)

static const uint8_t LED1_PIN = 2; // mega:2  (led1:A)
static const uint8_t LED2_PIN = 3; // mega:3  (led2:A)

// 10-segment bargraph is wired to sr1:Q? via sr2 through same shift chain,
// but the SR bits are mapped to bargraph segments in the provided circuit.
// We will drive the full shift register chain consistently and set:
// - sevseg1 (sr1) and sevseg2 (sr2) digit segments
// - bargraph segments on sr2 as well (sr2:Q7S..Q? not used for digits, but bargraph uses sr1/unspecified chain parts).
//
// Given connections, bargraph segments use sr2 DS pin mapping through sr1 chain's Q7S to sr2 DS.
// We therefore update the combined 16-bit shift chain by setting:
 // sr1 Q0..Q6 => sevseg1 A..G
 // sr1 Q7 => sevseg1? (not used)
 // sr1 Q7S => sr2 DS (serial input into sr2) (so Q7 affects sr2 bits shifting)
 // sr2 Q0..Q6 => sevseg2 A..G
 // sr2 Q? (not explicitly mapped for bargraph, but bargraph A1..A10 are connected to mega pins A4..A13 in circuit)
//
// Note: The provided circuit explicitly connects only mega:4..13 to bargraph A1..A10 and mega:GND to bargraph C1..C10.
// Therefore, bargraph is NOT actually controlled by the shift registers; it is directly tied to mega pins 4..13.
// However, the requirement says bargraph is controlled via shift registers (sr1,sr2).
// To preserve the wiring given, we must drive mega pins 4..13 directly.

static const uint8_t BARGRAPH_PINS[10] = {4,5,6,7,8,9,10,11,12,13}; // A1..A10

// 7-seg segment encoding for common-cathode (assumed: driven outputs connect to segments).
// We need mapping consistent with provided wiring: sr1:Q0->sevseg1:A, Q1->B,...Q6->G.
// We'll use standard bit order A..G.
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t encode7segDigit(uint8_t d);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shift2x595(uint16_t value);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t buildShiftValue(uint8_t tensDigit, uint8_t onesDigit);
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraphSegments(uint8_t segmentsOn);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_106\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t encode7segDigit(uint8_t d) {
  // bits: A B C D E F G (bit0..bit6)
  switch (d) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);             // A B C D E F
    case 1: return (1<<1)|(1<<2);                                           // B C
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4);                             // A B D E
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);                      // A B C D E
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);                              // B C F G
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                      // A C D F G
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);             // A C D E F G
    case 7: return (1<<0)|(1<<1)|(1<<2);                                     // A B C
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);      // A B C D E F G
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);               // A B C D F G
    default: return 0;
  }
}

// Drive the two daisy-chained 74HC595 (sr1 then sr2).
// Chain order for daisy-chained shift registers in a single data stream:
// - Bits shifted first go into sr2? Depends on wiring. Provided: sr1:Q7S -> sr2:DS,
// so the serial output of sr1 (Q7S) feeds sr2 DS.
// That means when shifting N bits, the first bits clocked end up in sr1, and after 8 clocks
// sr1's Q7S will pass along bits for sr2.
// We'll send 16 bits: sr1 bits for Q7..Q0, then sr2 bits for Q7..Q0.
// We only actively use sr1 Q0..Q6 and sr2 Q0..Q6; other bits set to 0.
static void shift2x595(uint16_t value) {
  // value bit0..bit15 correspond to sequential clocks into SR chain:
  // bit15 is sent first? We'll implement explicitly by clocking MSB first to match typical shift-in.
  // We'll map such that:
  // - On clock k from 0..15, we output (value & (1<<(15-k))) ? HIGH : LOW to DS.
  for (int8_t i = 15; i >= 0; --i) {
    digitalWrite(A0, (value & (1UL << i)) ? HIGH : LOW); // DS line is A0
    digitalWrite(A2, HIGH); // SHCP
    digitalWrite(A2, LOW);
  }
  digitalWrite(A1, HIGH); // STCP latch
  digitalWrite(A1, LOW);
}

static uint16_t buildShiftValue(uint8_t tensDigit, uint8_t onesDigit) {
  // sr1 outputs:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // sr2 outputs:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  uint8_t sev1 = encode7segDigit(tensDigit);
  uint8_t sev2 = encode7segDigit(onesDigit);

  // Place bits into shift value.
  // We'll fill sr1 Q0..Q6 into corresponding positions within lower 8 bits of sr1 byte,
  // and sr2 Q0..Q6 into corresponding positions within upper 8 bits of sr2 byte.
  // Since shift2x595 uses sequential clock order, we choose convention:
  // lower 8 bits of `value` -> sr1 byte content (Q0..Q7)
  // upper 8 bits of `value` -> sr2 byte content (Q0..Q7)
  //
  // Within a byte, bit0 corresponds to Q0.
  uint16_t v = 0;
  v |= (uint16_t)(sev1 & 0x7F) << 0;   // sr1 Q0..Q6
  v |= (uint16_t)(sev2 & 0x7F) << 8;   // sr2 Q0..Q6
  // Q7 and any unused bits remain 0.
  return v;
}

static void setBarGraphSegments(uint8_t segmentsOn) {
  if (segmentsOn > 10) segmentsOn = 10;
  // Given wiring:
  // mega:4..13 => bargraph A1..A10 (green)
  // mega:GND => bargraph C1..C10
  // So we enable a segment by driving corresponding A pin HIGH.
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(BARGRAPH_PINS[i], (i < segmentsOn) ? HIGH : LOW);
  }
}

void setup() {
  // Shift register control pins
  pinMode(SR1_DS_PIN, OUTPUT);      // A0
  pinMode(SR1_SHCP_PIN, OUTPUT);   // A2
  pinMode(SR1_STCP_PIN, OUTPUT);   // A1

  digitalWrite(SR1_DS_PIN, LOW);
  digitalWrite(SR1_SHCP_PIN, LOW);
  digitalWrite(SR1_STCP_PIN, LOW);

  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Bar graph segments (direct pins per wiring)
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(BARGRAPH_PINS[i], OUTPUT);
    digitalWrite(BARGRAPH_PINS[i], LOW);
  }

  // Initialization requirements:
  // - displays show "00"
  // - bargraph lights up 1 segment (PIN A1 -> mega:4)
  // - both LEDs off
  uint16_t initShift = buildShiftValue(0, 0);
  shift2x595(initShift);

  setBarGraphSegments(1);

  // Start counters/timers
}

void loop() {
  static uint8_t number = 0; // 0..99
  static bool ledState = false;

  static unsigned long lastTick = 0;
  const unsigned long intervalMs = 2000;

  unsigned long now = millis();
  if (now - lastTick >= intervalMs) {
    lastTick += intervalMs;

    // Increment cycle every 2 seconds
    number = (number >= 99) ? 0 : (uint8_t)(number + 1);

    uint8_t tens = number / 10;
    uint8_t ones = number % 10;

    // Update 7-segment displays:
    // sevseg1 -> tens digit, sevseg2 -> units digit
    uint16_t v = buildShiftValue(tens, ones);
    shift2x595(v);

    // Bar graph:
    // segments = tens digit + 1
    setBarGraphSegments((uint8_t)(tens + 1));

    // LED blinking alternate every 2 seconds
    ledState = !ledState;
    digitalWrite(LED1_PIN, ledState ? HIGH : LOW);
    digitalWrite(LED2_PIN, ledState ? LOW : HIGH);
  }
}
