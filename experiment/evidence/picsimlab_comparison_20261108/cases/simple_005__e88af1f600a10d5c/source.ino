#include <Arduino.h>

// Frozen interface contract
// io_0: input bool from D3 (active_high=true, pullup=false)
// io_1: output bool to D10 (active_high=true)

static const uint8_t IO0_PIN = 3;   // D3
static const uint8_t IO1_PIN = 10;  // D10

// Controller state (exactly as in accepted model)
static bool ledOn = false;            // initial false
static bool previousPressed = false; // initial false

// Control period (ms) = 5 (frozen contract)
static const uint32_t CONTROL_PERIOD_MS = 5;

static uint32_t nextTickMs = 0;

void setup() {
  pinMode(IO0_PIN, INPUT);     // pullup=false assumed
  pinMode(IO1_PIN, OUTPUT);

  digitalWrite(IO1_PIN, ledOn ? HIGH : LOW);

  nextTickMs = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) {
    return;
  }

  // Maintain fixed-rate ticks
  nextTickMs += CONTROL_PERIOD_MS;

  // Sample inputs
  bool io_0 = (digitalRead(IO0_PIN) == HIGH);

  // ACCEPTED executable model tick:
  // first start;
  // then if ((io_0) and ((not previousPressed))) { assign ledOn := (not ledOn); }
  // then assign previousPressed := io_0;
  if (io_0 && !previousPressed) {
    ledOn = !ledOn;
  }
  previousPressed = io_0;

  // Output assignment
  digitalWrite(IO1_PIN, ledOn ? HIGH : LOW);
}