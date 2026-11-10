#include <Arduino.h>

// Arduino Uno wiring:
// A0: LDR voltage divider (default: LDR to 5V, fixed resistor to GND).
// D8: HC-SR04 Trig; D3: HC-SR04 Echo.
// D9: PWM LED output (use a suitable series resistor/driver).
// D2: Button to GND, using INPUT_PULLUP.
// D13: Forced-on mode indicator.
// All devices must share GND.

constexpr uint8_t LDR_PIN = A0;
constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 3;
constexpr uint8_t LED_PIN = 9;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t MODE_INDICATOR_PIN = 13;

// Set false if the LDR divider produces higher readings in darkness.
constexpr bool LDR_HIGHER_WHEN_BRIGHTER = true;

// Darkness is normalized so that 0 means bright and 1023 means dark.
// Separate thresholds prevent flickering near the switching point.
constexpr uint16_t DARK_ENTER_THRESHOLD = 650;
constexpr uint16_t DARK_EXIT_THRESHOLD = 550;

// Presence hysteresis: enter within 100 cm, leave beyond 120 cm.
constexpr float PRESENCE_ENTER_CM = 100.0f;
constexpr float PRESENCE_EXIT_CM = 120.0f;
constexpr float MIN_VALID_DISTANCE_CM = 2.0f;

constexpr uint8_t LED_ON_PWM = 200;  // Adjustable brightness: 1..255.
constexpr unsigned long BUTTON_DEBOUNCE_MS = 30;
constexpr unsigned long LIGHT_SAMPLE_MS = 20;
constexpr unsigned long SONAR_INTERVAL_MS = 70;
constexpr unsigned long SONAR_TIMEOUT_US = 30000UL;
constexpr unsigned long PRESENCE_STALE_MS = 300;

bool forcedOn = false;
bool isDark = false;
bool personPresent = false;

bool rawButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long buttonChangedAt = 0;

unsigned long lastLightSampleAt = 0;
unsigned long lastSonarAt = 0;
unsigned long lastValidDistanceAt = 0;
bool haveValidDistance = false;

int32_t filteredDarknessQ4 = 0;

void updateButton(unsigned long now) {
  const bool reading = digitalRead(BUTTON_PIN);

  if (reading != rawButtonState) {
    rawButtonState = reading;
    buttonChangedAt = now;
  }

  if (reading != stableButtonState &&
      now - buttonChangedAt >= BUTTON_DEBOUNCE_MS) {
    stableButtonState = reading;

    // Toggle once per debounced press, not while held or on release.
    if (stableButtonState == LOW) {
      forcedOn = !forcedOn;
    }
  }
}

void updateLight(unsigned long now) {
  if (now - lastLightSampleAt < LIGHT_SAMPLE_MS) {
    return;
  }
  lastLightSampleAt = now;

  const int raw = analogRead(LDR_PIN);
  const int darkness = LDR_HIGHER_WHEN_BRIGHTER ? 1023 - raw : raw;

  // Low-pass filter, retaining fractional precision.
  filteredDarknessQ4 +=
      ((static_cast<int32_t>(darkness) << 4) - filteredDarknessQ4) / 8;

  const int filteredDarkness = filteredDarknessQ4 >> 4;

  if (!isDark && filteredDarkness >= DARK_ENTER_THRESHOLD) {
    isDark = true;
  } else if (isDark && filteredDarkness <= DARK_EXIT_THRESHOLD) {
    isDark = false;
  }
}

void updatePresence(unsigned long now) {
  if (now - lastSonarAt >= SONAR_INTERVAL_MS) {
    lastSonarAt = now;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // A bounded timeout prevents a disconnected sensor from hanging the loop.
    const unsigned long echoDuration =
        pulseIn(ECHO_PIN, HIGH, SONAR_TIMEOUT_US);

    if (echoDuration != 0) {
      const float distanceCm = echoDuration * 0.0343f / 2.0f;

      if (distanceCm >= MIN_VALID_DISTANCE_CM && distanceCm <= 400.0f) {
        lastValidDistanceAt = millis();
        haveValidDistance = true;

        if (!personPresent && distanceCm <= PRESENCE_ENTER_CM) {
          personPresent = true;
        } else if (personPresent && distanceCm >= PRESENCE_EXIT_CM) {
          personPresent = false;
        }
      }
    }
  }

  // Brief missed echoes are tolerated; sustained failure clears presence.
  if (!haveValidDistance ||
      millis() - lastValidDistanceAt >= PRESENCE_STALE_MS) {
    personPresent = false;
  }
}

void updateOutputs() {
  const bool lightRequested = forcedOn || (isDark && personPresent);
  analogWrite(LED_PIN, lightRequested ? LED_ON_PWM : 0);
  digitalWrite(MODE_INDICATOR_PIN, forcedOn ? HIGH : LOW);
}

void setup() {
  pinMode(LDR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_INDICATOR_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  analogWrite(LED_PIN, 0);
  digitalWrite(MODE_INDICATOR_PIN, LOW);

  const int raw = analogRead(LDR_PIN);
  const int darkness = LDR_HIGHER_WHEN_BRIGHTER ? 1023 - raw : raw;
  filteredDarknessQ4 = static_cast<int32_t>(darkness) << 4;
  isDark = darkness >= DARK_ENTER_THRESHOLD;

  // Start in automatic mode. A button held at startup must be released
  // before its next press can toggle the mode.
  rawButtonState = digitalRead(BUTTON_PIN);
  stableButtonState = rawButtonState;
  buttonChangedAt = millis();

  lastLightSampleAt = millis();
  lastSonarAt = millis() - SONAR_INTERVAL_MS;
}

void loop() {
  updateButton(millis());
  updateLight(millis());
  updatePresence(millis());
  updateButton(millis());
  updateOutputs();
}