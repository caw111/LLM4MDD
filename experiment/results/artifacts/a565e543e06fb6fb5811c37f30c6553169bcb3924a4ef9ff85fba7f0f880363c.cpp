#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t LIGHT_PIN = A0;
constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t LED_PIN = 13;

// Assumes the LDR divider produces lower readings in darkness.
// Adjust this threshold to suit the actual ambient light and divider.
constexpr int DARK_THRESHOLD = 400;
constexpr float ALARM_DISTANCE_CM = 20.0f;

constexpr unsigned long SENSOR_INTERVAL_MS = 60;
constexpr unsigned long BLINK_INTERVAL_MS = 250;
constexpr unsigned long SERIAL_INTERVAL_MS = 1000;
constexpr unsigned long ECHO_TIMEOUT_US = 25000;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 2000;

bool alarmActive = false;
bool alarmPhaseOn = false;

unsigned long lastSensorTime = 0;
unsigned long lastPhaseTime = 0;
unsigned long lastSerialTime = 0;

#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setAlarmOutputs(bool on);
#line 38 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
float readDistanceCm();
#line 56 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\complex_011\\direct\\attempt_0\\sketch\\sketch.ino"
void setAlarmOutputs(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);

  if (on) {
    tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  // A timeout is not a nearby object.
  if (duration == 0) {
    return -1.0f;
  }

  return duration * 0.0343f / 2.0f;
}

void setup() {
  pinMode(LIGHT_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  setAlarmOutputs(false);

  Serial.begin(9600);

  // Permit the first sensor measurement immediately.
  lastSensorTime = millis() - SENSOR_INTERVAL_MS;
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorTime >= SENSOR_INTERVAL_MS) {
    lastSensorTime = now;

    const int lightLevel = analogRead(LIGHT_PIN);
    const float distanceCm = readDistanceCm();

    const bool isDark = lightLevel < DARK_THRESHOLD;
    const bool objectNearby =
        distanceCm > 0.0f && distanceCm <= ALARM_DISTANCE_CM;
    const bool shouldAlarm = isDark && objectNearby;

    now = millis();

    if (shouldAlarm != alarmActive) {
      alarmActive = shouldAlarm;

      if (alarmActive) {
        alarmPhaseOn = true;
        lastPhaseTime = now;
        lastSerialTime = now;
        setAlarmOutputs(true);
        Serial.println(F("alarm"));
      } else {
        alarmPhaseOn = false;
        setAlarmOutputs(false);
      }
    }
  }

  now = millis();

  if (alarmActive) {
    if (now - lastPhaseTime >= BLINK_INTERVAL_MS) {
      lastPhaseTime = now;
      alarmPhaseOn = !alarmPhaseOn;
      setAlarmOutputs(alarmPhaseOn);
    }

    if (now - lastSerialTime >= SERIAL_INTERVAL_MS) {
      lastSerialTime = now;
      Serial.println(F("alarm"));
    }
  }
}
