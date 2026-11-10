#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract:
// io_0 input  bool  -> D2 (pullup), active_low (pressed when LOW)
// io_1 output f64 pwm_count -> D3 (R), 0 or 255
// io_2 output f64 pwm_count -> D4 (G), 0 or 255
// io_3 output f64 pwm_count -> D5 (B), 0 or 255
//
// ACCEPTED model: control_period_ms = 5
// States:
//   colorIndex initial 7
//   pressHoldMs initial 0
//   validPressPending initial false
//   previousPressed initial false
//   colorOnes initial 0 (unused in outputs; outputs force colorOnes := 0 each tick)
// Update order per tick:
//   1) if io_0: pressHoldMs += 5
//      else: if pressHoldMs >= 150: validPressPending=true else false; pressHoldMs=0
//   2) previousPressed := io_0
//   3) if validPressPending: colorIndex := (colorIndex + 1) - 8*floor((colorIndex+1)/8); validPressPending := false
//   4) colorOnes := 0
//
// Outputs computed from colorIndex:
//   io_1 (R) = 255 if (0 or 3 or 5 or 6) else 0
//   io_2 (G) = 255 if (1 or 3 or 4 or 6) else 0
//   io_3 (B) = 255 if (2 or 4 or 5 or 6) else 0

static constexpr uint8_t PIN_BTN = 2;   // io_0
static constexpr uint8_t PIN_R   = 3;   // io_1
static constexpr uint8_t PIN_G   = 4;   // io_2
static constexpr uint8_t PIN_B   = 5;   // io_3

static constexpr uint32_t CONTROL_PERIOD_MS = 5;

// Model attributes
static int32_t colorIndex = 7;
static int32_t pressHoldMs = 0;
static bool validPressPending = false;
static bool previousPressed = false;
static int32_t colorOnes = 0; // unused in outputs but kept per model

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputsFromColorIndex(int32_t idx);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_65\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputsFromColorIndex(int32_t idx) {
  // Map model outputs (0.0 or 255.0) to PWM
  // Using analogWrite with value in [0..255]
  uint8_t r = (idx == 0 || idx == 3 || idx == 5 || idx == 6) ? 255 : 0;
  uint8_t g = (idx == 1 || idx == 3 || idx == 4 || idx == 6) ? 255 : 0;
  uint8_t b = (idx == 2 || idx == 4 || idx == 5 || idx == 6) ? 255 : 0;

  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // active_high=false with pullup=true
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // Initialization requirement: RGB LED starts off (all channels disabled).
  // Model starts with colorIndex=7, and outputs for 7 yield all 0. Ensure PWM = 0.
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) < CONTROL_PERIOD_MS) {
    return;
  }
  lastTickMs = now;

  // Sample io_0: boolean modeled true when pressed (LOW because active_high=false)
  // io_0 meaning: pressed gate (pressed when modeled as true)
  bool io_0 = (digitalRead(PIN_BTN) == LOW);

  // ---- Tick update per model (preserve exact order) ----

  // Update 1:
  // if io_0 { pressHoldMs := pressHoldMs + 5; }
  // else { if pressHoldMs >= 150 validPressPending := true else false; pressHoldMs := 0; }
  if (io_0) {
    pressHoldMs = pressHoldMs + 5;
  } else {
    if (pressHoldMs >= 150) {
      validPressPending = true;
    } else {
      validPressPending = false;
    }
    pressHoldMs = 0;
  }

  // Update 2: previousPressed := io_0
  previousPressed = io_0;

  // Update 3:
  // if validPressPending { colorIndex := (colorIndex + 1) - 8*floor((colorIndex + 1)/8); validPressPending := false; }
  if (validPressPending) {
    int32_t next = colorIndex + 1;
    // floor(next/8) for non-negative integers equals integer division,
    // but keep explicit floor behavior by using integer division.
    int32_t flo = next / 8;
    colorIndex = next - 8 * flo;
    validPressPending = false;
  }

  // Update 4: colorOnes := 0;
  colorOnes = 0;

  // Outputs:
  applyOutputsFromColorIndex(colorIndex);
}
