#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_95\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560 wiring:
// LED anodes: D2 and D3; cathodes: GND.
// Bar graph A1-A10: D4-D13; C1-C10: GND.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr uint8_t BAR_PINS[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
constexpr uint8_t SEGMENT_COUNT = sizeof(BAR_PINS) / sizeof(BAR_PINS[0]);
constexpr unsigned long STEP_INTERVAL_MS = 2000UL;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_95\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_95\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_95\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Set output latches LOW before enabling outputs.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }
}

void loop() {
  for (uint8_t segment = 0; segment < SEGMENT_COUNT; ++segment) {
    // Previously illuminated segments remain on as the bar fills.
    digitalWrite(BAR_PINS[segment], HIGH);

    // Turn the previous activity LED off before turning the other on.
    if ((segment % 2) == 0) {
      digitalWrite(LED2_PIN, LOW);
      digitalWrite(LED1_PIN, HIGH);
    } else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, HIGH);
    }

    // Hold each loading step, including the fully illuminated bar.
    delay(STEP_INTERVAL_MS);
  }

  // Clear the entire bar, then restart the loading animation.
  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
  }
}
