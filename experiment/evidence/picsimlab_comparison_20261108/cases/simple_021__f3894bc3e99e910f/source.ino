#include <Arduino.h>

// Passive buzzer on D6, 1000 Hz tone.
// Pattern: initial silence; first tone after 1000 ms from startup,
// then every 1000 ms start a 100 ms tone. Start/end tolerance: 20 ms.

static const uint8_t BUZZER_PIN = 6;
static const unsigned long TONE_FREQ = 1000;      // Hz
static const unsigned long PERIOD_MS = 1000;      // every 1000 ms
static const unsigned long TONE_MS = 100;          // duration 100 ms
static const unsigned long FIRST_DELAY_MS = 1000;  // first start after 1000 ms

unsigned long startMillis;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  startMillis = millis();
}

void loop() {
  unsigned long now = millis();
  unsigned long elapsed = now - startMillis;

  unsigned long n = elapsed / PERIOD_MS;
  unsigned long phaseStart = n * PERIOD_MS;

  // Only begin beeping after FIRST_DELAY_MS
  if (elapsed < FIRST_DELAY_MS) {
    noTone(BUZZER_PIN);
    return;
  }

  unsigned long phaseElapsed = elapsed - FIRST_DELAY_MS - (n * PERIOD_MS - phaseStart);

  // Simpler: compute relative time to the first 1000ms-aligned slot.
  // Determine slot index based on (elapsed - FIRST_DELAY_MS) / PERIOD_MS
  unsigned long rel = elapsed - FIRST_DELAY_MS;
  unsigned long slot = rel / PERIOD_MS;
  unsigned long slotPhase = rel % PERIOD_MS;

  if (slotPhase < TONE_MS) {
    tone(BUZZER_PIN, TONE_FREQ);
  } else {
    noTone(BUZZER_PIN);
  }
}