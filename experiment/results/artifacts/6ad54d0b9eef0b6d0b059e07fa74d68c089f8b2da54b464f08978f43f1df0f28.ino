#include <Arduino.h>

constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 3;
constexpr unsigned long STEP_DELAY_MS = 5000UL;

void setup() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  // Initial delay, or the off interval between repetitions.
  delay(STEP_DELAY_MS);

  digitalWrite(LED1_PIN, HIGH);
  delay(STEP_DELAY_MS);

  digitalWrite(LED2_PIN, HIGH);
  delay(STEP_DELAY_MS);

  // Mega 2560 pins 2 and 3 are PE4 and PE5.
  // Clear both bits in one port write to turn both LEDs off simultaneously.
  const uint8_t savedSREG = SREG;
  noInterrupts();
  PORTE &= static_cast<uint8_t>(~(_BV(PE4) | _BV(PE5)));
  SREG = savedSREG;
}