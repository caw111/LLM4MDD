#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Pins per provided wiring (Arduino Mega 2560)
static const uint8_t PIN_BTN  = 4;   // btn1:1.l -> mega:4
static const uint8_t PIN_LED1 = 2;   // led1:A  -> mega:2
static const uint8_t PIN_LED2 = 3;   // led2:A  -> mega:3

// Bargraph segments A1..A10
static const uint8_t BAR_A_PINS[10] = {
  5, 6, 7, 8, 9, 10, 11, 12, 13, 44
};

// Bargraph cathodes are all common to GND (mega:GND.1), no need to control.

// State
static uint8_t counter = 0;          // 0..10 (display 1-10 segments for 1..10, 0 means all off)
static uint8_t overflow = 0;        // 0..3 (binary shown on led1/led2, wraps)

// Debounce parameters
static const uint32_t DEBOUNCE_STABLE_MS = 150; // minimum 0.15-second stable press detection
static const uint8_t BUTTON_ACTIVE_STATE = HIGH; // assume button press drives input HIGH

static bool lastStablePressed = false;
static bool lastReadPressed = false;
static uint32_t stableSinceMs = 0;

// Helpers to set outputs
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraph(uint8_t value1to10);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setOverflowLEDs(uint8_t ov);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void onValidPress();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_123\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraph(uint8_t value1to10) {
  // Requirement: "counter value (1-10 segments, PIN A1-A10)" and initial off state.
  // When counter is 0 (after overflow reset), all segments OFF.
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t segmentIndex = i + 1; // 1..10
    digitalWrite(BAR_A_PINS[i], (segmentIndex <= value1to10) ? HIGH : LOW);
  }
}

static void setOverflowLEDs(uint8_t ov) {
  // 00: 0 overflow, 01: 1 overflow, 10: 2 overflow, 11: 3 overflow
  // led1 = bit1 (LSB), led2 = bit2? The mapping isn't specified; we choose:
  // led1 represents the least significant bit (bit0), led2 represents bit1.
  uint8_t bit0 = (ov & 0x01) ? 1 : 0;
  uint8_t bit1 = (ov & 0x02) ? 1 : 0;

  digitalWrite(PIN_LED1, bit0 ? HIGH : LOW);
  digitalWrite(PIN_LED2, bit1 ? HIGH : LOW);
}

static void onValidPress() {
  // Each valid press increments the counter by 1.
  // When counter exceeds 10: reset to 0; overflow increments by 1 (0..3 wraps).
  counter++;
  if (counter > 10) {
    counter = 0;
    overflow = (overflow + 1) & 0x03; // wrap 00->01->10->11->00
  }

  // Maintain each valid state until next valid button press.
  setBargraph(counter);      // counter is 0..10; 0 => all off
  setOverflowLEDs(overflow);
}

void setup() {
  pinMode(PIN_BTN, INPUT_PULLUP); // typical for pushbuttons wired to 5V; keeps stable idle if not pressed
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
  }

  // Initialize all components OFF
  setBargraph(0);
  overflow = 0;
  counter = 0;
  setOverflowLEDs(overflow);

  // Initialize debounce trackers
  bool pressedNow = (digitalRead(PIN_BTN) == BUTTON_ACTIVE_STATE);
  lastReadPressed = pressedNow;
  lastStablePressed = pressedNow;
  stableSinceMs = millis();
}

void loop() {
  // Debounce using stable detection: require >=150ms of consistent press state.
  bool pressedNow = (digitalRead(PIN_BTN) == BUTTON_ACTIVE_STATE);
  uint32_t now = millis();

  if (pressedNow != lastReadPressed) {
    lastReadPressed = pressedNow;
    stableSinceMs = now; // restart stability timer
  }

  // If the read state has been stable long enough and differs from last stable state,
  // update stable state.
  if ((now - stableSinceMs) >= DEBOUNCE_STABLE_MS && pressedNow != lastStablePressed) {
    lastStablePressed = pressedNow;

    // Count only on transition into "pressed" stable state.
    if (lastStablePressed) {
      onValidPress();
    }
  }
}
