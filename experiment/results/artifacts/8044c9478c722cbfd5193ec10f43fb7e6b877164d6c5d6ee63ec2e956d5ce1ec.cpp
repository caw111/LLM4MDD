#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_RGB_R = 2;  // D2 -> rgb1:R
static const uint8_t PIN_RGB_G = 3;  // D3 -> rgb1:G
static const uint8_t PIN_RGB_B = 4;  // D4 -> rgb1:B

static const uint8_t PIN_SERVO_PWM = 5; // D5 -> servo1:PWM

static const uint8_t PIN_SR_DATA  = A0; // A0 -> sr1:DS
static const uint8_t PIN_SR_CLOCK = A2; // A2 -> sr1:SHCP
static const uint8_t PIN_SR_LATCH = A1; // A1 -> sr1:STCP
static const bool SR_ACTIVE_LOW_SEGMENTS = true; // frozen contract

static Servo servo1;

// ACCEPTED executable model states (must preserve exact controller state).
static int32_t colorIndex = 0;
static int32_t digit = 9;
static float servoAngle = 0.0f;
static bool phaseInitialized = false;

// Shift-register segment bits: mapping uses segment_bits [0..6] => Q0..Q6.
// For this frozen interface contract, the digit->segments wiring is assumed standard:
// segment_bits order [0,1,2,3,4,5,6] corresponds to [A,B,C,D,E,F,G] respectively.
// sevseg1 is active-low: active segment => output LOW at corresponding Q.
static const uint8_t SEG_A = 0; // bit index in shift register output Q0
static const uint8_t SEG_B = 1; // Q1
static const uint8_t SEG_C = 2; // Q2
static const uint8_t SEG_D = 3; // Q3
static const uint8_t SEG_E = 4; // Q4
static const uint8_t SEG_F = 5; // Q5
static uint8_t SEG_G = 6; // Q6

// Segment activation masks for digits 0..9 (standard 7-seg, common cathode assumed on module;
// but our shift-register outputs are active-low per frozen contract, so we will invert).
// Bits represent segments A..G where 1 means segment ON (before inversion).
static const uint8_t DIGIT_SEG_ON_MASK[10] = {
  // A B C D E F G
  // 0: A B C D E F
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_E)|(1u<<SEG_F),
  // 1: B C
  (1u<<SEG_B)|(1u<<SEG_C),
  // 2: A B D E G
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_D)|(1u<<SEG_E)|(1u<<SEG_G),
  // 3: A B C D G
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_G),
  // 4: B C F G
  (1u<<SEG_B)|(1u<<SEG_C)|(1u<<SEG_F)|(1u<<SEG_G),
  // 5: A C D F G
  (1u<<SEG_A)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_F)|(1u<<SEG_G),
  // 6: A C D E F G
  (1u<<SEG_A)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_E)|(1u<<SEG_F)|(1u<<SEG_G),
  // 7: A B C
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_C),
  // 8: A B C D E F G
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_E)|(1u<<SEG_F)|(1u<<SEG_G),
  // 9: A B C D F G
  (1u<<SEG_A)|(1u<<SEG_B)|(1u<<SEG_C)|(1u<<SEG_D)|(1u<<SEG_F)|(1u<<SEG_G)
};

// Initialize sr pins.
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srInit();
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srWriteByte(uint8_t value);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void output_sevseg_digit(int32_t d);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void output_rgb_pwm(float red, float green, float blue);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controller_tick();
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 177 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void srInit() {
  pinMode(PIN_SR_DATA, OUTPUT);
  pinMode(PIN_SR_CLOCK, OUTPUT);
  pinMode(PIN_SR_LATCH, OUTPUT);

  digitalWrite(PIN_SR_DATA, LOW);
  digitalWrite(PIN_SR_CLOCK, LOW);
  digitalWrite(PIN_SR_LATCH, LOW);
}

