#include <Arduino.h>

static uint32_t next_tick_ms = 0;

static int32_t compute_io_1_from_io_0(double io_0_f64) {
  // Model:
  // io_1 = if io_0 >= 1023.0 then 2000
  //        else if io_0 <= 0.0 then 0
  //        else floor((io_0 / 1023.0) * 2000.0)
  if (io_0_f64 >= 1023.0) return 2000;
  if (io_0_f64 <= 0.0) return 0;

  double scaled = (io_0_f64 / 1023.0) * 2000.0;
  long floored = (long)floor(scaled);

  // Preserve integer type semantics for Arduino UNO (int32).
  return (int32_t)floored;
}

static void apply_io_1_to_tone_pin(int32_t hz) {
  // Hardware contract: tone_output invalid_policy = silence on invalid_policy.
  // In model, io_1 is always >= 0; treat 0 as silence.
  const uint8_t pin = 6; // D6
  if (hz <= 0) {
    noTone(pin);
  } else {
    // Keep waveform default duty/etc. tone() uses square wave.
    tone(pin, (unsigned int)hz);
  }
}

void setup() {
  // Ports:
  // io_0 input analog on A0
  // io_1 tone output on D6
  pinMode(A0, INPUT);
  pinMode(6, OUTPUT);

  // Ensure deterministic initial state:
  // Model outputs io_1 based on initial io_0 reading (contract initial=0).
  // We read once and then apply.
  int raw = analogRead(A0);          // 0..1023
  double io_0 = (double)raw;         // adc_count
  int32_t io_1 = compute_io_1_from_io_0(io_0);
  apply_io_1_to_tone_pin(io_1);

  next_tick_ms = millis();
}

void loop() {
  // control_period_ms = 5
  uint32_t now = millis();
  if ((int32_t)(now - next_tick_ms) < 0) return;

  // Run one control tick per period (avoid missing ticks by stepping forward).
  // If the loop lags, catch up by advancing next_tick_ms in increments.
  do {
    next_tick_ms += 5;
  } while ((int32_t)(millis() - next_tick_ms) >= 0);

  // Sample io_0 (f64 adc_count)
  int raw = analogRead(A0);  // 0..1023
  double io_0 = (double)raw;

  // Update ordered outputs per model
  int32_t io_1 = compute_io_1_from_io_0(io_0);

  // Drive D6 tone output per model output expression
  apply_io_1_to_tone_pin(io_1);
}