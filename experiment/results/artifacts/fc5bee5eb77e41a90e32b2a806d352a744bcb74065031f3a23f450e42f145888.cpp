#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// RGB LED (common cathode)
static const uint8_t RGB_R = 4;
static const uint8_t RGB_G = 5;
static const uint8_t RGB_B = 6;

// Potentiometers
static const uint8_t POT1_PIN = A3; // pot1 -> red intensity
static const uint8_t POT2_PIN = A4; // pot2 -> green intensity
static const uint8_t POT3_PIN = A5; // pot3 -> blue intensity

// Buttons (active-high, with wiring as given)
static const uint8_t BTN1_PIN = 2;   // cycles modes
static const uint8_t BTN2_PIN = 3;   // reset to off and Mode 1

// Debounce
static const uint32_t DEBOUNCE_MS = 40;

static uint8_t mode = 1;          // Mode 1..4
static uint8_t lastBtn1 = HIGH;
static uint8_t lastBtn2 = HIGH;
static uint32_t lastDebounceBtn1 = 0;
static uint32_t lastDebounceBtn2 = 0;

#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToPWM(int adc);
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyOutput();
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllOff();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_84\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline int mapPotToPWM(int adc) {
  // adc from 0..1023 -> PWM 0..255
  if (adc < 0) adc = 0;
  if (adc > 1023) adc = 1023;
  return (adc * 255) / 1023;
}

static void applyOutput() {
  int adc1 = analogRead(POT1_PIN);
  int adc2 = analogRead(POT2_PIN);
  int adc3 = analogRead(POT3_PIN);

  int rVal = mapPotToPWM(adc1);
  int gVal = mapPotToPWM(adc2);
  int bVal = mapPotToPWM(adc3);

  // common cathode: HIGH means off, PWM=0 means full on.
  // So we invert PWM values: output = 255 - desiredIntensity(0..255)
  // Mode determines which components are active.
  uint8_t rOut = 255; // off
  uint8_t gOut = 255; // off
  uint8_t bOut = 255; // off

  switch (mode) {
    case 1: // only red
      rOut = (uint8_t)(255 - rVal);
      break;
    case 2: // only green
      gOut = (uint8_t)(255 - gVal);
      break;
    case 3: // only blue
      bOut = (uint8_t)(255 - bVal);
      break;
    case 4: // all
      rOut = (uint8_t)(255 - rVal);
      gOut = (uint8_t)(255 - gVal);
      bOut = (uint8_t)(255 - bVal);
      break;
    default:
      break;
  }

  analogWrite(RGB_R, rOut);
  analogWrite(RGB_G, gOut);
  analogWrite(RGB_B, bOut);
}

static void setAllOff() {
  // For common cathode, off = drive HIGH.
  analogWrite(RGB_R, 255);
  analogWrite(RGB_G, 255);
  analogWrite(RGB_B, 255);
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  // Ensure LED is off on power/reset.
  mode = 1;
  setAllOff();

  // Initialize button states
  lastBtn1 = digitalRead(BTN1_PIN);
  lastBtn2 = digitalRead(BTN2_PIN);

  // Ensure immediate update after startup? Requirements say: initialize off.
  // Keep off until potentiometer adjustment or button press.
}

void loop() {
  // Read buttons with debouncing; detect falling edge (HIGH->LOW) due to INPUT_PULLUP.
  uint8_t btn1Reading = digitalRead(BTN1_PIN);
  uint8_t btn2Reading = digitalRead(BTN2_PIN);

  uint32_t now = millis();

  bool btn1Pressed = false;
  bool btn2Pressed = false;

  if (btn1Reading != lastBtn1) {
    lastDebounceBtn1 = now;
    lastBtn1 = btn1Reading;
  }
  if ((now - lastDebounceBtn1) > DEBOUNCE_MS) {
    // Press event on transition to LOW (active press)
    static uint8_t stableBtn1 = HIGH;
    if (stableBtn1 != btn1Reading) {
      stableBtn1 = btn1Reading;
      if (stableBtn1 == LOW) btn1Pressed = true;
    }
  }

  if (btn2Reading != lastBtn2) {
    lastDebounceBtn2 = now;
    lastBtn2 = btn2Reading;
  }
  if ((now - lastDebounceBtn2) > DEBOUNCE_MS) {
    static uint8_t stableBtn2 = HIGH;
    if (stableBtn2 != btn2Reading) {
      stableBtn2 = btn2Reading;
      if (stableBtn2 == LOW) btn2Pressed = true;
    }
  }

  // Handle button actions
  if (btn1Pressed) {
    mode++;
    if (mode > 4) mode = 1;
    applyOutput(); // update immediately after button press
  }

  if (btn2Pressed) {
    setAllOff();   // reset LED to off
    mode = 1;      // set mode back to Mode 1
    // Requirement says "resets ... and sets the mode back to Mode 1".
    // It should remain off until potentiometer adjustment or next button press.
  }

  // Potentiometer interaction: update immediately after potentiometer adjustment.
  // We implement "change detection" so LED updates only on meaningful movement.
  static int lastAdc1 = -1, lastAdc2 = -1, lastAdc3 = -1;
  int adc1 = analogRead(POT1_PIN);
  int adc2 = analogRead(POT2_PIN);
  int adc3 = analogRead(POT3_PIN);

  // Threshold to avoid tiny ADC noise triggering updates constantly.
  // Still preserves behavior: any real adjustment will update immediately.
  const int THRESH = 4;

  bool potChanged =
      (lastAdc1 < 0) ||
      (abs(adc1 - lastAdc1) > THRESH) ||
      (lastAdc2 < 0) ||
      (abs(adc2 - lastAdc2) > THRESH) ||
      (lastAdc3 < 0) ||
      (abs(adc3 - lastAdc3) > THRESH);

  if (potChanged) {
    lastAdc1 = adc1;
    lastAdc2 = adc2;
    lastAdc3 = adc3;
    applyOutput();
  }

  // Small pause to reduce loop jitter
  delay(5);
}
