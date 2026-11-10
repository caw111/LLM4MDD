#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring per provided circuit
// Buttons (pushbuttons are between pin and GND, with internal pullups)
const uint8_t BTN1_PIN = 2;   // btn1 -> mega:2
const uint8_t BTN2_PIN = 3;   // btn2 -> mega:3
const uint8_t BTN3_PIN = 4;   // btn3 -> mega:4

// Potentiometers
const uint8_t POT1_PIN = A3;  // pot1 -> mega:A3
const uint8_t POT2_PIN = A4;  // pot2 -> mega:A4
const uint8_t POT3_PIN = A5;  // pot3 -> mega:A5

// RGB LED (common cathode)
// mega:5 -> R, mega:6 -> G, mega:7 -> B
const uint8_t RGB_R_PIN = 5;
const uint8_t RGB_G_PIN = 6;
const uint8_t RGB_B_PIN = 7;

// Debounce: 0.15 s press duration
static const uint32_t DEBOUNCE_MS = 150;

// Debounce state
struct DebouncedButton {
  uint8_t pin;
  bool stablePressed;            // stable logical pressed state (true = pressed)
  bool lastRawPressed;          // last raw reading
  uint32_t lastChangeTimeMs;    // last time raw reading changed
};

DebouncedButton b1{BTN1_PIN, false, false, 0};
DebouncedButton b2{BTN2_PIN, false, false, 0};
DebouncedButton b3{BTN3_PIN, false, false, 0};

// Color channel intensities to retain after release
uint8_t redVal = 0;
uint8_t greenVal = 0;
uint8_t blueVal = 0;

#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonPressed(uint8_t pin);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDebounced(DebouncedButton &b, uint32_t nowMs);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPot8(uint8_t analogPin);
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyRGB(uint8_t r, uint8_t g, uint8_t b);
#line 82 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_68\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonPressed(uint8_t pin) {
  // Buttons wired to GND when pressed => INPUT_PULLUP, pressed reads LOW
  return digitalRead(pin) == LOW;
}

static void updateDebounced(DebouncedButton &b, uint32_t nowMs) {
  bool rawPressed = readButtonPressed(b.pin);

  // initialize lastRawPressed on first call
  if (nowMs == 0 && b.lastChangeTimeMs == 0) {
    b.lastRawPressed = rawPressed;
    b.stablePressed = rawPressed;
    b.lastChangeTimeMs = nowMs;
    return;
  }

  if (rawPressed != b.lastRawPressed) {
    b.lastRawPressed = rawPressed;
    b.lastChangeTimeMs = nowMs;
  }

  // Accept new state only after it stays stable for debounce interval
  if ((nowMs - b.lastChangeTimeMs) >= DEBOUNCE_MS) {
    b.stablePressed = rawPressed;
  }
}

static uint8_t readPot8(uint8_t analogPin) {
  // Map 0..1023 to 0..255
  int v = analogRead(analogPin);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  return (uint8_t)((v * 255L) / 1023L);
}

static void applyRGB(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode => HIGH/analogWrite drives intensity directly
  analogWrite(RGB_R_PIN, r);
  analogWrite(RGB_G_PIN, g);
  analogWrite(RGB_B_PIN, b);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialize RGB LED to OFF (all channels 0)
  applyRGB(0, 0, 0);

  // Initialize debounce timing
  uint32_t nowMs = millis();
  b1.lastChangeTimeMs = nowMs;
  b2.lastChangeTimeMs = nowMs;
  b3.lastChangeTimeMs = nowMs;

  b1.lastRawPressed = readButtonPressed(b1.pin);
  b2.lastRawPressed = readButtonPressed(b2.pin);
  b3.lastRawPressed = readButtonPressed(b3.pin);

  b1.stablePressed = b1.lastRawPressed;
  b2.stablePressed = b2.lastRawPressed;
  b3.stablePressed = b3.lastRawPressed;

  // Ensure retained values start at 0 per requirement
  redVal = 0;
  greenVal = 0;
  blueVal = 0;
  applyRGB(redVal, greenVal, blueVal);
}

void loop() {
  uint32_t nowMs = millis();

  updateDebounced(b1, nowMs);
  updateDebounced(b2, nowMs);
  updateDebounced(b3, nowMs);

  // Real-time adjustment while button is held; retain after release
  if (b1.stablePressed) {
    redVal = readPot8(POT1_PIN);
  }
  if (b2.stablePressed) {
    greenVal = readPot8(POT2_PIN);
  }
  if (b3.stablePressed) {
    blueVal = readPot8(POT3_PIN);
  }

  // Reflect combined color immediately
  applyRGB(redVal, greenVal, blueVal);
}
