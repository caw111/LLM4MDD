#include <Arduino.h>

static const uint8_t BUZZER_PIN = 6;      // D6: passive buzzer
static const uint8_t BUTTON_PIN = 2;      // D2: INPUT_PULLUP

// Timing requirements (tolerance +/-20ms)
static const unsigned long PERIOD_MS = 3000UL;  // every 3000ms
static const unsigned long BEEP_MS   = 100UL;    // each beep duration
static const unsigned int  FREQ_HZ   = 1000U;    // 1000Hz

// State machine (pause/resume without restarting cycle)
static bool paused = false;
static bool lastBtnLevel = HIGH; // pull-up: released=HIGH, pressed=LOW

// Accumulated running time since last beep start attempt (while not paused)
static unsigned long elapsedSinceStart = 0;

// Remaining beep time when paused mid-beep (ms)
static unsigned long beepRemainingMs = 0;
static bool beeping = false;

// For keeping pause/resume timing accurate using millis()
static unsigned long lastMillis = 0;

static void startBeep() {
  // Set beep timer variables; actual tone starts immediately unless paused is asserted
  beeping = true;
  beepRemainingMs = BEEP_MS;
  if (!paused) {
    tone(BUZZER_PIN, FREQ_HZ);
  } else {
    noTone(BUZZER_PIN); // ensure immediate silence when paused
  }
}

static void stopBeep() {
  beeping = false;
  beepRemainingMs = 0;
  noTone(BUZZER_PIN);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initial running: start ticking, but keep silent.
  // elapsedSinceStart counts time toward next beep while not paused.
  noTone(BUZZER_PIN);

  lastMillis = millis();
  lastBtnLevel = digitalRead(BUTTON_PIN);

  paused = false;
  beeping = false;
  elapsedSinceStart = 0;
  beepRemainingMs = 0;
}

void loop() {
  unsigned long now = millis();
  unsigned long dt = now - lastMillis;
  lastMillis = now;

  // Read button and toggle pause on falling edge (HIGH->LOW).
  bool btnLevel = digitalRead(BUTTON_PIN);
  if (lastBtnLevel == HIGH && btnLevel == LOW) {
    paused = !paused;

    // Pause must immediately silence; resume continues remaining time.
    if (paused) {
      noTone(BUZZER_PIN);
    } else {
      // When resuming:
      // - if beeping was in progress, resume the remaining portion
      if (beeping) {
        tone(BUZZER_PIN, FREQ_HZ);
      }
      // - if not beeping, keep silent until time to beep
    }
  }
  lastBtnLevel = btnLevel;

  if (paused) {
    // While paused: do not advance elapsedSinceStart and do not consume beep time.
    return;
  }

  // If currently beeping, consume beepRemainingMs first.
  if (beeping) {
    if (dt >= beepRemainingMs) {
      // Finish beep
      stopBeep();
      // After finishing, clear elapsedSinceStart to start counting toward next beep
      elapsedSinceStart = 0;
    } else {
      beepRemainingMs -= dt;
    }
  } else {
    // Not beeping: accumulate toward next beep trigger
    elapsedSinceStart += dt;
    if (elapsedSinceStart >= PERIOD_MS) {
      startBeep();
      // Continue beeping immediately (tone already started in startBeep when not paused)
      // Any overrun beyond PERIOD_MS will be naturally tolerated within +/-20ms.
      // beepRemainingMs is exactly BEEP_MS; then loop will consume with dt.
    }
  }
}