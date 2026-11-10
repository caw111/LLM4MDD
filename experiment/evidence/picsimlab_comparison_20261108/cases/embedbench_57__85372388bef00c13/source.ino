#include <Arduino.h>

// --- Shift registers + 7-seg wiring assumptions from circuit ---
static const uint8_t SR1_SHCP = A2;
static const uint8_t SR1_STCP = A1;
static const uint8_t SR1_DS   = A0;

static const uint8_t SR2_SHCP = A2; // tied via SR1:SHCP -> SR2:SHCP
static const uint8_t SR2_STCP = A1; // tied via SR1:STCP -> SR2:STCP
static const uint8_t SR2_DS   = A0; // driven through SR1:Q7S -> SR2:DS (but we can't address that via code)
                                   // In the provided wiring, SR2:DS is connected to sr1:Q7S, which implies a daisy-chain.
                                   // We'll implement daisy-chain by shifting 16 bits: first byte to SR2, second to SR1.
                                   // DS pin we can control is SR1:DS (A0).
// Note: MR is tied to VCC in the circuit, OE tied to GND (always enabled).

// --- Pin used for daisy-chain shifting data: SR1:DS ---
static const uint8_t DS_PIN = A0;
static const uint8_t SHCP_PIN = A2;
static const uint8_t STCP_PIN = A1;

// --- Color sequence (common cathode RGB LEDs) ---
// We have discrete pins for each color channel of each RGB LED:
// rgb1: R=2, G=3, B=4
// rgb2: R=5, G=6, B=7
// rgb3: R=8, G=9, B=10
static const uint8_t RGB1_R = 2,  RGB1_G = 3,  RGB1_B = 4;
static const uint8_t RGB2_R = 5,  RGB2_G = 6,  RGB2_B = 7;
static const uint8_t RGB3_R = 8,  RGB3_G = 9,  RGB3_B = 10;

static const uint8_t commonCathodeLevelOn  = LOW;
static const uint8_t commonCathodeLevelOff = HIGH;

static const uint8_t NUM_COLORS = 6;

// Each entry: {r1,g1,b1, r2,g2,b2, r3,g3,b3} with HIGH=off LOW=on
static const uint8_t colorSeq[NUM_COLORS][9] = {
  // Red: rgb1 red on; rgb2&rgb3 off
  {commonCathodeLevelOn,  commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff},
  // Green: rgb2 green on; others off
  {commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOn,  commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff},
  // Blue: rgb3 blue on; others off
  {commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOn},
  // Purple: rgb1 red + blue on
  {commonCathodeLevelOn,  commonCathodeLevelOff, commonCathodeLevelOn,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff},
  // Yellow: rgb2 red + green on
  {commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOn,  commonCathodeLevelOn,  commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff},
  // Cyan: rgb3 green + blue on
  {commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOff, commonCathodeLevelOff,
   commonCathodeLevelOff, commonCathodeLevelOn,  commonCathodeLevelOn}
};

// 7-seg segments mapping from circuit (each sr Qx -> sevseg segment):
// sr1: Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S->(to sr2 DS).
// For sr2 similarly: Q0->A ... Q6->G.
static const uint8_t SR_QA = 0;
static const uint8_t SR_QB = 1;
static const uint8_t SR_QC = 2;
static const uint8_t SR_QD = 3;
static const uint8_t SR_QE = 4;
static const uint8_t SR_QF = 5;
static const uint8_t SR_QG = 6;
// Q7S is the "spare" used only for daisy-chain; we must shift 8 bits regardless.

