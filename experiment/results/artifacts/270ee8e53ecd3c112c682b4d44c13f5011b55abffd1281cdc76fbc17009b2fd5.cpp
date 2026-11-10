#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static const uint8_t PIN_BTN1 = 2;   // io_0, active HIGH in model terms when pressed; hardware uses pullup with true_level LOW
static const uint8_t PIN_POT1 = A3;  // io_1
static const uint8_t PIN_POT2 = A4;  // io_2

static const uint8_t PIN_SEG[10] = { 3,4,5,6,7,8,9,10,11,12 }; // io_4_segment_0..9 -> D3..D12

// Model state
static bool previousPressed = false;     // initial false
static int32_t pressedStableMs = 0;     // initial 0
static bool activeSourcePot1 = true;    // initial true

// Control period (requirement: 5ms)
static const uint32_t CONTROL_PERIOD_MS = 5;
static const uint32_t DEBOUNCE_MS = 150;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setAllSegmentsOff();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void applyOutputs(bool seg[10]);
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_75\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setAllSegmentsOff() {
  for (int i = 0; i < 10; i++) digitalWrite(PIN_SEG[i], LOW);
}

static inline void applyOutputs(bool seg[10]) {
  for (int i = 0; i < 10; i++) digitalWrite(PIN_SEG[i], seg[i] ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_BTN1, INPUT_PULLUP);

  for (int i = 0; i < 10; i++) {
    pinMode(PIN_SEG[i], OUTPUT);
  }

  // Initialization requirement: all LEDs off at start/reset
  setAllSegmentsOff();
}

void loop() {
  // Sample inputs
  // io_0: model bool true when pressed
  // Hardware: wiring contract says pullup=true and active_high=false => pressed corresponds to LOW.
  bool btnPressed = (digitalRead(PIN_BTN1) == LOW);

  // io_1, io_2: analog readings 0..1023
  int pot1Raw = analogRead(PIN_POT1);
  int pot2Raw = analogRead(PIN_POT2);

  // io_3: elapsed_ms delta since previous control step
  // As we run at fixed CONTROL_PERIOD_MS, use millis delta for robustness.
  static uint32_t lastStepMs = millis();
  uint32_t nowMs = millis();
  uint32_t deltaMsU = nowMs - lastStepMs;
  lastStepMs = nowMs;
  int32_t io_3 = (int32_t)deltaMsU;

  // ---- Accepted executable model tick ----
  // perform action tick
  // Update pressedStableMs:
  if (btnPressed) {
    pressedStableMs = pressedStableMs + io_3;
  } else {
    pressedStableMs = 0;
  }

  // Toggle activeSourcePot1 on valid press:
  if (btnPressed && (!previousPressed) && (pressedStableMs >= (int32_t)DEBOUNCE_MS)) {
    activeSourcePot1 = !activeSourcePot1;
  }

  previousPressed = btnPressed;

  // Compute outputs io_4_segment_0..9 using exact model thresholds
  // Model uses comparisons against Real thresholds (0.0, 102.3, ..., 1023.0).
  // We replicate behavior with integer adc values:
  // For thresholds T = 102.3, 204.6, ... we check adc >= ceil(T) to match (io > 0.0) / (io >= T) logic.
  // But to preserve exact boundary behavior, handle the special segment_0 case exactly as model:
  // segment_0:
  // if activeSourcePot1 and io_1 > 0.0 then (io_1 >= 102.3) else (not activeSourcePot1 and io_2 > 0.0) ...
  // Note: with integer adc, io_x > 0.0 <=> adc_x != 0.
  // and io_x == 0 maps to false unless the ">" path is taken.
  bool seg[10] = {false};

  // segment 0 special logic
  if (activeSourcePot1 && (pot1Raw > 0)) {
    seg[0] = (pot1Raw >= 102.3f);
  } else if ((!activeSourcePot1) && (pot2Raw > 0)) {
    seg[0] = (pot2Raw >= 102.3f);
  } else if (activeSourcePot1) {
    seg[0] = false; // activeSourcePot1 && io_1 == 0.0
  } else if (!activeSourcePot1 && (pot2Raw == 0)) {
    seg[0] = false;
  } else {
    // if activeSourcePot1 then io_1 >= 102.3 else io_2 >= 102.3
    seg[0] = (activeSourcePot1 ? (pot1Raw >= 102.3f) : (pot2Raw >= 102.3f));
  }

  // segments 1..9:
  // seg1..seg9 are simply (activeSourcePot1 ? io_1 >= threshold : io_2 >= threshold)
  // with thresholds: 204.6, 307.0, 409.2, 511.5, 613.8, 716.1, 818.4, 920.7, 1023.0
  // Using integer comparisons with float thresholds yields correct boundary for adc ints.
  auto ge = [](int adc, float thr) -> bool {
    return ((float)adc) >= thr;
  };

  seg[1] = activeSourcePot1 ? ge(pot1Raw, 204.6f) : ge(pot2Raw, 204.6f);
  seg[2] = activeSourcePot1 ? ge(pot1Raw, 307.0f) : ge(pot2Raw, 307.0f);
  seg[3] = activeSourcePot1 ? ge(pot1Raw, 409.2f) : ge(pot2Raw, 409.2f);
  seg[4] = activeSourcePot1 ? ge(pot1Raw, 511.5f) : ge(pot2Raw, 511.5f);
  seg[5] = activeSourcePot1 ? ge(pot1Raw, 613.8f) : ge(pot2Raw, 613.8f);
  seg[6] = activeSourcePot1 ? ge(pot1Raw, 716.1f) : ge(pot2Raw, 716.1f);
  seg[7] = activeSourcePot1 ? ge(pot1Raw, 818.4f) : ge(pot2Raw, 818.4f);
  seg[8] = activeSourcePot1 ? ge(pot1Raw, 920.7f) : ge(pot2Raw, 920.7f);
  seg[9] = activeSourcePot1 ? ge(pot1Raw, 1023.0f) : ge(pot2Raw, 1023.0f);

  applyOutputs(seg);

  // Wait remaining time to maintain 5ms control period
  // (Not strictly required for functional correctness but matches planning/control_period_ms)
  uint32_t endMs = millis();
  uint32_t elapsed = endMs - nowMs;
  if (elapsed < CONTROL_PERIOD_MS) {
    delay(CONTROL_PERIOD_MS - elapsed);
  }
}
