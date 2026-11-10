#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t PIN_POT = A3;
static const uint8_t PIN_R = 2;
static const uint8_t PIN_G = 3;
static const uint8_t PIN_B = 4;

// rgb1 is common cathode -> HIGH turns the LED channel ON, LOW turns it OFF.

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_23\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_POT, INPUT);

  // Start with all LEDs off.
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_B, LOW);
}

void loop() {
  int pot = analogRead(PIN_POT); // 0..1023

  bool r = false, g = false, b = false;

  // Map 0-1023 into 8 equal ranges: 0-127,128-255,...,896-1023
  if (pot <= 127) {
    // all off
  } else if (pot <= 255) {
    r = true;
  } else if (pot <= 383) {
    g = true;
  } else if (pot <= 511) {
    b = true;
  } else if (pot <= 639) {
    r = true; g = true; // Yellow
  } else if (pot <= 767) {
    r = true; b = true; // Magenta
  } else if (pot <= 895) {
    g = true; b = true; // Cyan
  } else {
    r = true; g = true; b = true; // White
  }

  digitalWrite(PIN_R, r ? HIGH : LOW);
  digitalWrite(PIN_G, g ? HIGH : LOW);
  digitalWrite(PIN_B, b ? HIGH : LOW);
}
