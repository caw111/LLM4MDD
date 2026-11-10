#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring (from circuit)
// LEDs
// led1:A -> Mega pin 2,  led1:C -> GND
// led2:A -> Mega pin 3,  led2:C -> GND
// led3:A -> Mega pin 4,  led3:C -> GND

// Buttons (using internal pullups; circuit shows pull to GND)
// btn1:1.l -> Mega pin 5, btn1:2.r -> GND
// btn2:1.l -> Mega pin 6, btn2:2.r -> GND
// btn3:1.l -> Mega pin 7, btn3:2.r -> GND
const uint8_t BTN1_PIN = 5;
const uint8_t BTN2_PIN = 6;
const uint8_t BTN3_PIN = 7;

// Bar graph segments
// Segment mapping:
// A1->pin 8,  A2->pin 9,  A3->pin 10, A4->pin 11,
// A5->pin 12, A6->pin 13, A7->pin 44, A8->pin 45
// Common cathode C1..C8 -> GND (all GND.1)
const uint8_t BAR_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45}; // indices 0..7 => values 1..8 (A1..A8)

// Bit LEDs
const uint8_t LED_PINS[3] = {2, 3, 4}; // bit0->LED1, bit1->LED2, bit2->LED3

// Debounce / ignore-short-press requirement
// Ignore presses shorter than 0.15 seconds.
static const uint32_t MIN_PRESS_MS = 150;

// Track button press state
struct ButtonState {
  bool lastStableLevel;        // last sampled stable level
  bool pressInProgress;       // currently waiting for release to validate duration
  uint32_t pressStartMs;      // millis() when press began
  uint32_t lastEventMs;       // for extra safety (optional)
};

ButtonState b1{true, false, 0, 0};
ButtonState b2{true, false, 0, 0};
ButtonState b3{true, false, 0, 0};

// Read button as active-high press (pressed => HIGH because of pullup inverted)
// With INPUT_PULLUP: not pressed = HIGH, pressed = LOW.
// We'll convert to pressedActive = (digitalRead==LOW)
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool pressedActive(uint8_t pin);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateButton(ButtonState &bs, uint8_t pin, bool &toggleRequest);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBit(uint8_t bitIndex, bool value);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraph(uint8_t value0to7);
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_122\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline bool pressedActive(uint8_t pin) {
  return (digitalRead(pin) == LOW);
}

static void updateButton(ButtonState &bs, uint8_t pin, bool &toggleRequest) {
  toggleRequest = false;

  bool pressedNow = pressedActive(pin);

  // Simple stability: we treat level changes as state changes when observed,
  // since the requirement is to ignore presses shorter than 0.15 s.
  // However, we must record press start on transition into pressed.
  if (!bs.pressInProgress) {
    // Not currently timing a press
    if (pressedNow) {
      bs.pressInProgress = true;
      bs.pressStartMs = millis();
    }
  } else {
    // Press in progress; look for release
    if (!pressedNow) {
      uint32_t heldMs = millis() - bs.pressStartMs;
      bs.pressInProgress = false;

      // Ignore short presses
      if (heldMs >= MIN_PRESS_MS) {
        toggleRequest = true;
      }
    }
  }

  bs.lastStableLevel = pressedNow;
}

static void setBit(uint8_t bitIndex, bool value) {
  // bitIndex: 0..2
  digitalWrite(LED_PINS[bitIndex], value ? HIGH : LOW);
}

static void updateBargraph(uint8_t value0to7) {
  // Requirement: bar graph lights segment corresponding to decimal equivalent (0-7)
  // Given wiring uses A1..A8. We map:
  //  value 0 -> A1, value 1 -> A2, ..., value 7 -> A8
  for (uint8_t i = 0; i < 8; i++) {
    bool on = (i == value0to7); // i==value (0..7) => A(i+1)
    digitalWrite(BAR_PINS[i], on ? HIGH : LOW);
  }
}

void setup() {
  // LEDs off initially
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Bar graph all segments off initially
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  // Buttons with internal pullups
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Initialize internal states
  b1.lastStableLevel = pressedActive(BTN1_PIN);
  b2.lastStableLevel = pressedActive(BTN2_PIN);
  b3.lastStableLevel = pressedActive(BTN3_PIN);

  // Binary starts at 0 (all bits off)
  uint8_t binaryValue = 0;
  for (uint8_t bit = 0; bit < 3; bit++) setBit(bit, (binaryValue >> bit) & 1U);
  updateBargraph(binaryValue);
}

void loop() {
  static uint8_t binaryValue = 0; // 0..7

  bool req1 = false, req2 = false, req3 = false;

  updateButton(b1, BTN1_PIN, req1);
  updateButton(b2, BTN2_PIN, req2);
  updateButton(b3, BTN3_PIN, req3);

  if (req1) {
    // Requirement: Pressing btn1 toggles LED1/lowest bit (bit0)
    binaryValue ^= 0b001;
    setBit(0, (binaryValue & 0b001) != 0);
    updateBargraph(binaryValue);
  }

  if (req2) {
    // Requirement: Pressing btn2 toggles LED2/middle bit (bit1)
    binaryValue ^= 0b010;
    setBit(1, (binaryValue & 0b010) != 0);
    updateBargraph(binaryValue);
  }

  if (req3) {
    // Requirement: Pressing btn3 toggles LED3/highest bit (bit2)
    binaryValue ^= 0b100;
    setBit(2, (binaryValue & 0b100) != 0);
    updateBargraph(binaryValue);
  }

  // Small non-blocking delay
  delay(5);
}
