#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_DS_TENS = A0;     // sr1 DS
static const uint8_t PIN_SHCP      = A2;     // shared SHCP (shift clock)
static const uint8_t PIN_STCP      = A1;     // shared STCP (latch clock)

// Button: D2, INPUT_PULLUP (LOW when pressed)
static const uint8_t PIN_BTN       = 2;

static const uint8_t DIGIT_TO_7SEG[10] = {
  // a b c d e f g (active HIGH segments, common anode)
  // 0: a b c d e f = 1, g = 0
  // We'll map using typical 7-seg for common anode.
  // a b c d e f g
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftWriteDigit(uint8_t sr1_state, uint8_t sr2_state);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayNumber(uint8_t value);
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_16\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void shiftWriteDigit(uint8_t sr1_state, uint8_t sr2_state) {
  // sr1_state controls Q0..Q7 of sr1 (we use Q0..Q6 only for segments a..g).
  // sr2_state controls Q0..Q7 of sr2 (we use Q0..Q6 only for segments a..g).
  //
  // Wiring notes:
  // - mega:A0 -> sr1 DS (sr1 DS)
  // - sr1:SHCP/STCP shared with sr2
  // - sr1 Q7S -> sr2 DS
  // Therefore, we must shift 16 bits: 8 for sr1 then 8 for sr2.
  // With Q7 in each SR left unused, we force Q7=0 bits.
  //
  // Bit order for 74HC595 usually: shift in MSB-first? In Arduino typical wiring,
  // but no ordering specified in circuit. We will implement a common assumption:
  // SHCP clocks Q7S from Q7 -> SR2 DS after each clock and shifts towards Q0.
  //
  // To ensure correct mapping to Q0..Q6 -> segments A..G, we will shift LSB-first
  // so that first shifted bit ends up at Q0. We'll do 16 clocks with LSB-first.
  //
  // sr_state bit positions:
  // bit0 -> Q0 (segment A)
  // bit1 -> Q1 (segment B)
  // ...
  // bit6 -> Q6 (segment G)
  // bit7 -> Q7 (unused)
  uint16_t combined = ((uint16_t)sr2_state << 8) | (uint16_t)sr1_state;

  // Latch low then shift
  digitalWrite(PIN_STCP, LOW);

  // 16 clocks, LSB-first of combined, 74HC595 samples DS on rising edge of SHCP.
  for (uint8_t i = 0; i < 16; i++) {
    digitalWrite(PIN_SHCP, LOW);
    digitalWrite(PIN_DS_TENS, (combined >> i) & 0x01);
    digitalWrite(PIN_SHCP, HIGH);
  }

  // Latch high to update outputs
  digitalWrite(PIN_STCP, HIGH);
  digitalWrite(PIN_STCP, LOW);
}

static void displayNumber(uint8_t value) {
  uint8_t tens = value / 10;
  uint8_t units = value % 10;

  uint8_t segTens  = DIGIT_TO_7SEG[tens];  // Q0..Q6 -> a..g for sr1
  uint8_t segUnits = DIGIT_TO_7SEG[units]; // Q0..Q6 -> a..g for sr2

  // Ensure Q7 bits are 0 (unused)
  segTens &= 0x7F;
  segUnits &= 0x7F;

  shiftWriteDigit(segTens, segUnits);
}

static bool lastBtnPressed = false;

void setup() {
  pinMode(PIN_DS_TENS, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);

  pinMode(PIN_BTN, INPUT_PULLUP);

  // Initial states
  digitalWrite(PIN_DS_TENS, LOW);
  digitalWrite(PIN_SHCP, LOW);
  digitalWrite(PIN_STCP, LOW);

  displayNumber(0); // "00"
  lastBtnPressed = (digitalRead(PIN_BTN) == LOW);
}

void loop() {
  bool btnPressed = (digitalRead(PIN_BTN) == LOW);

  // Detect press edge: "each press ... not long press and not on release"
  if (btnPressed && !lastBtnPressed) {
    // increment by 37; if exceeds 99 reset to 0
    // Keep value in static so it's preserved across loop iterations.
    static uint8_t value = 0;
    uint16_t next = (uint16_t)value + 37;

    if (next > 99) {
      value = 0;
    } else {
      value = (uint8_t)next;
    }

    displayNumber(value); // immediate update (within 20ms by design)
  }

  lastBtnPressed = btnPressed;
}
