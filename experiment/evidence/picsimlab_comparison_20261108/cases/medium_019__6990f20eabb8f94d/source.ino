#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

#define BUTTON_PIN 2

#define TRIG_PIN 8
#define ECHO_PIN 9

// DHT
DHT dht(DHTPIN, DHTTYPE);

// Button handling (toggle display mode)
static bool lastButtonReading = HIGH;
static bool buttonStableState = HIGH;
static unsigned long lastDebounceMs = 0;
const unsigned long debounceDelayMs = 30;

static bool displayModeIsDistance = false; // false: Temperature, true: Distance
static bool lastToggleAction = false;

// Timing for periodic reads
static unsigned long lastTempReadMs = 0;
static unsigned long lastDistReadMs = 0;
const unsigned long tempReadIntervalMs = 500;
const unsigned long distReadIntervalMs = 60;

// Distance measurement parameters
// HC-SR04 typical: speed of sound ~343 m/s; for cm: distance_cm = duration_us / 58.0
static float readDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width with timeout
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~5m timeout
  if (durationUs == 0) return -1.0f;

  return (float)durationUs / 58.0f;
}

static float readTemperatureC() {
  // DHT11 read: temperature in C
  // Returns NAN if read fails
  float t = dht.readTemperature();
  return t;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // If serial is not requested, no serial output required.
  // Still keep behavior silent unless user changes requirements.
}

void loop() {
  // Read button with debounce
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceMs = millis();
    lastButtonReading = reading;
  }

  if (millis() - lastDebounceMs >= debounceDelayMs) {
    if (buttonStableState != reading) {
      buttonStableState = reading;

      // Active-low button press: HIGH->LOW transition indicates press
      if (buttonStableState == LOW && !lastToggleAction) {
        displayModeIsDistance = !displayModeIsDistance;
        lastToggleAction = true;
      }
    }
  }

  // Reset toggle latch when button released
  if (buttonStableState == HIGH) {
    lastToggleAction = false;
  }

  // Display mode: Temperature or Distance
  // (Assumes some external display exists; no display pins specified in provided config.)
  // Preserve required behavior by performing the requested reads based on mode.
  if (!displayModeIsDistance) {
    if (millis() - lastTempReadMs >= tempReadIntervalMs) {
      lastTempReadMs = millis();

      float t = readTemperatureC();
      // No explicit display/output wiring provided in requirements.
      // Intentionally left without output changes.
      (void)t;
    }
  } else {
    if (millis() - lastDistReadMs >= distReadIntervalMs) {
      lastDistReadMs = millis();

      float d = readDistanceCm();
      // No explicit display/output wiring provided in requirements.
      // Intentionally left without output changes.
      (void)d;
    }
  }
}