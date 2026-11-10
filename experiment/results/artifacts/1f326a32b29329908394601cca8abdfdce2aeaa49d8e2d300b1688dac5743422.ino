#include <Arduino.h>

static const uint8_t BUZZER_PIN = 6;          // D6
static const uint32_t CONTROL_PERIOD_MS = 10; // Frozen contract

// Interface io_0: input i32, unit ms, delta_since_previous_step
// Interface io_1: output i32, unit Hz (1000Hz during beep, else 0)
static volatile int32_t io_0 = 0;
static volatile int32_t io_1 = 0;

// GeneratedModel::Controller internal attributes
static int32_t component_beep_phase = 0;
static bool component_beep_started = false;
static bool component_beep_enabled = false;
static int32_t component_beep_remainder = 0;
static bool component_beep_value = false;

// Timing for io_0: delta_since_previous_step
static unsigned long lastStepMs = 0;

// Tick implementation faithful to the provided GeneratedModel (including the exact
// arithmetic structure and guard ordering).
static void controller_tick(int32_t delta_ms)
{
  // perform action tick
  // in the model: first start;
  // then if component_beep_enabled and io_0 > 0 { ... }
  if (component_beep_enabled && delta_ms > 0)
  {
    // if component_beep_enabled and io_0 >= 2000 - component_beep_phase { component_beep_started := true; }
    if (delta_ms >= 2000 - component_beep_phase)
    {
      component_beep_started = true;
    }

    component_beep_remainder = delta_ms;

    // The following is a direct transcription of the model's repeated subtractions.
    if (component_beep_remainder >= 2097152000) { component_beep_remainder = component_beep_remainder - 2097152000; }
    if (component_beep_remainder >= 1048576000) { component_beep_remainder = component_beep_remainder - 1048576000; }
    if (component_beep_remainder >= 524288000)  { component_beep_remainder = component_beep_remainder - 524288000; }
    if (component_beep_remainder >= 262144000)  { component_beep_remainder = component_beep_remainder - 262144000; }
    if (component_beep_remainder >= 131072000)  { component_beep_remainder = component_beep_remainder - 131072000; }
    if (component_beep_remainder >= 65536000)   { component_beep_remainder = component_beep_remainder - 65536000; }
    if (component_beep_remainder >= 32768000)   { component_beep_remainder = component_beep_remainder - 32768000; }
    if (component_beep_remainder >= 16384000)   { component_beep_remainder = component_beep_remainder - 16384000; }
    if (component_beep_remainder >= 8192000)    { component_beep_remainder = component_beep_remainder - 8192000; }
    if (component_beep_remainder >= 4096000)    { component_beep_remainder = component_beep_remainder - 4096000; }
    if (component_beep_remainder >= 2048000)    { component_beep_remainder = component_beep_remainder - 2048000; }
    if (component_beep_remainder >= 1024000)    { component_beep_remainder = component_beep_remainder - 1024000; }
    if (component_beep_remainder >= 512000)     { component_beep_remainder = component_beep_remainder - 512000; }
    if (component_beep_remainder >= 256000)     { component_beep_remainder = component_beep_remainder - 256000; }
    if (component_beep_remainder >= 128000)     { component_beep_remainder = component_beep_remainder - 128000; }
    if (component_beep_remainder >= 64000)      { component_beep_remainder = component_beep_remainder - 64000; }
    if (component_beep_remainder >= 32000)      { component_beep_remainder = component_beep_remainder - 32000; }
    if (component_beep_remainder >= 16000)      { component_beep_remainder = component_beep_remainder - 16000; }
    if (component_beep_remainder >= 8000)       { component_beep_remainder = component_beep_remainder - 8000; }
    if (component_beep_remainder >= 4000)       { component_beep_remainder = component_beep_remainder - 4000; }
    if (component_beep_remainder >= 2000)       { component_beep_remainder = component_beep_remainder - 2000; }

    if (component_beep_remainder >= 2000 - component_beep_phase)
    {
      component_beep_started = component_beep_started; // no-op to preserve structure (model has a "first start" here)
      component_beep_phase = component_beep_remainder - (2000 - component_beep_phase);
    }
    else
    {
      component_beep_phase = component_beep_phase + component_beep_remainder;
    }
  }

  component_beep_value = (true) && component_beep_started && (component_beep_phase < 200);
  component_beep_enabled = true;

  // out attribute io_1 : Integer = (if component_beep_value ? 1000 else 0);
  io_1 = component_beep_value ? 1000 : 0;
}

static void apply_io_1_to_hardware(int32_t hz)
{
  // Hardware adapter replaced by direct code-gen for tone_output pin D6:
  // invalid_policy="silence" -> tone off when hz==0
  if (hz == 0)
  {
    noTone(BUZZER_PIN);
  }
  else
  {
    // tone() uses frequency in Hz
    tone(BUZZER_PIN, (unsigned int)hz);
  }
}

void setup()
{
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  component_beep_phase = 0;
  component_beep_started = false;
  component_beep_enabled = false;
  component_beep_remainder = 0;
  component_beep_value = false;

  io_0 = 0;
  io_1 = 0;

  noTone(BUZZER_PIN);

  lastStepMs = millis();
}

void loop()
{
  unsigned long now = millis();
  unsigned long elapsed = now - lastStepMs;

  // Maintain control period contract: 10ms
  if (elapsed < CONTROL_PERIOD_MS)
  {
    delay(CONTROL_PERIOD_MS - elapsed);
    now = millis();
    elapsed = now - lastStepMs;
  }

  lastStepMs = now;

  // io_0 = delta_since_previous_step (i32 ms)
  // Clamp to non-negative; for AVR millis() overflow wrap, elapsed is unsigned so still valid.
  int32_t delta_ms = (int32_t)elapsed;
  if (delta_ms < 0) delta_ms = 0;
  io_0 = delta_ms;

  // Model tick
  controller_tick(io_0);

  // Hardware output update for io_1
  apply_io_1_to_hardware(io_1);

  // preserve ordered updates: model then outputs (already done)
}