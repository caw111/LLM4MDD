#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static const uint8_t LED1_PIN = 2;   // io_0
static const uint8_t LED2_PIN = 3;   // io_1

static const uint8_t SR_CLOCK = A2; // io_2 shift595_number: clock_pin
static const uint8_t SR_LATCH = A1; // latch_pin
static const uint8_t SR_DATA  = A0; // data_pin

// sr1 wiring: OE tied to GND (always enabled). MR tied to VCC (no reset control).
// Active_low digit/segment bits: segment bits Q0..Q6 are active_low.
// We map digit 0..9 -> segment mask (bits A..G), where bit i corresponds to segment i (A=0..G=6).
// Then we convert to active_low by inverting bits before shifting into Q0..Q6 order.
static const uint8_t digitSegmentsCommonAnode_AtoG[10] = {
  // Standard 7-seg (common anode) segment ON values: 1 means segment ON.
  // Digit: 0 1 2 3 4 5 6 7 8 9
  0b0111111, // 0 => A,B,C,D,E,F
  0b0000110, // 1 => B,C
  0b1011011, // 2 => A,B,D,E,G
  0b1001111, // 3 => A,B,C,D,G
  0b1100110, // 4 => B,C,F,G
  0b1101101, // 5 => A,C,D,F,G
  0b1111101, // 6 => A,C,D,E,F,G
  0b0000111, // 7 => A,B,C
  0b1111111, // 8 => all
  0b1101111  // 9 => A,B,C,D,F,G
};

// Convert to active_low segment bits for shift register Q0..Q6 where Q0->A ... Q6->G.
// Active_low means: segment ON requires output LOW.
// We assume SR outputs drive segment lines directly.
// Therefore: activeLowBit = !segmentON.
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t encodeDigitToActiveLowBits(int digit);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sr1WriteSegmentsActiveLow(uint8_t activeLowAtoG);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void sevsegDisplayDigit(int countValue);
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick(int32_t io_3_delta_ms);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_101\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t encodeDigitToActiveLowBits(int digit)
{
  if (digit < 0) digit = 0;
  if (digit > 9) digit = 9;

  uint8_t segOn = digitSegmentsCommonAnode_AtoG[digit] & 0x7F; // A..G bits
  uint8_t activeLow = (~segOn) & 0x7F; // ON -> 0, OFF -> 1
  return activeLow; // bits 0..6 correspond to Q0..Q6 (A..G)
}

// Shift out 7 bits to sr1: Q0..Q6 correspond to segments A..G.
// Use bit order consistent with Q chain in wiring: Q0 first, then Q1..Q6.
// We'll clock MSB->LSB or LSB->MSB carefully by shifting 7 bits so that
// the last shifted bit ends up at Q6. In a typical 74HC595 chain,
// the first shifted bit goes to Q0 (closest to DS->Q0 path) after shifting 8 bits.
// Since only 7 bits are used, we shift exactly 7 bits and ensure Q0 receives bit0.
static void sr1WriteSegmentsActiveLow(uint8_t activeLowAtoG)
{
  // We'll shift bit0 first (A via Q0), then bit1..bit6.
  // After shifting, pulse latch to update outputs.
  digitalWrite(SR_LATCH, LOW);

  for (int i = 0; i < 7; i++) {
    // i=0 corresponds to Q0 -> segment A
    uint8_t bitVal = (activeLowAtoG >> i) & 0x01; // 1 = active_low output HIGH
    digitalWrite(SR_DATA, bitVal ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    // small delay not required but keep stable
    digitalWrite(SR_CLOCK, LOW);
  }

  digitalWrite(SR_LATCH, HIGH);
  digitalWrite(SR_LATCH, LOW);
}

static void sevsegDisplayDigit(int countValue)
{
  uint8_t activeLowBits = encodeDigitToActiveLowBits(countValue);
  sr1WriteSegmentsActiveLow(activeLowBits);
}

// Controller state (exactly as specified)
static int32_t blinkAccumMs = 0; // i32
static bool led1On = false;      // bool
static int32_t countAccumMs = 0; // i32
static int32_t countValue = 9;   // i32

static uint32_t lastStepMs = 0;

static void controllerTick(int32_t io_3_delta_ms)
{
  // perform action tick
  blinkAccumMs = blinkAccumMs + io_3_delta_ms;

  if (blinkAccumMs >= 2000) {
    blinkAccumMs = blinkAccumMs - 2000;
    led1On = !led1On;
  }

  countAccumMs = countAccumMs + io_3_delta_ms;

  if (countAccumMs >= 4000) {
    countAccumMs = countAccumMs - 4000;

    if (countValue == 0) {
      countValue = 9;
    } else {
      countValue = countValue - 1;
      if (countValue - 1 == 0) {
        countValue = 9;
      }
    }
  }

  // outputs mapping:
  // io_0 = led1On
  // io_1 = not led1On
  // io_2 = countValue
}

void setup()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_DATA, OUTPUT);

  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_DATA, LOW);

  // Initialize controller to specified initial values
  blinkAccumMs = 0;
  led1On = false;
  countAccumMs = 0;
  countValue = 9;

  // Output initial state: LEDs off, sevseg shows '9'
  digitalWrite(LED1_PIN, LOW);           // io_0 false => LOW
  digitalWrite(LED2_PIN, HIGH);          // io_1 true  => HIGH (not led1On)
  sevsegDisplayDigit((int)countValue);

  lastStepMs = millis();
}

void loop()
{
  // Control period: 10 ms (interface contract indicates 10ms sample period).
  // We compute actual elapsed delta ms since last loop.
  uint32_t now = millis();
  int32_t io_3 = (int32_t)(now - lastStepMs);
  lastStepMs = now;

  // Ensure non-negative delta; if millis overflow or scheduling jitter causes 0, keep as is.
  if (io_3 < 0) io_3 = 0;

  controllerTick(io_3);

  // Apply outputs after updates (in the model they are outputs of state after tick)
  digitalWrite(LED1_PIN, led1On ? HIGH : LOW);   // io_0
  digitalWrite(LED2_PIN, (!led1On) ? HIGH : LOW); // io_1 = not led1On

  // Update sevseg after each control step so that it reflects new number immediately after decrement.
  // (Model output io_2 is countValue; updating continuously preserves immediacy.)
  sevsegDisplayDigit((int)countValue);

  // Wait to approximate 10 ms control period.
  delay(10);
}
