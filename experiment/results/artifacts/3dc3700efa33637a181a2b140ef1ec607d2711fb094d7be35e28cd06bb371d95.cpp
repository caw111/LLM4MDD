#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Servo.h>

static const uint8_t PIN_SHIFT_DATA = A0; // DS -> blue
static const uint8_t PIN_SHIFT_CLOCK = A2; // SHCP -> gray
static const uint8_t PIN_SHIFT_LATCH = A1; // STCP -> purple

// 7-seg segment lines (wired from sr1 outputs Qn)
static const uint8_t PIN_SEG_A = 0; // not used: wired via shift register outputs
static const uint8_t PIN_SEG_B = 0;
static const uint8_t PIN_SEG_C = 0;
static const uint8_t PIN_SEG_D = 0;
static const uint8_t PIN_SEG_E = 0;
static const uint8_t PIN_SEG_F = 0;
static const uint8_t PIN_SEG_G = 0;

static const uint8_t PIN_POT1 = A3; // pot1 SIG
static const uint8_t PIN_POT2 = A4; // pot2 SIG

// Servos (PWM pins as wired)
static const uint8_t PIN_SERVO1 = 2;
static const uint8_t PIN_SERVO2 = 3;
static const uint8_t PIN_SERVO3 = 4;

static Servo servo1;
static Servo servo2;
static Servo servo3;

// 74HC595 bit mapping from wiring:
// sr1 Q0..Q6 -> sevseg A,F,B,C,D,E,G respectively (per connections)
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G.
// Shift-model requires segment_bits [0,1,2,3,4,5,6] correspond to Q0..Q6, active_low=true.
static const uint8_t digitToShiftQ_activeLow[10] = {
  // segments for digits:
  // Using standard 7-seg (A,B,C,D,E,F,G). Active-low means 0 turns segment ON.
  // Segment ON sets corresponding Q bit to 0.
  // We build Q-bit pattern by: Qbit = 0 for ON, 1 for OFF.
  // Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  // Digit 0: A,B,C,D,E,F on; G off.
  // => Q0..Q5 =0, Q6=1 => b = (G only off)-> 1 at bit6 => 0b1000000 = 0x40
  0x40,
  // Digit 1: B,C on; A,D,E,F,G off
  // Q1,Q2=0; others 1 => bits: Q0=1,Q1=0,Q2=0,Q3=1,Q4=1,Q5=1,Q6=1 => 0b1111101? check:
  // bit6..0: G E D C B A? wait bit positions: bit6=Q6(G),5=Q5(F),4=Q4(E),3=Q3(D),2=Q2(C),1=Q1(B),0=Q0(A)
  // For digit 1: Q6=1,Q5=1,Q4=1,Q3=1,Q2=0,Q1=0,Q0=1 => binary 1 1 1 1 0 0 1 = 0b1111001 = 0x79
  0x79,
  // Digit 2: A,B,D,E,G on; C,F off
  // Q0(A)=0,Q1(B)=0,Q2(C)=1,Q3(D)=0,Q4(E)=0,Q5(F)=1,Q6(G)=0
  // bit6..0: 0 1 0 0 1 0 0 = 0b0100100? Let's compute in bit order:
  // Q6=0 => b6=0
  // Q5=1 => b5=1
  // Q4=0 => b4=0
  // Q3=0 => b3=0
  // Q2=1 => b2=1
  // Q1=0 => b1=0
  // Q0=0 => b0=0
  // => 0b0100100 = 0x24
  0x24,
  // Digit 3: A,B,C,D,G on; E,F off
  // Q0=0,Q1=0,Q2=0,Q3=0,Q4=1,Q5=1,Q6=0
  // b6..0: 0 1 1 0 0 0 0? Wait:
  // Q6=0(b6=0), Q5=1(b5=1), Q4=1(b4=1), Q3=0(b3=0), Q2=0(b2=0), Q1=0(b1=0), Q0=0(b0=0)
  // => 0b0110000 = 0x30
  0x30,
  // Digit 4: B,C,F,G on; A,D,E off
  // Q0=1,Q1=0,Q2=0,Q3=1,Q4=1,Q5=0,Q6=0
  // b6..0: Q6=0,b6=0; Q5=0,b5=0; Q4=1,b4=1; Q3=1,b3=1; Q2=0,b2=0; Q1=0,b1=0; Q0=1,b0=1
  // => 0b0011001 = 0x19
  0x19,
  // Digit 5: A,C,D,F,G on; B,E off
  // Q0=0,Q1=1,Q2=0,Q3=0,Q4=1,Q5=0,Q6=0
  // b6..0: Q6=0,b6=0; Q5=0,b5=0; Q4=1,b4=1; Q3=0,b3=0; Q2=0,b2=0; Q1=1,b1=1; Q0=0,b0=0
  // => 0b0100010 = 0x22
  0x22,
  // Digit 6: A,C,D,E,F,G on; B off
  // Q0=0,Q1=1,Q2=0,Q3=0,Q4=0,Q5=0,Q6=0
  // b6..0: 0 0 0 0 0 1 0? careful:
  // Q6=0(b6=0), Q5=0(b5=0), Q4=0(b4=0), Q3=0(b3=0), Q2=0(b2=0), Q1=1(b1=1), Q0=0(b0=0)
  // => 0b0000010 = 0x02
  0x02,
  // Digit 7: A,B,C on; D,E,F,G off
  // Q0=0,Q1=0,Q2=0,Q3=1,Q4=1,Q5=1,Q6=1
  // b6..0: Q6=1,b6=1; Q5=1,b5=1; Q4=1,b4=1; Q3=1,b3=1; Q2=0,b2=0; Q1=0,b1=0; Q0=0,b0=0
  // => 0b1111000 = 0x78
  0x78,
  // Digit 8: A,B,C,D,E,F,G on
  // all ON => all Q bits 0
  0x00,
  // Digit 9: A,B,C,D,F,G on; E off
  // Q0=0,Q1=0,Q2=0,Q3=0,Q4=1,Q5=0,Q6=0
  // b6..0: Q6=0,b6=0; Q5=0,b5=0; Q4=1,b4=1; Q3=0,b3=0; Q2=0,b2=0; Q1=0,b1=0; Q0=0,b0=0
  // => 0b0010000 = 0x10
  0x10
};

