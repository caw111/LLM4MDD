#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch
// Controls 3 LEDs (led1..led3) and a 10-segment LED bar graph (bargraph1 A1..A10).

// Wiring preserved from provided circuit:
// LEDs:
//  led1: cathode -> GND, anode  -> pin 2
//  led2: cathode -> GND, anode  -> pin 3
//  led3: cathode -> GND, anode  -> pin 4
//
// Bar graph:
//  segments A1..A10 -> pins 5..13
//  bargraph common cathode C1..C10 -> GND (already wired)

static const uint8_t LED_PINS[3] = {2, 3, 4};
static const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff();
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBarLitCount(uint8_t count);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_96\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void allOff() {
  for (uint8_t i = 0; i < 3; i++) digitalWrite(LED_PINS[i], LOW);
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
}

static void setBarLitCount(uint8_t count) {
  // count: 0..10, lights A1..Acount
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < count) ? HIGH : LOW);
  }
}

void setup() {
  // Ensure everything starts off after reset
  for (uint8_t i = 0; i < 3; i++) pinMode(LED_PINS[i], OUTPUT);
  for (uint8_t i = 0; i < 10; i++) pinMode(BAR_PINS[i], OUTPUT);

  allOff();
}

void loop() {
  // Sequential blinking: each LED active for exactly 2 seconds.
  for (uint8_t ledIndex = 0; ledIndex < 3; ledIndex++) {
    allOff();

    // Turn on LEDs up to and including current one
    for (uint8_t i = 0; i <= ledIndex; i++) {
      digitalWrite(LED_PINS[i], HIGH);
    }

    // Bargraph shows number of currently active LEDs (1 segment per active LED)
    setBarLitCount(ledIndex + 1);

    delay(2000);
  }

  // Final phase: bargraph all segments for 2 seconds
  allOff();
  setBarLitCount(10);
  delay(2000);
}
