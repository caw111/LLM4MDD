#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Pin mapping from circuit
// pot1 -> A3
static const uint8_t POT_PIN = A3;

// Shift registers (74HC595) control pins
// sr1
static const uint8_t SR1_SHCP = A2; // gray
static const uint8_t SR1_STCP = A1; // purple
static const uint8_t SR1_DS   = A0; // blue

// sr2 shares clock/latch pins with sr1 per circuit,
// but has its own data input DS (sr2:DS from sr1:Q7S) which is already driven
// by sr1's Q7S, so we only need to configure sr1 shift register pins.

// Common control
static const uint8_t SR_OE  = 0; // sr1:OE tied to GND (black) in circuit, not used as a pin
static const uint8_t SR_MR1 = 0; // sr1:MR tied to VCC (red) in circuit, not used as a pin

// 7-segment encoding (common approach: bits A..G in Q0..Q6 as per wiring)
// Display driver: outputs are connected as:
// sr1 Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// We'll assume the 74HC595 outputs drive the segment LEDs appropriately for a digit mapping.
// Many Wokwi 7-seg parts use "1 = segment ON". We'll use that convention.
static const uint8_t SEGMENTS_0_TO_9[10] = {
  // A B C D E F G
  // 0: A B C D E F
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                 // 0
  (1<<1)|(1<<2),                                             // 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                       // 2
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                       // 3
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                               // 4
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                       // 5
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),                // 6
  (1<<0)|(1<<1)|(1<<2),                                      // 7
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),       // 8
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                // 9
};

// Function to shift out 8 bits to the chained 74HC595 pair
// Chain: sr1 outputs feed sr2 data (sr1:Q7S -> sr2:DS).
// We'll clock bits such that the first 8 shifted bits end up at sr2 (last in chain),
// and the next 8 bits at sr1 (closest to the Arduino data path).
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOutChain8(uint8_t valueSr2, uint8_t valueSr1);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showTwoDigit(int value);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_35\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOutChain8(uint8_t valueSr2, uint8_t valueSr1) {
  // We need to shift 16 bits MSB-first: valueSr2 then valueSr1.
  // Using DS = sr1 DS pin.
  uint16_t combined = (uint16_t(valueSr2) << 8) | uint16_t(valueSr1);

  // DS signal bit 15..0; MSB first
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (combined >> i) & 0x01);

    // SHCP rising edge
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
  }

  // STCP latch pulse - updates outputs
  digitalWrite(SR1_STCP, LOW);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
}

static void showTwoDigit(int value) {
  // Range handling: outside 0-99 show "00"
  if (value < 0 || value > 99) value = 0;

  uint8_t tens = uint8_t((value / 10) % 10);
  uint8_t ones = uint8_t(value % 10);

  // sevseg1 is tens digit, connected to sr1 outputs (sr1 -> sevseg1)
  // sevseg2 is ones digit, connected to sr2 outputs (sr2 -> sevseg2)
  uint8_t segTens = SEGMENTS_0_TO_9[tens];
  uint8_t segOnes = SEGMENTS_0_TO_9[ones];

  // For 7-seg, bit positions correspond to segments A..G (Q0..Q6)
  // Q7 is unused; keep it 0.
  uint8_t sr1Byte = segTens & 0x7F;
  uint8_t sr2Byte = segOnes & 0x7F;

  // Shift to chain
  shiftOutChain8(sr2Byte, sr1Byte);
}

void setup() {
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  // Initial levels
  digitalWrite(SR1_DS, LOW);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // On power-up/reset show "00"
  showTwoDigit(0);
}

void loop() {
  int raw = analogRead(POT_PIN); // 0..1023

  // Map to 0..99 as required
  // Use long arithmetic to avoid overflow; ensure in range
  int mapped = (int)((long)raw * 99L / 1023L); // 0..99

  // Display dynamically
  showTwoDigit(mapped);

  // Small delay to reduce flicker but keep responsive
  delay(20);
}
