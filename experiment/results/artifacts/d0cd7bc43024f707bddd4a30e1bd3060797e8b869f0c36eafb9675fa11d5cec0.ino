#include <Arduino.h>

// Wiring from the provided circuit
// LEDs anodes to Mega pins 2, 3, 4; cathodes to GND
static const uint8_t LED1_PIN = 2;
static const uint8_t LED2_PIN = 3;
static const uint8_t LED3_PIN = 4;

// Bargraph segment anodes A1..A10 to Mega pins 5..13
static const uint8_t BAR_PINS[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

static const unsigned long STEP_MS = 2000;

enum Phase : uint8_t {
  PH_LED1 = 0,
  PH_LED2 = 1,
  PH_LED3 = 2,
  PH_ALL  = 3
};

static void setAllOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
}

static void setBarSegments(uint8_t litCount) {
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < litCount) ? HIGH : LOW);
  }
}

void setup() {
  // Ensure everything is off at start/reset
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  setAllOff();
}

void loop() {
  static Phase phase = PH_LED1;
  static unsigned long phaseStart = millis();

  unsigned long now = millis();
  if (now - phaseStart >= STEP_MS) {
    phase = static_cast<Phase>((static_cast<uint8_t>(phase) + 1) % 4);
    phaseStart = now;
  }

  // Drive outputs according to current phase.
  // Requirement: bargraph displays number of active LEDs, and final phase lights all 10 for 2s.
  setAllOff();

  switch (phase) {
    case PH_LED1: {
      digitalWrite(LED1_PIN, HIGH);
      setBarSegments(1);
    } break;

    case PH_LED2: {
      digitalWrite(LED2_PIN, HIGH);
      setBarSegments(1);
    } break;

    case PH_LED3: {
      digitalWrite(LED3_PIN, HIGH);
      setBarSegments(1);
    } break;

    case PH_ALL: {
      setBarSegments(10);
    } break;

    default:
      break;
  }
}