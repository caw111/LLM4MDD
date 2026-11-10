#include <Arduino.h>

// Frozen interface contract
// io_0: f64 adc_count from A0
// io_1: bool LED full-on output (active high) on D9
// io_2: i32 buzzer frequency (Hz) on D6 via tone() when alarm; 0 means silence

static const uint8_t PIN_LED = 9;     // D9
static const uint8_t PIN_BUZZER = 6;  // D6
static const uint8_t PIN_A0 = A0;

static const uint32_t CONTROL_PERIOD_MS = 5;

static uint32_t next_tick_ms = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  // Note: tone() manages the pin internally; no pinMode required, but harmless.
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  next_tick_ms = millis();
}

void loop() {
  uint32_t now = millis();
  if ((int32_t)(now - next_tick_ms) < 0) return; // not time yet
  next_tick_ms += CONTROL_PERIOD_MS;

  // Sample io_0
  int raw = analogRead(PIN_A0);     // 0..1023 (adc_count)
  float io_0 = (float)raw;          // f64 in model semantics

  // Controller outputs (exact expressions preserved)
  bool io_1 = (io_0 > 700.0f);
  int32_t io_2 = (io_0 > 700.0f) ? 1000 : 0;

  // Drive outputs
  digitalWrite(PIN_LED, io_1 ? HIGH : LOW);

  if (io_2 == 0) {
    noTone(PIN_BUZZER); // silence
  } else {
    tone(PIN_BUZZER, (unsigned int)io_2); // frequency in Hz
    // Duration policy is not specified; keep continuous tone updated each step.
  }
}