// Shift out 8 bits to 74HC595: only Q0..Q6 are used (segments A..G), Q7 unused.
static void srWriteByte(uint8_t value) {
  // LSB-first is typical for daisy chains; but only segment bits Q0..Q6 are read.
  // We'll shift Q0 corresponds to first bit clocked into DS, i.e., LSB.
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(PIN_SR_DATA, (value & (1u << i)) ? HIGH : LOW);
    // pulse clock
    digitalWrite(PIN_SR_CLOCK, HIGH);
    // small settling for stable sampling
    delayMicroseconds(2);
    digitalWrite(PIN_SR_CLOCK, LOW);
    delayMicroseconds(2);
  }
  // latch to outputs
  digitalWrite(PIN_SR_LATCH, HIGH);
  delayMicroseconds(2);
  digitalWrite(PIN_SR_LATCH, LOW);
}

// Outputs io_4 digit via shift595_number with invalid_policy="blank".
static void output_sevseg_digit(int32_t d) {
  // invalid_policy: blank when out-of-range/invalid.
  bool valid = (d >= 0 && d <= 9);
  uint8_t segOnMask = 0;
  if (valid) {
    segOnMask = DIGIT_SEG_ON_MASK[d];
  } else {
    segOnMask = 0; // blank
  }

  // Build shift register output bits for Q0..Q6.
  // Frozen contract: segment_bits active_low = true.
  // Meaning: when a segment is ON, its corresponding output should be LOW.
  // So output bit = inverted( segment_on ).
  uint8_t srValue = 0;

  for (uint8_t seg = 0; seg < 7; seg++) {
    bool segOn = (segOnMask & (1u << seg)) != 0;
    bool outBit = SR_ACTIVE_LOW_SEGMENTS ? !segOn : segOn;
    if (outBit) srValue |= (1u << seg); // Qseg
  }

  // Q7 is unused; keep it low (doesn't matter).
  srWriteByte(srValue);
}

static void output_rgb_pwm(float red, float green, float blue) {
  // Frozen contract uses pwm_count outputs; assumed in range [0..255] from SysML.
  // Model defines only 0.0 initial values; implement general mapping.
  auto clampU8 = [](float x) -> uint8_t {
    if (x <= 0.0f) return 0;
    if (x >= 255.0f) return 255;
    return (uint8_t)(x + 0.5f);
  };

  analogWrite(PIN_RGB_R, clampU8(red));
  analogWrite(PIN_RGB_G, clampU8(green));
  analogWrite(PIN_RGB_B, clampU8(blue));
}

// ACCEPTED model outputs io_0..io_4.
// In this accepted bundle, only initialization assigns fixed values;
// after initialization, no further updates occur.
static void controller_tick() {
  // perform action tick:
  // first start; then if (not phaseInitialized) { assign ... }
  if (!phaseInitialized) {
    // initialize exactly once
    colorIndex = 0;
    digit = 9;
    servoAngle = 0.0f;
    phaseInitialized = true;
  }

  // outputs:
  // io_0=0.0, io_1=0.0, io_2=0.0, io_3=servoAngle, io_4=digit
  float io_0 = 0.0f;
  float io_1 = 0.0f;
  float io_2 = 0.0f;
  float io_3 = servoAngle;
  int32_t io_4 = digit;

  // Synchronous updates: compute outputs then apply.
  output_rgb_pwm(io_0, io_1, io_2);
  servo1.write((int)(io_3 + 0.5f));
  output_sevseg_digit(io_4);
}

void setup() {
  // Wiring-preserving pin setup
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  srInit();

  servo1.attach(PIN_SERVO_PWM);
  servo1.write(0); // required initialization: servo at 0 degrees

  // Ensure RGB off at startup and sevseg shows 9 at startup
  // by running one controller tick to apply initialization.
  controller_tick();
}

void loop() {
  // control_period_ms = 5 (frozen contract). Keep exact period.
  static uint32_t nextMs = 0;
  uint32_t now = millis();
  if (now < nextMs) return;
  nextMs = now + 5;

  controller_tick();
}
