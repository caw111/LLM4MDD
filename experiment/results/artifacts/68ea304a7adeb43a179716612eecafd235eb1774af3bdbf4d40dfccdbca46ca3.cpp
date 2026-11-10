#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

enum SystemPhase : uint8_t { PH_LED = 0, PH_RGB = 1, PH_SERVO = 2 };

static const uint8_t LED_PIN = 2; // led1:A -> Mega 2
static const uint8_t RGB1_R_PIN = 3; // rgb1:R -> 3
static const uint8_t RGB1_G_PIN = 4; // rgb1:G -> 4
static const uint8_t RGB1_B_PIN = 5; // rgb1:B -> 5

static const uint8_t RGB2_R_PIN = 6; // rgb2:R -> 6
static const uint8_t RGB2_G_PIN = 7; // rgb2:G -> 7
static const uint8_t RGB2_B_PIN = 8; // rgb2:B -> 8

// Servos
static const uint8_t SERVO1_PIN = 9;  // servo1:PWM -> 9
static const uint8_t SERVO2_PIN = 10; // servo2:PWM -> 10
static const uint8_t SERVO3_PIN = 11; // servo3:PWM -> 11

// Rainbow colors in required sequence
struct RGBColor { uint8_t r, g, b; };
static const RGBColor rainbowColors[] = {
  {255, 0,   0},   // red
  {0,   255, 0},   // green
  {0,   0,   255}, // blue
  {255, 255, 0},   // yellow
  {0,   255, 255}, // cyan
  {255, 0,   255}, // magenta
  {255, 255, 255}  // white
};

static const uint8_t kRainbowCount = sizeof(rainbowColors) / sizeof(rainbowColors[0]);

// Required timing: 2 seconds interval for each subsystem step
static const unsigned long INTERVAL_MS = 2000UL;

Servo servo1;
Servo servo2;
Servo servo3;

// State tracking
static SystemPhase phase = PH_LED;
static unsigned long phaseStartMs = 0;

// LED
static bool ledOn = false;

// RGB
static uint8_t colorIndex = 0;

// Servo positions (as required)
static int servo1Pos = 0;   // init 0
static int servo2Pos = 180; // init 180
static int servo3Pos = 90;  // init 90

#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyPhaseOutputs();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_110\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t r2, uint8_t g2, uint8_t b2) {
  analogWrite(RGB1_R_PIN, r1);
  analogWrite(RGB1_G_PIN, g1);
  analogWrite(RGB1_B_PIN, b1);

  analogWrite(RGB2_R_PIN, r2);
  analogWrite(RGB2_G_PIN, g2);
  analogWrite(RGB2_B_PIN, b2);
}

static void applyPhaseOutputs() {
  // LED on/off
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

  // RGB show current color (both rgb1 & rgb2)
  const RGBColor &c = rainbowColors[colorIndex];
  setRGB(c.r, c.g, c.b, c.r, c.g, c.b);

  // Servos positions
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
}

void setup() {
  // LED pin
  pinMode(LED_PIN, OUTPUT);

  // RGB pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization requirements:
  // - led1 should be off
  ledOn = false;

  // - RGB LEDs display red
  colorIndex = 0; // red

  // - servos at 0, 180, 90 degrees respectively
  servo1Pos = 0;
  servo2Pos = 180;
  servo3Pos = 90;

  // Apply initial outputs
  applyPhaseOutputs();

  // Start with LED blinking phase to satisfy synchronization requirement
  phase = PH_LED;
  phaseStartMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - phaseStartMs < INTERVAL_MS) {
    return;
  }
  phaseStartMs = now;

  // Update exactly one subsystem per 2-second interval (synchronized in a repeating cycle)
  switch (phase) {
    case PH_LED: {
      // LED toggles on/off every 2 seconds
      ledOn = !ledOn;
      phase = PH_RGB;
      break;
    }
    case PH_RGB: {
      // Cycle through rainbow colors; each color held for 2 seconds
      colorIndex = (colorIndex + 1) % kRainbowCount;
      phase = PH_SERVO;
      break;
    }
    case PH_SERVO: {
      // Servo synchronous scans with required rules, each scan every 2 seconds
      // Servo1: 0 <-> 180
      servo1Pos = (servo1Pos == 0) ? 180 : 0;

      // Servo2: 180 <-> 0
      servo2Pos = (servo2Pos == 180) ? 0 : 180;

      // Servo3: 90 <-> 135
      servo3Pos = (servo3Pos == 90) ? 135 : 90;

      // After servo step, go back to LED
      phase = PH_LED;
      break;
    }
  }

  applyPhaseOutputs();
}
