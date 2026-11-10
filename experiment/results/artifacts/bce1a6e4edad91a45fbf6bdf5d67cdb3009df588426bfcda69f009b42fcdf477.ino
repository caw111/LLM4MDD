#include <Arduino.h>

class ShiftRegister595 {
public:
  uint8_t shcp; // SHCP / SRCLK
  uint8_t stcp; // STCP / RCLK
  uint8_t ds;   // DS / SER
  uint8_t oe;   // OE (if used, active-low). In circuit OE is tied to GND.

  ShiftRegister595(uint8_t shcpPin, uint8_t stcpPin, uint8_t dsPin, uint8_t oePin = 255)
    : shcp(shcpPin), stcp(stcpPin), ds(dsPin), oe(oePin) {}

  void begin() {
    pinMode(shcp, OUTPUT);
    pinMode(stcp, OUTPUT);
    pinMode(ds, OUTPUT);
    if (oe != 255) pinMode(oe, OUTPUT);

    digitalWrite(shcp, LOW);
    digitalWrite(stcp, LOW);
    digitalWrite(ds, LOW);
    if (oe != 255) digitalWrite(oe, LOW); // enable (active-low) when tied to GND
  }

  // Bits order: bit0 is Q0 (first shifted in), bit7 is Q7 (last).
  // This function shifts exactly 8 bits and latches them.
  void write8(uint8_t value) {
    if (oe != 255) digitalWrite(oe, HIGH); // disable if controlled

    // Shift out MSB first vs LSB first depends on wiring; we'll use LSB-first
    // with mapping bit i -> Q(i). This matches our explicit mapping below.
    for (int i = 0; i < 8; i++) {
      digitalWrite(shcp, LOW);
      digitalWrite(ds, (value & (1u << i)) ? HIGH : LOW);
      digitalWrite(shcp, HIGH);
    }

    digitalWrite(stcp, LOW);
    digitalWrite(stcp, HIGH);

    if (oe != 255) digitalWrite(oe, LOW); // enable
  }
};

// ----------- Pin mappings from the provided circuit (Mega 2560) -----------
// sr1: SHCP -> A2, STCP -> A1, DS -> A0
// sr2: SHCP -> A2, STCP -> A1, DS -> (from sr1: Q7S -> sr2:DS). In the circuit
//      "sr1:Q7S" is connected to "sr2:DS" and SHCP/STCP are shared.
// We must model the cascade: shift data for sr2 should be driven by sr1 Q7S.
// However, because both shift registers share SHCP and STCP and sr1 DS is A0,
// the cascaded SR chain implies serial data for sr2 comes from sr1's Q7S output.
//
// In Wokwi, the proper way is to treat it as two independent shift registers wired
// in cascade. We can accomplish this by shifting 16 bits through sr1 DS into the chain,
// where sr1's Q7S feeds sr2 DS internally.
//
// We'll generate a 16-bit chain write: first bits for sr2 (towards its Q0) will be shifted
// last, depending on cascade direction. Instead of guessing direction, we derive a stable
// implementation by directly writing sr1 and sr2 separately isn't possible because DS
// for sr2 is not externally connected; it's driven from sr1 Q7S.
// Therefore: we treat the chain as 16-bit shift register where DS is A0, SHCP=A2, STCP=A1.
//
// In cascade where the chain is: Arduino DS -> sr1 DS -> sr1 -> sr1 Q7S -> sr2 DS,
// the bit order through the chain for output Q0..Q7 of sr2 corresponds to:
 // - bits shifted out after 8th bit populate sr2.
 // - with DS value sequence applied to serial input, first shifted-in ends up in sr1 first.
 // We'll implement 16-bit LSB-first where bit i of the chain ends in Q(i mod 8) of
 // chain element (sr1 for i<8, sr2 for i>=8).
//
// Additionally, both 7-seg displays have their segment pins wired to SR Q outputs.
// We'll compute the 8-bit patterns per display and then pack them into 16 bits:
//   chain bits 0..7  -> sr1 Q0..Q7
//   chain bits 8..15 -> sr2 Q0..Q7
//
// ----------------- Segment wiring (from circuit) -----------------
// sr1 -> sevseg1 segments:
//   sr1:Q0 -> A
//   Q1 -> B
//   Q2 -> C
//   Q3 -> D
//   Q4 -> E
//   Q5 -> F
//   Q6 -> G
//   Q7 unused (often DP; not connected)
// sr2 -> sevseg2 segments:
//   sr2:Q0 -> A
//   Q1 -> B
//   Q2 -> C
//   Q3 -> D
//   Q4 -> E
//   Q5 -> F
//   Q6 -> G
//   Q7 unused
//
// The circuit does not show common anode/cathode polarity for 7-seg.
// Wokwi 7segment part default is common cathode? To avoid ambiguity,
// we can infer from "rgb led common: cathode" only; 7segment wiring shows COM.1 from SR
// via sr1:MR to sevseg COM.1. That suggests segments are active-high from SR outputs.
// We'll assume segment bits HIGH turn segments ON.
//
// We'll map digits for a standard 7-seg (A,B,C,D,E,F,G):
//   0: A,B,C,D,E,F (no G)
//   1: B,C
//   2: A,B,D,E,G
//   3: A,B,C,D,G
//   4: B,C,F,G
//   5: A,C,D,F,G
//   6: A,C,D,E,F,G
//   7: A,B,C
//   8: A,B,C,D,E,F,G
//   9: A,B,C,D,F,G
//
// with A=bit0, B=bit1, C=bit2, D=bit3, E=bit4, F=bit5, G=bit6.
static uint8_t digitToSegments(uint8_t d) {
  // bit positions: A=0, B=1, C=2, D=3, E=4, F=5, G=6
  // return 8-bit where bit7 is unused (0)
  switch (d) {
    case 0: return (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5);                         // A B C D E F
    case 1: return (1u<<1)|(1u<<2);                                                             // B C
    case 2: return (1u<<0)|(1u<<1)|(1u<<3)|(1u<<4)|(1u<<6);                                     // A B D E G
    case 3: return (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<6);                                     // A B C D G
    case 4: return (1u<<1)|(1u<<2)|(1u<<5)|(1u<<6);                                             // B C F G
    case 5: return (1u<<0)|(1u<<2)|(1u<<3)|(1u<<5)|(1u<<6);                                     // A C D F G
    case 6: return (1u<<0)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)|(1u<<6);                             // A C D E F G
    case 7: return (1u<<0)|(1u<<1)|(1u<<2);                                                     // A B C
    case 8: return (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)|(1u<<6);                     // A B C D E F G
    case 9: return (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<5)|(1u<<6);                             // A B C D F G
    default: return 0;
  }
}

