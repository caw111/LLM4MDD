#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract bindings (Arduino Mega 2560):
// servo1Angle (D2), servo2Angle (D3), servo3Angle (D4)
// bargraph segments: D5..D13 = A1..A9, and D44 = A10
// Common return on GND.1 for bargraph segments.

static constexpr uint8_t SERVO1_PIN = 2;   // D2
static constexpr uint8_t SERVO2_PIN = 3;   // D3
static constexpr uint8_t SERVO3_PIN = 4;   // D4

static constexpr uint8_t BAR_A1_PIN  = 5;   // D5
static constexpr uint8_t BAR_A2_PIN  = 6;   // D6
static constexpr uint8_t BAR_A3_PIN  = 7;   // D7
static constexpr uint8_t BAR_A4_PIN  = 8;   // D8
static constexpr uint8_t BAR_A5_PIN  = 9;   // D9
static constexpr uint8_t BAR_A6_PIN  = 10;  // D10
static constexpr uint8_t BAR_A7_PIN  = 11;  // D11
static constexpr uint8_t BAR_A8_PIN  = 12;  // D12
static constexpr uint8_t BAR_A9_PIN  = 13;  // D13
static constexpr uint8_t BAR_A10_PIN = 44; // D44

// Control model (Accepted executable model):
// - Initial state: servo1Angle=0, servo2Angle=0, servo3Angle=0; all bar segments off.
// - Every 2 seconds (synchronized): each servo +/- 18 degrees with limits [0,180] and reflects at boundaries.
// - LED bar: total = servo1Angle + servo2Angle + servo3Angle in [0..540]
//   LEDs correspond to floor(total / 54) clamped to [0..10].
//   Segment i (0..9) is on iff i < litCount.

static Servo servo1;
static Servo servo2;
static Servo servo3;

static constexpr unsigned long CONTROL_PERIOD_MS = 2000UL; // update simultaneously every 2 seconds
static constexpr double STEP_DEG = 18.0;
static constexpr double SERVO_MIN_DEG = 0.0;
static constexpr double SERVO_MAX_DEG = 180.0;

static constexpr double TOTAL_MIN = 0.0;
static constexpr double TOTAL_MAX = 540.0;
static constexpr double UNITS_PER_LED = 54.0; // 10 ranges -> 54 units per LED

// Controller state per contract
static double servo1Angle = 0.0;
static double servo2Angle = 0.0;
static double servo3Angle = 0.0;

// Direction state (not part of frozen interface outputs, but needed to implement the exact behavior)
static int servo1Dir = +1; // +1 means increase by 18; -1 means decrease by 18
static int servo2Dir = +1;
static int servo3Dir = +1;

static unsigned long nextUpdateAtMs = 0;

#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBarSegments(bool segOn[10]);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 90 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void stepServo(double &angle, int &dir);
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 142 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_27\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setBarSegments(bool segOn[10]) {
  static const uint8_t pins[10] = {
    BAR_A1_PIN, BAR_A2_PIN, BAR_A3_PIN, BAR_A4_PIN, BAR_A5_PIN,
    BAR_A6_PIN, BAR_A7_PIN, BAR_A8_PIN, BAR_A9_PIN, BAR_A10_PIN
  };

  for (int i = 0; i < 10; ++i) {
    digitalWrite(pins[i], segOn[i] ? HIGH : LOW);
  }
}

static void applyOutputs() {
  // drive servos
  servo1.write((int)servo1Angle);
  servo2.write((int)servo2Angle);
  servo3.write((int)servo3Angle);

  // compute bargraph output (floor(total/54), clamped 0..10)
  double total = servo1Angle + servo2Angle + servo3Angle;

  if (total < TOTAL_MIN) total = TOTAL_MIN;
  if (total > TOTAL_MAX) total = TOTAL_MAX;

  int litCount = (int)floor(total / UNITS_PER_LED); // 0..10
  if (litCount < 0) litCount = 0;
  if (litCount > 10) litCount = 10;

  bool segOn[10];
  for (int i = 0; i < 10; ++i) {
    segOn[i] = (i < litCount); // segment indices 0..9
  }
  setBarSegments(segOn);
}

static void stepServo(double &angle, int &dir) {
  // Update by +/-18, with exact boundary behavior:
  // - "increases until reaching 180°, then decreases until reaching 0°"
  // - Since step is 18 and starting at 0, it lands exactly on boundaries.
  // Implementation: propose next, reflect direction if at boundary.
  double next = angle + dir * STEP_DEG;

  // If we are at/above the boundary due to numerical reasons, clamp and reflect.
  if (next > SERVO_MAX_DEG) {
    next = SERVO_MAX_DEG;
    dir = -1;
  } else if (next < SERVO_MIN_DEG) {
    next = SERVO_MIN_DEG;
    dir = +1;
  }

  angle = next;
}

void setup() {
  // Initialize bargraph pins
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

  // Initial outputs per contract: all off, all servos at 0 degrees.
  // Attach servos and immediately write initial angle.
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  servo1Angle = 0.0;
  servo2Angle = 0.0;
  servo3Angle = 0.0;

  servo1Dir = +1;
  servo2Dir = +1;
  servo3Dir = +1;

  // Ensure initial bargraph is all segments off.
  applyOutputs();

  nextUpdateAtMs = millis() + CONTROL_PERIOD_MS; // first movement occurs after 2 seconds
}

void loop() {
  unsigned long now = millis();
  if ((long)(now - nextUpdateAtMs) >= 0) {
    // Synchronized update: step all servos simultaneously every 2 seconds.
    stepServo(servo1Angle, servo1Dir);
    stepServo(servo2Angle, servo2Dir);
    stepServo(servo3Angle, servo3Dir);

    applyOutputs();

    nextUpdateAtMs += CONTROL_PERIOD_MS;
  }
}
