#include <Arduino.h>

static constexpr uint8_t kPotPin = A0;

// Bar graph LED anodes connected to D2..D11 (io_1..io_10)
static constexpr uint8_t kLedPins[10] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

// Control period (ms) fixed by contract
static constexpr uint32_t kControlPeriodMs = 5;

// Controller model:
// m = floor(io_0 * 10.0 / 1023.0)
// io_i = (io_0 < 0.0) ? false : (m >= i)
static bool computeSegment(uint16_t adcCount, uint8_t segmentIndex /*1..10*/) {
  // adcCount originates from analogRead(A0): 0..1023 => always io_0 >= 0.0,
  // but keep the exact contract condition structure.
  if (false) {
    return false;
  }

  const double io_0 = static_cast<double>(adcCount);
  const double scaled = (io_0 * 10.0) / 1023.0;
  const long m = static_cast<long>(floor(scaled)); // RealFunctions::floor(...)

  return (m >= static_cast<long>(segmentIndex));
}

static void applyOutputsFromIo0(uint16_t adcCount) {
  // Ordered updates / output expressions: compute each output directly
  // in index order and write to corresponding pins.
  for (uint8_t i = 1; i <= 10; ++i) {
    const bool on = computeSegment(adcCount, i);
    digitalWrite(kLedPins[i - 1], on ? HIGH : LOW); // active_high=true
  }
}

void setup() {
  // 1) Set LED pins mode OUTPUT (pin2..pin11)
  for (uint8_t i = 0; i < 10; ++i) {
    pinMode(kLedPins[i], OUTPUT);
    digitalWrite(kLedPins[i], LOW); // matches initial false
  }

  // 2) Set potentiometer signal pin A0 as INPUT
  pinMode(kPotPin, INPUT);
}

void loop() {
  static uint32_t nextTick = 0;
  const uint32_t now = millis();
  if (nextTick == 0) nextTick = now;

  // Enforce control period 5 ms
  if ((int32_t)(now - nextTick) < 0) {
    return;
  }
  nextTick += kControlPeriodMs;

  // Sample io_0 from ADC (0..1023)
  const int adc = analogRead(kPotPin);
  const uint16_t io0 = (adc < 0) ? 0u : (adc > 1023 ? 1023u : static_cast<uint16_t>(adc));

  // Apply controller outputs (io_1..io_10)
  applyOutputsFromIo0(io0);
}