// We'll implement chain shift via sr1 pins (DS=A0, SHCP=A2, STCP=A1).
static const uint8_t SR_DS  = A0;
static const uint8_t SR_SHCP= A2;
static const uint8_t SR_STCP= A1;

// cascade chain control pins
static void beginShiftChain() {
  pinMode(SR_DS, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
}

// Shift 16 bits, LSB-first into cascaded chain:
// chain bits 0..7 map to sr1 Q0..Q7 (sevseg1 segments),
// chain bits 8..15 map to sr2 Q0..Q7 (sevseg2 segments).
static void writeChain8x2(uint8_t sr1Value, uint8_t sr2Value) {
  // disable latch while shifting
  // (OE is tied low in circuit, so always enabled)
  digitalWrite(SR_STCP, LOW);

  // Build 16-bit in LSB-first order: bit0..7 -> sr1Value bits, bit8..15 -> sr2Value bits.
  uint16_t chain = (uint16_t)sr1Value | (uint16_t)(sr2Value << 8);

  for (int i = 0; i < 16; i++) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (chain & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
  }

  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

// ----------- LEDs / RGB wiring from circuit -----------
// led1: Mega 2
// led2: Mega 3
// led3: Mega 4
static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

// rgb1: R=5, G=6, B=7
// rgb2: R=8, G=9, B=10
static const uint8_t RGB1_R = 5;
static const uint8_t RGB1_G = 6;
static const uint8_t RGB1_B = 7;
static const uint8_t RGB2_R = 8;
static const uint8_t RGB2_G = 9;
static const uint8_t RGB2_B = 10;

static void allOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // RGB off
  digitalWrite(RGB1_R, LOW); digitalWrite(RGB1_G, LOW); digitalWrite(RGB1_B, LOW);
  digitalWrite(RGB2_R, LOW); digitalWrite(RGB2_G, LOW); digitalWrite(RGB2_B, LOW);
}

static void setRGBBoth(uint8_t r, uint8_t g, uint8_t b) {
  digitalWrite(RGB1_R, r); digitalWrite(RGB1_G, g); digitalWrite(RGB1_B, b);
  digitalWrite(RGB2_R, r); digitalWrite(RGB2_G, g); digitalWrite(RGB2_B, b);
}

void setup() {
  // shift chain pins
  beginShiftChain();

  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // RGB pins
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);

  allOff();

  // Initial display "99"
  uint8_t seg9 = digitToSegments(9);
  writeChain8x2(seg9, seg9);
}

void loop() {
  for (int value = 9; value >= 0; value--) {
    // update 7-segment displays to current value (both displays)
    uint8_t seg = digitToSegments((uint8_t)value);
    writeChain8x2(seg, seg);

    if (value > 0) {
      // LEDs single-color light up in sequence (led1 then led2 then led3),
      // with each LED staying on for 2 seconds for each countdown step.
      // For value=9 -> step1: led1
      // value=8 -> led2
      // value=7 -> led3
      // value=6 -> led1 ...
      int stepIndex = 9 - value;         // 0..9
      int ledIndex = stepIndex % 3;     // 0,1,2
      digitalWrite(LED1_PIN, (ledIndex == 0) ? HIGH : LOW);
      digitalWrite(LED2_PIN, (ledIndex == 1) ? HIGH : LOW);
      digitalWrite(LED3_PIN, (ledIndex == 2) ? HIGH : LOW);

      // RGB cycle through red, green, blue every 2 seconds.
      // Step aligned to countdown steps.
      int rgbIndex = (9 - value) % 3; // 0:red,1:green,2:blue
      if (rgbIndex == 0) {
        setRGBBoth(HIGH, LOW, LOW);   // red
      } else if (rgbIndex == 1) {
        setRGBBoth(LOW, HIGH, LOW);   // green
      } else {
        setRGBBoth(LOW, LOW, HIGH);    // blue
      }

      delay(2000);
    } else {
      // When countdown reaches 0:
      //  - displays "00"
      //  - all single-color LEDs off
      //  - RGB steady white
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED3_PIN, LOW);

      setRGBBoth(HIGH, HIGH, HIGH); // white
      // Ensure displays show "00"
      uint8_t seg0 = digitToSegments(0);
      writeChain8x2(seg0, seg0);

      delay(2000);
    }
  }

  // After reaching 0, restart countdown from 9 (requirement).
  // loop() will run again.
}