#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void shift595WriteBits(uint8_t qbits_activeLow);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setDigitFromModel(int digit0to9);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float adcToReal(const int adc);
#line 131 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 152 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_37\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void shift595WriteBits(uint8_t qbits_activeLow) {
  // Shift in 8 bits: we only use Q0..Q6 (7 bits). Set Q7 to 1 (inactive) by default.
  // Many 74HC595 boards use MSB-first; we'll shift MSB->LSB so that the bit order matches Q0..Q7 mapping.
  // To ensure deterministic behavior, we shift all 8 bits with bit0 being Q0 at the first clock position.
  // Here, we construct full byte where bit0->Q0, ... bit6->Q6, bit7->Q7.
  uint8_t full = (uint8_t)(qbits_activeLow & 0x7F); // Q7=0 by default? set to 1 (off)
  full |= 0x80; // Q7=1 (off/inactive)
  // Shift out with MSB first:
  digitalWrite(PIN_SHIFT_CLOCK, LOW);
  digitalWrite(PIN_SHIFT_LATCH, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_SHIFT_DATA, (full >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(PIN_SHIFT_CLOCK, HIGH);
    digitalWrite(PIN_SHIFT_CLOCK, LOW);
  }
  digitalWrite(PIN_SHIFT_LATCH, HIGH);
  digitalWrite(PIN_SHIFT_LATCH, LOW);
}

static void setDigitFromModel(int digit0to9) {
  uint8_t d = (digit0to9 < 0) ? 0 : (digit0to9 > 9 ? 9 : digit0to9);
  shift595WriteBits(digitToShiftQ_activeLow[d]);
}

