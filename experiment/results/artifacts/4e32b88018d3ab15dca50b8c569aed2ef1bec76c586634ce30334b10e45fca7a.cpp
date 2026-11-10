#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const int BTN1_PIN = 2;   // btn1:1.l -> mega:2, btn1:2.r -> GND
const int BTN2_PIN = 3;   // btn2:1.l -> mega:3, btn2:2.r -> GND

const int POT_PIN  = A3;  // pot1:SIG -> mega:A3
// pot1:GND -> GND, pot1:VCC -> 5V (already wired)

// RGB LED (common cathode)
const int RGB_R_PIN = 4;  // rgb1:R -> mega:4
const int RGB_G_PIN = 5;  // rgb1:G -> mega:5
const int RGB_B_PIN = 6;  // rgb1:B -> mega:6

// Modes as per requirements
// Mode 0: red, green, blue
// Mode 1: yellow, cyan, magenta
enum Mode { MODE_FIRST = 0, MODE_SECOND = 1 };

static const uint8_t firstColors[3][3] = {
  {255, 0, 0},   // red
  {0, 255, 0},   // green
  {0, 0, 255}    // blue
};

static const uint8_t secondColors[3][3] = {
  {255, 255, 0}, // yellow
  {0, 255, 255}, // cyan
  {255, 0, 255}  // magenta
};

Mode mode = MODE_FIRST;
uint8_t colorIndex = 0;  // red at boot
bool ledEnabled = true;   // default ON (red)

uint8_t brightness = 0;

// Simple button debouncing
static bool lastBtn1Stable = HIGH;
static bool lastBtn2Stable = HIGH;
static bool lastBtn1Reading = HIGH;
static bool lastBtn2Reading = HIGH;
static unsigned long btn1LastChangeMs = 0;
static unsigned long btn2LastChangeMs = 0;

const unsigned long DEBOUNCE_MS = 30;

#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonEvent(int pin, bool &lastReading, bool &lastStable, unsigned long &lastChangeMs);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyColor(uint8_t r, uint8_t g, uint8_t b);
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFromPot();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButtonEvent(int pin, bool &lastReading, bool &lastStable, unsigned long &lastChangeMs) {
  bool reading = digitalRead(pin);

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeMs = millis();
  }

  if ((millis() - lastChangeMs) >= DEBOUNCE_MS) {
    if (reading != lastStable) {
      lastStable = reading;
      // Buttons are active LOW (wired to GND when pressed)
      if (lastStable == LOW) return true; // pressed event
    }
  }
  return false;
}

void applyColor(uint8_t r, uint8_t g, uint8_t b) {
  if (!ledEnabled || brightness == 0) {
    // common cathode: HIGH turns LED off
    analogWrite(RGB_R_PIN, 255);
    analogWrite(RGB_G_PIN, 255);
    analogWrite(RGB_B_PIN, 255);
    return;
  }

  // Scale by brightness: map potentiometer 0..1023 to 0..255
  // Resulting PWM value for common cathode: OFF=255, ON intensity reduces from 255.
  // intensityScaled is 0..255 representing desired ON intensity
  uint16_t intensityR = (uint16_t)r * brightness / 255;
  uint16_t intensityG = (uint16_t)g * brightness / 255;
  uint16_t intensityB = (uint16_t)b * brightness / 255;

  analogWrite(RGB_R_PIN, (int)(255 - intensityR));
  analogWrite(RGB_G_PIN, (int)(255 - intensityG));
  analogWrite(RGB_B_PIN, (int)(255 - intensityB));
}

void updateFromPot() {
  int potVal = analogRead(POT_PIN);         // 0..1023
  brightness = (uint8_t)map(potVal, 0, 1023, 0, 255);

  uint8_t r, g, b;
  if (mode == MODE_FIRST) {
    r = firstColors[colorIndex][0];
    g = firstColors[colorIndex][1];
    b = firstColors[colorIndex][2];
  } else {
    r = secondColors[colorIndex][0];
    g = secondColors[colorIndex][1];
    b = secondColors[colorIndex][2];
  }

  applyColor(r, g, b);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initial state: red and enabled
  mode = MODE_FIRST;
  colorIndex = 0;
  ledEnabled = true;

  // Initialize stable states for debounce
  lastBtn1Reading = digitalRead(BTN1_PIN);
  lastBtn2Reading = digitalRead(BTN2_PIN);
  lastBtn1Stable = lastBtn1Reading;
  lastBtn2Stable = lastBtn2Reading;
  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();

  // Initialize brightness from pot and apply red
  brightness = 0;
  // Apply red immediately (brightness will update in loop)
  analogWrite(RGB_R_PIN, 255); // off initially; will be set after brightness read
  analogWrite(RGB_G_PIN, 255);
  analogWrite(RGB_B_PIN, 255);
}

void loop() {
  // Read button presses with debouncing
  if (readButtonEvent(BTN1_PIN, lastBtn1Reading, lastBtn1Stable, btn1LastChangeMs)) {
    if (mode == MODE_FIRST) {
      // Cycle red -> green -> blue -> red
      colorIndex = (colorIndex + 1) % 3;
      ledEnabled = true;
    } else {
      // In second mode, btn1 turns LED off
      ledEnabled = false;
    }

    // Apply using current brightness
    uint8_t r, g, b;
    if (mode == MODE_FIRST) {
      r = firstColors[colorIndex][0];
      g = firstColors[colorIndex][1];
      b = firstColors[colorIndex][2];
    } else {
      r = secondColors[colorIndex][0];
      g = secondColors[colorIndex][1];
      b = secondColors[colorIndex][2];
    }
    applyColor(r, g, b);
  }

  if (readButtonEvent(BTN2_PIN, lastBtn2Reading, lastBtn2Stable, btn2LastChangeMs)) {
    if (mode == MODE_SECOND) {
      // Cycle yellow -> cyan -> magenta -> yellow
      colorIndex = (colorIndex + 1) % 3;
      ledEnabled = true;
    } else {
      // In first mode, btn2 turns LED off
      ledEnabled = false;
    }

    // If currently in second mode, cycling occurred; otherwise just off.
    // Mode change is not specified to switch modes; only cycling within mode.
    // Requirement only mentions off behavior when pressing the other button.
    uint8_t r, g, b;
    if (mode == MODE_FIRST) {
      r = firstColors[colorIndex][0];
      g = firstColors[colorIndex][1];
      b = firstColors[colorIndex][2];
    } else {
      r = secondColors[colorIndex][0];
      g = secondColors[colorIndex][1];
      b = secondColors[colorIndex][2];
    }
    applyColor(r, g, b);
  }

  // Real-time brightness update from potentiometer
  updateFromPot();
}
