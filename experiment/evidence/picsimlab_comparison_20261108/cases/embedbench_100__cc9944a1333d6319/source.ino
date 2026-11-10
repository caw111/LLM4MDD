#include <Arduino.h>

// Wiring for Arduino Mega 2560.
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;

constexpr uint8_t POT1_PIN = A3;
constexpr uint8_t POT2_PIN = A4;
constexpr uint8_t POT3_PIN = A5;

// Bar graph anodes A1-A10 connect to digital pins 4-13.
// All LED cathodes connect to GND (active HIGH).
constexpr uint8_t BAR_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
constexpr uint8_t SEGMENT_COUNT = sizeof(BAR_PINS) / sizeof(BAR_PINS[0]);

void setup() {
  // Preload LOW before enabling outputs to initialize every LED off.
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);

  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], LOW);
    pinMode(BAR_PINS[i], OUTPUT);
  }

  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);
}

void loop() {
  const int pot1Value = analogRead(POT1_PIN);
  const int pot2Value = analogRead(POT2_PIN);
  const int pot3Value = analogRead(POT3_PIN);

  const uint8_t brightness1 = map(pot1Value, 0, 1023, 0, 255);
  const uint8_t brightness2 = map(pot2Value, 0, 1023, 0, 255);
  const uint8_t litSegments = map(pot3Value, 0, 1023, 0, SEGMENT_COUNT);

  analogWrite(LED1_PIN, brightness1);
  analogWrite(LED2_PIN, brightness2);

  for (uint8_t i = 0; i < SEGMENT_COUNT; ++i) {
    digitalWrite(BAR_PINS[i], i < litSegments ? HIGH : LOW);
  }
}