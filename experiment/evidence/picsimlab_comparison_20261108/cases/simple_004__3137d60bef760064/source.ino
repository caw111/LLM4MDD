#include <Arduino.h>

static constexpr uint8_t PIN_ANALOG_IN = A0;
static constexpr uint8_t PIN_PWM_OUT   = 9;

static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// ACCEPTED executable model:
// out io_1 = io_0 (direct pass-through of ADC count as f64-like float).
static float io_0 = 0.0f;
static float io_1 = 0.0f;

static void apply_pwm_from_io1(float value) {
  // Contract declares io_1 unit: pwm_count with no scale/offset keys.
  // Arduino analogRead gives 0..1023; use it directly as "count" mapped to 0..255 duty.
  // This preserves a proportional behavior rather than clamping to 0..255 early.
  // If value exceeds 1023 (shouldn't happen), clamp to avoid overflow.
  long v = (long)value;
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;

  // Convert 0..1023 -> 0..255
  uint8_t duty = (uint8_t)((v * 255L) / 1023L);
  analogWrite(PIN_PWM_OUT, duty);
}

void setup() {
  pinMode(PIN_ANALOG_IN, INPUT);
  pinMode(PIN_PWM_OUT, OUTPUT);
  analogWrite(PIN_PWM_OUT, 0);

  // No serial; no other initialization required by contract.
}

void loop() {
  static uint32_t lastTick = 0;
  const uint32_t now = millis();

  if (now - lastTick < CONTROL_PERIOD_MS) {
    return;
  }
  lastTick = now;

  // Sample io_0 every 10ms
  int raw = analogRead(PIN_ANALOG_IN); // 0..1023
  io_0 = (float)raw;                    // unit: adc_count, scale=1 offset=0

  // Controller update (direct mapping)
  io_1 = io_0; // out io_1 = io_0

  // Output io_1 to PWM
  apply_pwm_from_io1(io_1);
}