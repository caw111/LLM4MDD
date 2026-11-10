#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_108\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t STATUS_LED_PIN = 2;

// Common-cathode RGB LEDs: HIGH / 255 turns a channel on.
constexpr uint8_t RGB_PINS[3][3] = {
  {3, 4, 5},    // rgb1: R, G, B
  {6, 7, 8},    // rgb2: R, G, B
  {9, 10, 11}   // rgb3: R, G, B
};

constexpr uint8_t COLORS[7][3] = {
  {255,   0,   0}, // Red
  {  0, 255,   0}, // Green
  {  0,   0, 255}, // Blue
  {255, 255,   0}, // Yellow
  {  0, 255, 255}, // Cyan
  {255,   0, 255}, // Magenta
  {255, 255, 255}  // White
};

constexpr uint8_t COLOR_COUNT = sizeof(COLORS) / sizeof(COLORS[0]);
constexpr unsigned long BLINK_INTERVAL_MS = 2000UL;
constexpr unsigned long COLOR_INTERVAL_MS = 2000UL;

bool statusLedOn = false;
uint8_t colorIndex = 0;
unsigned long lastBlinkTime = 0;
unsigned long lastColorTime = 0;

#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_108\\direct\\attempt_0\\sketch\\sketch.ino"
void displayColor(uint8_t index);
#line 39 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_108\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 57 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_108\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_108\\direct\\attempt_0\\sketch\\sketch.ino"
void displayColor(uint8_t index) {
  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      analogWrite(RGB_PINS[led][channel], COLORS[index][channel]);
    }
  }
}

void setup() {
  digitalWrite(STATUS_LED_PIN, LOW);
  pinMode(STATUS_LED_PIN, OUTPUT);

  for (uint8_t led = 0; led < 3; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], LOW);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  displayColor(colorIndex);

  const unsigned long startTime = millis();
  lastBlinkTime = startTime;
  lastColorTime = startTime;
}

void loop() {
  const unsigned long now = millis();

  // Independent, nonblocking status LED timing; subtraction handles rollover.
  const unsigned long blinkElapsed = now - lastBlinkTime;
  if (blinkElapsed >= BLINK_INTERVAL_MS) {
    const unsigned long intervals = blinkElapsed / BLINK_INTERVAL_MS;
    lastBlinkTime += intervals * BLINK_INTERVAL_MS;

    if (intervals & 1UL) {
      statusLedOn = !statusLedOn;
    }
    digitalWrite(STATUS_LED_PIN, statusLedOn ? HIGH : LOW);
  }

  // All three RGB LEDs advance together without blocking the status LED.
  const unsigned long colorElapsed = now - lastColorTime;
  if (colorElapsed >= COLOR_INTERVAL_MS) {
    const unsigned long intervals = colorElapsed / COLOR_INTERVAL_MS;
    lastColorTime += intervals * COLOR_INTERVAL_MS;

    colorIndex = static_cast<uint8_t>(
      (colorIndex + intervals % COLOR_COUNT) % COLOR_COUNT
    );
    displayColor(colorIndex);
  }
}
