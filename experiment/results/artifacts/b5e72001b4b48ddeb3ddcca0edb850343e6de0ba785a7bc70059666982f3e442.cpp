#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Pin mapping from provided wiring (Arduino Mega 2560)
// sr1
static const uint8_t SR1_SHCP = A1;   // purple
static const uint8_t SR1_STCP = A0;   // blue? (actually DS is blue on sr1:DS from mega:A0; purple is STCP from mega:A1)
static const uint8_t SR1_DS   = A0;   // blue
static const uint8_t SR1_LATCH = A1;  // STCP purple (mega:A1 -> sr1:STCP)
static const uint8_t SR1_CLOCK = A2;  // SHCP gray (mega:A2 -> sr1:SHCP)
static const uint8_t SR1_OE_PIN = 0;   // not directly wired to MCU

// sr2 shares SHCP and STCP
// DS is connected via sr1:Q7S to sr2:DS (internal serial chaining)

// NOTE: From the connections list:
//   mega:A2 -> sr1:SHCP
//   mega:A1 -> sr1:STCP
//   mega:A0 -> sr1:DS
// We'll use these MCU pins:
static const uint8_t MCU_DS   = A0; // sr1:DS (blue)
static const uint8_t MCU_STCP = A1; // sr1:STCP (purple)
static const uint8_t MCU_SHCP = A2; // sr1:SHCP (gray)

// Control lines: sr1:MR tied to sr1:VCC, sr1:OE tied to GND
// => No MCU control required for OE/MR; OE is always enabled, MR always inactive.

static const uint8_t DIGIT_SEGMENTS[10] = {
  // Segment order for outputs Q0..Q6:
  // Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // Seven-seg convention assumed: '1' turns segment ON.
  // For common requirement: display "0" at startup with sevseg2 showing "0"
  // We'll use standard patterns for common-cathode style.
  // Bit0=A, Bit1=B, Bit2=C, Bit3=D, Bit4=E, Bit5=F, Bit6=G
  0b0111111, // 0: A B C D E F
  0b0000110, // 1:     B C
  0b1011011, // 2: A B   D E   G
  0b1001111, // 3: A B C D     G
  0b1100110, // 4:   B C     E G
  0b1101101, // 5: A   C D   F G
  0b1111101, // 6: A   C D E F G
  0b0000111, // 7: A B C
  0b1111111, // 8: A B C D E F G
  0b1101111  // 9: A B C D   F G
};

static const uint8_t BLANK = 0b0000000; // all segments off

// Initialize both cascaded 74HC595 outputs: sr1 controls sevseg1, sr2 controls sevseg2.
// With chaining: shift register data stream goes first into sr1 (Q7S feeds sr2:DS).
// When we clock SR1 then SR2 then latch, the order of bytes pushed determines which end is updated.
// For 2 chained registers: first shifted byte ends up in sr2? Let's derive:
// Typical behavior: data bit shifted into DS goes into sr1's SER input, then on each SHCP it
// advances toward Q7S, feeding the next register. After 8 shifts, the byte that was shifted
// first resides in the last register (sr2), while the byte shifted last resides in sr1.
// However, because only one bitstream is used, we choose an order that matches the desired behavior:
// We'll explicitly write by testing logic: At startup, sevseg1 extinguished (blank), sevseg2 shows "0".
// We'll implement the update so that we push sr2 byte first, then sr1 byte.
//
// For two bytes:
//   shiftByte(sr2) then shiftByte(sr1), then latch -> sr2 outputs correspond to first byte,
//   sr1 outputs correspond to second byte.

#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pulseLatch();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftByte(uint8_t val);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void writeDisplays(uint8_t digit1, uint8_t digit2, bool sevseg1Blank);
#line 91 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_15\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void pulseLatch() {
  digitalWrite(MCU_STCP, HIGH);
  delayMicroseconds(5);
  digitalWrite(MCU_STCP, LOW);
}

static void shiftByte(uint8_t val) {
  // Shift MSB first (typical for 74HC595 when using bitwise order)
  for (int i = 7; i >= 0; --i) {
    digitalWrite(MCU_DS, (val & (1u << i)) ? HIGH : LOW);
    delayMicroseconds(1);
    digitalWrite(MCU_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(MCU_SHCP, LOW);
  }
}

static void writeDisplays(uint8_t digit1, uint8_t digit2, bool sevseg1Blank) {
  uint8_t sev1 = sevseg1Blank ? BLANK : DIGIT_SEGMENTS[digit1 % 10];
  uint8_t sev2 = DIGIT_SEGMENTS[digit2 % 10];

  // Since Q7S from sr1 -> ds of sr2, we push sr2 first, then sr1.
  shiftByte(sev2);
  shiftByte(sev1);
  pulseLatch();
}

void setup() {
  pinMode(MCU_DS, OUTPUT);
  pinMode(MCU_STCP, OUTPUT);
  pinMode(MCU_SHCP, OUTPUT);

  digitalWrite(MCU_DS, LOW);
  digitalWrite(MCU_STCP, LOW);
  digitalWrite(MCU_SHCP, LOW);

  // Upon powering on/reset:
  // - sevseg1 extinguished state
  // - sevseg2 shows "0"
  // Timer starts at 0 seconds.
  writeDisplays(0, 0, true);

  // Timer initialization
  // Start counting from now; every real second add 6 seconds to the displayed timer.
}

void loop() {
  static bool initialized = false;
  static unsigned long lastMs = 0;
  static uint16_t timerSeconds = 0; // internal timer in seconds, increments by 6 per real second

  if (!initialized) {
    initialized = true;
    lastMs = millis();
    timerSeconds = 0; // display "0" initially on sevseg2
  }

  unsigned long now = millis();
  while (now - lastMs >= 1000UL) {
    lastMs += 1000UL;
    timerSeconds = (uint16_t)(timerSeconds + 6);
  }

  // Display rules:
  // If timer exceeds 99, display only the last two digits on both displays.
  // The prompt example says: if timer reaches 102 seconds, displays should show "2".
  // With only two 7-seg displays, interpret this as showing "02":
  // sevseg1 = 0, sevseg2 = 2. (and sevseg1 is NOT "blank" in that case)
  uint8_t d1 = 0, d2 = 0;

  if (timerSeconds <= 99) {
    d1 = (timerSeconds / 10) % 10;
    d2 = timerSeconds % 10;
    // sevseg1 is not extinguished during timer operation (except startup).
    writeDisplays(d1, d2, false);
  } else {
    uint8_t lastTwo = (uint8_t)(timerSeconds % 100);
    d1 = (lastTwo / 10) % 10;
    d2 = lastTwo % 10;
    // "only the last two digits" -> show them, including leading zero if needed.
    writeDisplays(d1, d2, false);
  }

  // Small delay to reduce flicker; still updates each second due to millis() logic.
  delay(5);
}
