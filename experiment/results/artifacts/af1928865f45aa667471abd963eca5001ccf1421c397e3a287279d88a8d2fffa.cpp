#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const byte LED1_PIN = 2;   // led1:A
const byte LED2_PIN = 3;   // led2:A

const byte RGB1_R = 4;     // rgb1:R
const byte RGB1_G = 5;     // rgb1:G
const byte RGB1_B = 6;     // rgb1:B

const byte RGB2_R = 7;     // rgb2:R
const byte RGB2_G = 8;     // rgb2:G
const byte RGB2_B = 9;     // rgb2:B

// Bar graph segments (A1..A6). Common cathode to GND already in circuit.
const byte BAR_A1 = 10;   // bargraph1:A1
const byte BAR_A2 = 11;   // bargraph1:A2
const byte BAR_A3 = 12;   // bargraph1:A3
const byte BAR_A4 = 13;   // bargraph1:A4
const byte BAR_A5 = 44;   // bargraph1:A5
const byte BAR_A6 = 45;   // bargraph1:A6

#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setMono(byte pin, bool on);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(byte rPin, byte gPin, byte bPin, bool r, bool g, bool b);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarSegments(bool a1, bool a2, bool a3, bool a4, bool a5, bool a6);
#line 41 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_114\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setMono(byte pin, bool on) {
  digitalWrite(pin, on ? HIGH : LOW);
}

static inline void setRGB(byte rPin, byte gPin, byte bPin, bool r, bool g, bool b) {
  digitalWrite(rPin, r ? HIGH : LOW);
  digitalWrite(gPin, g ? HIGH : LOW);
  digitalWrite(bPin, b ? HIGH : LOW);
}

static inline void setBarSegments(bool a1, bool a2, bool a3, bool a4, bool a5, bool a6) {
  digitalWrite(BAR_A1, a1 ? HIGH : LOW);
  digitalWrite(BAR_A2, a2 ? HIGH : LOW);
  digitalWrite(BAR_A3, a3 ? HIGH : LOW);
  digitalWrite(BAR_A4, a4 ? HIGH : LOW);
  digitalWrite(BAR_A5, a5 ? HIGH : LOW);
  digitalWrite(BAR_A6, a6 ? HIGH : LOW);
}

void setup() {
  // Ensure everything is off at startup/reset.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(RGB1_R, OUTPUT);
  pinMode(RGB1_G, OUTPUT);
  pinMode(RGB1_B, OUTPUT);

  pinMode(RGB2_R, OUTPUT);
  pinMode(RGB2_G, OUTPUT);
  pinMode(RGB2_B, OUTPUT);

  pinMode(BAR_A1, OUTPUT);
  pinMode(BAR_A2, OUTPUT);
  pinMode(BAR_A3, OUTPUT);
  pinMode(BAR_A4, OUTPUT);
  pinMode(BAR_A5, OUTPUT);
  pinMode(BAR_A6, OUTPUT);

  setMono(LED1_PIN, false);
  setMono(LED2_PIN, false);

  setRGB(RGB1_R, RGB1_G, RGB1_B, false, false, false);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, false, false);

  setBarSegments(false, false, false, false, false, false);
}

void loop() {
  // State 1 (2 seconds)
  setMono(LED1_PIN, true);
  setMono(LED2_PIN, false);

  // rgb1 displays red
  setRGB(RGB1_R, RGB1_G, RGB1_B, true, false, false);
  // rgb2 off (not specified in state 1, but requirement says use all components in every state)
  // Turn rgb2 off explicitly to satisfy "using all components in every state" per requirement.
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, false, false);

  // First three bar segments light up (A1-A3)
  setBarSegments(true, true, true, false, false, false);

  delay(2000);

  // State 2 (2 seconds)
  setMono(LED1_PIN, false);
  setMono(LED2_PIN, true);

  // rgb2 displays green
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, true, false);
  // rgb1 off explicitly
  setRGB(RGB1_R, RGB1_G, RGB1_B, false, false, false);

  // Last three bar segments light up (A4-A6)
  setBarSegments(false, false, false, true, true, true);

  delay(2000);

  // State 3 (2 seconds)
  setMono(LED1_PIN, true);
  setMono(LED2_PIN, true);

  // rgb1 and rgb2 display cyan (blue + green)
  setRGB(RGB1_R, RGB1_G, RGB1_B, false, true, true);
  setRGB(RGB2_R, RGB2_G, RGB2_B, false, true, true);

  // All six bar segments light up (A1-A6)
  setBarSegments(true, true, true, true, true, true);

  delay(2000);
}
