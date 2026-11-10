#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

static const uint8_t BTN_PIN  = 5;   // btn1: left to mega:5, right to GND

// RGB1
static const uint8_t RGB1_R = 6;
static const uint8_t RGB1_G = 7;
static const uint8_t RGB1_B = 8;

// RGB2
static const uint8_t RGB2_R = 9;
static const uint8_t RGB2_G = 10;
static const uint8_t RGB2_B = 11;

// RGB3
static const uint8_t RGB3_R = 12;
static const uint8_t RGB3_G = 13;
static const uint8_t RGB3_B = 44;

enum Mode : uint8_t { MODE1 = 1, MODE2 = 2, MODE3 = 3, MODE4 = 4 };

static const unsigned long MIN_MODE_HOLD_MS = 2000;
static const unsigned long DEBOUNCE_MS = 25;

Mode currentMode = MODE4;

unsigned long lastModeChangeMs = 0;

// Button debounce/state
int lastBtnStable = HIGH;
int lastBtnRaw = HIGH;
unsigned long lastDebounceMs = 0;

// RGB transition timing for Mode 2
unsigned long lastRainbowStepMs = 0;
const unsigned long RAINBOW_STEP_MS = 15; // smooth transitions
// We'll cycle through three colors: Yellow -> Cyan -> Purple -> Yellow (smooth via interpolation)
const uint8_t N_PHASES = 3; // number of main colors
struct RGB { uint8_t r, g, b; };

static const RGB rainbowColors[N_PHASES] = {
  {255, 255, 0},   // Yellow
  {0, 255, 255},   // Cyan
  {128, 0, 128}    // Purple (warm-ish magenta)
};

static uint8_t phaseIndex = 0; // between colors [phaseIndex] and [next]
static int phaseProgress = 0;  // 0..255 interpolation factor

// Helpers
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSingleLed(uint8_t ledPin, bool on);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBChannel(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void modeIndicatorOn(Mode m);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyModeStatic(Mode m);
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateRainbowMode();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void enterMode(Mode m);
#line 205 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static Mode nextMode(Mode m);
#line 215 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_125\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setSingleLed(uint8_t ledPin, bool on) {
  digitalWrite(ledPin, on ? HIGH : LOW);
}

static void setRGBChannel(uint8_t rPin, uint8_t gPin, uint8_t bPin, uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(rPin, r);
  analogWrite(gPin, g);
  analogWrite(bPin, b);
}

static void allOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  // Use PWM channels; ensure off = 0
  analogWrite(RGB1_R, 0); analogWrite(RGB1_G, 0); analogWrite(RGB1_B, 0);
  analogWrite(RGB2_R, 0); analogWrite(RGB2_G, 0); analogWrite(RGB2_B, 0);
  analogWrite(RGB3_R, 0); analogWrite(RGB3_G, 0); analogWrite(RGB3_B, 0);
}

static void modeIndicatorOn(Mode m) {
  // Mode 1: led1 turns on in sequence to indicate mode
  // Mode 2: no single-color LED indicated in description (keep off unless Mode 1/3 specifies)
  // Mode 3: led3 turns on in sequence to indicate mode
  // Mode 4: all single-color LEDs remain off
  // Since only "in sequence" is specified for led1 and led3, we implement a clear sequential blink:
  // - Mode1: led1 blinks 3 short steps
  // - Mode3: led3 blinks 3 short steps
  // - Otherwise: all off

  // Start from all off to avoid ambiguity
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  if (m == MODE1) {
    for (int i = 0; i < 3; i++) {
      setSingleLed(LED1_PIN, true);
      delay(120);
      setSingleLed(LED1_PIN, false);
      delay(80);
    }
  } else if (m == MODE3) {
    for (int i = 0; i < 3; i++) {
      setSingleLed(LED3_PIN, true);
      delay(120);
      setSingleLed(LED3_PIN, false);
      delay(80);
    }
  }
}

static void applyModeStatic(Mode m) {
  // Mode 4: everything off (already handled by allOff, but also ensure indicator LEDs)
  if (m == MODE4) {
    allOff();
    return;
  }

  // Indicators
  if (m == MODE1) {
    // turn on RGB: red, green, blue respectively
    setRGBChannel(RGB1_R, RGB1_G, RGB1_B, 255, 0, 0); // rgb1 = red
    setRGBChannel(RGB2_R, RGB2_G, RGB2_B, 0, 255, 0); // rgb2 = green
    setRGBChannel(RGB3_R, RGB3_G, RGB3_B, 0, 0, 255); // rgb3 = blue
    // single-color LED sequence is handled separately
  } else if (m == MODE3) {
    // warm white: equal red, green, blue values
    // Choose warm-white-ish value: not max to reduce intensity; but still equal.
    uint8_t w = 200;
    setRGBChannel(RGB1_R, RGB1_G, RGB1_B, w, w, w);
    setRGBChannel(RGB2_R, RGB2_G, RGB2_B, w, w, w);
    setRGBChannel(RGB3_R, RGB3_G, RGB3_B, w, w, w);
  } else if (m == MODE2) {
    // rainbow effect will be applied in loop continuously
    // Ensure indicator LEDs are off for mode2
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  }
}

