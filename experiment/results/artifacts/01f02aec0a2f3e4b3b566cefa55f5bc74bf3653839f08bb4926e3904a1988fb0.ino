#include <Arduino.h>
#include <TM1637Display.h>

// --- Pin assignments (as provided) ---
static const uint8_t LED_PIN = 13;   // D13
static const uint8_t BTN_PIN = 2;    // D2
static const uint8_t POT_PIN = A0;   // A0

static const uint8_t TM1637_CLK = 4; // D4
static const uint8_t TM1637_DIO = 5; // D5

// --- TM1637 ---
TM1637Display display(TM1637_CLK, TM1637_DIO);

// --- Behavior requirements ---
static const uint16_t MIN_PERIOD_MS = 50;     // avoid too-fast flashing
static const uint16_t MAX_PERIOD_MS = 3000;   // reasonable upper bound
static const uint8_t  DISPLAY_DIGITS = 4;

// Button handling (start/stop toggle)
static bool running = false;
static bool lastBtnStable = HIGH;
static uint32_t lastDebounceMs = 0;
static const uint32_t DEBOUNCE_MS = 30;

// Current configured period (ms) derived from potentiometer
static uint16_t periodMs = 500;

// Blink state
static uint32_t lastToggleMs = 0;
static bool ledOn = false;

uint16_t readPeriodFromPot() {
  // Pot 0..1023 mapped to period range
  uint16_t raw = analogRead(POT_PIN);
  // Linear mapping
  uint32_t mapped = (uint32_t)raw * (MAX_PERIOD_MS - MIN_PERIOD_MS) / 1023UL + MIN_PERIOD_MS;
  return (uint16_t)mapped;
}

void showPeriodOnDisplay(uint16_t ms) {
  // Requirement: display "周期值"
  // Use ms as numeric value. Clamp to what fits in 4 digits.
  // If exceeds 9999, clamp.
  uint16_t value = ms;
  if (value > 9999) value = 9999;
  display.showNumberDecEx(value, 0b01000000, true); // leading zeros off
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BTN_PIN, INPUT_PULLUP);

  // Initialize TM1637
  display.setBrightness(0x0f, true); // max brightness

  // Initial configuration from potentiometer
  periodMs = readPeriodFromPot();
  showPeriodOnDisplay(periodMs);

  // Initialize button state
  lastBtnStable = digitalRead(BTN_PIN);
  lastDebounceMs = millis();

  // Start stopped by default (LED off but keep current period setting)
  running = false;
  ledOn = false;
  lastToggleMs = millis();
}

void loop() {
  // --- Read button with debounce; toggle running on press ---
  int btnReading = digitalRead(BTN_PIN);
  uint32_t now = millis();

  if ((uint32_t)(now - lastDebounceMs) > DEBOUNCE_MS) {
    if (btnReading != (int)lastBtnStable) {
      lastDebounceMs = now;
      lastBtnStable = (bool)btnReading;

      // Detect falling edge (HIGH->LOW) because INPUT_PULLUP: pressed == LOW
      if (lastBtnStable == LOW) {
        running = !running;

        if (!running) {
          // Requirement: stop blinking -> LED off, but keep current period setting.
          ledOn = false;
          digitalWrite(LED_PIN, LOW);
        } else {
          // When starting, reset toggle timer so it blinks immediately/cleanly
          lastToggleMs = now;
          ledOn = false;
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
  }

  // --- Potentiometer updates period setting continuously ---
  // Requirement: potentiometer controls blinking cycle; display shows period value.
  // Stop mode: LED off but keep current period setting (updated by pot).
  uint16_t newPeriod = readPeriodFromPot();
  if (newPeriod != periodMs) {
    periodMs = newPeriod;
    showPeriodOnDisplay(periodMs);

    if (running) {
      // Keep behavior responsive to new period
      // Reset toggle timing to avoid jitter from mid-cycle changes.
      lastToggleMs = now;
      ledOn = false;
      digitalWrite(LED_PIN, LOW);
    }
  }

  // --- Blinking logic when running ---
  if (running) {
    if ((uint32_t)(now - lastToggleMs) >= (uint32_t)periodMs) {
      lastToggleMs = now;
      ledOn = !ledOn;
      digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
  }

  // Loop continues
}