#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static const uint8_t PIN_BTN1 = 2;   // io_0 (digital_input, active on LOW, pullup enabled)
static const uint8_t PIN_POT1 = A3;  // io_1 (analog_input)
static const uint8_t PIN_R = 3;      // io_2 PWM (pwm_output)
static const uint8_t PIN_G = 4;      // io_3 PWM (pwm_output)
static const uint8_t PIN_B = 5;      // io_4 PWM (pwm_output)

// Controller state (ACCEPTED executable model)
static int32_t selectedChannel = 0;     // initial 0
static bool previousPressed = false;    // initial false

// Helpers
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float clamp0to255(float x);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readDebouncedPressed10ms();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t pwmFromModelReal(float v);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_69\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float clamp0to255(float x) {
  if (x <= 0.0f) return 0.0f;
  if (x >= 255.0f) return 255.0f;
  return x;
}

// Debounced press is "provided by the model interface"; here we implement stable debounced
// boolean to match "debounced press edge". Since only a single stable boolean is required,
// we perform a straightforward debounce filter and then treat "pressed" as debounced.
static bool readDebouncedPressed10ms() {
  // Simple debounce with counters at 10ms loop rate.
  // Active on LOW: pressed when digitalRead == LOW.
  static bool stablePressed = false;
  static bool lastRawPressed = false;
  static uint8_t stableCount = 0;

  bool rawPressed = (digitalRead(PIN_BTN1) == LOW);

  if (rawPressed == lastRawPressed) {
    if (stableCount < 5) stableCount++;
  } else {
    lastRawPressed = rawPressed;
    stableCount = 0;
  }

  if (stableCount >= 3) { // require 30-50ms stability to confirm
    stablePressed = rawPressed;
  }

  return stablePressed;
}

static inline uint8_t pwmFromModelReal(float v) {
  // Model outputs are Real in [0,255]. Hardware expects 0..255.
  // Exact mapping: cast after clamp in model computation.
  if (v <= 0.0f) return 0;
  if (v >= 255.0f) return 255;
  return (uint8_t)(v + 0.5f);
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // Initialization requirement: RGB LED must remain off on reset.
  analogWrite(PIN_R, 0);
  analogWrite(PIN_G, 0);
  analogWrite(PIN_B, 0);

  selectedChannel = 0;
  previousPressed = false;
}

void loop() {
  // control_period_ms = 10
  // Maintain a 10ms control loop period.
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + 10;

  // io_0: btn1 debounced press (model true on press), active-high false in hardware => press when LOW
  bool io_0 = readDebouncedPressed10ms();

  // io_1: pot1 brightness scalar mapped in model (0-255)
  // Contract says model maps analog value to 0-255 brightness scalar.
  int raw = analogRead(PIN_POT1); // 0..1023
  float io_1 = (raw * (255.0f / 1023.0f)); // map to [0,255] as Real
  // (No further clamping here; model outputs clamp)

  // ===== Controller tick action (exact model logic ordering) =====
  // perform action tick:
  // first start;
  // then if (io_0 and (not previousPressed)) { ... then previousPressed := io_0; }
  // else previousPressed := io_0;
  if (io_0 && (!previousPressed)) {
    if (selectedChannel == 0) {
      selectedChannel = 1;
    } else {
      if (selectedChannel == 1) {
        selectedChannel = 2;
      } else {
        selectedChannel = 0;
      }
    }
    previousPressed = io_0;
  } else {
    previousPressed = io_0;
  }

  // ===== Outputs (exact frozen model expressions) =====
  // io_2 (Red):
  // (if selectedChannel==0 then clamp(io_1 to 0..255) else 0)
  float out2 = 0.0f;
  float out3 = 0.0f;
  float out4 = 0.0f;

  if (selectedChannel == 0) out2 = clamp0to255(io_1);
  else if (selectedChannel == 1) out3 = clamp0to255(io_1);
  else if (selectedChannel == 2) out4 = clamp0to255(io_1);

  // Drive PWM outputs with model values
  analogWrite(PIN_R, pwmFromModelReal(out2)); // io_2
  analogWrite(PIN_G, pwmFromModelReal(out3)); // io_3
  analogWrite(PIN_B, pwmFromModelReal(out4)); // io_4
}
