#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_51\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Common-cathode RGB LEDs: HIGH turns a color channel on.
const uint8_t RGB_PINS[3][3] = {
  {2, 3, 4},   // RGB1: red, green, blue
  {5, 6, 7},   // RGB2: red, green, blue
  {8, 9, 10}   // RGB3: red, green, blue
};

// Bar graph terminals A1 through A5, in lighting order.
// These are digital pins on the Mega, not analog input pins.
const uint8_t BAR_PINS[5] = {11, 12, 13, 44, 45};

const unsigned long PHASE_DURATION_MS = 2000UL;
uint8_t currentPhase = 0;
unsigned long phaseStartedAt = 0;

// Each entry specifies the red, green, and blue channels for each LED.
const bool PHASE_COLORS[5][3][3] = {
  {{true,  false, false}, {false, false, false}, {false, false, false}},
  {{false, false, false}, {false, true,  false}, {false, false, false}},
  {{false, false, false}, {false, false, false}, {false, false, true }},
  {{true,  false, false}, {false, true,  false}, {false, false, false}},
  {{true,  true,  true }, {true,  true,  true }, {true,  true,  true }}
};

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_51\\direct\\attempt_0\\sketch\\sketch.ino"
void applyPhase(uint8_t phase);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_51\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_51\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_51\\direct\\attempt_0\\sketch\\sketch.ino"
void applyPhase(uint8_t phase) {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(
        RGB_PINS[led][channel],
        PHASE_COLORS[phase][led][channel] ? HIGH : LOW
      );
    }
  }

  for (uint8_t segment = 0; segment < 5; ++segment) {
    digitalWrite(BAR_PINS[segment], segment <= phase ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  for (uint8_t segment = 0; segment < 5; ++segment) {
    digitalWrite(BAR_PINS[segment], LOW);
    pinMode(BAR_PINS[segment], OUTPUT);
  }

  applyPhase(currentPhase);
  phaseStartedAt = millis();
}

void loop() {
  const unsigned long now = millis();

  if (now - phaseStartedAt >= PHASE_DURATION_MS) {
    phaseStartedAt += PHASE_DURATION_MS;
    currentPhase = (currentPhase + 1) % 5;
    applyPhase(currentPhase);
  }
}
