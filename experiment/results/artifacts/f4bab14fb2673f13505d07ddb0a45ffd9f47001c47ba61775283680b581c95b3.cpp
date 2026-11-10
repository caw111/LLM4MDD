#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t SR_CLOCK = A2; // SHCP
static const uint8_t SR_LATCH = A1; // STCP
static const uint8_t SR_DATA  = A0; // DS

// 74HC595 segment pins mapping (as per circuit):
// sr1 outputs to sevseg1 segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Active-low for segments: each segment bit is inverted in shift value.

static const uint8_t BAR_PINS[10] = {D5, D6, D7, D8, D9, D10, D11, D12, D13, D44};

// RGB PWM pins (as per contract)
static const uint8_t RGB_R_PIN = D2; // io_0
static const uint8_t RGB_G_PIN = D3; // io_1
static const uint8_t RGB_B_PIN = D4; // io_2

// Controller state (must match exact model)
static int32_t number = 0;          // initial 0
static int32_t accumulatedMs = 0;   // initial 0

// timing source for io_5 (delta_since_previous_step)
static uint32_t lastTickMs = 0;

// Shift register: send 8 bits, with bits 0..6 representing A..G (active-low).
// Segment active bits list in model: segment_bits [0..6] are used, digit 0 is only digit.
// For common-cathode display typical mapping differs; here we follow the model's expected bits.
// We'll implement standard 7-seg patterns but inverted due to active_low= true.
// Common Cathode + active_low segments via 74HC595 means: segment ON => output LOW.
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegmentsMask(uint8_t d);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595Write(uint8_t value);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595WriteLSBFirst(uint8_t value);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void write7SegNumber(uint8_t d);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeBarGraphForNumber(uint8_t n);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeRGBForNumber(uint8_t n);
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 169 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegmentsMask(uint8_t d) {
  // bits: [0:A,1:B,2:C,3:D,4:E,5:F,6:G]
  // Standard segments for digits 0..9 (ON for common-cathode segments):
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  bool A=false,B=false,C=false,Dd=false,E=false,F=false,G=false;
  switch(d) {
    case 0: A=true;B=true;C=true;Dd=true;E=true;F=true;G=false; break;
    case 1: A=false;B=true;C=true;Dd=false;E=false;F=false;G=false; break;
    case 2: A=true;B=true;C=false;Dd=true;E=true;F=false;G=true; break;
    case 3: A=true;B=true;C=true;Dd=true;E=false;F=false;G=true; break;
    case 4: A=false;B=true;C=true;Dd=false;E=false;F=true;G=true; break;
    case 5: A=true;B=false;C=true;Dd=true;E=false;F=true;G=true; break;
    case 6: A=true;B=false;C=true;Dd=true;E=true;F=true;G=true; break;
    case 7: A=true;B=true;C=true;Dd=false;E=false;F=false;G=false; break;
    case 8: A=true;B=true;C=true;Dd=true;E=true;F=true;G=true; break;
    default: // 9
      A=true;B=true;C=true;Dd=true;E=false;F=true;G=true; break;
  }

  uint8_t activeMask = 0;
  if (A)  activeMask |= (1u<<0);
  if (B)  activeMask |= (1u<<1);
  if (C)  activeMask |= (1u<<2);
  if (Dd) activeMask |= (1u<<3);
  if (E)  activeMask |= (1u<<4);
  if (F)  activeMask |= (1u<<5);
  if (G)  activeMask |= (1u<<6);

  // Active-low outputs: segment ON => bit LOW.
  // shift595_number uses "active_low": true, so output bit should be 0 when segment is active.
  // We produce the actual bit pattern for the shift register outputs (bit=1 means HIGH).
  uint8_t outBits = (~activeMask) & 0x7F; // keep bits 0..6, invert
  // Bit 7 unused -> keep HIGH (1) so it stays off.
  outBits |= (1u<<7);
  return outBits;
}

