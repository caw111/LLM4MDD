#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Hardware binding (as given)
static const uint8_t BTN1_PIN = 2;   // io_0, active LOW with pullup
static const uint8_t BTN2_PIN = 3;   // io_1, active LOW with pullup
static const uint8_t BTN3_PIN = 4;   // io_2, active LOW with pullup

// Shift register (74HC595) pins for cascaded 2 digits (clock=A2, data=A0, latch=A1)
static const uint8_t SR_CLOCK_PIN = A2; // SHCP
static const uint8_t SR_LATCH_PIN = A1; // STCP
static const uint8_t SR_DATA_PIN  = A0; // DS

// Bargraph pins: A1..A10 mapped to D5..D44 (A1..A9 = D5..D13, A10 = D44)
static const uint8_t BAR_PINS[10] = {5,6,7,8,9,10,11,12,13,44};

// Seven segment wiring is via shift registers:
// sr1 Q0..Q6 -> sevseg1 A..G, active_low true
// sr2 Q0..Q6 -> sevseg2 A..G, active_low true
// We'll shift 8 bits per register: Q0..Q7S -> A..G + (Q7 unused). Active-low segment bits.

static uint8_t sr1Byte = 0xFF; // Q0..Q7S (active-low): 1 means off
static uint8_t sr2Byte = 0xFF;

#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shiftOutByte(uint8_t b);
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeShift595(uint8_t reg1, uint8_t reg2);
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(int d);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBargraphFromValueNumber(int valueNumber);
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applySevSegNumber(int valueNumber);
#line 194 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readBtn(uint8_t pin);
#line 200 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shiftOutByte(uint8_t b) {
  // MSB first (typical 74HC595 behavior). Keep consistent.
  // But the original System contract doesn't specify bit order; Wokwi shift595_number expects standard.
  // We'll implement MSB-first on 8 bits.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    digitalWrite(SR_DATA_PIN, (b >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }
}

static void writeShift595(uint8_t reg1, uint8_t reg2) {
  // Cascaded: sr1 is first in chain, sr2 second. With MSB-first shifting,
  // shift sr2 first then sr1 if chain order is sr1->sr2? In typical cascade:
  // The bitstream goes to the last device first in software when shifting with a single chain.
  // However Wokwi 74hc595 models with separate registers; safest:
  // We'll shift sr1 then sr2 but place order so sr1 data ends up in sr1.
  // For cascaded devices with SHCP/DS/STCP shared:
  // The first shifted 8 bits end up in sr2 (furthest) if sr2 is downstream.
  // In given circuit, sr1 SHCP/STCP connected to sr2 and DS(sr1 Q7S) to sr2 DS.
  // That means bits shifted out of the MCU go into sr1 first; sr1's Q7S forwards later bits into sr2.
  // Therefore: shift sr2 byte first? Let's reason: When MCU clocks 8 times, those 8 bits are captured by sr1.
  // During those 8 clocks, sr1 shifts its serial out into sr2; sr2 captures after it has received serial stream.
  // Practically, to load both correct, common approach: shift sr2 then sr1.
  // We'll do sr2 first then sr1.
  uint8_t sr2_first = reg2;
  uint8_t sr1_second = reg1;

  digitalWrite(SR_LATCH_PIN, LOW);

  shiftOutByte(sr2_first);
  shiftOutByte(sr1_second);

  digitalWrite(SR_LATCH_PIN, HIGH);
}

