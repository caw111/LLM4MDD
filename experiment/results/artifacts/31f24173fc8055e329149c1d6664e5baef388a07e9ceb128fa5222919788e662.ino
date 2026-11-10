#include <Arduino.h>
#include <DHT.h>

static const uint8_t LED_PIN = 13;

// DHT11
static const uint8_t DHT_PIN = 7;
static const uint8_t DHT_TYPE = DHT11;
DHT dht(DHT_PIN, DHT_TYPE);

// HC-SR04
static const uint8_t TRIG_PIN = 8;
static const uint8_t ECHO_PIN = 9;

// Timing / thresholds
static const unsigned long POLL_INTERVAL_MS = 3000;
static const uint8_t DISTANCE_THRESHOLD_CM = 20;
static const uint8_t REQUIRED_CONSECUTIVE_HITS = 3;

static unsigned long lastPollMs = 0;

static uint8_t consecutiveDistanceLessThanThreshold = 0;

static float readTemperatureC() {
  // DHT reads are slow; using single read per poll as required.
  return dht.readTemperature(); // Celsius by default in DHT library
}

static long readDistanceCm() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse width
  // Timeout to avoid hanging if no echo.
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL); // up to ~30ms
  if (duration == 0) {
    // No echo; treat as large distance (not less than threshold)
    return 100000L;
  }

  // Speed of sound: 343 m/s => 0.0343 cm/us.
  // Distance (cm) = duration(us) * 0.0343 / 2
  long distanceCm = (long)(duration * 0.0343f / 2.0f);
  return distanceCm;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Serial.begin(9600);

  dht.begin();

  lastPollMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastPollMs < POLL_INTERVAL_MS) return;
  lastPollMs = now;

  // Poll temperature and distance
  float tempC = readTemperatureC();
  long distanceCm = readDistanceCm();

  // Output via serial each poll
  // Format (stable, line-based): temperature and distance, plus alarm state message when triggered.
  // Persistent behavior: once detected, LED remains on and message can be printed on each relevant poll.
  Serial.print("Temperature(C): ");
  if (isnan(tempC)) {
    Serial.print("NaN");
  } else {
    Serial.print(tempC, 1);
  }

  Serial.print(", Distance(cm): ");
  Serial.print(distanceCm);

  bool distanceLess = (distanceCm >= 0) && (distanceCm < (long)DISTANCE_THRESHOLD_CM);

  if (distanceLess) {
    consecutiveDistanceLessThanThreshold++;
  } else {
    consecutiveDistanceLessThanThreshold = 0;
  }

  if (consecutiveDistanceLessThanThreshold >= REQUIRED_CONSECUTIVE_HITS) {
    digitalWrite(LED_PIN, HIGH);
    Serial.print(", persistent object detected");
  }

  Serial.println();
}