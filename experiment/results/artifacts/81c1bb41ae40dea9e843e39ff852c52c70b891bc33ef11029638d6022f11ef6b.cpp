#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

/*
  Generated from ACCEPTED executable model / frozen hardware contract.

  Controller:
    - directionReversed toggles on rising "io_2" (button pressed) edge
    - previousPressed stores last io_2
  Outputs:
    - Ten LED bar segments mapped to D2..D11
    - io_0_segment_0 corresponds to D2 (A1)
    - io_0_segment_1 corresponds to D3 (A2)
    - ...
    - io_0_segment_9 corresponds to D11 (A10)

  NOTE: The provided executable model only drives io_0_segment_0 (D2) and forces
  segments 1..9 to false. This behavior is implemented exactly as specified.
*/

// --- Frozen interface bindings ---
static const uint8_t SEG_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // io_0_segment_0..9 -> D2..D11
static const uint8_t BTN_PIN = 12;   // io_2 (btn1), active_high=false, pullup=true => pressed => D12 LOW => io_2=true

static const uint8_t POT_PIN = A3;  // io_1 (pot1) analog input

// --- Controller state ---
static bool directionReversed = false; // initial false
static bool previousPressed  = false; // initial false

// --- Sampling / timing ---
static const uint32_t CONTROL_PERIOD_MS = 5;

static unsigned long lastTickMs = 0;

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float readPotAsF64();
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static bool readButtonIo2();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static int floorToInt(float x);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void controllerTick();
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_10\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float readPotAsF64() {
  // io_1 type is f64 adc_count; mapping scale=1.0 offset=0.0
  // Arduino analogRead returns 0..1023.
  return (float)analogRead(POT_PIN);
}

static inline bool readButtonIo2() {
  // Model semantics: true_level is LOW because active_high=false and pullup=true.
  // So when D12 is LOW => pressed => io_2 = true.
  return (digitalRead(BTN_PIN) == LOW);
}

static inline int floorToInt(float x) {
  // Implement floor for the limited ADC range; x is non-negative.
  // For safety, still handle negatives.
  int i = (int)x;
  if (x < 0.0f && (float)i != x) return i - 1;
  return i;
}

static void applyOutputs() {
  // Model outputs:
  // io_0_segment_0 =
  //   if directionReversed then (io_1 >= 0.0) or false
  //   else ((floor(io_1) >= 1) or false)
  //
  // Since io_1 is non-negative ADC counts:
  // - directionReversed => always true
  // - otherwise => true iff floor(io_1) >= 1
  //
  // io_0_segment_1..9 are all false.

  float pot = readPotAsF64();
  int potFloor = floorToInt(pot);

  bool seg0;
  if (directionReversed) {
    seg0 = ((pot >= 0.0f) || false); // exact structure
  } else {
    seg0 = ((potFloor >= 1) || false); // exact structure
  }

  // Write to hardware:
  // io_0_segment_0 -> D2, active_high true => HIGH means LED on.
  digitalWrite(SEG_PINS[0], seg0 ? HIGH : LOW);

  // Segments 1..9 forced false
  for (int i = 1; i < 10; i++) {
    digitalWrite(SEG_PINS[i], LOW);
  }
}

static void controllerTick() {
  // perform action tick:
  //   first start;
  //   then if ((io_2) and ((not previousPressed))) { directionReversed := not directionReversed; }
  //   then previousPressed := io_2;

  bool io_2 = readButtonIo2();
  if (io_2 && (!previousPressed)) {
    directionReversed = !directionReversed;
  }
  previousPressed = io_2;

  // Outputs depend on updated directionReversed and current io_1
  applyOutputs();
}

void setup() {
  // Pins
  for (int i = 0; i < 10; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
  }

  pinMode(BTN_PIN, INPUT_PULLUP); // pullup=true
  pinMode(POT_PIN, INPUT);

  // Initialization requirement: LED bar graph off after reset.
  for (int i = 0; i < 10; i++) {
    digitalWrite(SEG_PINS[i], LOW);
  }

  // Initial states per contract
  directionReversed = false;
  previousPressed = false;

  lastTickMs = millis();
}

void loop() {
  const unsigned long now = millis();
  if (now - lastTickMs >= CONTROL_PERIOD_MS) {
    // Keep a stable 5ms tick
    lastTickMs += CONTROL_PERIOD_MS;

    controllerTick();
  }
}
