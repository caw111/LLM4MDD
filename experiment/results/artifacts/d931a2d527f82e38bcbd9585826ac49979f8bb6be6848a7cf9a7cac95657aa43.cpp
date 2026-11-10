#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// ==== Frozen interface contract bindings ====
// Inputs (active_low with pullups): io_0->D2, io_1->D3, io_2->D4
static const uint8_t PIN_BTN1 = 2; // io_0
static const uint8_t PIN_BTN2 = 3; // io_1
static const uint8_t PIN_BTN3 = 4; // io_2

// Outputs:
// io_3: 7-seg digit via shift register sr1 (Mega: A2 clock, A0 data, A1 latch)
static const uint8_t SR_CLOCK = A2; // SHCP
static const uint8_t SR_LATCH = A1; // STCP
static const uint8_t SR_DATA  = A0;  // DS

// io_4: servo1 angle target on D5
static const uint8_t PIN_SERVO1 = 5; // io_4

// io_5: servo2 angle target on D6
static const uint8_t PIN_SERVO2 = 6; // io_5

// ==== Model state (exact) ====
static int32_t digit = 0;
static bool prevBtn1 = true;
static bool prevBtn2 = true;
static bool prevBtn3 = true;

// ==== 7-seg shift595_number segment bits mapping (active_low) ====
// segment_bits order: [0,1,2,3,4,5,6] corresponds to {A,B,C,D,E,F,G}.
// active_low: bit=0 means segment ON.
// Standard mapping for common anode (active low):
// 0: A B C D E F on, G off -> 0b0000001? careful with active low representation.
// We'll use conventional active-low bit pattern for segments A..G where 1=OFF, 0=ON.
// For digits 0..9, active-low segment bytes for A..G:
// 0: A B C D E F ON, G OFF  => {A..F}=0, G=1 => 0b00000010? but we need a 7-bit value.
// Let's define: bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G (active LOW).
static const uint8_t segActiveLow[10] = {
  // digit:  abcdefg (active low)
  // 0:  A B C D E F ON (0), G OFF (1) => 0 0 0 0 0 0 1
  (uint8_t)0b10000000 >> 1, // 0b00000001? incorrect shift; will override below
};

