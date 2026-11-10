#include <Arduino.h>

#define SR_LATCH_PIN  A1   // STCP (purple)
#define SR_CLOCK_PIN  A2   // SHCP (gray)
#define SR_DATA_PIN   A0   // DS   (blue)

static const uint8_t SEGMENT_OFF = 0x00;

// Segment-to-bit mapping (common for 7-seg with 74HC595):
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// This assumes segments are active-HIGH.
static uint8_t charToSegments(char c) {
  // Bits: [G F E D C B A]?? No, we map directly to Q0..Q6 -> A..G:
  // Q0=A bit0, Q1=B bit1, Q2=C bit2, Q3=D bit3, Q4=E bit4, Q5=F bit5, Q6=G bit6
  // So byte bits correspond: b0=A, b1=B, b2=C, b3=D, b4=E, b5=F, b6=G
  uint8_t b = 0;

  auto setSeg = [&](char seg) {
    switch (seg) {
      case 'A': b |= (1 << 0); break;
      case 'B': b |= (1 << 1); break;
      case 'C': b |= (1 << 2); break;
      case 'D': b |= (1 << 3); break;
      case 'E': b |= (1 << 4); break;
      case 'F': b |= (1 << 5); break;
      case 'G': b |= (1 << 6); break;
    }
  };

  // Standard 7-segment encodings (active-HIGH):
  // 0: A B C D E F
  // A: A B C E F G
  // P: A B E F G
  switch (c) {
    case '0':
      setSeg('A'); setSeg('B'); setSeg('C'); setSeg('D'); setSeg('E'); setSeg('F');
      break;
    case 'A':
      setSeg('A'); setSeg('B'); setSeg('C'); setSeg('E'); setSeg('F'); setSeg('G');
      break;
    case 'P':
      setSeg('A'); setSeg('B'); setSeg('E'); setSeg('F'); setSeg('G');
      break;
    default:
      // Unknown -> all off
      break;
  }
  return b;
}

static void shiftOut595(uint8_t data) {
  // Shift MSB first is typical; for Q0..Q7 wiring, we will shift out bits 7..0.
  // We only use bits 0..6; bit7 will be 0.
  digitalWrite(SR_LATCH_PIN, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (data >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
  // Latch to output
  digitalWrite(SR_LATCH_PIN, HIGH);
}

void setup() {
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  // Start with display OFF
  shiftOut595(SEGMENT_OFF);

  // Ensure stable idle levels
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);
}

void loop() {
  const unsigned long intervalMs = 2000UL;
  unsigned long t = millis();

  uint8_t seg = SEGMENT_OFF;
  unsigned long phase = (t / intervalMs) % 3UL;

  if (phase == 0) seg = charToSegments('0');
  else if (phase == 1) seg = charToSegments('A');
  else seg = charToSegments('P');

  static uint8_t lastSeg = 0xFF;
  if (seg != lastSeg) {
    shiftOut595(seg);
    lastSeg = seg;
  }
}