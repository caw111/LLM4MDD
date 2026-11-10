#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t BUTTON_PIN = 2;  // Button connects D2 to GND.
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

constexpr uint16_t AUTO_THRESHOLD_CM = 15;
constexpr uint16_t MANUAL_MIN_CM = 2;
constexpr uint16_t MANUAL_MAX_CM = 400;
constexpr uint16_t ALARM_FREQUENCY_HZ = 2000;

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long MEASUREMENT_INTERVAL_MS = 60;
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

bool automaticMode = true;
bool alarmActive = false;
bool distanceValid = false;
float distanceCm = 0.0f;

uint16_t thresholdCm = AUTO_THRESHOLD_CM;
uint16_t displayedThreshold = 0;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;
unsigned long lastButtonChangeMs = 0;
unsigned long lastMeasurementMs = 0;

void updateButton(unsigned long now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastButtonChangeMs = now;
  }

  if (reading != stableButtonState &&
      now - lastButtonChangeMs >= DEBOUNCE_MS) {
    stableButtonState = reading;

    if (stableButtonState == LOW) {
      automaticMode = !automaticMode;
    }
  }
}

void updateThreshold() {
  if (automaticMode) {
    thresholdCm = AUTO_THRESHOLD_CM;
  } else {
    const int potentiometerValue = analogRead(POT_PIN);
    thresholdCm = static_cast<uint16_t>(
        map(potentiometerValue, 0, 1023, MANUAL_MIN_CM, MANUAL_MAX_CM));
  }

  if (thresholdCm != displayedThreshold) {
    display.showNumberDec(thresholdCm, false);
    displayedThreshold = thresholdCm;
  }
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long durationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (durationUs == 0) {
    distanceValid = false;
    return;
  }

  distanceCm = durationUs * 0.0343f / 2.0f;
  distanceValid = true;
}

void updateAlarm() {
  const bool shouldAlarm =
      distanceValid && distanceCm <= static_cast<float>(thresholdCm);

  if (shouldAlarm == alarmActive) {
    return;
  }

  alarmActive = shouldAlarm;
  if (alarmActive) {
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(POT_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.setBrightness(5, true);
  display.clear();
  updateThreshold();

  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;
  lastButtonChangeMs = millis();

  // Schedule the first measurement immediately.
  lastMeasurementMs = millis() - MEASUREMENT_INTERVAL_MS;
}

void loop() {
  const unsigned long now = millis();

  updateButton(now);
  updateThreshold();

  if (now - lastMeasurementMs >= MEASUREMENT_INTERVAL_MS) {
    lastMeasurementMs = now;
    measureDistance();
  }

  updateAlarm();
}