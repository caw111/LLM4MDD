#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BTN1_PIN = 2;   // btn1:1.l -> mega:2
static const uint8_t BTN2_PIN = 3;   // btn2:1.l -> mega:3
static const uint8_t BTN3_PIN = 4;   // btn3:1.l -> mega:4

// RGB LED (common cathode)
static const uint8_t RGB_R_PIN = 5;  // rgb1:R -> mega:5
static const uint8_t RGB_G_PIN = 6;  // rgb1:G -> mega:6
static const uint8_t RGB_B_PIN = 7;  // rgb1:B -> mega:7

// Bar graph (A1-A8)
static const uint8_t BAR_A_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45}; // A1..A8 -> mega pins
// Common anode/cathode is wired to GND via Cx (bargraph1:C1..C8 -> mega:GND.1)

// Debounce
static const unsigned long DEBOUNCE_MS = 35;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonRaw(uint8_t pin);
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateDebounced(uint8_t pin, bool &stableState, bool &lastRawState, unsigned long &lastChangeMs);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r, uint8_t g, uint8_t b);
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearBargraph();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraphForIntensity(uint8_t intensity255);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void stepColorAndUpdate(uint8_t component);
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_87\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonRaw(uint8_t pin) {
  // Buttons are wired with one side to GND and the other to the Arduino pin:
  // mega:2/3/4 -> button; other side -> GND. With INPUT_PULLUP, pressed => LOW.
  return (digitalRead(pin) == LOW);
}

static bool updateDebounced(uint8_t pin, bool &stableState, bool &lastRawState, unsigned long &lastChangeMs) {
  bool raw = readButtonRaw(pin);

  if (raw != lastRawState) {
    lastChangeMs = millis();
    lastRawState = raw;
  }

  if ((millis() - lastChangeMs) >= DEBOUNCE_MS) {
    if (raw != stableState) {
      stableState = raw;
      // Return true when a stable state change occurs.
      return true;
    }
  }
  return false;
}

static uint8_t redVal = 0;
static uint8_t greenVal = 0;
static uint8_t blueVal = 0;

static void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: HIGH turns LED on, LOW turns it off.
  // Using PWM-capable pins would allow brightness; however requirements specify discrete levels in 0-255.
  // We'll drive intensity using analogWrite.
  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);
}

static void clearBargraph() {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_A_PINS[i], LOW);
  }
}

static void updateBargraphForIntensity(uint8_t intensity255) {
  // Map 0..255 to 1..8 LEDs in steps of 32:
  // 32 => 1 LED, 64 => 2 LEDs, ..., 255 => 8 LEDs.
  // Also supports 0 => 0 LEDs (all off).
  uint8_t lit = intensity255 / 32; // 0..7 for 0..224
  if (intensity255 >= 256) lit = 8; // not possible; kept for clarity
  if (intensity255 == 255) lit = 8; // ensure 255 lights all 8
  if (intensity255 > 0 && lit == 0) lit = 1; // in case of values 1..31

  // If intensity is 0, lit will be 0.
  clearBargraph();
  for (uint8_t i = 0; i < 8; i++) {
    if (i < lit) {
      digitalWrite(BAR_A_PINS[i], HIGH);
    }
  }
}

static void stepColorAndUpdate(uint8_t component) {
  // component: 0=red, 1=green, 2=blue
  uint8_t *target = nullptr;
  if (component == 0) target = &redVal;
  else if (component == 1) target = &greenVal;
  else target = &blueVal;

  uint16_t next = static_cast<uint16_t>(*target) + 32;
  if (next > 255) next = 0; // after reaching 255, next press resets to 0
  *target = static_cast<uint8_t>(next);

  // Update RGB combined color based on current intensities
  setRGB(redVal, greenVal, blueVal);

  // Update bargraph to display intensity of the component just adjusted
  updateBargraphForIntensity(*target);
}

void setup() {
  // Buttons with pullups
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // RGB pins
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Bargraph pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Initialization: RGB LED off, bar graph off
  redVal = 0;
  greenVal = 0;
  blueVal = 0;
  setRGB(0, 0, 0);
  clearBargraph();
}

void loop() {
  // Debounce state per button
  static bool stable1 = false, stable2 = false, stable3 = false; // pressed state (LOW)
  static bool lastRaw1 = false, lastRaw2 = false, lastRaw3 = false;
  static unsigned long lastChg1 = 0, lastChg2 = 0, lastChg3 = 0;

  // Ensure lastRaw states are initialized
  static bool init = false;
  if (!init) {
    lastRaw1 = readButtonRaw(BTN1_PIN);
    lastRaw2 = readButtonRaw(BTN2_PIN);
    lastRaw3 = readButtonRaw(BTN3_PIN);
    stable1 = lastRaw1;
    stable2 = lastRaw2;
    stable3 = lastRaw3;
    init = true;
  }

  // We react only on press *events* (transition from not-pressed to pressed),
  // after debounce stabilizes.
  bool changed1 = updateDebounced(BTN1_PIN, stable1, lastRaw1, lastChg1);
  if (changed1 && stable1) { // button pressed
    stepColorAndUpdate(0); // btn1 => red
  }

  bool changed2 = updateDebounced(BTN2_PIN, stable2, lastRaw2, lastChg2);
  if (changed2 && stable2) { // button pressed
    stepColorAndUpdate(1); // btn2 => green
  }

  bool changed3 = updateDebounced(BTN3_PIN, stable3, lastRaw3, lastChg3);
  if (changed3 && stable3) { // button pressed
    stepColorAndUpdate(2); // btn3 => blue
  }
}
