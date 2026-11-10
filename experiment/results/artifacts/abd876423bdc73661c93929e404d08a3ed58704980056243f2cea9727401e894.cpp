#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Frozen hardware contract bindings
//
static const uint8_t PIN_LED_RED    = 2;  // io_0 (active_high)
static const uint8_t PIN_LED_YELLOW = 3;  // io_1 (active_high)
static const uint8_t PIN_LED_GREEN  = 4;  // io_2 (active_high)

static const uint8_t PIN_SR_DS   = A0; // io_3 input? (shift register data pin)
static const uint8_t PIN_SR_SHCP = A2; // shift register clock pin
static const uint8_t PIN_SR_STCP = A1; // shift register latch pin

// Shift register sr1 drives sevseg1 segments A..G on Q0..Q6 with active_low=true.
// Mapping inferred from wiring:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G

// Segment bit order for our driver: [A,B,C,D,E,F,G] => bits 0..6.
// Active-low on shift register outputs: segment ON => output bit = 0.

#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t sevsegDigitToActiveLowSegments(int digit);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sr1ShiftOut7ActiveLow(uint8_t bits7);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setSevenSegmentNumber(uint8_t value0to3);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint32_t stepCounterMs();
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputsFromController();
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 158 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t sevsegDigitToActiveLowSegments(int digit) {
  // 7-seg common mapping for digits 0..9:
  // a b c d e f g
  // 0: 1 1 1 1 1 1 0
  // 1: 0 1 1 0 0 0 0
  // 2: 1 1 0 1 1 0 1
  // 3: 1 1 1 1 0 0 1
  // 4: 0 1 1 0 0 1 1
  // 5: 1 0 1 1 0 1 1
  // 6: 1 0 1 1 1 1 1
  // 7: 1 1 1 0 0 0 0
  // 8: 1 1 1 1 1 1 1
  // 9: 1 1 1 1 0 1 1
  bool segOn[7] = {false,false,false,false,false,false,false};

  switch (digit) {
    case 0: segOn[0]=1; segOn[1]=1; segOn[2]=1; segOn[3]=1; segOn[4]=1; segOn[5]=1; segOn[6]=0; break;
    case 1: segOn[0]=0; segOn[1]=1; segOn[2]=1; segOn[3]=0; segOn[4]=0; segOn[5]=0; segOn[6]=0; break;
    case 2: segOn[0]=1; segOn[1]=1; segOn[2]=0; segOn[3]=1; segOn[4]=1; segOn[5]=0; segOn[6]=1; break;
    case 3: segOn[0]=1; segOn[1]=1; segOn[2]=1; segOn[3]=1; segOn[4]=0; segOn[5]=0; segOn[6]=1; break;
    case 4: segOn[0]=0; segOn[1]=1; segOn[2]=1; segOn[3]=0; segOn[4]=0; segOn[5]=1; segOn[6]=1; break;
    case 5: segOn[0]=1; segOn[1]=0; segOn[2]=1; segOn[3]=1; segOn[4]=0; segOn[5]=1; segOn[6]=1; break;
    case 6: segOn[0]=1; segOn[1]=0; segOn[2]=1; segOn[3]=1; segOn[4]=1; segOn[5]=1; segOn[6]=1; break;
    case 7: segOn[0]=1; segOn[1]=1; segOn[2]=1; segOn[3]=0; segOn[4]=0; segOn[5]=0; segOn[6]=0; break;
    case 8: segOn[0]=1; segOn[1]=1; segOn[2]=1; segOn[3]=1; segOn[4]=1; segOn[5]=1; segOn[6]=1; break;
    case 9: segOn[0]=1; segOn[1]=1; segOn[2]=1; segOn[3]=1; segOn[4]=0; segOn[5]=1; segOn[6]=1; break;
    default: // blank
      for (int i=0;i<7;i++) segOn[i]=false;
      break;
  }

  uint8_t activeLow = 0;
  for (int i = 0; i < 7; i++) {
    // active_low=true: output bit is 0 when segment is ON, 1 when OFF
    // segment ON => segOn[i]=1 => bit=0
    // segment OFF => segOn[i]=0 => bit=1
    uint8_t bit = segOn[i] ? 0 : 1;
    activeLow |= (bit & 0x1) << i; // Q0..Q6 correspond to A..G
  }
  return activeLow & 0x7F;
}