static void shift595Write(uint8_t value) {
  // Using rising edge sensitive SR: set DATA, pulse CLOCK, then latch.
  // The model doesn't specify edge polarity; standard is pulse SHCP HIGH.
  // We'll clock MSB first for consistent 8-bit shifting in typical 595 usage.
  // Note: For Wokwi, either order might matter; but with our chosen mapping,
  // we ensure Q0..Q6 correspond to bit0..bit6 if Wokwi uses bit order as sent.
  // In most 74HC595 serial-in-parallel-out, the first shifted bit ends up at QA (Q0)
  // after 8 shifts when shifting MSB-first? Ambiguous.
  // We'll shift LSB-first so that bit0 corresponds to Q0 (as per model segment_bits).
  // Therefore: shift LSB, then next... for 8 bits.
  (void)value;
}

static void shift595WriteLSBFirst(uint8_t value) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_CLOCK, LOW);
    digitalWrite(SR_DATA, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
  }
  digitalWrite(SR_LATCH, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_LATCH, LOW);
}

static void write7SegNumber(uint8_t d) {
  // Digit 0 only
  uint8_t v = digitToActiveLowSegmentsMask(d);
  shift595WriteLSBFirst(v);
}

static void writeBarGraphForNumber(uint8_t n) {
  // Model: io_4_segment_0..8 = number>=k (k=1..9), io_4_segment_9 = false
  // So for count N (0..9): first N segments (0..N-1) are ON.
  // With pins ordered D5..D13 then D44.
  for (uint8_t i = 0; i < 10; i++) {
    bool on = false;
    if (i < 9) {
      // segment i corresponds to number >= (i+1)
      on = (n >= (i + 1));
    } else {
      // segment_9 stays false always
      on = false;
    }
    digitalWrite(BAR_PINS[i], on ? HIGH : LOW);
  }
}

static void writeRGBForNumber(uint8_t n) {
  // Model outputs:
  // io_0 (red)  = 255 if n<=3 else 0
  // io_1 (green)= 255 if 4<=n<=6 else 0
  // io_2 (blue) = 255 if 7<=n<=9 else 0
  uint8_t r = (n <= 3) ? 255 : 0;
  uint8_t g = (n >= 4 && n <= 6) ? 255 : 0;
  uint8_t b = (n >= 7 && n <= 9) ? 255 : 0;

  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);
}

static void applyOutputs() {
  uint8_t n = (uint8_t)(number & 0xFF);

  writeRGBForNumber(n);
  write7SegNumber(n);
  writeBarGraphForNumber(n);
}

void setup() {
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_DATA, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Ensure RGB pins configured for PWM
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  lastTickMs = millis();

  // Startup requirements from model initial states:
  // number=0, accumulatedMs=0
  // io_0 red intensity >0, io_1=0, io_2=0; sevseg shows 0; bar all off
  applyOutputs();
}

void loop() {
  // Control period 10ms; model uses io_5 = elapsed_ms delta since previous step.
  // We'll run as fast as needed but compute delta each iteration; actual tick updates happen each 10ms-ish.
  // To preserve deterministic stepping cadence, we pace to 10ms.
  static uint32_t nextDue = 0;
  uint32_t now = millis();
  if (nextDue == 0) nextDue = now;

  if ((int32_t)(now - nextDue) < 0) {
    // not due yet
    return;
  }
  nextDue += 10;

  uint32_t curMs = millis();
  int32_t io_5 = (int32_t)(curMs - lastTickMs);
  lastTickMs = curMs;
  if (io_5 < 0) io_5 = 0;

  // Model tick updates (exact order)
  if (io_5 > 0) {
    accumulatedMs = accumulatedMs + io_5;
  }

  if (accumulatedMs >= 2000) {
    accumulatedMs = accumulatedMs - 2000;
    if (number == 9) number = 0;
    else number = number + 1;
  }

  applyOutputs();
}
