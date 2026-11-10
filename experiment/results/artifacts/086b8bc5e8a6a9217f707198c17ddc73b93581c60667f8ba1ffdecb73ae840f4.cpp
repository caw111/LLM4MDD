#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <math.h>

// Arduino Mega 2560 wiring; both RGB LEDs are common-cathode.
const uint8_t RGB_PINS[2][3] = {
  {2, 3, 4},  // rgb1: red, green, blue
  {5, 6, 7}   // rgb2: red, green, blue
};

const uint8_t POT_PINS[2] = {A3, A4};

const unsigned long STARTUP_OFF_MS = 100;
const unsigned long UPDATE_INTERVAL_MS = 5;
const float FILTER_ALPHA = 0.10f;

float filteredPot[2];
unsigned long startupTime;
unsigned long lastUpdateTime;

#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t toPWM(float value);
#line 25 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
void setHSV(uint8_t led, float hue, float saturation, float value);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 96 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_47\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t toPWM(float value) {
  value = constrain(value, 0.0f, 1.0f);
  return static_cast<uint8_t>(value * 255.0f + 0.5f);
}

void setHSV(uint8_t led, float hue, float saturation, float value) {
  // Normalize 360 degrees to 0 degrees so the hue wheel is continuous.
  hue = fmodf(hue, 360.0f);
  if (hue < 0.0f) {
    hue += 360.0f;
  }

  const float chroma = value * saturation;
  const float sector = hue / 60.0f;
  const float x = chroma * (1.0f - fabsf(fmodf(sector, 2.0f) - 1.0f));
  const float offset = value - chroma;

  float red = 0.0f;
  float green = 0.0f;
  float blue = 0.0f;

  switch (static_cast<uint8_t>(sector)) {
    case 0:
      red = chroma;
      green = x;
      break;
    case 1:
      red = x;
      green = chroma;
      break;
    case 2:
      green = chroma;
      blue = x;
      break;
    case 3:
      green = x;
      blue = chroma;
      break;
    case 4:
      red = x;
      blue = chroma;
      break;
    default:
      red = chroma;
      blue = x;
      break;
  }

  // Common-cathode LEDs: larger PWM values mean brighter channels.
  analogWrite(RGB_PINS[led][0], toPWM(red + offset));
  analogWrite(RGB_PINS[led][1], toPWM(green + offset));
  analogWrite(RGB_PINS[led][2], toPWM(blue + offset));
}

void setup() {
  // Explicitly initialize all LED channels off.
  for (uint8_t led = 0; led < 2; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
      analogWrite(RGB_PINS[led][channel], 0);
    }
  }

  analogReference(DEFAULT);

  for (uint8_t pot = 0; pot < 2; ++pot) {
    pinMode(POT_PINS[pot], INPUT);
    analogRead(POT_PINS[pot]);  // Allow the ADC input to settle.
    filteredPot[pot] = analogRead(POT_PINS[pot]);
  }

  startupTime = millis();
  lastUpdateTime = startupTime;
}

void loop() {
  const unsigned long now = millis();

  // Keep both LEDs off briefly after power-on/reset before normal operation.
  if (now - startupTime < STARTUP_OFF_MS) {
    return;
  }

  if (now - lastUpdateTime < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateTime = now;

  for (uint8_t led = 0; led < 2; ++led) {
    analogRead(POT_PINS[led]);  // Discard the first sample after switching inputs.
    const int raw = analogRead(POT_PINS[led]);

    // Low-pass filtering suppresses ADC jitter and smooths slider movement.
    filteredPot[led] += FILTER_ALPHA * (raw - filteredPot[led]);

    const float hue = filteredPot[led] * (360.0f / 1023.0f);
    setHSV(led, hue, 1.0f, 1.0f);
  }
}
