#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ==== Hardware bindings from contract ====
static constexpr uint8_t POT_PIN = A3; // io_0
static constexpr uint8_t SR_CLOCK = A2; // io_1 contract clock_pin: A2 (SHCP)
static constexpr uint8_t SR_LATCH = A1; // io_1 contract latch_pin: A1 (STCP)
static constexpr uint8_t SR_DATA  = A0; // io_1 contract data_pin: A0 (DS)

// Bar graph outputs: indices 0..9 -> D2..D11
static constexpr uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // io_2..io_11

// ==== Shift register number encoding ====
/*
  Contract: shift595_number with invalid_policy="blank", leading_zero=true.
  Each digit uses segment_bits [0,1,2,3,4,5,6] mapped to inputs Q0..Q6:
    Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  active_low=true => we output LOW on active segments.

  segment_bits order corresponds to "digits" [segment a..g] in typical order:
    bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G, active low.
*/
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t segPatternActiveLow(char digit);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595Number(int value0to99);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_44\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t segPatternActiveLow(char digit) {
  // Returns 7-bit value for segments A..G, active-low (0=on, 1=off).
  // Common-cathode? Here we follow active_low contract (so 0 means segment lit).
  // Standard 7-seg segment definitions (a,b,c,d,e,f,g):
  // 0: a b c d e f
  // 1: b c
  // 2: a b d e g
  // 3: a b c d g
  // 4: b c f g
  // 5: a c d f g
  // 6: a c d e f g
  // 7: a b c
  // 8: a b c d e f g
  // 9: a b c d f g

  bool a=false,b=false,c=false,d=false,e=false,f=false,g=false;
  switch (digit) {
    case '0': a=b=c=d=e=f=true; g=false; break;
    case '1': b=c=true; a=d=e=f=g=false; break;
    case '2': a=b=d=e=true; c=false; f=false; g=true; break;
    case '3': a=b=c=d=true; e=false; f=false; g=true; break;
    case '4': b=c=f=true; a=d=e=false; g=true; break;
    case '5': a=c=d=f=true; b=false; e=false; g=true; break;
    case '6': a=c=d=e=f=true; b=false; g=true; break;
    case '7': a=b=c=true; d=e=f=false; g=false; break;
    case '8': a=b=c=d=e=f=g=true; break;
    case '9': a=b=c=d=f=true; e=false; g=true; break;
    default:  // invalid -> blank: all segments off
      a=b=c=d=e=f=g=false;
      break;
  }

  // active_low: output bit=0 when segment should be ON.
  // bit0=A ... bit6=G
  uint8_t bits = 0;
  bits |= (a ? 0 : 1) << 0;
  bits |= (b ? 0 : 1) << 1;
  bits |= (c ? 0 : 1) << 2;
  bits |= (d ? 0 : 1) << 3;
  bits |= (e ? 0 : 1) << 4;
  bits |= (f ? 0 : 1) << 5;
  bits |= (g ? 0 : 1) << 6;
  return bits & 0x7F;
}

