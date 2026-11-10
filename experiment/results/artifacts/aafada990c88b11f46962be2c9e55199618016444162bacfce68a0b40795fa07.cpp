#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_45\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

// Arduino Mega 2560 wiring: each row contains red, green, and blue.
const uint8_t RGB_PINS[3][3] = {
  {2, 3, 4},
  {5, 6, 7},
  {8, 9, 10}
};

const uint8_t SERVO_PINS[3] = {11, 12, 13};
Servo servos[3];

struct DisplayStep {
  uint8_t rgb[3];
  uint8_t angles[3];
};

const DisplayStep SEQUENCE[] = {
  {{HIGH, LOW,  LOW }, {  0,  45,  90}}, // Red
  {{LOW,  HIGH, LOW }, { 45,  90, 135}}, // Green
  {{LOW,  LOW,  HIGH}, { 90, 135, 180}}, // Blue
  {{HIGH, HIGH, LOW }, {135, 180,   0}}, // Yellow
  {{LOW,  HIGH, HIGH}, {180,   0,  45}}, // Cyan
  {{HIGH, LOW,  HIGH}, {  0,  45,  90}}, // Magenta
  {{HIGH, HIGH, HIGH}, { 90, 135, 180}}  // White
};

const uint8_t STEP_COUNT = sizeof(SEQUENCE) / sizeof(SEQUENCE[0]);
const unsigned long COLOR_DURATION_MS = 2000UL;
const unsigned long INITIAL_SETTLE_MS = 1000UL;

#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_45\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_45\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_45\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Common-cathode LEDs: LOW turns a channel off.
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  for (uint8_t i = 0; i < 3; ++i) {
    // Preload zero before enabling servo pulses to avoid a 90-degree start.
    servos[i].write(0);
    servos[i].attach(SERVO_PINS[i]);
    servos[i].write(0);
  }

  // Keep LEDs off while the servos reach their initial positions.
  delay(INITIAL_SETTLE_MS);
}

void loop() {
  for (uint8_t step = 0; step < STEP_COUNT; ++step) {
    for (uint8_t i = 0; i < 3; ++i) {
      servos[i].write(SEQUENCE[step].angles[i]);
    }

    // Full-intensity colors need only digital outputs, not PWM.
    for (uint8_t led = 0; led < 3; ++led) {
      for (uint8_t channel = 0; channel < 3; ++channel) {
        digitalWrite(RGB_PINS[led][channel], SEQUENCE[step].rgb[channel]);
      }
    }

    delay(COLOR_DURATION_MS);
  }
}
