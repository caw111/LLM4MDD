#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// --- Pin assignments (Mega 2560 as per provided wiring) ---
const uint8_t LED1_PIN = 2;  // led1:A -> mega:2
const uint8_t LED2_PIN = 3;  // led2:A -> mega:3
const uint8_t LED3_PIN = 4;  // led3:A -> mega:4

// 7-seg shift registers (74HC595)
const uint8_t SR1_SHCP = A2; // mega:A2 -> sr1:SHCP
const uint8_t SR1_STCP = A1; // mega:A1 -> sr1:STCP
const uint8_t SR1_DS   = A0; // mega:A0 -> sr1:DS

// Potentiometer
const uint8_t POT_PIN = A3; // pot1:SIG -> mega:A3

// Common pins wired together across sr1/sr2:
// mega:5V -> sr1:VCC, sr1:MR -> sr1:MR tied to VCC, sr1:OE tied to GND (always enabled)

// --- Segment bit mapping ---
// Outputs from Q0..Q7S (sr1) to sevseg1: A..G
// Outputs from Q0..Q6 (sr2) to sevseg2: A..G
// We'll generate 8 bits per shift register: bit0->Q0(A), ... bit6->Q6(G), bit7->Q7 unused for each segment driver.
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegmentsCommonCathode(uint8_t d);
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite16(uint16_t data16);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBothDisplays(uint8_t tens, uint8_t ones);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLEDs(uint8_t ledState);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_105\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegmentsCommonCathode(uint8_t d) {
  // seg bits: A,B,C,D,E,F,G -> bits 0..6
  // Assumes active-high segment control.
  // 0: A B C D E F (G off)
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
    case 0: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);                 // A B C D E F
    case 1: return (1<<1) | (1<<2);                                                   // B C
    case 2: return (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<6);                         // A B D E G
    case 3: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<6);                         // A B C D G
    case 4: return (1<<1) | (1<<2) | (1<<5) | (1<<6);                                 // B C F G
    case 5: return (1<<0) | (1<<2) | (1<<3) | (1<<5) | (1<<6);                         // A C D F G
    case 6: return (1<<0) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);             // A C D E F G
    case 7: return (1<<0) | (1<<1) | (1<<2);                                           // A B C
    case 8: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6);     // A B C D E F G
    default: return (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<5) | (1<<6);             // 9: A B C D F G
  }
}

// Shift out 16 bits to two daisy-chained 74HC595:
// sr1 data goes to sevseg1, sr2 to sevseg2.
// Wired: sr1:DS -> sr2:SHCP via SHCP tie, sr1:Q7S -> sr2:DS (SER chain).
// That means when we clock 16 bits with DS, the first 8 bits end up in sr2 and the next 8 in sr1
// depending on shift direction. To ensure correctness with the given chain, we send sr2 first then sr1,
// using MSB-first shifting consistent with typical 74HC595 usage in Arduino libraries.
// We'll implement explicit bit order: for each bit i from 15 downto 0, pulse DS with (data>>i)&1.
static void srWrite16(uint16_t data16) {
  // Ensure STCP low during shifting
  digitalWrite(SR1_STCP, LOW);

  // Shift 16 bits MSB -> LSB
  for (int8_t i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (data16 & (1UL << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
    // small hold time
    delayMicroseconds(1);
    digitalWrite(SR1_SHCP, LOW);
  }

  // Latch
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, LOW);
}

static void setBothDisplays(uint8_t tens, uint8_t ones) {
  // Compose segment bytes for each display
  // sevseg1 driven by sr1 outputs Q0..Q6 -> A..G
  uint8_t seg1 = digitToSegmentsCommonCathode(tens) & 0x7F; // bits 0..6
  uint8_t seg2 = digitToSegmentsCommonCathode(ones) & 0x7F;

  // Build 16-bit word: [sr1 byte][sr2 byte] in our shift order assumptions.
  // To match the daisy-chain: sr1 outputs are farther from DS than sr2.
  // With typical MSB-first and 16 shifts, sending sr1 byte in lower bits ensures it lands in sr1.
  // We'll pack as: high byte -> sr1, low byte -> sr2, then output as MSB-first.
  uint16_t word = ((uint16_t)seg1 << 8) | (uint16_t)seg2;

  srWrite16(word);
}

static void setLEDs(uint8_t ledState) {
  // ledState: 1->only LED1, 2->LED1+LED2, 3->all
  digitalWrite(LED1_PIN, (ledState >= 1) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (ledState >= 2) ? HIGH : LOW);
  digitalWrite(LED3_PIN, (ledState >= 3) ? HIGH : LOW);
}

void setup() {
  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Start with LEDs OFF
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // Shift register control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  // Default levels
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Initial display "00"
  setBothDisplays(0, 0);

  // Pot input
  pinMode(POT_PIN, INPUT);

  // Keep state stable at least 2 seconds after power/reset for verification
  delay(2000);
}

void loop() {
  // Read potentiometer and map to 0..99 for display
  int pot = analogRead(POT_PIN); // 0..1023
  int mapped = map(pot, 0, 1023, 0, 99);
  if (mapped < 0) mapped = 0;
  if (mapped > 99) mapped = 99;

  uint8_t tens = (uint8_t)(mapped / 10);
  uint8_t ones = (uint8_t)(mapped % 10);

  // LED logic based on mapped value ranges
  // 0..33 -> only LED1
  // 34..66 -> LED1+LED2
  // 67..99 -> all three
  uint8_t ledState;
  if (mapped <= 33) ledState = 1;
  else if (mapped <= 66) ledState = 2;
  else ledState = 3;

  // Update LEDs and displays
  setLEDs(ledState);
  setBothDisplays(tens, ones);

  // State maintenance: at least 2 seconds per state for verification
  delay(2000);
}