static inline void srWriteByteLSBFirst(uint8_t v, bool clearBit7 = true) {
  // Hardware is two chained 74HC595s: sr1 feeds sevseg1 segments; sr2 feeds sevseg2 segments.
  // We shift 16 bits total: lower 8 bits correspond to sr1 outputs, upper 8 to sr2 outputs.
  // For this contract, only Q0..Q6 are used; Q7 (bit7) should be kept off (inactive-high),
  // matching our assumption of bits active_low: we set bit7=1 (off).
  if (clearBit7) v |= 0x80;

  // Shift MSB-first or LSB-first depends on wiring and shift register conventions.
  // In standard Arduino shiftOut, MSBFIRST is common; but contract doesn't specify.
  // We'll use MSB-first to match typical shiftOut default usage. Segment mapping:
  // Q0..Q6 correspond to outputs after 8 clocks; we construct the byte so that:
  //   bit0 -> Q0 ... bit6 -> Q6 with active_low encoded.
  // With MSB-first shifting, the first bit clocked ends up at MSB position.
  // To avoid ambiguity, we shift LSB-first so that bit0 clocks first to Q0 position.
  // Many Wokwi/74HC595 examples expect MSB-first; however contract defines Q0..Q6 mapping
  // with segment_bits [0..6]. We implement LSB-first to directly map bit indices -> Q indices.
  for (int i = 0; i < 8; i++) {
    digitalWrite(SR_DATA, (v & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    digitalWrite(SR_CLOCK, LOW);
  }
}

static inline void shift595Number(int value0to99) {
  int tens = value0to99 / 10;
  int units = value0to99 % 10;

  // Construct segment bytes for sr1(sevseg1 tens digit) and sr2(sevseg2 units digit).
  // Each byte uses active_low for bits 0..6, bit7 left off (1).
  uint8_t byteSr1 = segPatternActiveLow(char('0' + tens));  // sevseg1
  uint8_t byteSr2 = segPatternActiveLow(char('0' + units)); // sevseg2

  // Need 16-bit chained write:
  // The order of shifting into the chain determines which register receives which bits.
  // With two cascaded 74HC595:
  // - bits shifted first end up in the last device (furthest from MCU).
  // We'll shift 16 bits such that sr1 gets byteSr1 and sr2 gets byteSr2.
  // Using LSB-first per byte, easiest consistent chaining:
  //   shift sr2 byte first then sr1 byte.
  // This ensures the first shifted byte reaches the farthest register (sr2).
  digitalWrite(SR_LATCH, LOW);
  // shift sr2 (upper byte in chain) first
  srWriteByteLSBFirst(byteSr2, true);
  // shift sr1 (lower byte in chain) next
  srWriteByteLSBFirst(byteSr1, true);
  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

void setup() {
  // Contract-compatible initialization:
  // - Both 7-seg show "00"
  // - All bargraph segments remain off.
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_DATA, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Initialize displays to "00"
  shift595Number(0);
  // Bar graph already off
}

void loop() {
  // Control period: 5ms (contract). Use simple timing gate.
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < 5) return;
  last = now;

  // io_0: read A3 as analog and map to 0..99 using contract:
  // if io_0 <= 0.0 -> 0; else if io_0 >= 99.0 -> 99; else floor(io_0)
  // The contract treats io_0 as Real in adc_count space. We quantize:
  // Map ADC reading (0..1023) to 0..99 with linear scaling; then apply the exact clamp+floor policy.
  int adc = analogRead(POT_PIN); // 0..1023
  double io0_real = (adc * 99.0) / 1023.0; // adc_count -> "io_0" in 0..99

  int potValue;
  if (io0_real <= 0.0) potValue = 0;
  else if (io0_real >= 99.0) potValue = 99;
  else potValue = (int)floor(io0_real);

  // io_1: potValue displayed on sevseg tens (sr1) and units (sr2)
  shift595Number(potValue);

  // Bar graph outputs io_2..io_11:
  // io_2..io_10 correspond to indices 0..8 thresholds: potValue>=10,20,...,90
  // io_11 (index 9) is always false.
  digitalWrite(BAR_PINS[0], (potValue >= 10) ? HIGH : LOW);
  digitalWrite(BAR_PINS[1], (potValue >= 20) ? HIGH : LOW);
  digitalWrite(BAR_PINS[2], (potValue >= 30) ? HIGH : LOW);
  digitalWrite(BAR_PINS[3], (potValue >= 40) ? HIGH : LOW);
  digitalWrite(BAR_PINS[4], (potValue >= 50) ? HIGH : LOW);
  digitalWrite(BAR_PINS[5], (potValue >= 60) ? HIGH : LOW);
  digitalWrite(BAR_PINS[6], (potValue >= 70) ? HIGH : LOW);
  digitalWrite(BAR_PINS[7], (potValue >= 80) ? HIGH : LOW);
  digitalWrite(BAR_PINS[8], (potValue >= 90) ? HIGH : LOW);
  digitalWrite(BAR_PINS[9], LOW); // io_11 always false; index 9 must stay off even at 99
}
