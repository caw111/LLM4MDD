#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring (from provided circuit)
// Button
static const uint8_t BTN_PIN = 2;      // btn1:1.l -> mega:2 (with internal pull-down/up inferred)

// RGB1
static const uint8_t RGB1_R = 3;        // mega:3  -> rgb1:R
static const uint8_t RGB1_G = 4;        // mega:4  -> rgb1:G
static const uint8_t RGB1_B = 5;        // mega:5  -> rgb1:B
static const uint8_t RGB1_COM = 19;     // mega:GND.1 -> rgb1:COM (common cathode)

// RGB2
static const uint8_t RGB2_R = 6;        // mega:6  -> rgb2:R
static const uint8_t RGB2_G = 7;        // mega:7  -> rgb2:G
static const uint8_t RGB2_B = 8;        // mega:8  -> rgb2:B
static const uint8_t RGB2_COM = 19;     // mega:GND.1 -> rgb2:COM (common cathode)

// Bar graph (pins A1..A7 on A1..A7; C1..C7 grounded)
static const uint8_t BAR_A[7] = {9, 10, 11, 12, 13, 44, 45}; // A1..A7
// C1..C7 are wired to GND; no need to drive.

// Common cathode bar LEDs: A pins HIGH light the LEDs (C pins are LOW).
// For safety, start all A pins LOW.

enum ModeColorIndex : uint8_t {
  COLOR_SEQ_RED_BLUE = 0,
  COLOR_SEQ_GREEN_CYAN,
  COLOR_SEQ_YELLOW_MAGENTA,
  COLOR_SEQ_CYAN_YELLOW,
  COLOR_SEQ_MEAGENTA_CYAN, // keeping spelling as given: "meagenta"
  COLOR_SEQ_WHITE_YELLOW,
  COLOR_SEQ_BLACK_WHITE
};

struct ColorRGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// Sequence: (red/blue, green/cyan, yellow/magenta, cyan/yellow, meagenta/cyan, white/yellow, black/white)
// Interpreting as: rgb1 shows first color, rgb2 shows second color per mode.
static const ColorRGB rgb1Seq[7] = {
  {255,   0,   0}, // red
  {  0, 255,   0}, // green
  {255, 255,   0}, // yellow
  {  0, 255, 255}, // cyan
  {255,   0, 255}, // meagenta (magenta)
  {255, 255, 255}, // white
  {  0,   0,   0}  // black
};

static const ColorRGB rgb2Seq[7] = {
  {  0,   0, 255}, // blue
  {  0, 255, 255}, // cyan
  {255,   0, 255}, // magenta
  {255, 255,   0}, // yellow
  {  0, 255, 255}, // cyan
  {255, 255,   0}, // yellow
  {255, 255, 255}  // white
};

static uint8_t mode = 0;

// Button debouncing/hold-to-trigger
static const unsigned long HOLD_MS = 150;
static bool prevBtnRaw = LOW;
static bool holdTriggered = false;
static unsigned long holdStartMs = 0;

// Helpers to set RGB for common cathode
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBPins(uint8_t rPin, uint8_t gPin, uint8_t bPin, const ColorRGB &c);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyMode(uint8_t m);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_86\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBPins(uint8_t rPin, uint8_t gPin, uint8_t bPin, const ColorRGB &c) {
  // If common cathode, HIGH turns ON.
  analogWrite(rPin, c.r > 0 ? 255 : 0);
  analogWrite(gPin, c.g > 0 ? 255 : 0);
  analogWrite(bPin, c.b > 0 ? 255 : 0);
}

static void allOff() {
  // RGB off
  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 0);
  analogWrite(RGB1_B, 0);

  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 0);

  // Bar off
  for (uint8_t i = 0; i < 7; i++) digitalWrite(BAR_A[i], LOW);
}

static void applyMode(uint8_t m) {
  // Mode 0: all OFF.
  if (m == 0) {
    allOff();
    return;
  }

  // Modes 1-7: set colors and bargraph count=m LEDs.
  // Ensure synchronous transition: set both RGBs and bar outputs together.
  uint8_t idx = m - 1; // 0..6

  // Turn everything off first (avoids transient mixed states).
  // Still within one loop iteration, but ensures clean transition.
  analogWrite(RGB1_R, 0);
  analogWrite(RGB1_G, 0);
  analogWrite(RGB1_B, 0);
  analogWrite(RGB2_R, 0);
  analogWrite(RGB2_G, 0);
  analogWrite(RGB2_B, 0);
  for (uint8_t i = 0; i < 7; i++) digitalWrite(BAR_A[i], LOW);

  // Set colors according to required sequence.
  setRGBPins(RGB1_R, RGB1_G, RGB1_B, rgb1Seq[idx]);
  setRGBPins(RGB2_R, RGB2_G, RGB2_B, rgb2Seq[idx]);

  // Bargraph: light up number of LEDs equal to current mode.
  for (uint8_t i = 0; i < m; i++) {
    digitalWrite(BAR_A[i], HIGH);
  }
}

void setup() {
  // Button input (no explicit pull resistor given in circuit).
  // Using INPUT_PULLUP is common for pushbuttons, but wiring indicates:
  // - btn1:2.r -> GND, and btn1:1.l -> mega:2
  // That means pressing connects mega:2 to GND, so released reads HIGH via pullup; pressed reads LOW.
  // We'll configure with pull-up.
  pinMode(BTN_PIN, INPUT_PULLUP);

  // RGB pins: use PWM-capable pins? On Mega, 3,4,5,6,7,8 are PWM-capable.
  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  // Bar graph A pins
  for (uint8_t i = 0; i < 7; i++) {
    pinMode(BAR_A[i], OUTPUT);
    digitalWrite(BAR_A[i], LOW);
  }

  // Start OFF state
  mode = 0;
  // Ensure RGBs and bargraph are off on boot/reset
  allOff();
}

void loop() {
  bool btnRaw = digitalRead(BTN_PIN); // HIGH when released, LOW when pressed (with pull-up)

  // Detect start of hold
  if (btnRaw == HIGH) {
    // Released: reset hold tracking
    prevBtnRaw = btnRaw;
    holdTriggered = false;
    holdStartMs = 0;
  } else {
    // Pressed
    if (!prevBtnRaw) {
      // already considered pressed; continue timing
    } else {
      // transition from released to pressed
      holdStartMs = millis();
      holdTriggered = false;
    }

    // If pressed long enough and not yet triggered, count as a valid press
    unsigned long now = millis();
    if (!holdTriggered && holdStartMs != 0 && (now - holdStartMs) >= HOLD_MS) {
      holdTriggered = true;

      // Increment mode with wrap 7->0
      mode = (uint8_t)((mode + 1) % 8);

      // Apply the new mode
      applyMode(mode);
    }

    prevBtnRaw = btnRaw;
  }

  // No further behavior specified; loop continues.
}
