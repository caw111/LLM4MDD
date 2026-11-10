#include <Arduino.h>

// Bargraph1 LEDs:
// A1..A10 are connected to Mega pins 2..11 respectively.
// Bargraph1 C1..C10 are all connected to Mega GND.1 (common cathode).

static const uint8_t LED_PINS[10] = {2,3,4,5,6,7,8,9,10,11}; // A1..A10

static inline void setAllOff() {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(LED_PINS[i], LOW);
  }
}

static inline void setEvenOdd(bool evenOn) {
  // Even positions: A0 A2 .... (as written in task)
  // Bargraph is wired as A1..A10; interpret "even positions" as indices with even number:
  // - Even position numbers => 2,4,6,8,10 => indices 1,3,5,7,9
  // - Odd position numbers  => 1,3,5,7,9    => indices 0,2,4,6,8
  for (uint8_t idx = 0; idx < 10; idx++) {
    uint8_t positionNumber = idx + 1; // 1..10 corresponding to A1..A10
    bool isEvenPosition = (positionNumber % 2 == 0);
    digitalWrite(LED_PINS[idx], (evenOn == isEvenPosition) ? HIGH : LOW);
  }
}

void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Initialization: all LEDs in even positions lit (A0 A2 ....)
  // Implemented for wired positions A1..A10 as even-numbered positions.
  setEvenOdd(true);
}

void loop() {
  static bool evenPhase = true;
  static unsigned long lastSwitchMs = millis();

  unsigned long now = millis();
  if (now - lastSwitchMs >= 2000UL) {
    evenPhase = !evenPhase; // toggle between even and odd every 2 seconds
    setAllOff();
    setEvenOdd(evenPhase);
    lastSwitchMs = now;
  }
}