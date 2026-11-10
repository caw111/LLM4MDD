#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

namespace Pins {
  // Monochrome LEDs (active-high)
  constexpr uint8_t LED1_PIN = 2;   // io_0
  constexpr uint8_t LED2_PIN = 3;   // io_1

  // RGB LEDs (PWM channels)
  constexpr uint8_t RGB1_R_PIN = 4; // io_2
  constexpr uint8_t RGB1_G_PIN = 5; // io_3
  constexpr uint8_t RGB1_B_PIN = 6; // io_4

  constexpr uint8_t RGB2_R_PIN = 7; // io_5
  constexpr uint8_t RGB2_G_PIN = 8; // io_6
  constexpr uint8_t RGB2_B_PIN = 9; // io_7

  // Bargraph segments (active-high)
  constexpr uint8_t BAR_A1 = 10; // io_8_segment_0
  constexpr uint8_t BAR_A2 = 11; // io_8_segment_1
  constexpr uint8_t BAR_A3 = 12; // io_8_segment_2
  constexpr uint8_t BAR_A4 = 13; // io_8_segment_3
  constexpr uint8_t BAR_A5 = 44; // io_8_segment_4
  constexpr uint8_t BAR_A6 = 45; // io_8_segment_5
}

static constexpr uint32_t STATE_DURATION_MS = 2000;

// PWM scale: model uses pwm_count. Use full 8-bit PWM for "on".
static constexpr uint8_t PWM_ON = 255;
static constexpr uint8_t PWM_OFF = 0;

#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyMonochrome(bool led1_on, bool led2_on);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyRGB1(uint8_t r, uint8_t g, uint8_t b);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyRGB2(uint8_t r, uint8_t g, uint8_t b);
#line 49 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyBargraph(bool s0, bool s1, bool s2, bool s3, bool s4, bool s5);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void allOff();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyState(uint8_t state);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_114\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyMonochrome(bool led1_on, bool led2_on) {
  digitalWrite(Pins::LED1_PIN, led1_on ? HIGH : LOW);
  digitalWrite(Pins::LED2_PIN, led2_on ? HIGH : LOW);
}

static void applyRGB1(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(Pins::RGB1_R_PIN, r);
  analogWrite(Pins::RGB1_G_PIN, g);
  analogWrite(Pins::RGB1_B_PIN, b);
}

static void applyRGB2(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(Pins::RGB2_R_PIN, r);
  analogWrite(Pins::RGB2_G_PIN, g);
  analogWrite(Pins::RGB2_B_PIN, b);
}

static void applyBargraph(bool s0, bool s1, bool s2, bool s3, bool s4, bool s5) {
  digitalWrite(Pins::BAR_A1, s0 ? HIGH : LOW);
  digitalWrite(Pins::BAR_A2, s1 ? HIGH : LOW);
  digitalWrite(Pins::BAR_A3, s2 ? HIGH : LOW);
  digitalWrite(Pins::BAR_A4, s3 ? HIGH : LOW);
  digitalWrite(Pins::BAR_A5, s4 ? HIGH : LOW);
  digitalWrite(Pins::BAR_A6, s5 ? HIGH : LOW);
}

static void allOff() {
  // Monochrome off
  applyMonochrome(false, false);

  // RGB all channels off
  applyRGB1(PWM_OFF, PWM_OFF, PWM_OFF);
  applyRGB2(PWM_OFF, PWM_OFF, PWM_OFF);

  // Bargraph all segments off
  applyBargraph(false, false, false, false, false, false);
}

static void applyState(uint8_t state) {
  // Preserve exact semantic intent from requirements:
  // State 1: led1 on, led2 off; rgb1 red; bargraph A1-A3 on.
  // State 2: led2 on, led1 off; rgb2 green; bargraph A4-A6 on.
  // State 3: both monochrome on; both rgb1 & rgb2 cyan (blue+green); all segments on.
  switch (state) {
    case 1: {
      applyMonochrome(true, false);

      // rgb1 red; rgb2 off
      applyRGB1(PWM_ON, PWM_OFF, PWM_OFF);
      applyRGB2(PWM_OFF, PWM_OFF, PWM_OFF);

      // bargraph A1..A3 on
      applyBargraph(true, true, true, false, false, false);
      break;
    }
    case 2: {
      applyMonochrome(false, true);

      // rgb2 green; rgb1 off
      applyRGB1(PWM_OFF, PWM_OFF, PWM_OFF);
      applyRGB2(PWM_OFF, PWM_ON, PWM_OFF);

      // bargraph A4..A6 on
      applyBargraph(false, false, false, true, true, true);
      break;
    }
    case 3: {
      applyMonochrome(true, true);

      // both rgb1 and rgb2 cyan = (blue on, green on, red off)
      applyRGB1(PWM_OFF, PWM_ON, PWM_ON);
      applyRGB2(PWM_OFF, PWM_ON, PWM_ON);

      // all segments on
      applyBargraph(true, true, true, true, true, true);
      break;
    }
    default:
      // Should never happen; keep outputs safe/off.
      allOff();
      break;
  }
}

void setup() {
  // Configure I/O
  pinMode(Pins::LED1_PIN, OUTPUT);
  pinMode(Pins::LED2_PIN, OUTPUT);

  // RGB pins are PWM capable; set as OUTPUT
  pinMode(Pins::RGB1_R_PIN, OUTPUT);
  pinMode(Pins::RGB1_G_PIN, OUTPUT);
  pinMode(Pins::RGB1_B_PIN, OUTPUT);
  pinMode(Pins::RGB2_R_PIN, OUTPUT);
  pinMode(Pins::RGB2_G_PIN, OUTPUT);
  pinMode(Pins::RGB2_B_PIN, OUTPUT);

  // Bargraph pins
  pinMode(Pins::BAR_A1, OUTPUT);
  pinMode(Pins::BAR_A2, OUTPUT);
  pinMode(Pins::BAR_A3, OUTPUT);
  pinMode(Pins::BAR_A4, OUTPUT);
  pinMode(Pins::BAR_A5, OUTPUT);
  pinMode(Pins::BAR_A6, OUTPUT);

  // Initialization contract: all LEDs and bar graph segments off at start/reset.
  allOff();
}

void loop() {
  static uint8_t state = 1;
  static uint32_t stateStartMs = 0;

  uint32_t now = millis();

  // Ensure deterministic state entry on startup
  if (stateStartMs == 0) {
    state = 1;
    stateStartMs = now;
    applyState(state);
    return;
  }

  if (now - stateStartMs >= STATE_DURATION_MS) {
    // Advance state 1->2->3->1 ...
    state = (state % 3) + 1;
    stateStartMs = now;
    applyState(state);
  }

  // No other behavior; keep outputs as set for current state.
}
