#include <Arduino.h>

// Frozen hardware contract / interface:
// - io_0 is output, unit Hz, initial = 262
// - Binding: tone_output, invalid_policy="silence", pin="D2"
static constexpr uint8_t BUZZER_PIN = 2;
static constexpr int32_t IO0_INITIAL_HZ = 262;

void setup() {
  // Requirement: Set the pin mode as OUTPUT
  pinMode(BUZZER_PIN, OUTPUT);

  // Deterministic behavior: continuously play 262Hz tone after power-up
  tone(BUZZER_PIN, (unsigned int)IO0_INITIAL_HZ);
}

void loop() {
  // Continue continuous tone at 262 Hz.
  // Keep calling tone to ensure persistence across any scheduler variations.
  static uint32_t lastCallMs = 0;
  const uint32_t now = millis();
  if (now - lastCallMs >= 5) { // matches control_period_ms = 5
    lastCallMs = now;
    tone(BUZZER_PIN, (unsigned int)IO0_INITIAL_HZ);
  }
}