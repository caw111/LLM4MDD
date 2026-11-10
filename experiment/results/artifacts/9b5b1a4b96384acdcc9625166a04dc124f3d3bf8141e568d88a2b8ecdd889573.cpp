#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch to control rgb1 using pot1 (slide potentiometer)
// Wiring preserved as provided in the circuit.

const int POT_PIN = A3;   // pot1:SIG -> mega:A3
const int RED_PIN = 2;   // mega:2 -> rgb1:R
const int GRN_PIN = 3;   // mega:3 -> rgb1:G
const int BLU_PIN = 4;   // mega:4 -> rgb1:B

// Active-low outputs (RGB LED common cathode):
// COM -> GND, so driving R/G/B HIGH turns LEDs ON.
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setColor(bool rOn, bool gOn, bool bOn);
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setColor(bool rOn, bool gOn, bool bOn) {
  digitalWrite(RED_PIN,  rOn ? HIGH : LOW);
  digitalWrite(GRN_PIN,  gOn ? HIGH : LOW);
  digitalWrite(BLU_PIN,  bOn ? HIGH : LOW);
}

void setup() {
  pinMode(POT_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GRN_PIN, OUTPUT);
  pinMode(BLU_PIN, OUTPUT);

  // Start with all LEDs off (per 0-127).
  setColor(false, false, false);
}

void loop() {
  int potVal = analogRead(POT_PIN); // 0..1023

  // Map potVal to the 8 predefined ranges exactly as specified:
  // 0-127: off
  // 128-255: Red
  // 256-383: Green
  // 384-511: Blue
  // 512-639: Red + Green (Yellow)
  // 640-767: Red + Blue (Magenta)
  // 768-895: Green + Blue (Cyan)
  // 896-1023: all on (White)

  if (potVal >= 0 && potVal <= 127) {
    setColor(false, false, false);        // All LEDs off
  } else if (potVal <= 255) {
    setColor(true, false, false);         // Red
  } else if (potVal <= 383) {
    setColor(false, true, false);         // Green
  } else if (potVal <= 511) {
    setColor(false, false, true);         // Blue
  } else if (potVal <= 639) {
    setColor(true, true, false);          // Red + Green (Yellow)
  } else if (potVal <= 767) {
    setColor(true, false, true);          // Red + Blue (Magenta)
  } else if (potVal <= 895) {
    setColor(false, true, true);          // Green + Blue (Cyan)
  } else { // 896-1023
    setColor(true, true, true);           // All LEDs on (White)
  }

  // Update immediately when potentiometer is adjusted.
  // No delay to ensure fast response.
}
