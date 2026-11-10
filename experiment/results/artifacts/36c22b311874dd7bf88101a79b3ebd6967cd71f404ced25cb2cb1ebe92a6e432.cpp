#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// Frozen interface contract bindings
static constexpr uint8_t BTN1_PIN = 2;   // io_0 (active low with pullup)
static constexpr uint8_t SEG_PINS[10] = { // io_1_segment_0..9
  3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setSegment(uint8_t idx, bool on);
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
int main();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_72\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline void setSegment(uint8_t idx, bool on) {
  // active_high true_level = HIGH
  digitalWrite(SEG_PINS[idx], on ? HIGH : LOW);
}

int main() {
  init();

  // --- setup() equivalent ---
  pinMode(BTN1_PIN, INPUT_PULLUP); // pullup true, active_high=false => LOW means pressed

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
    setSegment(i, false); // litCount initial false => all LEDs off
  }

  // Controller state (initial per model)
  int32_t litCount = 0;
  bool previousPressed = false;
  bool debounceArmed = true;
  int32_t debounceRemainingMs = 0;

  // --- loop() equivalent with control_period_ms = 5 ---
  uint32_t lastTickMs = millis();

  while (true) {
    uint32_t now = millis();
    // 5 ms fixed-rate tick (ignore drift by catching up)
    if ((uint32_t)(now - lastTickMs) >= 5) {
      lastTickMs += 5;

      // Sample io_0
      // model semantics: true_level = LOW, so io_0 (bool) = (digitalRead == LOW)
      bool io_0 = (digitalRead(BTN1_PIN) == LOW);

      // perform tick (preserve ordered updates exactly)
      // note: "first start" blocks imply atomic sequencing; we keep the order as written.
      if (io_0 && (!previousPressed) && debounceArmed) {
        if (litCount == 10) {
          litCount = 0;
        } else {
          litCount = litCount + 1;
        }
        debounceArmed = false;
        debounceRemainingMs = 150;
      }

      if ((debounceRemainingMs >= 5) && (!debounceArmed)) {
        debounceRemainingMs = debounceRemainingMs - 5;
        if ((debounceRemainingMs - 5) <= 0) {
          debounceArmed = true;
        }
      }

      if ((debounceRemainingMs < 5) && (!debounceArmed)) {
        debounceArmed = true;
      }

      previousPressed = io_0;

      // outputs based on litCount (ordered updates not required; set all)
      setSegment(0, litCount >= 1);
      setSegment(1, litCount >= 2);
      setSegment(2, litCount >= 3);
      setSegment(3, litCount >= 4);
      setSegment(4, litCount >= 5);
      setSegment(5, litCount >= 6);
      setSegment(6, litCount >= 7);
      setSegment(7, litCount >= 8);
      setSegment(8, litCount >= 9);
      setSegment(9, litCount >= 10);
    }
  }

  return 0;
}

// Arduino requires setup/loop for some toolchains; provide them to satisfy expectations.
void setup() { /* handled in main() */ }
void loop()  { /* handled in main() */ }
