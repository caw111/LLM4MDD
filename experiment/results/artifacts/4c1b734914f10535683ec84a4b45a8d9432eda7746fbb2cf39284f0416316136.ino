#include <Arduino.h>

// Wiring from circuit:
// led1: cathode -> GND.1 ; anode -> Mega pin 2
const uint8_t LED_PIN = 2;

// pot1: VCC -> 5V, GND -> GND.1, SIG -> A3
const uint8_t POT_PIN = A3;

// bargraph1 segments:
// A1..A10 -> Mega pins 3..12 respectively
// C1..C10 -> GND.1
const uint8_t BAR_PINS[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

static void setBarGraph(uint8_t litSegments) {
  // litSegments: 0..10
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < litSegments) ? HIGH : LOW);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Initialization: LED and all bargraph segments off.
  digitalWrite(LED_PIN, LOW);
  setBarGraph(0);
}

void loop() {
  int potVal = analogRead(POT_PIN); // 0..1023

  // 2) Bar Graph Activation: sequential segments from 1 to 10.
  // Requirement mapping:
  // 0-102 -> 0 segments
  // 103-204 -> 1 segment (A1)
  // ...
  // 921-1023 -> 10 segments (A10)
  uint8_t litSegments;
  if (potVal <= 102) {
    litSegments = 0;
  } else {
    // For potVal in 103..1023:
    // segment = ceil((potVal - 102) / 102)
    // which yields 1..10 with the given ranges.
    litSegments = (uint8_t)((potVal - 102 + 101) / 102);
    if (litSegments > 10) litSegments = 10;
  }
  setBarGraph(litSegments);

  // 3) LED Blinking Condition:
  // When pot1 is maximum value (1023), blink at 0.25 Hz (on 2s, off 2s).
  if (potVal == 1023) {
    // 0.25 Hz => period 4 seconds. On for 2 seconds.
    unsigned long t = millis();
    bool ledOn = ((t % 4000UL) < 2000UL);
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  } else {
    // 2) LED Brightness proportional to pot value: 0=off, 1023=full brightness.
    // Using analogWrite on Mega: pins 2..13 support digital only,
    // but requirement says preserve wiring to Mega pin 2.
    // We'll approximate brightness by time-proportional control (software PWM).
    static unsigned long pwmStart = millis();
    const unsigned long pwmPeriodUs = 2000UL; // 2 ms period (~500 Hz), sufficient for perceived brightness.
    static int lastPot = -1;

    // Use potVal directly for duty cycle.
    // duty: 0..1023 mapped to 0..pwmPeriodUs
    unsigned long nowUs = (micros());
    unsigned long phaseUs = (nowUs - (pwmStart * 1000UL)) % pwmPeriodUs;

    // Scale duty from 0..1023 to 0..pwmPeriodUs
    unsigned long dutyUs = (unsigned long)((uint32_t)potVal * pwmPeriodUs / 1023UL);

    digitalWrite(LED_PIN, (phaseUs < dutyUs) ? HIGH : LOW);

    // Keep pwmStart fresh periodically to avoid overflow drift
    if (potVal != lastPot) {
      lastPot = potVal;
      pwmStart = millis();
    }
  }
}