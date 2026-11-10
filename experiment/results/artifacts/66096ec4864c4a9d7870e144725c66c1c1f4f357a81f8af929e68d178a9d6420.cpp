#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch controlling three common-cathode RGB LEDs
// rgb1: R=2, G=3, B=4, COM=GND
// rgb2: R=5, G=6, B=7, COM=GND
// rgb3: R=8, G=9, B=10, COM=GND

const uint8_t rgb1R = 2;
const uint8_t rgb1G = 3;
const uint8_t rgb1B = 4;

const uint8_t rgb2R = 5;
const uint8_t rgb2G = 6;
const uint8_t rgb2B = 7;

const uint8_t rgb3R = 8;
const uint8_t rgb3G = 9;
const uint8_t rgb3B = 10;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB1(bool r, bool g, bool b);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB2(bool r, bool g, bool b);
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB3(bool r, bool g, bool b);
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_18\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setRGB1(bool r, bool g, bool b) {
  digitalWrite(rgb1R, r ? HIGH : LOW);
  digitalWrite(rgb1G, g ? HIGH : LOW);
  digitalWrite(rgb1B, b ? HIGH : LOW);
}

static inline void setRGB2(bool r, bool g, bool b) {
  digitalWrite(rgb2R, r ? HIGH : LOW);
  digitalWrite(rgb2G, g ? HIGH : LOW);
  digitalWrite(rgb2B, b ? HIGH : LOW);
}

static inline void setRGB3(bool r, bool g, bool b) {
  digitalWrite(rgb3R, r ? HIGH : LOW);
  digitalWrite(rgb3G, g ? HIGH : LOW);
  digitalWrite(rgb3B, b ? HIGH : LOW);
}

static inline void allOff() {
  setRGB1(false, false, false);
  setRGB2(false, false, false);
  setRGB3(false, false, false);
}

void setup() {
  pinMode(rgb1R, OUTPUT);
  pinMode(rgb1G, OUTPUT);
  pinMode(rgb1B, OUTPUT);

  pinMode(rgb2R, OUTPUT);
  pinMode(rgb2G, OUTPUT);
  pinMode(rgb2B, OUTPUT);

  pinMode(rgb3R, OUTPUT);
  pinMode(rgb3G, OUTPUT);
  pinMode(rgb3B, OUTPUT);

  allOff(); // On power-up/reset: all LEDs off
}

void loop() {
  // Pattern 1 (2 seconds):
  // LED1 red, LED2 blue, LED3 green
  setRGB1(true,  false, false);
  setRGB2(false, false, true);
  setRGB3(false, true,  false);
  delay(2000);

  // Pattern 2 (2 seconds):
  // LED1 green, LED2 red, LED3 blue
  setRGB1(false, true,  false);
  setRGB2(true,  false, false);
  setRGB3(false, false, true);
  delay(2000);

  // Pattern 3 (2 seconds):
  // LED1 blue, LED2 green, LED3 red
  setRGB1(false, false, true);
  setRGB2(false, true,  false);
  setRGB3(true,  false, false);
  delay(2000);
}
