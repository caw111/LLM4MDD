#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t BUZZER_PIN = 6;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 1000;

constexpr uint32_t PERIOD_MS = 3000;
constexpr uint32_t BEEP_DURATION_MS = 100;
constexpr uint32_t DEBOUNCE_MS = 8;

bool running = true;
bool buzzerOn = false;
bool firstPeriodCompleted = false;

uint32_t phaseMs = 0;
uint32_t lastUpdateMs = 0;
uint32_t buttonChangedMs = 0;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);

  lastButtonReading = digitalRead(BUTTON_PIN);
  lastUpdateMs = millis();
  buttonChangedMs = lastUpdateMs;
}

void loop() {
  const uint32_t now = millis();
  const uint32_t elapsed = now - lastUpdateMs;
  lastUpdateMs = now;

  // Advance only while running; pausing preserves both period and beep phase.
  if (running) {
    const uint32_t nextPhase = phaseMs + (elapsed % PERIOD_MS);

    if (elapsed >= PERIOD_MS || nextPhase >= PERIOD_MS) {
      firstPeriodCompleted = true;
    }

    phaseMs = nextPhase % PERIOD_MS;
  }

  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    buttonChangedMs = now;
  }

  if (reading != stableButtonState &&
      static_cast<uint32_t>(now - buttonChangedMs) >= DEBOUNCE_MS) {
    stableButtonState = reading;

    // Only a debounced falling edge toggles pause/resume.
    if (stableButtonState == LOW) {
      running = !running;
    }
  }

  const bool shouldSound =
      running && firstPeriodCompleted && phaseMs < BEEP_DURATION_MS;

  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;

    if (buzzerOn) {
      tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}