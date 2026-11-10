#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

#define TRIG_PIN 8
#define ECHO_PIN 9

const int LED_PIN = 13;

DHT dht(DHTPIN, DHTTYPE);

// Timing: poll every 3 seconds
const unsigned long POLL_INTERVAL_MS = 3000;

// Distance threshold and consecutive count rule
const int DIST_THRESHOLD_CM = 20;
const int CONSECUTIVE_REQUIRED = 3;

// Track consecutive "object detected" distances
int consecutiveCloseCount = 0;

// For periodic scheduling
unsigned long lastPollMs = 0;

static float readTemperatureCelsius() {
  // DHT11 temperature is typically integer-like; keep as float.
  float t = dht.readTemperature();
  return t;
}

static long readDistanceCm() {
  // HC-SR04 measurement (blocking), using standard trigger/echo pulses.
  // Ensure pins are correct; defaults should already be set in setup().
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // pulseIn returns 0 on timeout.
  unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, 30000UL); // ~30ms timeout (~5m max)

  if (durationUs == 0) {
    // If no echo received, treat as "far" so it won't satisfy the consecutive rule.
    return 100000L;
  }

  // Speed of sound: 343 m/s = 0.343 mm/us
  // Distance (cm) = duration_us / 58
  long distanceCm = (long)(durationUs / 58UL);
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
  if (now - lastPollMs < POLL_INTERVAL_MS) {
    return;
  }
  lastPollMs = now;

  // Read temperature
  float tempC = readTemperatureCelsius();

  // Read distance
  long distCm = readDistanceCm();

  // Determine consecutive close distances
  if (distCm < DIST_THRESHOLD_CM) {
    consecutiveCloseCount++;
  } else {
    consecutiveCloseCount = 0;
  }

  // LED and persistent output object detected when condition met
  bool objectDetected = (consecutiveCloseCount >= CONSECUTIVE_REQUIRED);

  if (objectDetected) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("persistent object detected");
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Output temperature and distance every poll via serial.
  // Also print even if DHT read fails; preserve behavior by reporting values directly.
  Serial.print("Temperature(C): ");
  if (isnan(tempC)) {
    Serial.print("NaN");
  } else {
    Serial.print(tempC, 1);
  }
  Serial.print(", Distance(cm): ");
  Serial.println(distCm);
}