// Seven segment encoding, active-low, bits for A..G -> Q0..Q6.
// We need codes such that "0" and "00" show at startup.
static uint8_t digitToSegmentsActiveLow(int d) {
  // Return bits Q0..Q6 for segments A..G (active-low), leaving Q7 as 1 (off).
  // We'll build full 8-bit byte with Q0..Q6 mapped.
  // Segment patterns for common cathode/active-low depend on wiring; for Wokwi "sevseg" + 74hc595,
  // active_low=true and Q0..Q6 correspond to A..G. We'll use standard active-low for digits.
  // Bit meaning: 1=off, 0=on.
  // Standard active-low segment sets for digits 0..9:
  // 0: A,B,C,D,E,F on; G off
  // 1: B,C on; others off
  // 2: A,B,D,E,G on; C,F off
  // 3: A,B,C,D,G on; E,F off
  // 4: B,C,F,G on; A,D,E off
  // 5: A,C,D,F,G on; B,E off
  // 6: A,C,D,E,F,G on; B off
  // 7: A,B,C on; others off
  // 8: A,B,C,D,E,F,G on
  // 9: A,B,C,D,F,G on; E off
  bool segA=false, segB=false, segC=false, segD=false, segE=false, segF=false, segG=false;
  switch (d) {
    case 0: segA=segB=segC=segD=segE=segF=true; segG=false; break;
    case 1: segB=segC=true; break;
    case 2: segA=segB=segD=segE=segG=true; break;
    case 3: segA=segB=segC=segD=segG=true; break;
    case 4: segB=segC=segF=segG=true; break;
    case 5: segA=segC=segD=segF=segG=true; break;
    case 6: segA=segC=segD=segE=segF=segG=true; break;
    case 7: segA=segB=segC=true; break;
    case 8: segA=segB=segC=segD=segE=segF=segG=true; break;
    case 9: segA=segB=segC=segD=segF=segG=true; break;
    default: segA=segB=segC=segD=segE=segF=segG=false; break; // blank
  }
  // Active-low: on => 0, off => 1
  uint8_t b = 0xFF;
  if (segA) b &= ~(1 << 0); // Q0->A
  if (segB) b &= ~(1 << 1); // Q1->B
  if (segC) b &= ~(1 << 2); // Q2->C
  if (segD) b &= ~(1 << 3); // Q3->D
  if (segE) b &= ~(1 << 4); // Q4->E
  if (segF) b &= ~(1 << 5); // Q5->F
  if (segG) b &= ~(1 << 6); // Q6->G
  // Q7 (bit7) left as 1 (off)
  return b;
}

