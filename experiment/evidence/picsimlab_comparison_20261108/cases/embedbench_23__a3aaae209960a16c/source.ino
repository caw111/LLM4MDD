#include <Arduino.h>

static constexpr uint8_t PIN_POT  = A3; // io_0
static constexpr uint8_t PIN_RED  = 2;  // io_1
static constexpr uint8_t PIN_GRN  = 3;  // io_2
static constexpr uint8_t PIN_BLU  = 4;  // io_3

// Frozen contract: control_period_ms = 5
static constexpr uint32_t CONTROL_PERIOD_MS = 5;

static inline float mapAdcToF64(uint16_t adc)
{
  // Contract uses "f64 adc_count" with scale 1, offset 0.
  return static_cast<float>(adc);
}

static inline void writePwmCounts(uint8_t pin, float pwmCount)
{
  // Contract: pwm_count scaled by 255 to "0..255".
  // We clamp defensively to preserve behavior.
  int v = static_cast<int>(pwmCount + 0.5f);
  if (v < 0) v = 0;
  if (v > 255) v = 255;
  analogWrite(pin, v);
}

void setup()
{
  // PWM outputs
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GRN, OUTPUT);
  pinMode(PIN_BLU, OUTPUT);

  // Ensure known state at start (io outputs initial = 0)
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GRN, 0);
  analogWrite(PIN_BLU, 0);

  // POT input
  pinMode(PIN_POT, INPUT);

  // Let analogWrite establish PWM.
  delay(1);
}

void loop()
{
  static uint32_t lastTickMs = 0;
  uint32_t now = millis();
  if (now - lastTickMs < CONTROL_PERIOD_MS) {
    return;
  }
  lastTickMs = now;

  // io_0 input (adc_count)
  uint16_t adc = analogRead(PIN_POT);
  float io_0 = mapAdcToF64(adc); // 0..1023 as f64

  // Frozen ACCEPTED executable model expressions (exact boundary logic).
  // io_1 (Red):
  // if ( (io_0 in [128,255]) OR (io_0 in [512,639]) OR (io_0 in [640,767]) OR (io_0 in [896,1023]) ) then 255 else 0
  float io_1 =
      (((((io_0 >= 128.0f) && (io_0 <= 255.0f)) ||
         ((io_0 >= 512.0f) && (io_0 <= 639.0f))) ||
        ((io_0 >= 640.0f) && (io_0 <= 767.0f))) ||
       ((io_0 >= 896.0f) && (io_0 <= 1023.0f)))
          ? 255.0f
          : 0.0f);

  // io_2 (Green):
  // if ( (io_0 in [256,383]) OR (io_0 in [512,639]) OR (io_0 in [768,895]) OR (io_0 in [896,1023]) ) then 255 else 0
  float io_2 =
      (((((io_0 >= 256.0f) && (io_0 <= 383.0f)) ||
         ((io_0 >= 512.0f) && (io_0 <= 639.0f))) ||
        ((io_0 >= 768.0f) && (io_0 <= 895.0f))) ||
       ((io_0 >= 896.0f) && (io_0 <= 1023.0f)))
          ? 255.0f
          : 0.0f);

  // io_3 (Blue):
  // if ( (io_0 in [384,511]) OR (io_0 in [640,767]) OR (io_0 in [768,895]) OR (io_0 in [896,1023]) ) then 255 else 0
  float io_3 =
      (((((io_0 >= 384.0f) && (io_0 <= 511.0f)) ||
         ((io_0 >= 640.0f) && (io_0 <= 767.0f))) ||
        ((io_0 >= 768.0f) && (io_0 <= 895.0f))) ||
       ((io_0 >= 896.0f) && (io_0 <= 1023.0f)))
          ? 255.0f
          : 0.0f);

  // Apply outputs in the same control step (ordered updates).
  writePwmCounts(PIN_RED, io_1);  // io_1
  writePwmCounts(PIN_GRN, io_2);  // io_2
  writePwmCounts(PIN_BLU, io_3);  // io_3
}