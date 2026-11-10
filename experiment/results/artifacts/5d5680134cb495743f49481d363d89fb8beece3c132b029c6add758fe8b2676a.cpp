#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED1_PIN = 2;  // led1:A -> mega:2
static const uint8_t LED2_PIN = 3;  // led2:A -> mega:3
static const uint8_t LED3_PIN = 4;  // led3:A -> mega:4

// Pushbuttons (wired as: btnx:2.r to GND, btnx:1.l to pin via green)
// Therefore use INPUT_PULLUP and treat LOW as pressed.
static const uint8_t BTN1_PIN = 5;  // mega:5 -> btn1:1.l
static const uint8_t BTN2_PIN = 6;  // mega:6 -> btn2:1.l
static const uint8_t BTN3_PIN = 7;  // mega:7 -> btn3:1.l

// Bargraph segments (A1-A7)
static const uint8_t BAR_A1 = 8;   // mega:8  -> bargraph1:A1
static const uint8_t BAR_A2 = 9;   // mega:9  -> bargraph1:A2
static const uint8_t BAR_A3 = 10;  // mega:10 -> bargraph1:A3
static const uint8_t BAR_A4 = 11;  // mega:11 -> bargraph1:A4
static const uint8_t BAR_A5 = 12;  // mega:12 -> bargraph1:A5
static const uint8_t BAR_A6 = 13;  // mega:13 -> bargraph1:A6
static const uint8_t BAR_A7 = 44;  // mega:44 -> bargraph1:A7

static const uint8_t BAR_PINS[7] = {BAR_A1, BAR_A2, BAR_A3, BAR_A4, BAR_A5, BAR_A6, BAR_A7};

static uint8_t led1State = 0; // lowest bit
static uint8_t led2State = 0; // middle bit
static uint8_t led3State = 0; // highest bit

// Debounce / press-length filter: ignore presses shorter than 0.15s.
static const uint16_t MIN_PRESS_MS = 150;

// Track per-button press timing to determine press duration
static bool btnPrevPressed[3] = {false, false, false};
static uint32_t btnPressStartMs[3] = {0, 0, 0};

#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllOff();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t currentBinaryValue();
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateLedsAndBargraph();
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isPressed(uint8_t pin);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 35 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setAllOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  for (uint8_t i = 0; i < 7; i++) digitalWrite(BAR_PINS[i], LOW);
}

static inline uint8_t currentBinaryValue() {
  // Bit mapping:
  // btn1 toggles LED1 -> lowest bit
  // btn2 toggles LED2 -> middle bit
  // btn3 toggles LED3 -> highest bit
  return (led3State ? 4 : 0) | (led2State ? 2 : 0) | (led1State ? 1 : 0);
}

static inline void updateLedsAndBargraph() {
  digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
  digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
  digitalWrite(LED3_PIN, led3State ? HIGH : LOW);

  uint8_t val = currentBinaryValue(); // 0-7
  // Bar graph lights the segment corresponding to decimal value (0-7).
  // Given wiring only A1-A7 exist (7 segments), so represent:
  // 0 -> no segment (all off), 1..7 -> A1..A7 respectively.
  for (uint8_t i = 0; i < 7; i++) digitalWrite(BAR_PINS[i], LOW);

  if (val >= 1 && val <= 7) {
    digitalWrite(BAR_PINS[val - 1], HIGH); // val=1 -> A1 ... val=7 -> A7
  }
}

static inline bool isPressed(uint8_t pin) {
  // INPUT_PULLUP: pressed => LOW
  return digitalRead(pin) == LOW;
}

void setup() {
  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Buttons
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Bargraph segments
  for (uint8_t i = 0; i < 7; i++) pinMode(BAR_PINS[i], OUTPUT);

  setAllOff();

  // Ensure initial states reflect "all off"
  led1State = 0;
  led2State = 0;
  led3State = 0;
  updateLedsAndBargraph();

  // Initialize button tracking based on current level
  btnPrevPressed[0] = isPressed(BTN1_PIN);
  btnPrevPressed[1] = isPressed(BTN2_PIN);
  btnPrevPressed[2] = isPressed(BTN3_PIN);
  btnPressStartMs[0] = 0;
  btnPressStartMs[1] = 0;
  btnPressStartMs[2] = 0;
}

void loop() {
  const uint8_t pins[3] = {BTN1_PIN, BTN2_PIN, BTN3_PIN};

  for (uint8_t idx = 0; idx < 3; idx++) {
    bool pressedNow = isPressed(pins[idx]);
    bool pressedPrev = btnPrevPressed[idx];

    // Transition: not pressed -> pressed => start timer
    if (!pressedPrev && pressedNow) {
      btnPressStartMs[idx] = millis();
    }

    // Transition: pressed -> not pressed => evaluate duration
    if (pressedPrev && !pressedNow) {
      uint32_t start = btnPressStartMs[idx];
      uint32_t elapsed = (start == 0) ? 0 : (millis() - start);

      // Ignore if shorter than 0.15s
      if (elapsed >= MIN_PRESS_MS) {
        // Toggle corresponding LED state
        if (idx == 0) led1State = !led1State; // btn1 -> LED1/bit0
        if (idx == 1) led2State = !led2State; // btn2 -> LED2/bit1
        if (idx == 2) led3State = !led3State; // btn3 -> LED3/bit2

        updateLedsAndBargraph();
      }
      btnPressStartMs[idx] = 0;
    }

    btnPrevPressed[idx] = pressedNow;
  }
}
