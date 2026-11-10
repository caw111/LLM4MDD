#include <Arduino.h>
#include <TM1637Display.h>

// Pin assignments (as specified)
static const uint8_t PIN_TM1637_CLK = D4; // CLK=D4
static const uint8_t PIN_TM1637_DIO = D5; // DIO=D5
static const uint8_t PIN_BTN_START_PAUSE = D2; // D2
static const uint8_t PIN_BTN_RESET = D3;       // D3
static const uint8_t PIN_BUZZER = D6;          // D6

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Button handling
static const unsigned long DEBOUNCE_MS = 30;

static bool lastStartPauseRaw = HIGH;
static bool lastResetRaw = HIGH;
static bool startPauseStable = HIGH;
static bool resetStable = HIGH;

static unsigned long lastStartPauseChangeMs = 0;
static unsigned long lastResetChangeMs = 0;

static bool startPauseWasPressed = false;
static bool resetWasPressed = false;

// Timer state
static bool running = false;
static unsigned long elapsedMs = 0;
static unsigned long lastTickMs = 0;

// Beep behavior
static bool beeping = false;
static unsigned long beepStartMs = 0;
static bool beepArmedForCurrentMinute = true; // after reset, will beep at 60s point

static inline void updateDebouncedButtons() {
  // Read raw
  bool spRaw = digitalRead(PIN_BTN_START_PAUSE);
  bool rRaw  = digitalRead(PIN_BTN_RESET);

  unsigned long now = millis();

  // Debounce Start/Pause (D2)
  if (spRaw != lastStartPauseRaw) {
    lastStartPauseRaw = spRaw;
    lastStartPauseChangeMs = now;
  }
  if ((now - lastStartPauseChangeMs) >= DEBOUNCE_MS) {
    if (spRaw != startPauseStable) {
      startPauseStable = spRaw;
      if (startPauseStable == LOW) {
        startPauseWasPressed = true; // active-low press
      }
    }
  }

  // Debounce Reset (D3)
  if (rRaw != lastResetRaw) {
    lastResetRaw = rRaw;
    lastResetChangeMs = now;
  }
  if ((now - lastResetChangeMs) >= DEBOUNCE_MS) {
    if (rRaw != resetStable) {
      resetStable = rRaw;
      if (resetStable == LOW) {
        resetWasPressed = true; // active-low press
      }
    }
  }
}

static inline void startBeep() {
  beeping = true;
  beepStartMs = millis();
  digitalWrite(PIN_BUZZER, HIGH); // tone_output behavior: simple on for 1s
}

static inline void stopBeep() {
  beeping = false;
  digitalWrite(PIN_BUZZER, LOW);
}

void setup() {
  pinMode(PIN_BTN_START_PAUSE, INPUT_PULLUP);
  pinMode(PIN_BTN_RESET, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // TM1637 initialization (brightness default; auto mode)
  display.setBrightness(0x0f);
  display.clear();

  // Initialize button stable states
  lastStartPauseRaw = digitalRead(PIN_BTN_START_PAUSE);
  lastResetRaw = digitalRead(PIN_BTN_RESET);
  startPauseStable = lastStartPauseRaw;
  resetStable = lastResetRaw;

  lastTickMs = millis();
}

void loop() {
  updateDebouncedButtons();

  // Handle Reset (D3): clears timer and stops beep
  if (resetWasPressed) {
    resetWasPressed = false;

    running = false;
    elapsedMs = 0;
    lastTickMs = millis();

    // Reset beep state and arm for next 60s crossing
    stopBeep();
    beepArmedForCurrentMinute = true;

    // Update display immediately
    display.showNumberDec(0, false, 2);
  }

  // Handle Start/Pause (D2)
  if (startPauseWasPressed) {
    startPauseWasPressed = false;

    // Toggle running
    running = !running;

    // When starting/resuming, set tick reference to avoid jump
    if (running) {
      lastTickMs = millis();
    } else {
      // keep elapsedMs as-is
      lastTickMs = millis();
    }
  }

  unsigned long now = millis();

  // Beep timing: beep for 1 second when triggered
  if (beeping) {
    if (now - beepStartMs >= 1000UL) {
      stopBeep();
    }
  }

  // Timer update
  if (running) {
    unsigned long dt = now - lastTickMs;
    lastTickMs = now;
    elapsedMs += dt;

    // Display seconds (0..59) on demand; at 60+ continues counting display in seconds modulo 100?
    // Requirement: "数码管显示秒数，到达 60 秒时蜂鸣器鸣叫 1 秒，并继续计时。"
    // Commonly display shows current second count; since a 2-digit display can only show 0..59,
    // we show seconds modulo 60 as elapsed time continues.
    uint32_t secondsTotal = elapsedMs / 1000UL;
    uint8_t secondsShown = (uint8_t)(secondsTotal % 60UL);

    display.showNumberDec(secondsShown, false, 2);

    // Trigger beep when reaching each 60-second boundary
    // i.e., when secondsTotal is exactly multiple of 60 and beep hasn't been triggered for this boundary.
    if (secondsTotal > 0) {
      // Determine if we are at a 60s boundary for the first time since last arming.
      // Using boundary detection based on elapsedMs to preserve exact behavior.
      uint32_t boundary = (secondsTotal / 60UL); // counts completed 60-second intervals
      // We arm per boundary interval; when at new boundary and not yet armed, beep.
      // To avoid repeated triggering within the same second boundary, arm resets after beep triggers.
      // The simplest robust approach: beep when secondsTotal % 60 == 0 (including immediately after crossing),
      // but only once per boundary crossing.
      bool atBoundary = (secondsTotal % 60UL) == 0;

      if (atBoundary && beepArmedForCurrentMinute) {
        startBeep();
        beepArmedForCurrentMinute = false; // disarm until next second leaves boundary
      }

      // Re-arm once we move away from the exact boundary second
      if (!atBoundary && !beepArmedForCurrentMinute) {
        beepArmedForCurrentMinute = true;
      }
    } else {
      // At zero, ensure arming is true
      beepArmedForCurrentMinute = true;
    }
  } else {
    // If paused, keep showing current seconds (mod 60) continuously
    uint32_t secondsTotal = elapsedMs / 1000UL;
    uint8_t secondsShown = (uint8_t)(secondsTotal % 60UL);
    display.showNumberDec(secondsShown, false, 2);
  }
}