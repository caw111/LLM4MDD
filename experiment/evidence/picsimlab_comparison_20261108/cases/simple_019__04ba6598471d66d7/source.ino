#include <Arduino.h>
#include <TM1637Display.h>

// Frozen hardware contract
static const uint8_t TM1637_CLK_PIN = D4; // CLK=D4
static const uint8_t TM1637_DIO_PIN = D5; // DIO=D5

// Control period granularity: 10 ms (frozen contract)
static const uint32_t CONTROL_PERIOD_MS = 10;

// Model mapping: A0 ADC read (0..1023 linear) -> floor(ADC * 9999 / 1023)
static inline uint16_t mapAdcTo0_9999_floor(uint16_t adc0to1023) {
  // Use integer math to guarantee floor and exact contract behavior.
  // adc * 9999 / 1023, with floor from integer division.
  return (uint32_t(adc0to1023) * 9999u) / 1023u;
}

// TM1637 display "always on"
TM1637Display tm1637(T M1637_CLK_PIN, TM1637_DIO_PIN);

static uint16_t lastDisplayedValue = 0xFFFF;
static uint32_t lastUpdateMs = 0;

void setup() {
  // Ensure stable timing
  lastUpdateMs = millis();

  // Display: brightness 7, "invalid_policy=blank" in model notes,
  // but for this mapping the value is always valid (0..9999),
  // and display must remain enabled.
  tm1637.setBrightness(7);

  // Initialize with mapped value from current ADC reading
  uint16_t adc = analogRead(A0); // 0..1023
  uint16_t mapped = mapAdcTo0_9999_floor(adc);
  lastDisplayedValue = mapped;

  // TM1637 displays 4 digits; TM1637Display::showNumberDecEx supports leading zeros.
  // Freeze contract: leading_zero=true. We'll show leading zeros.
  // Range 0..9999 inclusive.
  tm1637.showNumberDecEx(mapped, 0b01000000, true); 
  // Explanation:
  // - The library uses a "colon" bitmask and digit control; passing flags can vary by version.
  // - The last argument 'true' requests leading zeros in TM1637Display.
  // If your library version differs, this line may need adjustment.
  // Behavior goal: always show full decimal value (0..9999) continuously.
}

void loop() {
  uint32_t now = millis();
  // Enforce 10 ms control period to meet timing constraints deterministically.
  if (now - lastUpdateMs < CONTROL_PERIOD_MS) return;
  lastUpdateMs += CONTROL_PERIOD_MS;

  // Sample analog input (A0). Model treats io_0 as Real ADC count.
  uint16_t adc = analogRead(A0); // 0..1023 (linear)

  // Model output: io_1 = floor(io_0 * 9999 / 1023)
  uint16_t mapped = mapAdcTo0_9999_floor(adc);

  // Update output (TM1637) each control period. If value unchanged, still
  // showing again is unnecessary; but not harmful. Keep deterministic ordered update:
  // update only if changed to reduce bus time (still within <=50ms after changes).
  if (mapped != lastDisplayedValue) {
    lastDisplayedValue = mapped;

    // Display full decimal number 0..9999, leading zeros allowed.
    // Always keep display on.
    tm1637.showNumberDecEx(mapped, 0b01000000, true);
  }
}