// Build correct table explicitly (bit0=A ... bit6=G, active LOW).
// For each digit, provide 7-bit where bit=0 means ON for that segment.
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t segBits(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srWrite7SegDigit(int32_t d);
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 159 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_76\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t segBits(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) {
  // inputs are active_low bit values (0=ON,1=OFF) for segments A..G
  return (uint8_t)((a ? 1 : 0) << 0) |
         (uint8_t)((b ? 1 : 0) << 1) |
         (uint8_t)((c ? 1 : 0) << 2) |
         (uint8_t)((d ? 1 : 0) << 3) |
         (uint8_t)((e ? 1 : 0) << 4) |
         (uint8_t)((f ? 1 : 0) << 5) |
         (uint8_t)((g ? 1 : 0) << 6);
}

// Explicit active-low patterns for A..G (common anode behavior).
// Segment ON => bit=0, OFF => bit=1.
static const uint8_t segMapActiveLow[10] = {
  // 0: A,B,C,D,E,F ON; G OFF
  segBits(0,0,0,0,0,0,1),
  // 1: B,C ON
  segBits(1,0,0,1,1,1,1),
  // 2: A,B,D,E,G ON
  segBits(0,0,1,0,0,1,0),
  // 3: A,B,C,D,G ON
  segBits(0,0,0,0,1,1,0),
  // 4: B,C,F,G ON
  segBits(1,0,0,1,1,0,0),
  // 5: A,C,D,F,G ON
  segBits(0,1,0,0,1,0,0),
  // 6: A,C,D,E,F,G ON
  segBits(0,1,0,0,0,0,0),
  // 7: A,B,C ON
  segBits(0,0,0,1,1,1,1),
  // 8: A,B,C,D,E,F,G ON
  segBits(0,0,0,0,0,0,0),
  // 9: A,B,C,D,F,G ON
  segBits(0,0,0,0,1,0,0)
};

static Servo servo1;
static Servo servo2;

// Shift register: output 7-seg segments as static digit (leading_zero=false, invalid_policy=blank not needed).
// We'll shift out 8 bits (Q0..Q7). Only Q0..Q6 used. Q7 unused set to 1 (OFF).
static void srWrite7SegDigit(int32_t d) {
  if (d < 0 || d > 9) {
    // invalid_policy blank: turn all segments off => all bits 1 (OFF) for A..G
    uint8_t allOff = segBits(1,1,1,1,1,1,1);
    uint8_t byteToShift = (uint8_t)(allOff & 0x7F); // Q0..Q6
    // Q7=1 (OFF)
    uint8_t out = (uint8_t)(byteToShift | (0x80));
    // Latch disabled during shifting then latch.
    digitalWrite(SR_LATCH, LOW);
    for (int i = 7; i >= 0; --i) {
      digitalWrite(SR_CLOCK, LOW);
      digitalWrite(SR_DATA, (out >> i) & 0x01 ? HIGH : LOW);
      digitalWrite(SR_CLOCK, HIGH);
    }
    digitalWrite(SR_LATCH, HIGH);
    digitalWrite(SR_LATCH, LOW);
    return;
  }

  uint8_t bits = segMapActiveLow[d] & 0x7F; // Q0..Q6 = A..G active-low bits
  uint8_t out = (uint8_t)(bits | 0x80); // Q7=1 (OFF)
  digitalWrite(SR_LATCH, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_CLOCK, LOW);
    digitalWrite(SR_DATA, (out >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
  }
  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

// Apply outputs exactly as model outputs after each update step
static void applyOutputs() {
  // io_3: shift register digit = digit
  srWrite7SegDigit(digit);

  // io_4: servo1 = (digit>=5) ? 90 : 0
  double target1 = (digit >= 5) ? 90.0 : 0.0;
  // io_5: servo2 = (digit>=5) ? 180 : 90
  double target2 = (digit >= 5) ? 180.0 : 90.0;

  servo1.write((int)target1);
  servo2.write((int)target2);
}

void setup() {
  // Inputs: internal pullups, active_high=false
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  pinMode(PIN_BTN3, INPUT_PULLUP);

  // Shift register pins
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_DATA, LOW);

  // Attach servos
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);

  // Initialize model outputs: digit=0, servos at 0-degree positions (servo1=0, servo2=90)
  digit = 0;
  prevBtn1 = true;
  prevBtn2 = true;
  prevBtn3 = true;

  // Immediately drive outputs based on initial state
  applyOutputs();
}

void loop() {
  // control_period_ms = 5
  static uint32_t lastMs = 0;
  uint32_t now = millis();
  if (now - lastMs < 5) return;
  lastMs = now;

  // Normalize inputs according to contract: true corresponds to pressed state,
  // active_high=false means model true_level is LOW. So:
  // io = (digitalRead(pin) == LOW)
  bool io_0 = (digitalRead(PIN_BTN1) == LOW);
  bool io_1 = (digitalRead(PIN_BTN2) == LOW);
  bool io_2 = (digitalRead(PIN_BTN3) == LOW);

  // Model tick updates: ordered updates exactly:
  // if io_0 && !prevBtn1 => digit = (digit+1>9 ? 0 : digit+1)
  if (io_0 && (!prevBtn1)) {
    int32_t next = digit + 1;
    digit = (next > 9) ? 0 : next;
  }

  // then if io_1 && !prevBtn2 => digit = (digit+2>9 ? 0 : digit+2)
  if (io_1 && (!prevBtn2)) {
    int32_t next = digit + 2;
    digit = (next > 9) ? 0 : next;
  }

  // then if io_2 && !prevBtn3 => digit = (digit+3>9 ? 0 : digit+3)
  if (io_2 && (!prevBtn3)) {
    int32_t next = digit + 3;
    digit = (next > 9) ? 0 : next;
  }

  // then assign prevBtn1=io_0, prevBtn2=io_1, prevBtn3=io_2
  prevBtn1 = io_0;
  prevBtn2 = io_1;
  prevBtn3 = io_2;

  // Apply outputs immediately after update
  applyOutputs();
}