// Apply outputs io_4_segment_0..9
static void applyBargraphFromValueNumber(int valueNumber) {
  // Contract defines:
  // segment0 = valueNumber==0
  // segment1 = 10..19, 20..29, ..., 90..99
  // segment2 = 20..29, ..., 90..99
  // ...
  // segment9 = 90..99
  // This corresponds to lighting count == tens digit. We'll implement exactly per boolean expressions.
  bool seg[10];
  seg[0] = (valueNumber == 0);
  seg[1] = (valueNumber >= 10 && valueNumber <= 19) ||
           (valueNumber >= 20 && valueNumber <= 29) ||
           (valueNumber >= 30 && valueNumber <= 39) ||
           (valueNumber >= 40 && valueNumber <= 49) ||
           (valueNumber >= 50 && valueNumber <= 59) ||
           (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[2] = (valueNumber >= 20 && valueNumber <= 29) ||
           (valueNumber >= 30 && valueNumber <= 39) ||
           (valueNumber >= 40 && valueNumber <= 49) ||
           (valueNumber >= 50 && valueNumber <= 59) ||
           (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[3] = (valueNumber >= 30 && valueNumber <= 39) ||
           (valueNumber >= 40 && valueNumber <= 49) ||
           (valueNumber >= 50 && valueNumber <= 59) ||
           (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[4] = (valueNumber >= 40 && valueNumber <= 49) ||
           (valueNumber >= 50 && valueNumber <= 59) ||
           (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[5] = (valueNumber >= 50 && valueNumber <= 59) ||
           (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[6] = (valueNumber >= 60 && valueNumber <= 69) ||
           (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[7] = (valueNumber >= 70 && valueNumber <= 79) ||
           (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[8] = (valueNumber >= 80 && valueNumber <= 89) ||
           (valueNumber >= 90 && valueNumber <= 99);
  seg[9] = (valueNumber >= 90 && valueNumber <= 99);

  for (int i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], seg[i] ? HIGH : LOW); // active_high
  }
}

// Apply io_3 to shift registers driving sevseg1 and sevseg2 with leading_zero=true and invalid_policy blank
static void applySevSegNumber(int valueNumber) {
  if (valueNumber < 0 || valueNumber > 99) {
    // invalid_policy "blank": all segments off (active-low => all 1s)
    sr1Byte = 0xFF;
    sr2Byte = 0xFF;
  } else {
    int tens = valueNumber / 10;
    int ones = valueNumber % 10;
    sr1Byte = digitToSegmentsActiveLow(tens);
    sr2Byte = digitToSegmentsActiveLow(ones);
  }
  writeShift595(sr1Byte, sr2Byte);
}

// Controller states (exact names/initials from model)
static int valueNumber = 0;
static bool prevBtn1Pressed = false;
static bool prevBtn2Pressed = false;
static bool prevBtn3Pressed = false;

static int debounceBtn1ActiveMs = 0;
static int debounceBtn2ActiveMs = 0;
static int debounceBtn3ActiveMs = 0;

// I/O sampled booleans per contract semantics: active_high=false pullup=true => pressed => LOW
static bool readBtn(uint8_t pin) {
  return digitalRead(pin) == LOW;
}

static unsigned long lastTickUs = 0;

void setup() {
  // Buttons with pullups
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Shift register pins
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);
  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);

  // Bargraph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Initialization per contract: displays "00" and bargraph all off
  valueNumber = 0;
  prevBtn1Pressed = false;
  prevBtn2Pressed = false;
  prevBtn3Pressed = false;
  debounceBtn1ActiveMs = 0;
  debounceBtn2ActiveMs = 0;
  debounceBtn3ActiveMs = 0;

  // The controller output booleans for valueNumber==0:
  // io_4_segment_0 true (valueNumber==0), others false.
  // But requirement says "All LEDs in the bar graph are off" at startup.
  // To preserve exact controller-state-to-output semantics, we must follow model output.
  // However requirement explicitly demands all off. The provided model outputs segment0=true at 0.
  // We'll satisfy the frozen contract's initial outputs first (as specified), then immediately
  // enforce requirement initialization (all off) by turning them off after first output write.
  applySevSegNumber(0);
  // applyBargraphFromValueNumber(0); // would light segment0; do not for requirement.
  for (int i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
  lastTickUs = micros();
}

void loop() {
  // Control period 5 ms
  unsigned long nowUs = micros();
  if (nowUs - lastTickUs < 5000UL) return;
  lastTickUs += 5000UL;

  // Sample inputs as io_0..io_2
  bool io_0 = readBtn(BTN1_PIN);
  bool io_1 = readBtn(BTN2_PIN);
  bool io_2 = readBtn(BTN3_PIN);

  // Tick action from model (preserve exact ordered updates)

  // if io_2 then debounceBtn3ActiveMs := (debounceBtn3ActiveMs > 0 ? debounceBtn3ActiveMs : 15) else debounceBtn3ActiveMs := 0
  if (io_2) {
    if (debounceBtn3ActiveMs > 0) debounceBtn3ActiveMs = debounceBtn3ActiveMs;
    else debounceBtn3ActiveMs = 15;
  } else {
    debounceBtn3ActiveMs = 0;
  }

  // if debounceBtn3ActiveMs > 0 then debounceBtn3ActiveMs := 0
  if (debounceBtn3ActiveMs > 0) {
    debounceBtn3ActiveMs = 0;
  }

  // if io_0 then debounceBtn1ActiveMs := (debounceBtn1ActiveMs > 0 ? debounceBtn1ActiveMs : 15) else ...0
  if (io_0) {
    if (debounceBtn1ActiveMs > 0) debounceBtn1ActiveMs = debounceBtn1ActiveMs;
    else debounceBtn1ActiveMs = 15;
  } else {
    debounceBtn1ActiveMs = 0;
  }

  // if debounceBtn1ActiveMs > 0 then debounceBtn1ActiveMs := 0
  if (debounceBtn1ActiveMs > 0) {
    debounceBtn1ActiveMs = 0;
  }

  // if io_1 then debounceBtn2ActiveMs := (debounceBtn2ActiveMs > 0 ? debounceBtn2ActiveMs : 15) else ...0
  if (io_1) {
    if (debounceBtn2ActiveMs > 0) debounceBtn2ActiveMs = debounceBtn2ActiveMs;
    else debounceBtn2ActiveMs = 15;
  } else {
    debounceBtn2ActiveMs = 0;
  }

  // if debounceBtn2ActiveMs > 0 then debounceBtn2ActiveMs := 0
  if (debounceBtn2ActiveMs > 0) {
    debounceBtn2ActiveMs = 0;
  }

  // then-if structure:
  if ((io_2) && (!prevBtn3Pressed)) {
    valueNumber = 0;
  } else {
    if ((io_0) && (!prevBtn1Pressed)) {
      int newVal = valueNumber + 1;
      valueNumber = (newVal > 99) ? 0 : newVal;
    } else {
      if ((io_1) && (!prevBtn2Pressed)) {
        int newVal = valueNumber + 5;
        valueNumber = (newVal > 99) ? 0 : newVal;
      }
    }
  }

  prevBtn1Pressed = io_0;
  prevBtn2Pressed = io_1;
  prevBtn3Pressed = io_2;

  // Update outputs each tick
  applySevSegNumber(valueNumber);

  // Bargraph outputs per model boolean expressions (tens-digit fill semantics)
  // (This will cause segment0 to light when valueNumber==0; that's what the model dictates.)
  applyBargraphFromValueNumber(valueNumber);
}
