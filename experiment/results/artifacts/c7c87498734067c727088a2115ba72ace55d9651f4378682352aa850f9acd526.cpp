#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Arduino Mega 2560: preserve the supplied circuit wiring.
constexpr uint8_t RGB_PINS[2][3] = {
  {2, 3, 4},  // rgb1: red, green, blue
  {5, 6, 7}   // rgb2: red, green, blue
};

constexpr uint8_t DATA_PIN  = A0;
constexpr uint8_t LATCH_PIN = A1;
constexpr uint8_t CLOCK_PIN = A2;

constexpr unsigned long UPDATE_INTERVAL_MS = 2000UL;

// Common-cathode RGB LEDs: HIGH turns a color channel on.
constexpr uint8_t COLORS[7][3] = {
  {HIGH, LOW,  LOW },  // Red
  {LOW,  HIGH, LOW },  // Green
  {LOW,  LOW,  HIGH},  // Blue
  {HIGH, HIGH, LOW },  // Yellow
  {LOW,  HIGH, HIGH},  // Cyan
  {HIGH, LOW,  HIGH},  // Magenta
  {HIGH, HIGH, HIGH}   // White
};

// Segment bits: bit 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G.
// Display commons are wired to +5 V, so segment outputs are active LOW.
// Q7 is unused and remains HIGH.
constexpr uint8_t DIGIT_PATTERNS[10] = {
  0xC0,  // 0
  0xF9,  // 1
  0xA4,  // 2
  0xB0,  // 3
  0x99,  // 4
  0x92,  // 5
  0x82,  // 6
  0xF8,  // 7
  0x80,  // 8
  0x90   // 9
};

uint8_t colorIndex = 0;
uint8_t displayedNumber = 0;
unsigned long lastUpdateMs = 0;

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBColor(uint8_t index);
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
void showNumber(uint8_t number);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\embedbench_56\\direct\\attempt_0\\sketch\\sketch.ino"
void setRGBColor(uint8_t index) {
  for (uint8_t led = 0; led < 2; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], COLORS[index][channel]);
    }
  }
}

void showNumber(uint8_t number) {
  const uint8_t tens = number / 10;
  const uint8_t units = number % 10;

  digitalWrite(LATCH_PIN, LOW);

  // sr2 is downstream: send its units byte first, then sr1's tens byte.
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[units]);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, DIGIT_PATTERNS[tens]);

  // Both digits update together on the latch's rising edge.
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  digitalWrite(DATA_PIN, LOW);
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(LATCH_PIN, LOW);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  for (uint8_t led = 0; led < 2; ++led) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      digitalWrite(RGB_PINS[led][channel], COLORS[0][channel]);
      pinMode(RGB_PINS[led][channel], OUTPUT);
    }
  }

  setRGBColor(colorIndex);
  showNumber(displayedNumber);
  lastUpdateMs = millis();
}

void loop() {
  const unsigned long now = millis();
  const unsigned long elapsed = now - lastUpdateMs;

  if (elapsed >= UPDATE_INTERVAL_MS) {
    // Keep both independent sequences on the same drift-free timebase.
    // Account for missed intervals without blocking or replaying stale states.
    const unsigned long steps = elapsed / UPDATE_INTERVAL_MS;
    lastUpdateMs += steps * UPDATE_INTERVAL_MS;

    colorIndex = (colorIndex + (steps % 7UL)) % 7;
    displayedNumber = (displayedNumber + (steps % 100UL)) % 100;

    setRGBColor(colorIndex);
    showNumber(displayedNumber);
  }
}