static void updateRainbowMode() {
  // Smoothly transition between three colors across rgb1..rgb3
  // "RGB LEDs should smoothly transition between colors in Mode 2"
  // We'll apply same instantaneous rainbow color to all RGB LEDs, but also stagger slightly for visual richness.
  // However the requirement only specifies effect; keeping all synced maintains clear behavior.
  // We'll sync all three to the same interpolated color.

  unsigned long now = millis();
  if (now - lastRainbowStepMs >= RAINBOW_STEP_MS) {
    lastRainbowStepMs = now;

    // Move interpolation progress
    phaseProgress += 2; // speed of transition; 2/255 per step
    if (phaseProgress >= 255) {
      phaseProgress = 0;
      phaseIndex = (phaseIndex + 1) % N_PHASES;
    }
  }

  uint8_t nextIndex = (phaseIndex + 1) % N_PHASES;
  uint8_t t = (uint8_t)phaseProgress; // 0..255

  auto lerp8 = [](uint8_t a, uint8_t b, uint8_t tt) -> uint8_t {
    // linear interpolation: a*(255-tt)+b*tt over 255
    uint16_t x = (uint16_t)a * (255 - tt) + (uint16_t)b * tt;
    return (uint8_t)(x / 255);
  };

  RGB c0 = rainbowColors[phaseIndex];
  RGB c1 = rainbowColors[nextIndex];

  uint8_t r = lerp8(c0.r, c1.r, t);
  uint8_t g = lerp8(c0.g, c1.g, t);
  uint8_t b = lerp8(c0.b, c1.b, t);

  setRGBChannel(RGB1_R, RGB1_G, RGB1_B, r, g, b);
  setRGBChannel(RGB2_R, RGB2_G, RGB2_B, r, g, b);
  setRGBChannel(RGB3_R, RGB3_G, RGB3_B, r, g, b);
}

static void enterMode(Mode m) {
  currentMode = m;

  // Turn everything off first on mode change to ensure deterministic behavior
  allOff();

  // Reset rainbow state when entering mode2
  if (currentMode == MODE2) {
    phaseIndex = 0;
    phaseProgress = 0;
    lastRainbowStepMs = millis();
    // Keep indicators off for mode2
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  }

  // Apply static colors
  applyModeStatic(currentMode);

  // Indicators sequence for mode1 and mode3
  if (currentMode == MODE1 || currentMode == MODE3) {
    // Sequence uses delays; it occurs immediately upon mode entry
    modeIndicatorOn(currentMode);
  }
}

static Mode nextMode(Mode m) {
  switch (m) {
    case MODE1: return MODE2;
    case MODE2: return MODE3;
    case MODE3: return MODE4;
    case MODE4: return MODE1;
    default:    return MODE1;
  }
}

void setup() {
  // Single-color LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Button input with pull-up (btn1: mega:5 is left, mega:GND via btn1:2.r)
  pinMode(BTN_PIN, INPUT_PULLUP);

  // RGB pins
  // Use PWM-capable pins where applicable on Mega2560; analogWrite works on them.
  pinMode(RGB1_R, OUTPUT); pinMode(RGB1_G, OUTPUT); pinMode(RGB1_B, OUTPUT);
  pinMode(RGB2_R, OUTPUT); pinMode(RGB2_G, OUTPUT); pinMode(RGB2_B, OUTPUT);
  pinMode(RGB3_R, OUTPUT); pinMode(RGB3_G, OUTPUT); pinMode(RGB3_B, OUTPUT);

  // Initialize everything off per requirements
  allOff();
  // Start in Mode 4 (all off)
  currentMode = MODE4;
  lastModeChangeMs = millis();
  lastBtnStable = digitalRead(BTN_PIN);
  lastBtnRaw = lastBtnStable;
}

void loop() {
  // Read raw button state
  int btnRaw = digitalRead(BTN_PIN);
  unsigned long now = millis();

  // Debounce
  if (btnRaw != lastBtnRaw) {
    lastDebounceMs = now;
    lastBtnRaw = btnRaw;
  }
  if ((now - lastDebounceMs) >= DEBOUNCE_MS) {
    if (btnRaw != lastBtnStable) {
      lastBtnStable = btnRaw;

      // Detect press (assuming active LOW because pull-up and button to GND)
      if (lastBtnStable == LOW) {
        // Maintain at least 2 seconds before cycling to next mode
        if (now - lastModeChangeMs >= MIN_MODE_HOLD_MS) {
          Mode nm = nextMode(currentMode);
          lastModeChangeMs = now;
          enterMode(nm);
        }
      }
    }
  }

  // Mode behavior
  if (currentMode == MODE2) {
    updateRainbowMode();
    // Indicators remain off in Mode 2
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  } else if (currentMode == MODE1) {
    // Ensure indicators are correct (static mode colors, but indicators already sequenced on entry)
    // Keep colors steady
    applyModeStatic(MODE1);
    // Leave single LEDs as last sequenced state; to keep "clearly indicate current mode"
    // we will also ensure LED1 is on after the entry sequence ends.
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
  } else if (currentMode == MODE3) {
    applyModeStatic(MODE3);
    // Ensure LED3 stays on to indicate mode
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, HIGH);
  } else { // MODE4
    allOff();
  }
}
