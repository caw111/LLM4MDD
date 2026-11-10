#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_4\\direct\\attempt_0\\sketch\\sketch.ino"
const uint8_t RED_PIN = 2;
const uint8_t GREEN_PIN = 3;
const uint8_t BLUE_PIN = 4;

const unsigned long STEP_DURATION_MS = 3000UL;

// Common-cathode RGB LED: HIGH turns a color channel on.
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_4\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue);
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_4\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_4\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_4\\direct\\attempt_0\\sketch\\sketch.ino"
void setColor(bool red, bool green, bool blue) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

void setup() {
  // Initialize output latches LOW before enabling the outputs.
  setColor(false, false, false);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  setColor(true, false, false);  // Red
  delay(STEP_DURATION_MS);

  setColor(true, true, false);   // Yellow
  delay(STEP_DURATION_MS);

  setColor(true, true, true);    // White
  delay(STEP_DURATION_MS);

  setColor(false, false, false); // Off
  delay(STEP_DURATION_MS);
}
