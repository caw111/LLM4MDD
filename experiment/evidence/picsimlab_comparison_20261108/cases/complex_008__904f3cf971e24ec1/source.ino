#include <Arduino.h>
#include <TM1637Display.h>

constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;
constexpr uint8_t LED_PIN = 13;
constexpr uint8_t BUZZER_PIN = 6;

constexpr unsigned long MEASUREMENT_INTERVAL_MS = 60;
constexpr unsigned long ECHO_TIMEOUT_US = 30000;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 2000;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum AlarmMode : uint8_t {
  ALARM_OFF,
  ALARM_SLOW,
  ALARM_FAST
};

AlarmMode alarmMode = ALARM_OFF;
bool buzzerOn = false;
unsigned long buzzerPhaseStartedMs = 0;
unsigned long lastMeasurementMs = 0;

void showNoReading() {
  const uint8_t dashes[] = {SEG_G, SEG_G, SEG_G, SEG_G};
  display.setSegments(dashes);
}

void setBuzzer(bool enabled) {
  if (enabled == buzzerOn) {
    return;
  }

  buzzerOn = enabled;
  if (enabled) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setAlarmMode(AlarmMode newMode, unsigned long now) {
  if (newMode == alarmMode) {
    return;
  }

  alarmMode = newMode;
  buzzerPhaseStartedMs = now;
  setBuzzer(alarmMode != ALARM_OFF);
}

void updateBuzzer(unsigned long now) {
  if (alarmMode == ALARM_OFF) {
    setBuzzer(false);
    return;
  }

  const unsigned long onTimeMs =
      (alarmMode == ALARM_FAST) ? 80UL : 150UL;
  const unsigned long offTimeMs =
      (alarmMode == ALARM_FAST) ? 120UL : 850UL;
  const unsigned long phaseDurationMs = buzzerOn ? onTimeMs : offTimeMs;

  if (now - buzzerPhaseStartedMs >= phaseDurationMs) {
    buzzerPhaseStartedMs = now;
    setBuzzer(!buzzerOn);
  }
}

void measureAndDisplayDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long echoDurationUs =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  const unsigned long now = millis();

  if (echoDurationUs == 0) {
    showNoReading();
    digitalWrite(LED_PIN, LOW);
    setAlarmMode(ALARM_OFF, now);
    return;
  }

  const float distanceCm = echoDurationUs * 0.0343f / 2.0f;
  const int displayedDistance = static_cast<int>(distanceCm + 0.5f);
  display.showNumberDec(displayedDistance, false);

  digitalWrite(LED_PIN, distanceCm < 30.0f ? HIGH : LOW);

  if (distanceCm < 8.0f) {
    setAlarmMode(ALARM_FAST, now);
  } else if (distanceCm < 15.0f) {
    setAlarmMode(ALARM_SLOW, now);
  } else {
    setAlarmMode(ALARM_OFF, now);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);

  display.setBrightness(7, true);
  showNoReading();

  lastMeasurementMs = millis() - MEASUREMENT_INTERVAL_MS;
}

void loop() {
  unsigned long now = millis();
  updateBuzzer(now);

  if (now - lastMeasurementMs >= MEASUREMENT_INTERVAL_MS) {
    lastMeasurementMs = now;
    measureAndDisplayDistance();
  }

  updateBuzzer(millis());
}