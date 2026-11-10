#include <Arduino.h>

// LED bar graph connections:
// A1..A10 are wired to Mega pins 2..11 respectively.
// For "even positions" use A2, A4, A6, A8, A10 => Mega pins 3,5,7,9,11
// For "odd positions"  use A1, A3, A5, A7, A9  => Mega pins 2,4,6,8,10

const uint8_t evenPins[5] = {3, 5, 7, 9, 11};
const uint8_t oddPins[5]  = {2, 4, 6, 8, 10};

static void setAllEvenOnOddOff(bool evenOn) {
  for (uint8_t i = 0; i < 5; i++) {
    if (evenOn) {
      digitalWrite(evenPins[i], HIGH);
      digitalWrite(oddPins[i], LOW);
    } else {
      digitalWrite(evenPins[i], LOW);
      digitalWrite(oddPins[i], HIGH);
    }
  }
}

void setup() {
  // Initialize all involved pins as outputs
  for (uint8_t i = 0; i < 5; i++) {
    pinMode(evenPins[i], OUTPUT);
    pinMode(oddPins[i], OUTPUT);
  }

  // Upon powering on: all LEDs in even positions lit (A0 A2 ....)
  // Mapped positions to available bargraph outputs A1..A10 in this circuit.
  setAllEvenOnOddOff(true);
}

void loop() {
  // After 2 seconds, switch from even to odd
  delay(2000);
  setAllEvenOnOddOff(false);

  // After another 2 seconds, switch back to even
  delay(2000);
  setAllEvenOnOddOff(true);
}