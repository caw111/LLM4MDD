#include <Arduino.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t BUZZER_PIN = 6;

constexpr float NEAR_DISTANCE_CM = 10.0f;
constexpr unsigned long SAMPLE_INTERVAL_MS = 60;
constexpr unsigned long ECHO_TIMEOUT_US = 25000;
constexpr unsigned long STANDBY_DELAY_MS = 5000;
constexpr unsigned long LED_BLINK_INTERVAL_MS = 100;
constexpr unsigned long BEEP_ON_MS = 100;
constexpr unsigned long BEEP_OFF_MS = 100;
constexpr uint8_t BEEP_COUNT = 3;
constexpr unsigned int BEEP_FREQUENCY_HZ = 2000;

bool active = false;
bool buzzerOn = false;
unsigned long lastSampleMs = 0;
unsigned long lastNearMs = 0;
unsigned long activationMs = 0;

bool personIsNear() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // A timeout is not a valid zero-distance reading.
  if (duration == 0) {
    return false;
  }

  const float distanceCm = duration * 0.0343f / 2.0f;
  return distanceCm <= NEAR_DISTANCE_CM;
}

void enterActive(unsigned long now) {
  active = true;
  activationMs = now;
  lastNearMs = now;

  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, BEEP_FREQUENCY_HZ);
  buzzerOn = true;

  Serial.println(F("welcome"));
}

void enterStandby() {
  active = false;
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  buzzerOn = false;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  enterStandby();

  Serial.begin(9600);
  lastSampleMs = millis() - SAMPLE_INTERVAL_MS;
}

void loop() {
  unsigned long now = millis();

  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    const bool near = personIsNear();
    now = millis();

    if (near) {
      if (!active) {
        enterActive(now);
      }
      lastNearMs = now;
    }
  }

  if (!active) {
    return;
  }

  if (now - lastNearMs >= STANDBY_DELAY_MS) {
    enterStandby();
    return;
  }

  const unsigned long elapsed = now - activationMs;

  // Continue rapid LED flashing until five seconds without detection.
  digitalWrite(
      LED_PIN,
      ((elapsed / LED_BLINK_INTERVAL_MS) % 2 == 0) ? HIGH : LOW);

  // Exactly three short chirps per activation; no repeating alarm.
  const unsigned long beepPeriodMs = BEEP_ON_MS + BEEP_OFF_MS;
  const bool shouldBeep =
      elapsed < beepPeriodMs * BEEP_COUNT &&
      elapsed % beepPeriodMs < BEEP_ON_MS;

  if (shouldBeep != buzzerOn) {
    buzzerOn = shouldBeep;
    if (buzzerOn) {
      tone(BUZZER_PIN, BEEP_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}