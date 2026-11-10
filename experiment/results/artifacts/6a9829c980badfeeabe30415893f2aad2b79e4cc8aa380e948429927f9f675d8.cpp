#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Wiring (preserved from circuit)
static const uint8_t POT1_PIN = A3;
static const uint8_t POT2_PIN = A4;

// rgb1 (common cathode)
static const uint8_t RGB1_R_PIN = 2;
static const uint8_t RGB1_G_PIN = 3;
static const uint8_t RGB1_B_PIN = 4;

// rgb2 (common cathode)
static const uint8_t RGB2_R_PIN = 5;
static const uint8_t RGB2_G_PIN = 6;
static const uint8_t RGB2_B_PIN = 7;

// bargraph1 (8 LEDs, each with one "anode" pin A1..A8 and one common cathode C1..C8 all tied to GND.1)
static const uint8_t BAR_A_PINS[8] = {8, 9, 10, 11, 12, 13, 44, 45};
static const uint8_t BAR_C_PINS[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // not driven individually; all common is on GND.1 in the circuit

// Thresholds for 10-bit ADC values (0..1023)
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t colorFromPot10bit(int v);
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbOff(uint8_t rPin, uint8_t gPin, uint8_t bPin);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRgbColorFromPot(uint8_t rPin, uint8_t gPin, uint8_t bPin, int potValue);
#line 65 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void bargraphSetLevel(uint8_t level0to8);
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline uint8_t colorFromPot10bit(int v) {
  // lower third 0–340 -> red
  // middle third 341–681 -> green
  // upper third 682–1023 -> blue
  if (v <= 340) return 0;      // RED
  if (v <= 681) return 1;      // GREEN
  return 2;                     // BLUE
}

static inline void setRgbOff(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);
  digitalWrite(bPin, LOW);
}

static void setRgbColorFromPot(uint8_t rPin, uint8_t gPin, uint8_t bPin, int potValue) {
  // Use potValue as intensity (0..1023).
  // Since circuit uses direct pins to LED cathodes/anodes, we PWM the chosen color pin.
  // For common-cathode RGB LED:
  // - Drive the chosen color pin with PWM
  // - Drive the other two pins LOW (off)
  uint8_t color = colorFromPot10bit(potValue);

  // 10-bit to 8-bit PWM
  uint8_t pwmVal = (uint32_t)potValue * 255UL / 1023UL;

  // Off-state when pot is stationary near 0; still should reflect immediately.
  // Always update all channels to preserve behavior.
  if (color == 0) { // RED
    analogWrite(rPin, pwmVal);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
  } else if (color == 1) { // GREEN
    digitalWrite(rPin, LOW);
    analogWrite(gPin, pwmVal);
    digitalWrite(bPin, LOW);
  } else { // BLUE
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, LOW);
    analogWrite(bPin, pwmVal);
  }
}

static void bargraphSetLevel(uint8_t level0to8) {
  // Level proportional to average of both potentiometers:
  // If average is 0..1023, light number of LEDs 0..8.
  // Example: average 512 => 4 LEDs (as specified).
  // We'll map using integer math: level = floor(avg * 8 / 1023).
  // Ensure range clamps 0..8.
  if (level0to8 > 8) level0to8 = 8;

  for (uint8_t i = 0; i < 8; i++) {
    // For bar graph, each LED is between its anode (A1..A8) and common cathode (C.. tied to GND.1).
    // We'll drive anode HIGH for lit, LOW for off.
    digitalWrite(BAR_A_PINS[i], (i < level0to8) ? HIGH : LOW);
  }
}

void setup() {
  // ADC
  analogReadResolution(10);

  // RGB pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Bar graph anode pins
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // All components start in off state
  setRgbOff(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN);
  setRgbOff(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN);

  // Ensure PWM pins start from 0
  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);
  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);
}

void loop() {
  // Read potentiometers (0..1023)
  int pot1 = analogRead(POT1_PIN);
  int pot2 = analogRead(POT2_PIN);

  // Update RGB LEDs immediately
  setRgbColorFromPot(RGB1_R_PIN, RGB1_G_PIN, RGB1_B_PIN, pot1);
  setRgbColorFromPot(RGB2_R_PIN, RGB2_G_PIN, RGB2_B_PIN, pot2);

  // Bar graph shows average intensity
  int avg = (pot1 + pot2) / 2; // 0..1023

  // Map avg to number of LEDs 0..8.
  // Example: avg=512 => floor(512*8/1023)=4
  uint8_t level = (uint32_t)avg * 8UL / 1023UL;
  bargraphSetLevel(level);

  // No additional timing needed:
  // - Continuous updates reflect movements immediately.
  // - If potentiometers remain stationary, outputs remain unchanged for at least 2 seconds automatically.
  delay(10);
}
