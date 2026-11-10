#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PINS_BTN1 = 2;   // btn1: green to mega:2, other side to GND
static const uint8_t PINS_BTN2 = 3;   // btn2: green to mega:3, other side to GND
static const uint8_t PINS_BTN3 = 4;   // btn3: green to mega:4, other side to GND

// Bar graph 12 LEDs via Mega pins 5..44 -> actually mega:5,6,7,8,9,10,11,12,13,44 -> 10 pins total
// (From provided wiring)
static const uint8_t PINS_BAR[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};
static const uint8_t BAR_COUNT = sizeof(PINS_BAR) / sizeof(PINS_BAR[0]);

// 74HC595 + 2x7seg
// sr1:
// SHCP -> A2, STCP -> A1, DS -> A0
static const uint8_t SR1_SHCP = A2;
static const uint8_t SR1_STCP = A1;
static const uint8_t SR1_DS   = A0;

// sr1 control
static const uint8_t SR1_OE  = A3; // wired? Provided: sr1:OE to sr2:OE black and sr1:GND to OE black.
// But actual wiring isn't given to a Mega pin; OE is tied to GND. We'll just not control OE and keep it enabled.
// We'll implement assuming OE is tied LOW so outputs are always enabled.

// sr2 shares SHCP/STCP with sr1 via jumpers; OE tied low, MR tied high via VCC.
// sr2 DS is Q7S from sr1 (cascaded).
// In this cascade: shift 8 bits into sr1 (left digits), then sr2 will receive via Q7S.
// We must provide 16 bits total in correct order with one STCP pulse.
static const uint8_t SR_COMMON_SHCP = A2;
static const uint8_t SR_COMMON_STCP = A1;

// Segment outputs mapping on each 7-seg module:
// sr1 Q0..Q7S to sevseg1 A,B,C,D,E, F, G, DP(S). In provided connections:
// sr1:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S -> sr2:DS (cascade)

// sevseg2 segments:
// sr2 Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Therefore, we need bit order for each shift register's outputs Q0..Q6:
//
// We'll define digit segments as bits for A..G in that order mapped to Q0..Q6:
// bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
//
// Since wiring is generic and no polarity is specified, typical 7-seg with shift register expects HIGH=segment ON.
// We'll assume active-high segments, no decimal point used.

static const uint8_t DIGIT_SEG_A_TO_G[10] = {
  // A B C D E F G
  // 0: 1 1 1 1 1 1 0
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
  // 1: 0 1 1 0 0 0 0
  (1<<1)|(1<<2),
  // 2: 1 1 0 1 1 0 1
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
  // 3: 1 1 1 1 0 0 1
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
  // 4: 0 1 1 0 0 1 1
  (1<<1)|(1<<2)|(1<<5)|(1<<6),
  // 5: 1 0 1 1 0 1 1
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
  // 6: 1 0 1 1 1 1 1
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 7: 1 1 1 0 0 0 0
  (1<<0)|(1<<1)|(1<<2),
  // 8: 1 1 1 1 1 1 1
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
  // 9: 1 1 1 1 0 1 1
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
};

// Debounce parameters
static const uint32_t DEBOUNCE_MS = 35;     // mechanical debounce
static const uint32_t HOLD_CHECK_MS = 5;    // not necessary, but keeps loop responsive

struct Button {
  uint8_t pin;
  bool lastStableState;
  bool lastRawState;
  uint32_t lastChangeMs;
};

static Button btn1, btn2, btn3;

static int numberValue = 0;

#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initButton(Button &b, uint8_t pin);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDebouncedPressed(Button &b);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut16ToTwo595(uint8_t lowByte, uint8_t highByte);
#line 151 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void set7SegDigits(uint8_t tensDigit, uint8_t onesDigit);
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraphTens(uint8_t tensDigit);
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 196 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void initButton(Button &b, uint8_t pin) {
  b.pin = pin;
  pinMode(b.pin, INPUT_PULLUP); // buttons go to GND when pressed (from wiring)
  b.lastRawState = digitalRead(b.pin);
  b.lastStableState = b.lastRawState;
  b.lastChangeMs = millis();
}

static bool readDebouncedPressed(Button &b) {
  // Returns true once per press (rising edge from unpressed to pressed),
  // assuming active-low: pressed means LOW.
  bool raw = digitalRead(b.pin);

  if (raw != b.lastRawState) {
    b.lastRawState = raw;
    b.lastChangeMs = millis();
  }

  uint32_t now = millis();
  if ((now - b.lastChangeMs) >= DEBOUNCE_MS) {
    if (raw != b.lastStableState) {
      b.lastStableState = raw;
      // pressed edge: stable state becomes LOW
      if (b.lastStableState == LOW) return true;
    }
  }
  return false;
}

