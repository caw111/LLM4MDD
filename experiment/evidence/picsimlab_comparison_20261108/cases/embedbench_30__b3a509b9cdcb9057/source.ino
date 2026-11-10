#include <Arduino.h>

// Arduino Mega 2560 wiring: bargraph A1-A10 -> digital pins 2-11.
// All bargraph cathodes are connected to GND (active-HIGH LEDs).
constexpr uint8_t LED_COUNT = 10;
constexpr uint8_t LED_PINS[LED_COUNT] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

constexpr uint8_t POT_LOWER_PIN = A3;
constexpr uint8_t POT_UPPER_PIN = A4;
constexpr uint8_t POT_FILL_PIN  = A5;

uint8_t readBound(uint8_t pin) {
  // Divide the ADC range into ten equal-width bins, indexed 0-9.
  return static_cast<uint8_t>(
      (static_cast<uint32_t>(analogRead(pin)) * LED_COUNT) / 1024UL);
}

void setup() {
  // Initialize every segment OFF before beginning display updates.
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(POT_LOWER_PIN, INPUT);
  pinMode(POT_UPPER_PIN, INPUT);
  pinMode(POT_FILL_PIN, INPUT);
}

void loop() {
  const uint8_t lowerBound = readBound(POT_LOWER_PIN);
  const uint8_t upperBound = readBound(POT_UPPER_PIN);
  const uint16_t fillReading = analogRead(POT_FILL_PIN);

  // For either bound order, filling starts at the smaller index.
  const uint8_t first = min(lowerBound, upperBound);
  const uint8_t last = max(lowerBound, upperBound);
  const uint8_t rangeLength = last - first + 1;

  // Convert the 0-100% fill setting to the nearest whole LED count.
  // ADC 0 lights none; ADC 1023 lights the entire inclusive range.
  const uint8_t litCount = static_cast<uint8_t>(
      (static_cast<uint32_t>(fillReading) * rangeLength + 511UL) / 1023UL);

  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    const bool lit = (i >= first) && (i < first + litCount);
    digitalWrite(LED_PINS[i], lit ? HIGH : LOW);
  }

  delay(10);
}