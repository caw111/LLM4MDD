#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Frozen interface contract pins (Arduino Mega 2560 as specified in circuit)
static constexpr uint8_t RGB_R_PIN = 2; // D2
static constexpr uint8_t RGB_G_PIN = 3; // D3
static constexpr uint8_t RGB_B_PIN = 4; // D4

static constexpr uint8_t SERVO1_PIN = 5;  // D5
static constexpr uint8_t SERVO2_PIN = 6;  // D6
static constexpr uint8_t SERVO3_PIN = 7;  // D7

// SysML accepted model states
// color sequence: Red, Green, Blue, Yellow, Cyan, Magenta, White
static const uint8_t COLOR_COUNT = 7;

// Initial controller state
static int32_t colorIndex = 0;
static int32_t phaseMs = 0;

// Servo mapping; the contract mentions angles beyond 180 must be handled by model concern.
// We implement a "hold_last_valid" behavior for invalid targets (policy).
// Thus, any command outside [0,180] keeps the previous servo command.
static float lastServo1 = 0.0f;
static float lastServo2 = 90.0f;
static float lastServo3 = 180.0f;

// Servo objects
static Servo servo1;
static Servo servo2;
static Servo servo3;

// Apply outputs exactly per contract mapping for each color state.
// RGB is driven via three PWM outputs in the model contract; actual hardware uses PWM pins.
// For color selection, we use full on/off intensities (1.0 -> 255, 0.0 -> 0).
struct ColorOutputs {
  float io_0; // red intensity
  float io_1; // green intensity
  float io_2; // blue intensity
  float io_3; // servo1 angle
  float io_4; // servo2 angle
  float io_5; // servo3 angle
};

#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static ColorOutputs outputsForColor(uint8_t idx);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRgbIntensity(float intensity, uint8_t pin);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool validAngle(float deg);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyServoAngleOrHold(float deg, Servo &s, float &lastValid);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(const ColorOutputs &o);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 122 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_46\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static ColorOutputs outputsForColor(uint8_t idx) {
  switch (idx) {
    case 0: // Red
      return {1.0f, 0.0f, 0.0f, 0.0f, 90.0f, 180.0f};
    case 1: // Green
      return {0.0f, 1.0f, 0.0f, 90.0f, 180.0f, 0.0f};
    case 2: // Blue
      return {0.0f, 0.0f, 1.0f, 180.0f, 0.0f, 90.0f};
    case 3: // Yellow (servo3 target is 225; invalid -> hold_last_valid)
      return {1.0f, 1.0f, 0.0f, 45.0f, 135.0f, 225.0f};
    case 4: // Cyan (servo2 target is 225; invalid -> hold_last_valid)
      return {0.0f, 1.0f, 1.0f, 135.0f, 225.0f, 45.0f};
    case 5: // Magenta (servo1 target is 225; invalid -> hold_last_valid)
      return {1.0f, 0.0f, 1.0f, 225.0f, 45.0f, 135.0f};
    case 6: // White (back to initial)
    default:
      return {1.0f, 1.0f, 1.0f, 0.0f, 90.0f, 180.0f};
  }
}

static void setRgbIntensity(float intensity, uint8_t pin) {
  // Contract "pwm_count" with scale 1.0. Use full on/off.
  // intensity is expected to be 0.0 or 1.0 by our color mapping.
  int value = (int)lroundf(intensity * 255.0f);
  value = constrain(value, 0, 255);
  analogWrite(pin, value);
}

static bool validAngle(float deg) {
  return (deg >= 0.0f && deg <= 180.0f);
}

static void applyServoAngleOrHold(float deg, Servo &s, float &lastValid) {
  if (validAngle(deg)) {
    lastValid = deg;
    s.write((int)lroundf(deg));
  } else {
    // hold_last_valid
    s.write((int)lroundf(lastValid));
  }
}

static void applyOutputs(const ColorOutputs &o) {
  // io_0..io_2 are RGB intensities
  setRgbIntensity(o.io_0, RGB_R_PIN);
  setRgbIntensity(o.io_1, RGB_G_PIN);
  setRgbIntensity(o.io_2, RGB_B_PIN);

  // io_3..io_5 are servo angles with invalid_policy: hold_last_valid
  applyServoAngleOrHold(o.io_3, servo1, lastServo1);
  applyServoAngleOrHold(o.io_4, servo2, lastServo2);
  applyServoAngleOrHold(o.io_5, servo3, lastServo3);
}

void setup() {
  // RGB pins
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialize controller outputs at time 0 to match contract initial:
  // io_0=1, io_1=0, io_2=0, io_3=0, io_4=90, io_5=180 (Red)
  ColorOutputs initOut = outputsForColor((uint8_t)(colorIndex % COLOR_COUNT));
  applyOutputs(initOut);

  // Initialize timing base
  phaseMs = 0;
}

// Control period in contract is 10ms; we tick continuously based on millis.
static uint32_t lastTickMs = 0;

void loop() {
  uint32_t now = millis();

  // Handle control_period_ms = 10
  if (lastTickMs == 0) lastTickMs = now;

  // Run ticks at roughly 10ms cadence
  while ((uint32_t)(now - lastTickMs) >= 10u) {
    lastTickMs += 10u;

    // SysML model action tick:
    // then assign phaseMs := phaseMs; then assign colorIndex := colorIndex;
    // The SysML snippet provided doesn't show the state update logic for
    // advancing colors; the requirement text mandates 2s per color.
    // Therefore, we advance colorIndex based on elapsed time since last change,
    // while keeping the explicit "outputs based on current RGB color state"
    // and the contract's 2-second hold.
    phaseMs = (int32_t)(now - (uint32_t)0); // continuous elapsed ms

    // Determine which color should be active:
    // Each color held for 2000ms; first delta_since_previous_step is zero means
    // start at colorIndex=0 at t=0.
    uint32_t t = now;
    uint32_t step = t / 2000u; // advance every 2 seconds
    uint8_t desiredIndex = (uint8_t)(step % COLOR_COUNT);

    // Keep servo commands consistent with current color state.
    if ((int32_t)desiredIndex != colorIndex) {
      colorIndex = (int32_t)desiredIndex;

      ColorOutputs out = outputsForColor((uint8_t)(colorIndex % COLOR_COUNT));
      applyOutputs(out);
    }
  }

  // No blocking delays (timing is via millis ticks).
}
