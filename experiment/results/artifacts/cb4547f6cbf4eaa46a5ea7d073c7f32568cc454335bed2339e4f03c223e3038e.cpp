#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// RGB LED (common cathode) pins
static const uint8_t LED_R = 2;
static const uint8_t LED_G = 3;
static const uint8_t LED_B = 4;

// Push buttons (k1, k2, k3)
static const uint8_t BTN1 = 5;
static const uint8_t BTN2 = 6;
static const uint8_t BTN3 = 7;

enum Mode : uint8_t {
  CYCLING = 0,
  HOLD    = 1,
  OFF     = 2
};

static Mode mode = CYCLING;

// 0=Red, 1=Green, 2=Blue
static uint8_t currentColorIndex = 0;

// Timing for cycling
static const uint32_t COLOR_DURATION_MS = 2000;
static uint32_t lastColorChangeMs = 0;

// Simple debounce
static const uint32_t DEBOUNCE_MS = 30;

struct ButtonState {
  uint8_t pin;
  bool stableLevel;       // stable sampled level
  bool lastSampledLevel;  // last raw sample level
  uint32_t lastChangeMs;  // last time raw level changed
};

static ButtonState b1{BTN1, false, false, 0};
static ButtonState b2{BTN2, false, false, 0};
static ButtonState b3{BTN3, false, false, 0};

#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r, uint8_t g, uint8_t b);
#line 50 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyColorIndex(uint8_t idx);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initButton(ButtonState &bs);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateButtonPressed(ButtonState &bs);
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 110 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_3\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // common cathode:
  // HIGH -> off, LOW -> on
  digitalWrite(LED_R, r ? LOW  : HIGH);
  digitalWrite(LED_G, g ? LOW  : HIGH);
  digitalWrite(LED_B, b ? LOW  : HIGH);
}

static inline void allOff() {
  // Completely dark
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
}

static inline void applyColorIndex(uint8_t idx) {
  switch (idx % 3) {
    case 0: setRGB(1, 0, 0); break; // Red
    case 1: setRGB(0, 1, 0); break; // Green
    default: setRGB(0, 0, 1); break; // Blue
  }
}

static inline void initButton(ButtonState &bs) {
  pinMode(bs.pin, INPUT_PULLUP);
  bool level = digitalRead(bs.pin);
  bs.stableLevel = level;
  bs.lastSampledLevel = level;
  bs.lastChangeMs = millis();
}

// Returns true exactly once per button press event (press = goes LOW)
static bool updateButtonPressed(ButtonState &bs) {
  bool raw = digitalRead(bs.pin);
  uint32_t now = millis();

  if (raw != bs.lastSampledLevel) {
    bs.lastSampledLevel = raw;
    bs.lastChangeMs = now;
  }

  if ((now - bs.lastChangeMs) >= DEBOUNCE_MS) {
    if (raw != bs.stableLevel) {
      bs.stableLevel = raw;
      // Falling edge (HIGH->LOW) indicates press (with INPUT_PULLUP)
      if (bs.stableLevel == LOW) return true;
    }
  }
  return false;
}

void setup() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  initButton(b1);
  initButton(b2);
  initButton(b3);

  // Upon powering on/reset: start cycling red->green->blue each for 2 seconds
  mode = CYCLING;
  currentColorIndex = 0;

  lastColorChangeMs = millis();
  applyColorIndex(currentColorIndex);
}

void loop() {
  // Handle immediate button presses
  bool p1 = updateButtonPressed(b1);
  bool p2 = updateButtonPressed(b2);
  bool p3 = updateButtonPressed(b3);

  if (p3) {
    mode = OFF;
    allOff();
  } else if (p1) {
    // Stop cycling and keep LED displaying the current color
    if (mode != OFF) {
      mode = HOLD;
      applyColorIndex(currentColorIndex);
    }
  } else if (p2) {
    // Resume cycling from the point it was stopped
    if (mode != OFF) {
      mode = CYCLING;
      lastColorChangeMs = millis(); // restart timing from now
      applyColorIndex(currentColorIndex);
    }
  }

  // Mode behavior
  if (mode == CYCLING) {
    uint32_t now = millis();
    if ((now - lastColorChangeMs) >= COLOR_DURATION_MS) {
      lastColorChangeMs = now;
      currentColorIndex = (currentColorIndex + 1) % 3;
      applyColorIndex(currentColorIndex);
    }
  } else if (mode == HOLD) {
    // Keep the LED on the last color (no changes).
    // (No action needed; set once on transition to HOLD.)
  } else { // OFF
    // Keep it off (no action needed; set once on transition to OFF).
  }
}
