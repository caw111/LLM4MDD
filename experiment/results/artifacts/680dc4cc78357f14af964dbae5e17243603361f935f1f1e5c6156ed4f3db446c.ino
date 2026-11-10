#include <Arduino.h>
#include <TM1637Display.h>

// Arduino UNO pin assignments (numeric, to avoid D4/D5/etc compile issues)
// CLK=D4, DIO=D5, buttons D2/D3, buzzer D6 (remapped as specified)
static const uint8_t PIN_TM1637_CLK = 4; // D4
static const uint8_t PIN_TM1637_DIO = 5; // D5
static const uint8_t PIN_BTN_START_PAUSE = 2; // D2
static const uint8_t PIN_BTN_RESET = 3;       // D3
static const uint8_t PIN_BUZZER = 6;          // D6

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// Button debounce
static const unsigned long DEBOUNCE_MS = 30;

// Raw/stable states (active-low buttons using INPUT_PULLUP)
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

// Beep behavior: beep 1s when reaching each 60-second boundary, then continue counting
static bool beeping = false;
static unsigned long beepStartMs = 0;
static bool beepArmedForCurrentMinute = true; // armed at boundary 0/initial

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

static inline void showSeconds(uint32_t secondsTotal) {
  // 2-digit display: show seconds modulo 60 (continues counting beyond 60)
  uint8_t secondsShown = (uint8_t)(secondsTotal % 60UL);
  display.showNumberDec(secondsShown, false, 2);
}

void setup() {
  pinMode(PIN_BTN_START_PAUSE, INPUT_PULLUP);
  pinMode(PIN_BTN_RESET, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // TM1637 initialization
  display.setBrightness(0x0f);
  display.clear();

  // Initialize button stable states
  lastStartPauseRaw = digitalRead(PIN_BTN_START_PAUSE);
  lastResetRaw = digitalRead(PIN_BTN_RESET);
  startPauseStable = lastStartPauseRaw;
  resetStable = lastResetRaw;

  lastTickMs = millis();

  // Initial display
  showSeconds(elapsedMs / 1000UL);
}

void loop() {
  updateDebouncedButtons();

  // Reset handling (D3): clears timer and stops beep
  if (resetWasPressed) {
    resetWasPressed = false;

    running = false;
    elapsedMs = 0;
    lastTickMs = millis();

    stopBeep();
    beepArmedForCurrentMinute = true;

    display.showNumberDec(0, false, 2);
  }

  // Start/Pause handling (D2): toggle running
  if (startPauseWasPressed) {
    startPauseWasPressed = false;

    running = !running;
    if (running) {
      lastTickMs = millis();
    } else {
      lastTickMs = millis(); // keep elapsedMs as-is
    }
  }

  unsigned long now = millis();

  // Beep timing: beep for 1 second
  if (beeping) {
    if (now - beepStartMs >= 1000UL) {
      stopBeep();
    }
  }

  // Timer update + display
  if (running) {
    unsigned long dt = now - lastTickMs;
    lastTickMs = now;
    elapsedMs += dt;

    uint32_t secondsTotal = elapsedMs / 1000UL;
    showSeconds(secondsTotal);

    // Trigger beep when reaching each 60-second boundary (once per boundary)
    if (secondsTotal > 0) {
      bool atBoundary = (secondsTotal % 60UL) == 0;

      if (atBoundary && beepArmedForCurrentMinute) {
        startBeep();
        beepArmedForCurrentMinute = false;
      }

      if (!atBoundary && !beepArmedForCurrentMinute) {
        beepArmedForCurrentMinute = true;
      }
    } else {
      beepArmedForCurrentMinute = true;
    }
  } else {
    // Paused: keep showing current seconds (mod 60)
    uint32_t secondsTotal = elapsedMs / 1000UL;
    showSeconds(secondsTotal);
  }
}