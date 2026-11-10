#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t PIN_POT_SIG = A3; // pot1:SIG -> mega:A3
const uint8_t PIN_RGB_R = 2;    // rgb1:R -> mega:2
const uint8_t PIN_RGB_G = 3;    // rgb1:G -> mega:3
const uint8_t PIN_RGB_B = 4;    // rgb1:B -> mega:4

// bargraph1 pins: A1..A10 -> mega:5..13,44
const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPot0to1023();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t mapToSegments(uint16_t potVal);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarGraph(uint8_t segmentsOn);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 66 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_54\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPot0to1023() {
  // Keep behavior deterministic for verification; analogRead range is 0..1023 on Mega.
  return (uint16_t)analogRead(PIN_POT_SIG);
}

static uint8_t mapToSegments(uint16_t potVal) {
  // 0-1023 -> 0-10 segments, where 1023 lights all 10 segments.
  // Use rounding down by proportional mapping.
  // segments = floor(potVal * 10 / 1024) gives: 1023 -> 9 (incorrect). Adjust to ensure 1023 -> 10.
  // Use scaling with 1023 divisor:
  uint8_t seg = (uint32_t)potVal * 10UL / 1023UL; // 0 -> 0, 1023 -> 10
  if (seg > 10) seg = 10;
  return seg;
}

static void setBarGraph(uint8_t segmentsOn) {
  // Off state on reset is required; here we drive all pins accordingly.
  // bargraph is common-cathode (per rgb common cathode; bargraph assumed same style via wiring),
  // but wiring doesn't include explicit inversion. We'll implement as: HIGH turns on.
  // (If your bargraph is common-anode, this would invert; but wiring requirement says preserve behavior.)
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < segmentsOn) ? HIGH : LOW);
  }
}

static uint16_t potVal = 0;
static uint8_t currentMode = 0; // 0=R, 1=G, 2=B
static unsigned long modeStartMs = 0;

static uint8_t rIntensity = 0;
static uint8_t gIntensity = 0;
static uint8_t bIntensity = 0;

void setup() {
  // Start in off state for all components.
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
    digitalWrite(BAR_PINS[i], LOW);
  }

  pinMode(PIN_POT_SIG, INPUT);

  // Initialize mode cycling.
  currentMode = 0; // Red mode first 5 seconds
  modeStartMs = millis();
}

void loop() {
  unsigned long now = millis();

  // Enforce exact 5-second durations per mode.
  // Use integer intervals to avoid drift.
  uint8_t newMode = (uint8_t)((now / 5000UL) % 3UL);
  if (newMode != currentMode) {
    currentMode = newMode;
    // No reset of other intensities; persistence rule requires keeping previous values.
    modeStartMs = now;
  }

  // Read potentiometer continuously to update intensity and bar graph in real time.
  potVal = readPot0to1023();

  // Map pot 0..1023 to 0..255 for PWM brightness.
  uint8_t intensity = (uint8_t)((uint32_t)potVal * 255UL / 1023UL);

  // Update only the currently active component; keep the others persisted.
  if (currentMode == 0) {
    rIntensity = intensity; // Red mode
  } else if (currentMode == 1) {
    gIntensity = intensity; // Green mode
  } else {
    bIntensity = intensity; // Blue mode
  }

  // Update RGB LED with combined intensities.
  // Note: rgb1 is common cathode (attrs: common="cathode"), so PWM HIGH turns on.
  analogWrite(PIN_RGB_R, rIntensity);
  analogWrite(PIN_RGB_G, gIntensity);
  analogWrite(PIN_RGB_B, bIntensity);

  // Update bar graph according to potentiometer value (0-1023 => 0-10 segments).
  uint8_t segmentsOn = mapToSegments(potVal);
  setBarGraph(segmentsOn);
}