// We'll drive the 74HC595 outputs to the common cathode display (in circuit COM.1 tied together to SR MR path?).
// The circuit wiring indicates: sr1:OE tied to black (GND), and sr1:MR tied to VCC.
// This means outputs are active as normal 74HC595 (high=logic 1).
// For common cathode displays, turning a segment "on" typically means driving its line HIGH
// (current flows from segment anode through resistor to common cathode at COM).
// We'll implement segment bits for digits with bit=1 => segment ON.
// If your display is inverted, you can invert the pattern, but we must follow the provided wiring intent.
static const uint8_t digitToSegmentsCommonCathode[10] = {
  // bits: A B C D E F G (bit0=A ... bit6=G)
  // 0: A B C D E F
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: B C
  (1<<1)|(1<<2),
  // 2: A B D E G
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: A B C D G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: B C F G
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: A C D F G
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: A C D E F G
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: A B C
  (1<<0)|(1<<1)|(1<<2),
  // 8: A B C D E F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: A B C D F G
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

static inline void setRGBColor(uint8_t idx) {
  // idx 0..5
  for (uint8_t i = 0; i < 9; i++) {
    // Apply pins in order: rgb1(R,G,B), rgb2(R,G,B), rgb3(R,G,B)
    uint8_t level = colorSeq[idx][i];
    switch (i) {
      case 0: digitalWrite(RGB1_R, level); break;
      case 1: digitalWrite(RGB1_G, level); break;
      case 2: digitalWrite(RGB1_B, level); break;
      case 3: digitalWrite(RGB2_R, level); break;
      case 4: digitalWrite(RGB2_G, level); break;
      case 5: digitalWrite(RGB2_B, level); break;
      case 6: digitalWrite(RGB3_R, level); break;
      case 7: digitalWrite(RGB3_G, level); break;
      case 8: digitalWrite(RGB3_B, level); break;
    }
  }
}

// Shift out 16 bits to two daisy-chained 74HC595:
// - First shifted byte ends up in SR2 (farther), second byte in SR1 (near).
// - STCP latch after all bits.
static void update7Seg(uint8_t value) {
  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  uint8_t segsTens = digitToSegmentsCommonCathode[tens];
  uint8_t segsOnes = digitToSegmentsCommonCathode[ones];

  // Convert to 74HC595 output bits across Q0..Q6.
  // We'll also set Q7 (MSB) to 0 (don't care / used for shifting chain).
  uint8_t sr2_byte = segsTens & 0x7F; // Q0..Q6
  uint8_t sr1_byte = segsOnes & 0x7F;

  // Compose 16-bit stream as: [sr2_byte][sr1_byte], MSB first.
  // We'll shift MSB-first using digitalWrite on DS_PIN and manual clocking.
  uint16_t stream = ((uint16_t)sr2_byte << 8) | sr1_byte;

  // Latch low, then shift
  digitalWrite(STCP_PIN, LOW);

  for (int8_t bit = 15; bit >= 0; --bit) {
    digitalWrite(SHCP_PIN, LOW);
    digitalWrite(DS_PIN, (stream & (1u << bit)) ? HIGH : LOW);
    // rising edge clock
    digitalWrite(SHCP_PIN, HIGH);
  }

  // Latch to outputs
  digitalWrite(STCP_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(STCP_PIN, LOW);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT); pinMode(RGB3_G, OUTPUT); pinMode(RGB3_B, OUTPUT);

  // Initialize shift register control pins
  pinMode(DS_PIN, OUTPUT);
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);

  digitalWrite(DS_PIN, LOW);
  digitalWrite(SHCP_PIN, LOW);
  digitalWrite(STCP_PIN, LOW);

  // Initial state: first RGB color (red) and displays show "99"
  setRGBColor(0);       // red
  update7Seg(99);      // "99"
}

void loop() {
  static int timerVal = 99;
  static uint8_t colorIdx = 0;

  static unsigned long lastTick = 0;
  unsigned long now = millis();

  if (now - lastTick >= 2000UL) {
    lastTick += 2000UL; // keep cadence synchronized

    // Synchronize updates: decrement timer and advance color simultaneously
    timerVal -= 1;

    colorIdx = (colorIdx + 1) % NUM_COLORS;

    if (timerVal < 0) {
      // Reset condition: when timer reaches 0, reset to 99 and restart RGB from red
      timerVal = 99;
      colorIdx = 0;
    } else if (timerVal == 0) {
      // At the moment it reaches 0, it should reset to 99 (same 2-second tick),
      // and RGB LEDs restart from red.
      timerVal = 99;
      colorIdx = 0;
    }

    // Apply updated synchronized states
    setRGBColor(colorIdx);
    update7Seg((uint8_t)timerVal);
  }
}