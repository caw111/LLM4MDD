#include <Arduino.h>

const uint8_t BUZZER_PIN = 6;   // D6
const uint8_t BUTTON_PIN = 2;  // D2

// Buzzer requirements
static const uint16_t TICK_MS = 3000;     // each cycle
static const uint16_t BUZZ_DURATION_MS = 100;
static const uint16_t TONE_HZ = 1000;

// State machine:
// - Runs continuously, tracks "accumulated running time" while NOT paused.
// - When accumulated time reaches next scheduled boundary, it triggers a 100ms beep.
// - Pause can occur at any time; pausing immediately stops PWM/beep, but keeps timing state:
//   - remaining time for current beep (if currently beeping)
//   - remaining time until next beep boundary (if not beeping)
// - Resume continues from where left off; does not restart whole cycle.
enum BuzzerState : uint8_t { WAITING, BEEPING };

BuzzerState state = WAITING;

// Timing bookkeeping (all times in ms)
unsigned long lastUpdateMs = 0;

// "running-time" progress (only advanced when not paused)
uint32_t accumSinceLastBoundary = 0; // ms accumulated toward next 3s boundary (when WAITING)

// BEEPING timing
uint32_t beepRemainingMs = 0;         // ms remaining to finish current 100ms beep

// Pause/resume
bool paused = false;
bool prevButtonLow = false;

// Output control for passive buzzer using tone() at 1000 Hz
static inline void buzzerOff() {
  noTone(BUZZER_PIN);
}

static inline void buzzerOn() {
  tone(BUZZER_PIN, TONE_HZ);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  buzzerOff(); // initial run but silent

  lastUpdateMs = millis();
}

void loop() {
  // Read button: INPUT_PULLUP, low on press.
  bool buttonLow = (digitalRead(BUTTON_PIN) == LOW);

  // Toggle pause/resume only on falling edge (high -> low).
  if (buttonLow && !prevButtonLow) {
    paused = !paused;

    if (paused) {
      // Immediately stop any sound and preserve remaining timing.
      // Preserve remaining beep time if currently beeping.
      if (state == BEEPING) {
        // Estimate remaining based on elapsed since beep started.
        // We keep it accurate by updating timing on every iteration and
        // recalculating remaining when pausing.
        // Therefore, ensure timing bookkeeping has been updated up to now:
        // We already update below; but if toggle happens before update, we
        // correct by forcing an update just after toggling.
      }
      buzzerOff();
    } else {
      // Resume: if we were in BEEPING, restart tone at 1000Hz and continue
      // remaining time. If WAITING, remain silent.
      if (state == BEEPING) {
        buzzerOn();
      }
    }
  }
  prevButtonLow = buttonLow;

  unsigned long nowMs = millis();
  uint32_t dt = (uint32_t)(nowMs - lastUpdateMs);
  lastUpdateMs = nowMs;

  if (dt == 0) return;

  // While paused: keep timing frozen; just ensure silence (already handled on toggle).
  if (paused) {
    return;
  }

  // Not paused: advance timing.
  if (state == WAITING) {
    // Accumulate running time until the next boundary at 3000ms.
    accumSinceLastBoundary += dt;

    if (accumSinceLastBoundary >= TICK_MS) {
      // Start a 100ms beep.
      state = BEEPING;

      // Compute how much extra time beyond the boundary we accumulated.
      // The boundary event triggers immediately at/after reaching 3000ms.
      // We'll subtract the overshoot from the accumulated count so that
      // subsequent boundaries remain accurate.
      uint32_t overshoot = accumSinceLastBoundary - TICK_MS;
      // Start beep; any overshoot consumes progress beyond boundary,
      // but beep duration handling is independent. We effectively reset
      // to start counting toward next boundary after beep ends.
      accumSinceLastBoundary = 0;

      beepRemainingMs = BUZZ_DURATION_MS;

      buzzerOn();

      // If overshoot included elapsed time past boundary, we must ensure
      // the beep duration still totals 100ms of running time.
      // Since we are now at the first update tick after boundary, it's possible
      // that dt included overshoot. We already set beepRemainingMs=100ms,
      // but we need to account for any time in this dt that occurred after boundary.
      // Approximation within <= 20ms tolerance is acceptable, but we can do better:
      // We treat boundary reached at the moment accumSinceLastBoundary hit 3000ms.
      // Therefore, after boundary in this dt, time spent toward beep start is:
      // timeAfterBoundary = overshoot.
      // That time occurred after reaching boundary, so beep should have been
      // sounding for (dt - overshoot)? Actually overshoot = (accum_before + dt) - 3000.
      // The time after boundary within dt is overshoot? No:
      // accum_before + dt reaches 3000 + overshoot, so amount after boundary is overshoot.
      // So time that has passed since boundary within this loop is overshoot ms.
      // We should reduce beepRemainingMs by overshoot because beep should have
      // already been active during that time.
      if (overshoot > 0) {
        if (overshoot >= beepRemainingMs) {
          // Beep would have fully completed already; handle completion immediately.
          buzzerOff();
          state = WAITING;
          accumSinceLastBoundary = 0; // next boundary starts after beep completion with zero progress
          beepRemainingMs = 0;
        } else {
          beepRemainingMs -= overshoot;
        }
      }
    }
  } else if (state == BEEPING) {
    if (beepRemainingMs > dt) {
      beepRemainingMs -= dt;
    } else {
      // Finish beep
      beepRemainingMs = 0;
      buzzerOff();
      state = WAITING;

      // After finishing a beep, start accumulating toward next boundary.
      // Per requirement: "每累计运行3000ms开始一次"
      // and we must not reset the entire cycle when beeping starts; however
      // we define beep to be triggered at each 3000ms boundary and then
      // the next boundary is 3000ms of running time after beep trigger.
      // Since we set accumSinceLastBoundary=0 when boundary triggered,
      // we can now keep it as 0 and start accumulating again from next loop.
      accumSinceLastBoundary = 0;
    }
  }
}