static float adcToReal(const int adc) {
  // Model treats inputs as Real and compares to 0/1023. Using adc count directly matches this.
  return (float)adc;
}

// "State maintenance": model business-time delay/transition of >=2s for each state.
// Since this controller updates every tick, we emulate by holding each quantized state for >=2s
// before allowing changes to take effect, while still continuously sampling within that period.
static unsigned long holdUntilMs = 0;
static int heldDigit = -1;
static float heldS1 = 0, heldS2 = 90, heldS3 = 180;

void setup() {
  pinMode(PIN_SHIFT_DATA, OUTPUT);
  pinMode(PIN_SHIFT_CLOCK, OUTPUT);
  pinMode(PIN_SHIFT_LATCH, OUTPUT);

  // Initialize servos at model initial states
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  servo1.write((int)heldS1);
  servo2.write((int)heldS2);
  servo3.write((int)heldS3);

  // Initialize display to digit 0
  heldDigit = 0;
  setDigitFromModel(0);

  holdUntilMs = millis() + 2000UL;
}

void loop() {
  const unsigned long now = millis();

  // Read inputs every 5ms control step (best-effort; exact period depends on Arduino scheduling).
  // We'll implement a simple 5ms pacing.
  static unsigned long nextTick = 0;
  if (nextTick == 0) nextTick = now;
  while (now < nextTick) { /* busy wait */ }
  nextTick += 5;

  int adc0 = analogRead(PIN_POT1);
  int adc1 = analogRead(PIN_POT2);

  float io_0 = adcToReal(adc0); // pot1 command
  float io_1 = adcToReal(adc1); // pot2 command

  // Model tick computation (ordered updates)
  int sevsegHoldDigit_new;
  if (io_0 <= 0.0f) sevsegHoldDigit_new = 0;
  else if (io_0 >= 1023.0f) sevsegHoldDigit_new = 9;
  else sevsegHoldDigit_new = (int)floorf(io_0 * 10.0f / 1023.0f);

  float servo1HoldAngle_new;
  if (io_1 <= 0.0f) servo1HoldAngle_new = 0.0f;
  else if (io_1 >= 1023.0f) servo1HoldAngle_new = 90.0f;
  else servo1HoldAngle_new = io_1 * 90.0f / 1023.0f;

  float servo2HoldAngle_new;
  if (io_1 <= 0.0f) servo2HoldAngle_new = 90.0f;
  else if (io_1 >= 1023.0f) servo2HoldAngle_new = 180.0f;
  else servo2HoldAngle_new = 90.0f + (io_1 * 90.0f / 1023.0f);

  float servo3HoldAngle_new;
  if (io_1 <= 0.0f) servo3HoldAngle_new = 180.0f;
  else if (io_1 >= 1023.0f) servo3HoldAngle_new = 270.0f;
  else servo3HoldAngle_new = 180.0f + (io_1 * 90.0f / 1023.0f);

  // Clamp policy: servo supported endpoint defaults 544..2400us; for safety use 0..180.
  // (Contract notes clamp handled in model; implement clamp here.)
  auto clamp180 = [](float a) -> float {
    if (a < 0.0f) return 0.0f;
    if (a > 180.0f) return 180.0f;
    return a;
  };
  servo3HoldAngle_new = clamp180(servo3HoldAngle_new);

  // Apply with 2s state maintenance: only update held outputs when hold period expires.
  if (now >= holdUntilMs) {
    heldDigit = sevsegHoldDigit_new;
    heldS1 = servo1HoldAngle_new;
    heldS2 = servo2HoldAngle_new;
    heldS3 = servo3HoldAngle_new;

    // Output expressions / pin bindings
    setDigitFromModel(heldDigit);

    // Smooth movement: Servo library moves smoothly by updating pulse width; we set every tick only when state changes.
    servo1.write((int)heldS1);
    servo2.write((int)heldS2);
    servo3.write((int)heldS3);

    holdUntilMs = now + 2000UL;
  }

  // During hold, do not change outputs; servos keep holding last valid state.
}
