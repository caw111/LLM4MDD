#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_4\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t RGB_R_PIN = 2; // mega:2 -> rgb1:R
const uint8_t RGB_G_PIN = 3; // mega:3 -> rgb1:G
const uint8_t RGB_B_PIN = 4; // mega:4 -> rgb1:B

// Common cathode RGB LED: COM to GND, so:
// HIGH turns a color ON, LOW turns it OFF.
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_4\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(bool r, bool g, bool b);
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_4\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_4\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 9 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_4\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(bool r, bool g, bool b) {
  digitalWrite(RGB_R_PIN, r ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, g ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, b ? HIGH : LOW);
}

void setup() {
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialization: RGB LED off
  setRGB(false, false, false);
}

void loop() {
  // Each color lasts for 3 seconds.
  // Repeat indefinitely after turning off at the end.

  // Red
  setRGB(true, false, false);
  delay(3000);

  // Yellow (red + green)
  setRGB(true, true, false);
  delay(3000);

  // White (red + green + blue)
  setRGB(true, true, true);
  delay(3000);

  // Off
  setRGB(false, false, false);
  delay(3000);
}
