#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Controller states
static int32_t phase = 1;          // initial 1
static int32_t phaseAccumMs = 0;   // initial 0

// Frozen timing contract
static constexpr uint32_t CONTROL_PERIOD_MS = 10;
static constexpr uint32_t PHASE_DURATION_MS = 2000;

// PWM outputs (f64 pwm_count expected: 0..255.0 mapped to analogWrite 0..255)
static constexpr uint8_t RGB1_R_PIN = 2;   // io_0
static constexpr uint8_t RGB1_G_PIN = 3;   // io_1
static constexpr uint8_t RGB1_B_PIN = 4;   // io_2
static constexpr uint8_t RGB2_R_PIN = 5;   // io_3
static constexpr uint8_t RGB2_G_PIN = 6;   // io_4
static constexpr uint8_t RGB2_B_PIN = 7;   // io_5
static constexpr uint8_t RGB3_R_PIN = 8;   // io_6
static constexpr uint8_t RGB3_G_PIN = 9;   // io_7
static constexpr uint8_t RGB3_B_PIN = 10;  // io_8

// Bargraph segments (active high)
static constexpr uint8_t SEG0_PIN = 11;   // io_9_segment_0 -> A1
static constexpr uint8_t SEG1_PIN = 12;   // io_9_segment_1 -> A2
static constexpr uint8_t SEG2_PIN = 13;   // io_9_segment_2 -> A3
static constexpr uint8_t SEG3_PIN = 44;   // io_9_segment_3 -> A4
static constexpr uint8_t SEG4_PIN = 45;   // io_9_segment_4 -> A5

#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t f64ToPwmCount255(float v);
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 83 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void tickControllerStep();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_51\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t f64ToPwmCount255(float v) {
  // Contract outputs are exactly 0.0 or 255.0; keep robust saturation.
  if (v <= 0.0f) return 0;
  if (v >= 255.0f) return 255;
  return (uint8_t)lroundf(v);
}

static void applyOutputs() {
  // Outputs as defined by model:
  // io_0: phase==1 or 4 or 5 ? 255 : 0
  // io_1: phase==2 or 4 or 5 ? 255 : 0
  // io_2: phase==3 or 5 ? 255 : 0
  // io_3: phase==2 or 4 or 5 ? 255 : 0
  // io_4: phase==2 or 4 or 5 ? 255 : 0
  // io_5: phase==5 ? 255 : 0
  // io_6: phase==5 ? 255 : 0
  // io_7: phase==5 ? 255 : 0
  // io_8: phase==3 or 5 ? 255 : 0
  const bool p1 = (phase == 1);
  const bool p2 = (phase == 2);
  const bool p3 = (phase == 3);
  const bool p4 = (phase == 4);
  const bool p5 = (phase == 5);

  const float io0 = (p1 || p4 || p5) ? 255.0f : 0.0f;
  const float io1 = (p2 || p4 || p5) ? 255.0f : 0.0f;
  const float io2 = (p3 || p5) ? 255.0f : 0.0f;
  const float io3 = (p2 || p4 || p5) ? 255.0f : 0.0f;
  const float io4 = (p2 || p4 || p5) ? 255.0f : 0.0f;
  const float io5 = (p5) ? 255.0f : 0.0f;
  const float io6 = (p5) ? 255.0f : 0.0f;
  const float io7 = (p5) ? 255.0f : 0.0f;
  const float io8 = (p3 || p5) ? 255.0f : 0.0f;

  analogWrite(RGB1_R_PIN, f64ToPwmCount255(io0));
  analogWrite(RGB1_G_PIN, f64ToPwmCount255(io1));
  analogWrite(RGB1_B_PIN, f64ToPwmCount255(io2));

  analogWrite(RGB2_R_PIN, f64ToPwmCount255(io3));
  analogWrite(RGB2_G_PIN, f64ToPwmCount255(io4));
  analogWrite(RGB2_B_PIN, f64ToPwmCount255(io5));

  analogWrite(RGB3_R_PIN, f64ToPwmCount255(io6));
  analogWrite(RGB3_G_PIN, f64ToPwmCount255(io7));
  analogWrite(RGB3_B_PIN, f64ToPwmCount255(io8));

  // io_9_segment_i: phase >= (i+1) ? true : false
  digitalWrite(SEG0_PIN, (phase >= 1) ? HIGH : LOW);
  digitalWrite(SEG1_PIN, (phase >= 2) ? HIGH : LOW);
  digitalWrite(SEG2_PIN, (phase >= 3) ? HIGH : LOW);
  digitalWrite(SEG3_PIN, (phase >= 4) ? HIGH : LOW);
  digitalWrite(SEG4_PIN, (phase >= 5) ? HIGH : LOW);
}

static void tickControllerStep() {
  // Model update:
  // if phaseAccumMs >= 2000:
  //   phase := (phase == 5 ? 1 : phase + 1)
  //   phaseAccumMs := 0
  // else:
  //   phaseAccumMs := phaseAccumMs + 10
  if (phaseAccumMs >= (int32_t)PHASE_DURATION_MS) {
    phase = (phase == 5) ? 1 : (phase + 1);
    phaseAccumMs = 0;
  } else {
    phaseAccumMs = phaseAccumMs + (int32_t)CONTROL_PERIOD_MS;
  }
}

void setup() {
  // Set PWM-capable pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Bargraph segments (digital outputs, active high)
  pinMode(SEG0_PIN, OUTPUT);
  pinMode(SEG1_PIN, OUTPUT);
  pinMode(SEG2_PIN, OUTPUT);
  pinMode(SEG3_PIN, OUTPUT);
  pinMode(SEG4_PIN, OUTPUT);

  // Ensure model initial state outputs applied immediately
  applyOutputs();
}

void loop() {
  static uint32_t lastMs = 0;
  const uint32_t now = millis();

  // Enforce exact 10ms control period stepping
  if ((uint32_t)(now - lastMs) >= CONTROL_PERIOD_MS) {
    // Step as many whole periods as have elapsed to preserve timing semantics
    while ((uint32_t)(millis() - lastMs) >= CONTROL_PERIOD_MS) {
      lastMs += CONTROL_PERIOD_MS;
      tickControllerStep();
      applyOutputs();
    }
  }
}
