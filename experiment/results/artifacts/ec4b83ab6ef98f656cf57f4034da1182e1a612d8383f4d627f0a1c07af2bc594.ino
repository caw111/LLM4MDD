#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t TRIG_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr uint8_t LED_PIN = 13;

constexpr unsigned long POLL_INTERVAL_MS = 3000UL;
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;
constexpr float DISTANCE_THRESHOLD_CM = 20.0f;
constexpr uint8_t REQUIRED_CONSECUTIVE_READINGS = 3;

DHT dht(DHT_PIN, DHT11);

unsigned long lastPollMs = 0;
uint8_t consecutiveNearReadings = 0;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (durationUs == 0) {
    return NAN;
  }

  return durationUs * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  dht.begin();
  lastPollMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastPollMs < POLL_INTERVAL_MS) {
    return;
  }
  lastPollMs = now;

  float temperatureC = dht.readTemperature();
  float distanceCm = readDistanceCm();

  Serial.print(F("Temperature: "));
  if (isnan(temperatureC)) {
    Serial.print(F("read error"));
  } else {
    Serial.print(temperatureC, 1);
    Serial.print(F(" C"));
  }

  Serial.print(F(", Distance: "));
  if (isnan(distanceCm)) {
    Serial.println(F("no echo"));
  } else {
    Serial.print(distanceCm, 1);
    Serial.println(F(" cm"));
  }

  if (!isnan(distanceCm) && distanceCm < DISTANCE_THRESHOLD_CM) {
    if (consecutiveNearReadings < REQUIRED_CONSECUTIVE_READINGS) {
      ++consecutiveNearReadings;
    }
  } else {
    consecutiveNearReadings = 0;
  }

  bool persistentObject =
      consecutiveNearReadings >= REQUIRED_CONSECUTIVE_READINGS;

  digitalWrite(LED_PIN, persistentObject ? HIGH : LOW);

  if (persistentObject) {
    Serial.println(F("persistent object detected"));
  }
}