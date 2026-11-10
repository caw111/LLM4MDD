#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
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

// Thresholds for 10-bit ADC values (0..1023)
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint8_t colorFromPot10bit(int v);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setRgbOff(uint8_t rPin, uint8_t gPin, uint8_t bPin);
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setRgbColorFromPot(uint8_t rPin, uint8_t gPin, uint8_t bPin, int potValue);
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void bargraphSetLevel(uint8_t level0to8);
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 21 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_55\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static inline uint8_t colorFromPot10bit(int v) {
  // lower third 0–340 -> red
  // middle third 341–681 -> green
  // upper third 682–1023 -> blue
  if (v <= 340) return 0; // RED
  if (v <= 681) return 1; // GREEN
  return 2;                // BLUE
}

static inline void setRgbOff(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
  digitalWrite(rPin, LOW);
  digitalWrite(gPin, LOW);
  digitalWrite(bPin, LOW);
}

static void setRgbColorFromPot(uint8_t rPin, uint8_t gPin, uint8_t bPin, int potValue) {
  // potValue is 0..1023. Use it as intensity.
  uint8_t color = colorFromPot10bit(potValue);

  // 10-bit to 8-bit PWM
  uint8_t pwmVal = (uint32_t)(potValue < 0 ? 0 : (potValue > 1023 ? 1023 : potValue)) * 255UL / 1023UL;

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
  if (level0to8 > 8) level0to8 = 8;
  for (uint8_t i = 0; i < 8; i++) {
    // Drive bargraph anodes HIGH for lit
    digitalWrite(BAR_A_PINS[i], (i < level0to8) ? HIGH : LOW);
  }
}

void setup() {
  // On Mega/UNO AVR, analogRead is 10-bit by default; no analogReadResolution available.
  // Ensure all components start in an off state when powered on.

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

  // Map avg to number of LEDs 0..8 (avg=512 -> floor(512*8/1023)=4)
  uint8_t level = (uint32_t)avg * 8UL / 1023UL;
  bargraphSetLevel(level);

  // Small delay to reduce jitter; outputs remain stable for >=2s if pots are stationary.
  delay(10);
}
