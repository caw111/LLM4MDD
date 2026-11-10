#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// --- Shift register (sr1) pins from circuit / contract ---
// clock_pin: A2 (SHCP) -> Arduino Mega pin A2
// data_pin : A0 (DS)   -> Arduino Mega pin A0
// latch_pin: A1 (STCP) -> Arduino Mega pin A1
static const uint8_t SR_CLOCK_PIN = A2; // SHCP
static const uint8_t SR_LATCH_PIN = A1; // STCP
static const uint8_t SR_DATA_PIN  = A0; // DS

// --- Bar graph bargraph1 pins from contract (D2..D11) ---
// LED index A1..A10 maps to D2..D11
static const uint8_t BAR_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// --- 7-seg segment mapping (active_low outputs from shift register) ---
// Contract states active_low, leading_zero=false, invalid_policy=blank.
// Segment bits order: [0,1,2,3,4,5,6] for A,B,C,D,E,F,G corresponds to sr1 Q0..Q6.
// We infer typical 7-seg (A,B,C,D,E,F,G) encoding for digits 0-9.
// Because outputs are active_low, stored value is the bitwise NOT of active_high pattern.
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegments(uint8_t digit);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sr1Write7SegmentsActiveLow(uint8_t activeLowSegments7bit);
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBargraphValue(uint8_t value0to10);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_40\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToActiveLowSegments(uint8_t digit) {
  // Active-high segment bits [A,B,C,D,E,F,G]
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
  uint8_t a=0,b=0,c=0,d=0,e=0,f=0,g=0;
  switch (digit) {
    case 0: a=1;b=1;c=1;d=1;e=1;f=1;g=0; break;
    case 1: a=0;b=1;c=1;d=0;e=0;f=0;g=0; break;
    case 2: a=1;b=1;c=0;d=1;e=1;f=0;g=1; break;
    case 3: a=1;b=1;c=1;d=1;e=0;f=0;g=1; break;
    case 4: a=0;b=1;c=1;d=0;e=0;f=1;g=1; break;
    case 5: a=1;b=0;c=1;d=1;e=0;f=1;g=1; break;
    case 6: a=1;b=0;c=1;d=1;e=1;f=1;g=1; break;
    case 7: a=1;b=1;c=1;d=0;e=0;f=0;g=0; break;
    case 8: a=1;b=1;c=1;d=1;e=1;f=1;g=1; break;
    case 9: a=1;b=1;c=1;d=1;e=0;f=1;g=1; break;
    default:
      // invalid -> blank (all segments off active_high => all 0)
      a=b=c=d=e=f=g=0;
      break;
  }

  uint8_t activeHigh = 0;
  activeHigh |= (a ? (1u<<0) : 0); // Q0 -> A
  activeHigh |= (b ? (1u<<1) : 0); // Q1 -> B
  activeHigh |= (c ? (1u<<2) : 0); // Q2 -> C
  activeHigh |= (d ? (1u<<3) : 0); // Q3 -> D
  activeHigh |= (e ? (1u<<4) : 0); // Q4 -> E
  activeHigh |= (f ? (1u<<5) : 0); // Q5 -> F
  activeHigh |= (g ? (1u<<6) : 0); // Q6 -> G

  // active_low segments: 1 means segment is ON is inverted.
  // If activeHigh bit=1 (segment should be ON), active_low output should be 0.
  // If activeHigh bit=0 (segment OFF), output should be 1.
  uint8_t activeLow = (uint8_t)(~activeHigh) & 0x7F;

  return activeLow;
}

static void sr1Write7SegmentsActiveLow(uint8_t activeLowSegments7bit) {
  // sr1 is 74hc595 with 8 outputs Q0..Q7; we need Q0..Q6 for A..G.
  // Q7 unused; set it to 0 (doesn't matter; but keep off).
  // We'll shift MSB first typical implementation; however Q mapping depends on shifting order.
  // Contract specifies "segment_bits": [0,1,2,3,4,5,6] for digit register.
  // We assume bit i shifted ends up at Q_i when shifting LSB-first (common in model codegen).
  // To ensure segment_bits correspondence, shift out LSB-first for Q0..Q7.
  uint8_t outByte = (activeLowSegments7bit & 0x7F) | (0u << 7);

  // Latch low->shift->latch high
  digitalWrite(SR_LATCH_PIN, LOW);

  for (int i = 0; i < 8; i++) {
    uint8_t bitVal = (outByte >> i) & 0x01; // LSB-first -> Q0 first
    digitalWrite(SR_DATA_PIN, bitVal ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
    // small pulse width; not simplifying behavioral model
    digitalWrite(SR_CLOCK_PIN, LOW);
  }

  digitalWrite(SR_LATCH_PIN, HIGH);
}

static void setBargraphValue(uint8_t value0to10) {
  // outputs io_1..io_10 correspond to value >= idx
  // For value in [0..9], only first 'value' LEDs should be HIGH.
  // For value>=10 (never in this model), io_10 true. We'll implement generically.
  for (int idx = 1; idx <= 10; idx++) {
    bool on = (value0to10 >= (uint8_t)idx);
    digitalWrite(BAR_PINS[idx-1], on ? HIGH : LOW); // active-high assumed
  }
}

int value_i32 = 0;
unsigned long phaseMs_i32 = 0; // model variable type Integer (we keep non-negative)

static const unsigned long CONTROL_PERIOD_MS = 10; // contract

void setup() {
  // Initialize SR control pins
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);

  // Bar graph pins
  for (int i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW); // startup: no LEDs lit (value=0)
  }

  // Startup: sevseg shows "0"
  uint8_t segActiveLow = digitToActiveLowSegments((uint8_t)value_i32);
  sr1Write7SegmentsActiveLow(segActiveLow);
}

void loop() {
  // SysML model tick with control_period_ms=10ms.
  // The model increments phaseMs by +10 every tick unless phaseMs in [2000..2147483647] then resets.
  static unsigned long lastTickUs = 0;
  unsigned long nowUs = micros();
  if (lastTickUs == 0) lastTickUs = nowUs;

  // Wait until next 10ms boundary
  unsigned long elapsedUs = nowUs - lastTickUs;
  if (elapsedUs < CONTROL_PERIOD_MS * 1000UL) {
    return;
  }
  // Adjust tick to keep consistent step behavior
  lastTickUs += CONTROL_PERIOD_MS * 1000UL;

  // tick update logic (exactly like model)
  if ((phaseMs_i32 >= 2000UL) && (phaseMs_i32 <= 2147483647UL)) {
    phaseMs_i32 = 0;
    if (value_i32 == 9) value_i32 = 0;
    else value_i32 = value_i32 + 1;
  } else {
    phaseMs_i32 = phaseMs_i32 + CONTROL_PERIOD_MS; // +10
  }

  // Outputs (io_0..io_10) as per model
  // io_0 -> value to shift register 7-seg
  uint8_t segActiveLow = digitToActiveLowSegments((uint8_t)value_i32);
  sr1Write7SegmentsActiveLow(segActiveLow);

  // io_1..io_10 bargraph LEDs active-high, controlled directly
  setBargraphValue((uint8_t)value_i32);
}
