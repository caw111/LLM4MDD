#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Pin mapping per provided circuit (Arduino Mega 2560)
const uint8_t BTN1_PIN = 2;
const uint8_t BTN2_PIN = 3;

const uint8_t RGB_R_PIN = 4;
const uint8_t RGB_G_PIN = 5;
const uint8_t RGB_B_PIN = 6;

const uint8_t POT_PIN  = A3;

// Common cathode RGB LED => HIGH means OFF, LOW means ON (active-low)
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t clampU8(int v);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t buttonRead(uint8_t pin);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void applyRGB(uint8_t r, uint8_t g, uint8_t b);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateColorFromState();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleBtn1Press();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleBtn2Press();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButtons();
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 184 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_85\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline uint8_t clampU8(int v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return (uint8_t)v;
}

enum ColorMode : uint8_t {
  MODE_NONE = 0,
  MODE_FIRST = 1,   // R, G, B
  MODE_SECOND = 2   // Y, C, M
};

ColorMode mode = MODE_FIRST;
uint8_t stepIndex = 0; // 0..2 within the current mode
bool ledOff = false;

int potRaw = 0;
uint8_t brightness = 0; // 0..255

// Button debouncing
const unsigned long DEBOUNCE_MS = 25;

uint8_t lastStableBtn1 = HIGH;
uint8_t lastStableBtn2 = HIGH;

uint8_t lastReadingBtn1 = HIGH;
uint8_t lastReadingBtn2 = HIGH;

unsigned long lastDebounceTimeBtn1 = 0;
unsigned long lastDebounceTimeBtn2 = 0;

bool pendingPress1 = false;
bool pendingPress2 = false;

uint8_t buttonRead(uint8_t pin) {
  return digitalRead(pin);
}

void applyRGB(uint8_t r, uint8_t g, uint8_t b) {
  // active-low, common cathode
  analogWrite(RGB_R_PIN, 255 - r);
  analogWrite(RGB_G_PIN, 255 - g);
  analogWrite(RGB_B_PIN, 255 - b);
}

void updateColorFromState() {
  if (ledOff || brightness == 0) {
    // All off => set outputs to HIGH via active-low inversion (analogWrite expects 0..255)
    applyRGB(0, 0, 0); // brightness 0 will already turn off; keep consistent
    // Note: applyRGB(0..) results analogWrite(255) => output HIGH => off.
    return;
  }

  switch (mode) {
    case MODE_FIRST: // R, G, B
      if (stepIndex == 0)       applyRGB(brightness, 0, 0);       // Red
      else if (stepIndex == 1)  applyRGB(0, brightness, 0);       // Green
      else                      applyRGB(0, 0, brightness);       // Blue
      break;

    case MODE_SECOND: // Yellow, Cyan, Magenta
      if (stepIndex == 0)       applyRGB(brightness, brightness, 0);     // Yellow
      else if (stepIndex == 1)  applyRGB(0, brightness, brightness);     // Cyan
      else                      applyRGB(brightness, 0, brightness);     // Magenta
      break;

    default:
      applyRGB(0, 0, 0);
      break;
  }
}

void handleBtn1Press() {
  // If in second mode and btn1 pressed => turn off
  if (mode == MODE_SECOND && !ledOff) {
    ledOff = true;
    updateColorFromState();
    return;
  }

  // Otherwise cycle through first mode (R->G->B->R)
  if (ledOff) {
    // If currently off, resume first mode cycle at current stepIndex
    // (keeps "state maintenance" deterministic; brightness still from pot).
    ledOff = false;
  }

  mode = MODE_FIRST;
  stepIndex = (stepIndex + 1) % 3; // cycle on each press: R->G->B->R (initial is red)
  updateColorFromState();
}

void handleBtn2Press() {
  // If in first mode and btn2 pressed => turn off
  if (mode == MODE_FIRST && !ledOff) {
    ledOff = true;
    updateColorFromState();
    return;
  }

  // Otherwise cycle through second mode (Y->C->M->Y)
  if (ledOff) {
    ledOff = false;
  }

  mode = MODE_SECOND;
  stepIndex = (stepIndex + 1) % 3; // cycle on each press: Yellow->Cyan->Magenta->Yellow
  updateColorFromState();
}

void updateButtons() {
  // BTN1
  uint8_t reading1 = buttonRead(BTN1_PIN);
  if (reading1 != lastReadingBtn1) {
    lastDebounceTimeBtn1 = millis();
    lastReadingBtn1 = reading1;
  }
  if ((millis() - lastDebounceTimeBtn1) > DEBOUNCE_MS) {
    if (reading1 != lastStableBtn1) {
      lastStableBtn1 = reading1;
      // Button is wired to Mega pin: likely pull-up/down not specified in circuit,
      // but typical Wokwi pushbuttons connect to GND and pin with pullup disabled.
      // Here we enable INPUT_PULLUP, so pressed => LOW.
      if (lastStableBtn1 == LOW) pendingPress1 = true;
    }
  }

  // BTN2
  uint8_t reading2 = buttonRead(BTN2_PIN);
  if (reading2 != lastReadingBtn2) {
    lastDebounceTimeBtn2 = millis();
    lastReadingBtn2 = reading2;
  }
  if ((millis() - lastDebounceTimeBtn2) > DEBOUNCE_MS) {
    if (reading2 != lastStableBtn2) {
      lastStableBtn2 = reading2;
      if (lastStableBtn2 == LOW) pendingPress2 = true;
    }
  }
}

void setup() {
  // Buttons
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  // RGB pins (PWM capable on Mega)
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initial state: RGB LED initialized to red
  mode = MODE_FIRST;
  stepIndex = 0; // Red
  ledOff = false;

  // Initialize button debouncing values
  lastStableBtn1 = buttonRead(BTN1_PIN);
  lastStableBtn2 = buttonRead(BTN2_PIN);
  lastReadingBtn1 = lastStableBtn1;
  lastReadingBtn2 = lastStableBtn2;

  // Initial brightness from potentiometer
  potRaw = analogRead(POT_PIN);
  brightness = clampU8(map(potRaw, 0, 1023, 0, 255));

  // Apply initial red (with current pot brightness)
  updateColorFromState();
}

void loop() {
  // Update button press detection
  updateButtons();

  // Handle button events (keep same behavior even if both pressed in same loop)
  if (pendingPress1) {
    pendingPress1 = false;
    handleBtn1Press();
  }
  if (pendingPress2) {
    pendingPress2 = false;
    handleBtn2Press();
  }

  // Continuous potentiometer -> brightness update
  int newPot = analogRead(POT_PIN);
  if (newPot != potRaw) {
    potRaw = newPot;
    brightness = clampU8(map(potRaw, 0, 1023, 0, 255));
    // Brightness should update in real-time based on current state.
    updateColorFromState();
  } else {
    // Even if potRaw unchanged, ensure brightness 0 still keeps LED off correctly.
    // (No extra action needed.)
  }
}
