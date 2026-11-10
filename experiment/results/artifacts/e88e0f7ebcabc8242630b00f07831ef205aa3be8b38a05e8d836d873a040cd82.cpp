#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---- Pin assignments (from provided authoritative configuration) ----
static const uint8_t PIN_POT   = A0;  // analog_input
static const uint8_t PIN_BTN   = 2;   // digital_input (D2)
static const uint8_t PIN_TM_CLK = 4;  // TM1637 CLK (D4)
static const uint8_t PIN_TM_DIO = 5;  // TM1637 DIO (D5)

// ---- TM1637 ----
TM1637Display display(PIN_TM_CLK, PIN_TM_DIO);

// ---- Button / mode switching ----
enum DisplayMode : uint8_t {
  MODE_COUNT = 0,
  MODE_POT   = 1,
  MODE_1234  = 2
};

static volatile bool buttonPressedFlag = false;

static DisplayMode mode = MODE_COUNT;
static unsigned long lastButtonEdgeMs = 0;

// Basic debouncing for button edges
static const unsigned long DEBOUNCE_MS = 30;

// ---- Counting behavior (per requirement: show count when in count mode) ----
static long counterValue = 0;

// Button press ISR: just set a flag, real work in loop()
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void IRAM_ATTR onButtonISR();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleButtonPress();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPotValue();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay();
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_020\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void IRAM_ATTR onButtonISR() {
  buttonPressedFlag = true;
}

static void handleButtonPress() {
  unsigned long now = millis();
  if (now - lastButtonEdgeMs < DEBOUNCE_MS) return;
  lastButtonEdgeMs = now;

  // Cycle modes on each button press: count -> potentiometer value -> 1234 -> ...
  switch (mode) {
    case MODE_COUNT: mode = MODE_POT;   break;
    case MODE_POT:   mode = MODE_1234; break;
    case MODE_1234:  mode = MODE_COUNT; break;
    default:         mode = MODE_COUNT; break;
  }
}

// Convert potentiometer analog reading to a 4-digit display value.
// Requirement says: display "电位器值" (potentiometer value).
// We display the raw 0..1023 mapped into 0..1023 (up to 4 digits).
static uint16_t readPotValue() {
  return (uint16_t)analogRead(PIN_POT); // 0..1023
}

// Update display based on current mode
static void updateDisplay() {
  if (mode == MODE_COUNT) {
    // Display count; constrain to 0..9999 for 4-digit TM1637.
    long v = counterValue;
    if (v < 0) v = 0;
    if (v > 9999) v = 9999;
    display.showNumberDec((int)v, false, 4, 0); // right-aligned, no leading zeros
  } else if (mode == MODE_POT) {
    uint16_t pot = readPotValue(); // 0..1023
    display.showNumberDec((int)pot, false, 4, 0); // right-aligned, no leading zeros
  } else { // MODE_1234
    display.showNumberDec(1234, false, 4, 0);
  }
}

// Optional: increment count at a reasonable interval.
// The requirement does not specify the rate; to preserve "计数" behavior,
// we increment periodically.
static unsigned long lastCountMs = 0;
static const unsigned long COUNT_PERIOD_MS = 250;

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP);

  display.setBrightness(0x0f, true); // max brightness (typical safe default)

  // Edge-triggered interrupt for D2 (INT0 on UNO).
  attachInterrupt(digitalPinToInterrupt(PIN_BTN), onButtonISR, FALLING);
}

void loop() {
  // Handle button (mode switch)
  if (buttonPressedFlag) {
    noInterrupts();
    buttonPressedFlag = false;
    interrupts();
    handleButtonPress();
  }

  // Update count periodically (even if not displayed, keeps behavior consistent)
  unsigned long now = millis();
  if (now - lastCountMs >= COUNT_PERIOD_MS) {
    lastCountMs = now;
    counterValue++;
  }

  // Refresh display
  updateDisplay();
}
