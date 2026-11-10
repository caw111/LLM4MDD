#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 sketch
// Controls: led1 (PWM brightness) and bargraph1 (10 segments) via slide potentiometer pot1

#include <Arduino.h>

static const uint8_t LED_PIN = 2;     // led1:A -> mega:2
static const uint8_t POT_PIN = A3;    // pot1:SIG -> mega:A3

// Bar graph segment pins:
// mega:3  -> A1
// mega:4  -> A2
// mega:5  -> A3
// mega:6  -> A4
// mega:7  -> A5
// mega:8  -> A6
// mega:9  -> A7
// mega:10 -> A8
// mega:11 -> A9
// mega:12 -> A10
static const uint8_t BAR_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static const int POT_MAX = 1023;

// Blink parameters for 0.25 Hz:
// on for 2 seconds, off for 2 seconds
static const unsigned long BLINK_ON_MS = 2000UL;
static const unsigned long BLINK_OFF_MS = 2000UL;

unsigned long blinkPhaseStartMs = 0;
bool ledCurrentlyOn = false;

#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllBargraphOff();
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBargraphSegments(int litSegments);
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_99\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllBargraphOff() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], LOW);
  }
}

void setBargraphSegments(int litSegments) {
  // litSegments: 0..10
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < (uint8_t)litSegments) ? HIGH : LOW);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Initialization requirement:
  // LED and all bar graph segments must be off on startup.
  analogWrite(LED_PIN, 0);
  setAllBargraphOff();

  // Initialize blink state
  blinkPhaseStartMs = millis();
  ledCurrentlyOn = false;
}

void loop() {
  int pot = analogRead(POT_PIN); // 0..1023

  // Bar graph activation based on potentiometer value
  // Range mapping per requirement:
  // 0-102 -> 0 segments
  // 103-204 -> 1 segment (A1)
  // ...
  // 921-1023 -> 10 segments (A10)
  int litSegments = 0;
  if (pot <= 102) {
    litSegments = 0;
  } else {
    // Determine 1..10 by 102-width intervals:
    // segments = floor((pot - 103) / 102) + 1
    // Ensure 10 max.
    litSegments = ((pot - 103) / 102) + 1;
    if (litSegments > 10) litSegments = 10;
    if (litSegments < 0) litSegments = 0;
  }
  setBargraphSegments(litSegments);

  // LED brightness control with blinking at maximum (1023)
  if (pot >= POT_MAX) {
    // Blink at 0.25 Hz: 2s ON / 2s OFF
    unsigned long now = millis();

    if (ledCurrentlyOn) {
      if (now - blinkPhaseStartMs >= BLINK_ON_MS) {
        ledCurrentlyOn = false;
        blinkPhaseStartMs = now;
        analogWrite(LED_PIN, 0);
      }
    } else {
      if (now - blinkPhaseStartMs >= BLINK_OFF_MS) {
        ledCurrentlyOn = true;
        blinkPhaseStartMs = now;
        // Full brightness during ON phase
        analogWrite(LED_PIN, 255);
      }
    }
  } else {
    // LED brightness directly proportional to potentiometer value
    // analogWrite expects 0..255, map 0..1023 -> 0..255
    int pwm = (pot * 255L) / 1023L;
    analogWrite(LED_PIN, pwm);

    // Keep blink state consistent so that when it reaches 1023,
    // it starts blinking from a known phase.
    ledCurrentlyOn = false;
    blinkPhaseStartMs = millis();
  }
}