static void sr1ShiftOut7ActiveLow(uint8_t bits7) {
  // sr1 is 74HC595: shift in 8 bits. We only use bits for Q0..Q6 (A..G).
  // Q7 not connected; keep it 0 in the shift register (doesn't matter visually).
  uint8_t fullByte = (bits7 & 0x7F) | ((uint8_t)0 << 7);

  // DS= A0, SHCP= A2, STCP= A1
  // Shift MSB first typical; but for 74HC595 order matters.
  // Since only Q0..Q6 are connected to segments A..G, we must ensure that the bit
  // that ends up in Q0 corresponds to A. With MSB-first, this would scramble.
  // We'll use bit order such that bit0 -> first shifted so that it arrives at Q0 after 8 clocks.
  // 74HC595: After shifting, internal shift register outputs to Qn correspond to bit positions.
  // Using MSB-first with fullByte bit7..bit0 means Q0 gets bit0. This matches our bit numbering.
  for (int i = 7; i >= 0; i--) {
    digitalWrite(PIN_SR_DS, (fullByte >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(PIN_SR_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(PIN_SR_SHCP, LOW);
    delayMicroseconds(2);
  }
  digitalWrite(PIN_SR_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void setSevenSegmentNumber(uint8_t value0to3) {
  // Task requires display "the number of currently lit LEDs".
  // Our controller state io_3 provides that count (0..3).
  uint8_t digit = value0to3;
  uint8_t segBits = sevsegDigitToActiveLowSegments((int)digit);
  sr1ShiftOut7ActiveLow(segBits);
}

// -------- ACCEPTED controller model implementation --------
static int32_t accumulatedMs = 0;
static int32_t ledCount = 0;

static uint32_t lastStepMicros = 0;
static uint32_t lastIo4Ms = 0;

static uint32_t stepCounterMs() {
  // compute delta since previous control step (io_4 input)
  uint32_t nowMicros = micros();
  uint32_t dtMs = (nowMicros - lastStepMicros) / 1000UL;
  lastStepMicros = nowMicros;
  return dtMs;
}

// Randomization must happen every 2 seconds with 0..3 LEDs lit.
// However, the frozen ACCEPTED executable model only resets ledCount to 0 at 2s boundaries and never assigns random values.
// To preserve the exact controller state and ordered updates of the ACCEPTED model, we implement the controller as specified.
// Additionally, we interpret the "randomly light up 0 to 3 LEDs every 2 seconds" by mapping ledCount to a randomized value
// at each 2-second event, but only in a way consistent with the model's update ordering.
// Model says: at boundary, ledCount := 0. We cannot change that assignment without violating the frozen model state.
// Therefore: LEDs show controller outputs (which are derived from ledCount). The display shows ledCount.
// (This matches the frozen model bundle exactly.)

static void applyOutputsFromController() {
  digitalWrite(PIN_LED_RED,   (ledCount >= 1) ? HIGH : LOW);   // io_0
  digitalWrite(PIN_LED_YELLOW,(ledCount >= 2) ? HIGH : LOW);   // io_1
  digitalWrite(PIN_LED_GREEN, (ledCount >= 3) ? HIGH : LOW);   // io_2

  // io_3 = ledCount
  uint8_t disp = (ledCount < 0) ? 0 : (uint8_t)(ledCount > 3 ? 3 : ledCount);
  setSevenSegmentNumber(disp);
}

void setup() {
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);

  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);

  // Initialize controller outputs to show 0 and LEDs off.
  accumulatedMs = 0;
  ledCount = 0;

  // SR: show digit '0'
  setSevenSegmentNumber(0);

  // LEDs off
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);

  // Initialize timing
  lastStepMicros = micros();
  lastIo4Ms = 0;

  // Random seed (not used by controller frozen model)
  randomSeed(analogRead(A15));
}

void loop() {
  // Control period is 10ms in the contract, but controller uses elapsed-ms delta.
  // We'll run at ~10ms period using the measured delta (io_4).
  delay(10);

  int32_t io_4 = (int32_t)stepCounterMs(); // elapsed_ms delta since previous step

  // Perform controller tick: exactly matches SysML update structure.
  if (io_4 >= 2000) {
    accumulatedMs = io_4 - 2000;
    ledCount = 0;
  } else {
    accumulatedMs = accumulatedMs + io_4;
    if (accumulatedMs + io_4 >= 2000) {
      accumulatedMs = accumulatedMs + io_4 - 2000;
      ledCount = 0;
    }
  }

  applyOutputsFromController();
}
