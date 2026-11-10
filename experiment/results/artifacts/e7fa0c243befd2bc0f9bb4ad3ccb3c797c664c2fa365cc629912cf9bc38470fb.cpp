#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

#define TM1637_CLK 4   // D4
#define TM1637_DIO 5   // D5

// Analog input: A0 potentiometer
static const uint8_t POT_PIN = A0;

// Display
TM1637Display display(TM1637_CLK, TM1637_DIO);

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Initialize TM1637 (with auto brightness; can be adjusted if needed)
  display.setBrightness(0x0f);
  display.clear();

  // Ensure deterministic analog reads
  analogReference(DEFAULT);
}

void loop() {
  // Read potentiometer and map to 0~9999
  int pot = analogRead(POT_PIN);              // 0..1023
  int value = map(pot, 0, 1023, 0, 9999);   // 0..9999

  // Show value on TM1637 as 4-digit number
  display.showNumberDecEx(value, 0b01000000, true, 4);
}