static void shiftOut16ToTwo595(uint8_t lowByte, uint8_t highByte) {
  // Cascade: sr1 outputs feed sr2. With two cascaded 74HC595s,
  // shifting 16 bits MSB-first will place the first 8 bits into sr2 and second 8 into sr1
  // depending on wiring. Our DS is connected to sr1:DS; sr1's Q7S goes to sr2:DS.
  //
  // To ensure sr1 gets 'lowByte' and sr2 gets 'highByte' in typical cascade:
  // - Shift out MSB first: the first 8 shifted bits end up in sr2.
  // - The next 8 shifted bits end up in sr1.
  //
  // We want sevseg1 (sr1) to display ones digit, and sevseg2 (sr2) to display tens digit
  // as implied by circuit: sevseg1 on sr1, sevseg2 on sr2.
  // Therefore:
  //   sr1 should receive ones digit segments -> lowByte
  //   sr2 should receive tens digit segments -> highByte
  //
  // We'll shift highByte first then lowByte.

  uint16_t data = (uint16_t(highByte) << 8) | uint16_t(lowByte);

  // MSB-first for 16 bits
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR_COMMON_SHCP, LOW);
    digitalWrite(SR1_DS, (data >> i) & 0x01 ? HIGH : LOW); // SR1_DS is common DS for cascade input
    // Pulse shift clock
    digitalWrite(SR_COMMON_SHCP, HIGH);
    // small delay for stability
    delayMicroseconds(2);
    digitalWrite(SR_COMMON_SHCP, LOW);
  }

  // Latch
  digitalWrite(SR_COMMON_STCP, LOW);
  delayMicroseconds(2);
  digitalWrite(SR_COMMON_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR_COMMON_STCP, LOW);
}

static void set7SegDigits(uint8_t tensDigit, uint8_t onesDigit) {
  // Each register provides bits Q0..Q6 to segments A..G.
  // Q7 is unused (sr1 Q7S goes to sr2 DS), but still must be shifted.
  // We'll set bit7=0.
  uint8_t segOnes = DIGIT_SEG_A_TO_G[onesDigit] & 0x7F;
  uint8_t segTens = DIGIT_SEG_A_TO_G[tensDigit] & 0x7F;

  // sr1 -> sevseg1 -> ones digit (lowByte)
  // sr2 -> sevseg2 -> tens digit (highByte)
  shiftOut16ToTwo595(segOnes, segTens);
}

static void setBarGraphTens(uint8_t tensDigit) {
  // bargraph1 lights LEDs equal to tens digit (0..9).
  // Wiring indicates 10 LEDs from pins: 5..13 and 44 (10 total).
  // We'll turn on the first tensDigit LEDs.
  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    digitalWrite(PINS_BAR[i], (i < tensDigit) ? HIGH : LOW);
  }
}

void setup() {
  pinMode(SR_COMMON_SHCP, OUTPUT);
  pinMode(SR_COMMON_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR_COMMON_SHCP, LOW);
  digitalWrite(SR_COMMON_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  for (uint8_t i = 0; i < BAR_COUNT; ++i) {
    pinMode(PINS_BAR[i], OUTPUT);
    digitalWrite(PINS_BAR[i], LOW); // All LEDs off initially
  }

  initButton(btn1, PINS_BTN1);
  initButton(btn2, PINS_BTN2);
  initButton(btn3, PINS_BTN3);

  // Initialization: display "00" and bar graph all off
  numberValue = 0;
  set7SegDigits(0, 0);
  setBarGraphTens(0);
}

void loop() {
  // Read debounced buttons (must update within 0.15s after valid press)
  // We'll check frequently.
  bool p1 = readDebouncedPressed(btn1);
  bool p2 = readDebouncedPressed(btn2);
  bool p3 = readDebouncedPressed(btn3);

  bool changed = false;

  if (p3) {
    numberValue = 0;
    changed = true;
  } else if (p1) {
    numberValue = (numberValue + 1 > 99) ? 0 : (numberValue + 1);
    changed = true;
  } else if (p2) {
    numberValue = (numberValue + 5 > 99) ? 0 : (numberValue + 5);
    changed = true;
  }

  if (changed) {
    uint8_t tens = (uint8_t)(numberValue / 10);
    uint8_t ones = (uint8_t)(numberValue % 10);

    set7SegDigits(tens, ones);
    setBarGraphTens(tens);
  }

  // Keep loop responsive; no long delays.
  // But we can yield briefly.
  delay(HOLD_CHECK_MS);
}
