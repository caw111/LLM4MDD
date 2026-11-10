#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static constexpr uint8_t PIN_LED1 = 2;   // led1 -> D2
static constexpr uint8_t PIN_LED2 = 3;   // led2 -> D3
static constexpr uint8_t PIN_LED3 = 4;   // led3 -> D4

static constexpr uint8_t PIN_SERVO1 = 5; // servo1 -> D5
static constexpr uint8_t PIN_SERVO2 = 6; // servo2 -> D6
static constexpr uint8_t PIN_SERVO3 = 7; // servo3 -> D7

// bargraph1 segments:
// A1..A6 -> D8..D13, A7..A8 -> D44..D45
static constexpr uint8_t PIN_BG_SEG_0 = 8;   // A1
static constexpr uint8_t PIN_BG_SEG_1 = 9;   // A2
static constexpr uint8_t PIN_BG_SEG_2 = 10;  // A3
static constexpr uint8_t PIN_BG_SEG_3 = 11;  // A4
static constexpr uint8_t PIN_BG_SEG_4 = 12;  // A5
static constexpr uint8_t PIN_BG_SEG_5 = 13;  // A6
static constexpr uint8_t PIN_BG_SEG_6 = 44;  // A7
static constexpr uint8_t PIN_BG_SEG_7 = 45;  // A8

static constexpr uint32_t STATE_PERIOD_MS = 2000;

Servo servo1;
Servo servo2;
Servo servo3;

static constexpr uint8_t NUM_BG_SEGS = 8;
static const uint8_t bgSegPins[NUM_BG_SEGS] = {
  PIN_BG_SEG_0, PIN_BG_SEG_1, PIN_BG_SEG_2, PIN_BG_SEG_3,
  PIN_BG_SEG_4, PIN_BG_SEG_5, PIN_BG_SEG_6, PIN_BG_SEG_7
};

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBoolPin(uint8_t pin, bool v);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyControllerOutputs( float servo1_deg, float servo2_deg, float servo3_deg, bool led1_on, bool led2_on, bool led3_on, const bool bgSegOn[NUM_BG_SEGS] );
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void outputsForState(State s, float &out_servo1_deg, float &out_servo2_deg, float &out_servo3_deg, bool &out_led1, bool &out_led2, bool &out_led3, bool out_bg[NUM_BG_SEGS]);
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static State nextState(State s);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void enterState(State s);
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 167 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBoolPin(uint8_t pin, bool v) {
  digitalWrite(pin, v ? HIGH : LOW);
}

static void applyControllerOutputs(
  float servo1_deg, float servo2_deg, float servo3_deg,
  bool led1_on, bool led2_on, bool led3_on,
  const bool bgSegOn[NUM_BG_SEGS]
) {
  // Servo outputs
  servo1.write((int)servo1_deg);
  servo2.write((int)servo2_deg);
  servo3.write((int)servo3_deg);

  // LED outputs (active high)
  setBoolPin(PIN_LED1, led1_on);
  setBoolPin(PIN_LED2, led2_on);
  setBoolPin(PIN_LED3, led3_on);

  // Bar graph segments (active high)
  for (uint8_t i = 0; i < NUM_BG_SEGS; ++i) {
    setBoolPin(bgSegPins[i], bgSegOn[i]);
  }
}

enum class State : uint8_t {
  Init = 0,   // initial outputs persist for 2s: servos 0, all off
  S1,          // 2s
  S2,          // 2s
  S3,          // 2s
  Reset        // 2s
};

static void outputsForState(State s,
                             float &out_servo1_deg, float &out_servo2_deg, float &out_servo3_deg,
                             bool &out_led1, bool &out_led2, bool &out_led3,
                             bool out_bg[NUM_BG_SEGS]) {
  // default: everything off, servos 0
  out_servo1_deg = 0.0f;
  out_servo2_deg = 0.0f;
  out_servo3_deg = 0.0f;
  out_led1 = false;
  out_led2 = false;
  out_led3 = false;
  for (uint8_t i = 0; i < NUM_BG_SEGS; ++i) out_bg[i] = false;

  switch (s) {
    case State::Init:
      // all off, servos already at 0 (persist 2 seconds)
      break;

    case State::S1:
      out_servo1_deg = 60.0f;
      out_led1 = true;
      // first two segments: A1-A2 -> seg0, seg1
      out_bg[0] = true;
      out_bg[1] = true;
      break;

    case State::S2:
      out_servo2_deg = 120.0f;
      out_led2 = true;
      // next three segments: A3-A5 -> seg2, seg3, seg4
      out_bg[2] = true;
      out_bg[3] = true;
      out_bg[4] = true;
      break;

    case State::S3:
      out_servo3_deg = 180.0f;
      out_led3 = true;
      // final three segments: A6-A8 -> seg5, seg6, seg7
      out_bg[5] = true;
      out_bg[6] = true;
      out_bg[7] = true;
      break;

    case State::Reset:
      // all servos return to 0, all LEDs and bar graph off
      break;
  }
}

static State nextState(State s) {
  switch (s) {
    case State::Init:  return State::S1;
    case State::S1:    return State::S2;
    case State::S2:    return State::S3;
    case State::S3:    return State::Reset;
    case State::Reset: return State::S1; // sequence repeats indefinitely after reset
    default:           return State::Init;
  }
}

static void enterState(State s) {
  float s1 = 0.0f, s2 = 0.0f, s3 = 0.0f;
  bool l1 = false, l2 = false, l3 = false;
  bool bg[NUM_BG_SEGS];

  outputsForState(s, s1, s2, s3, l1, l2, l3, bg);
  applyControllerOutputs(s1, s2, s3, l1, l2, l3, bg);
}

void setup() {
  // Pin configuration
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);

  for (uint8_t i = 0; i < NUM_BG_SEGS; ++i) {
    pinMode(bgSegPins[i], OUTPUT);
  }

  // Ensure all outputs start off
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);
  digitalWrite(PIN_LED3, LOW);
  for (uint8_t i = 0; i < NUM_BG_SEGS; ++i) digitalWrite(bgSegPins[i], LOW);

  // Servo attach and initialize to 0°
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Persist initial state for 2 seconds
  enterState(State::Init);
}

void loop() {
  static State state = State::Init;
  static unsigned long enteredAtMs = 0;

  unsigned long now = millis();
  if (enteredAtMs == 0) enteredAtMs = now;

  // Maintain each state configuration for at least STATE_PERIOD_MS
  if ((now - enteredAtMs) >= STATE_PERIOD_MS) {
    state = nextState(state);
    enteredAtMs = now;
    enterState(state); // simultaneous updates at start of each state
  }

  // No additional timers or partial transitions; just stay in the current state.
}
