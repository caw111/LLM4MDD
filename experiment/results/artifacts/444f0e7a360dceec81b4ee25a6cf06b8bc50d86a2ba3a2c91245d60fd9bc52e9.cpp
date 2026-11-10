#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t POT1_PIN = A3;  // io_0
static constexpr uint8_t POT2_PIN = A4;  // io_1

// Frozen hardware contract pins
static constexpr uint8_t SERVO1_PIN = 2;   // io_2
static constexpr uint8_t SERVO2_PIN = 3;   // io_3
static constexpr uint8_t SERVO3_PIN = 4;   // io_4

static constexpr uint8_t BAR_A1_PIN  = 5;   // io_5
static constexpr uint8_t BAR_A2_PIN  = 6;   // io_6
static constexpr uint8_t BAR_A3_PIN  = 7;   // io_7
static constexpr uint8_t BAR_A4_PIN  = 8;   // io_8
static constexpr uint8_t BAR_A5_PIN  = 9;   // io_9
static constexpr uint8_t BAR_A6_PIN  = 10;  // io_10
static constexpr uint8_t BAR_A7_PIN  = 11;  // io_11
static constexpr uint8_t BAR_A8_PIN  = 12;  // io_12
static constexpr uint8_t BAR_A9_PIN  = 13;  // io_13
static constexpr uint8_t BAR_A10_PIN = 44;  // io_14

static Servo servo1;
static Servo servo2;
static Servo servo3;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readAdcCountF(uint8_t analogPin);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controlStep();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_31\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float readAdcCountF(uint8_t analogPin) {
  // adc_count in [0..1023]
  return static_cast<float>(analogRead(analogPin));
}

// Update all outputs according to Accepted executable model equations.
// - io_2 = io_0 * 180 / 1023
// - io_3 = io_1 * 180 / 1023
// - io_4 = floor(((io_0+io_1)/2) * 180 / 1023 + 0.5)  (rounded-to-nearest integer via +0.5 then floor)
// - bar segments: floor(((io_0+io_1)/2) * 10 / 1023) >= n   for n=1..10
static void controlStep() {
  const float io_0 = readAdcCountF(POT1_PIN); // pot1
  const float io_1 = readAdcCountF(POT2_PIN); // pot2

  const float io_2 = io_0 * 180.0f / 1023.0f; // servo1 angle
  const float io_3 = io_1 * 180.0f / 1023.0f; // servo2 angle

  const float avgCounts = (io_0 + io_1) / 2.0f;

  const float tmpServo3 = (avgCounts * 180.0f / 1023.0f) + 0.5f;
  const float io_4 = floorf(tmpServo3); // rounded to nearest integer degrees

  // Bar display uses the model's floor without +0.5:
  // k = floor(avgCounts * 10 / 1023)
  const float kF = floorf((avgCounts * 10.0f) / 1023.0f);
  const int k = (kF < 0.0f) ? 0 : ((kF > 10.0f) ? 10 : static_cast<int>(kF));

  const bool b1  = (k >= 1);
  const bool b2  = (k >= 2);
  const bool b3  = (k >= 3);
  const bool b4  = (k >= 4);
  const bool b5  = (k >= 5);
  const bool b6  = (k >= 6);
  const bool b7  = (k >= 7);
  const bool b8  = (k >= 8);
  const bool b9  = (k >= 9);
  const bool b10 = (k >= 10);

  // Update servo angles (invalid_policy hold_last_valid is moot here because computations are always valid)
  servo1.write(static_cast<int>(lroundf(io_2)));
  servo2.write(static_cast<int>(lroundf(io_3)));
  servo3.write(static_cast<int>(io_4));

  // Active-high digital outputs
  digitalWrite(BAR_A1_PIN,  b1  ? HIGH : LOW);
  digitalWrite(BAR_A2_PIN,  b2  ? HIGH : LOW);
  digitalWrite(BAR_A3_PIN,  b3  ? HIGH : LOW);
  digitalWrite(BAR_A4_PIN,  b4  ? HIGH : LOW);
  digitalWrite(BAR_A5_PIN,  b5  ? HIGH : LOW);
  digitalWrite(BAR_A6_PIN,  b6  ? HIGH : LOW);
  digitalWrite(BAR_A7_PIN,  b7  ? HIGH : LOW);
  digitalWrite(BAR_A8_PIN,  b8  ? HIGH : LOW);
  digitalWrite(BAR_A9_PIN,  b9  ? HIGH : LOW);
  digitalWrite(BAR_A10_PIN, b10 ? HIGH : LOW);
}

void setup() {
  // Frozen initialization: servos at 0 degrees; bargraph completely off
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  pinMode(BAR_A1_PIN, OUTPUT);
  pinMode(BAR_A2_PIN, OUTPUT);
  pinMode(BAR_A3_PIN, OUTPUT);
  pinMode(BAR_A4_PIN, OUTPUT);
  pinMode(BAR_A5_PIN, OUTPUT);
  pinMode(BAR_A6_PIN, OUTPUT);
  pinMode(BAR_A7_PIN, OUTPUT);
  pinMode(BAR_A8_PIN, OUTPUT);
  pinMode(BAR_A9_PIN, OUTPUT);
  pinMode(BAR_A10_PIN, OUTPUT);

  digitalWrite(BAR_A1_PIN, LOW);
  digitalWrite(BAR_A2_PIN, LOW);
  digitalWrite(BAR_A3_PIN, LOW);
  digitalWrite(BAR_A4_PIN, LOW);
  digitalWrite(BAR_A5_PIN, LOW);
  digitalWrite(BAR_A6_PIN, LOW);
  digitalWrite(BAR_A7_PIN, LOW);
  digitalWrite(BAR_A8_PIN, LOW);
  digitalWrite(BAR_A9_PIN, LOW);
  digitalWrite(BAR_A10_PIN, LOW);
}

void loop() {
  // Control period frozen as 5 ms; use elapsed time to pace updates.
  static unsigned long lastMs = 0;
  const unsigned long now = millis();
  if (now - lastMs >= 5) {
    lastMs = now;
    controlStep();
  }
}
