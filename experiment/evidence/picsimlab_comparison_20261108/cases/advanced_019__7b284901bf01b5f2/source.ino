#include <Arduino.h>

// HC-SR04 wiring.
const uint8_t VEHICLE_TRIG_PIN = 8;
const uint8_t VEHICLE_ECHO_PIN = 9;
const uint8_t PEDESTRIAN_TRIG_PIN = 10;
const uint8_t PEDESTRIAN_ECHO_PIN = 11;

// Indicators and authoritative additional digital output.
const uint8_t RED_LED_PIN = 12;
const uint8_t GREEN_LED_PIN = 13;
const uint8_t BUZZER_PIN = 6;
const uint8_t AUX_DANGER_PIN = 2;

// TM1637 wiring.
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;

// Adjustable proximity thresholds, in centimeters.
// Separate exit thresholds prevent rapid switching near a boundary.
const uint16_t VEHICLE_ENTER_CM = 200;
const uint16_t VEHICLE_EXIT_CM = 220;
const uint16_t PEDESTRIAN_ENTER_CM = 150;
const uint16_t PEDESTRIAN_EXIT_CM = 170;

const unsigned long ECHO_TIMEOUT_US = 25000UL;
const unsigned long PING_INTERVAL_MS = 60UL;
const unsigned long SAMPLE_MAX_AGE_MS = 400UL;

struct SensorState {
  uint8_t trigPin;
  uint8_t echoPin;
  uint16_t distanceCm;
  unsigned long sampledAt;
  bool valid;
  bool nearby;
};

SensorState vehicle = {
  VEHICLE_TRIG_PIN, VEHICLE_ECHO_PIN, 0, 0, false, false
};
SensorState pedestrian = {
  PEDESTRIAN_TRIG_PIN, PEDESTRIAN_ECHO_PIN, 0, 0, false, false
};

bool sampleVehicleNext = true;
unsigned long lastPingAt = 0;

bool dangerActive = false;
unsigned long dangerStartedAt = 0;
unsigned long lastDangerDurationMs = 0;
uint8_t currentDangerLevel = 0;

bool alarmPhase = false;
unsigned long lastAlarmToggleAt = 0;

const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Standalone TM1637 driver; no external library is required.
void displayDelay() {
  delayMicroseconds(5);
}

void displayDataLow() {
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
}

void displayDataRelease() {
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
}

void displayStart() {
  displayDataRelease();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  displayDataLow();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void displayStop() {
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  displayDataLow();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  displayDataRelease();
  displayDelay();
}

bool displayWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    digitalWrite(DISPLAY_CLK_PIN, LOW);
    if (value & 0x01) {
      displayDataRelease();
    } else {
      displayDataLow();
    }
    displayDelay();
    digitalWrite(DISPLAY_CLK_PIN, HIGH);
    displayDelay();
    value >>= 1;
  }

  digitalWrite(DISPLAY_CLK_PIN, LOW);
  displayDataRelease();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  bool acknowledged = (digitalRead(DISPLAY_DIO_PIN) == LOW);
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  displayDelay();
  return acknowledged;
}

void displayDangerLevel(uint8_t level) {
  // Blank leading digits; 0 means safe, 1/2/3 are danger levels.
  displayStart();
  displayWriteByte(0x40);  // Automatic address increment.
  displayStop();

  displayStart();
  displayWriteByte(0xC0);
  displayWriteByte(0x00);
  displayWriteByte(0x00);
  displayWriteByte(0x00);
  displayWriteByte(DIGIT_SEGMENTS[level <= 3 ? level : 3]);
  displayStop();

  displayStart();
  displayWriteByte(0x8B);  // Display enabled, moderate brightness.
  displayStop();
}

void sampleSensor(SensorState &sensor,
                  uint16_t enterThresholdCm,
                  uint16_t exitThresholdCm) {
  digitalWrite(sensor.trigPin, LOW);
  delayMicroseconds(3);
  digitalWrite(sensor.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trigPin, LOW);

  unsigned long echoUs =
      pulseIn(sensor.echoPin, HIGH, ECHO_TIMEOUT_US);

  sensor.sampledAt = millis();
  sensor.valid = (echoUs != 0);

  if (sensor.valid) {
    sensor.distanceCm = (uint16_t)((echoUs + 29UL) / 58UL);
    sensor.valid =
        (sensor.distanceCm >= 2 && sensor.distanceCm <= 400);
  }

  if (!sensor.valid) {
    sensor.distanceCm = 0;
    sensor.nearby = false;
    return;
  }

  uint16_t threshold =
      sensor.nearby ? exitThresholdCm : enterThresholdCm;
  sensor.nearby = (sensor.distanceCm <= threshold);
}

bool isFreshAndNearby(const SensorState &sensor, unsigned long now) {
  return sensor.valid && sensor.nearby &&
         (unsigned long)(now - sensor.sampledAt) <= SAMPLE_MAX_AGE_MS;
}

uint8_t calculateDangerLevel(unsigned long now) {
  if (!isFreshAndNearby(vehicle, now) ||
      !isFreshAndNearby(pedestrian, now)) {
    return 0;
  }

  // Either object becoming very close increases the warning severity.
  // Both objects must still be within their respective proximity zones.
  if (vehicle.distanceCm <= 70 || pedestrian.distanceCm <= 50) {
    return 3;
  }
  if (vehicle.distanceCm <= 130 || pedestrian.distanceCm <= 100) {
    return 2;
  }
  return 1;
}

void applyAlarmPhase(uint8_t level) {
  digitalWrite(RED_LED_PIN, alarmPhase ? HIGH : LOW);

  if (alarmPhase) {
    const unsigned int frequencies[] = {0, 1500, 2200, 3000};
    tone(BUZZER_PIN, frequencies[level]);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void updateWarning(uint8_t newLevel, unsigned long now) {
  bool wasDanger = dangerActive;
  bool levelChanged = (newLevel != currentDangerLevel);
  dangerActive = (newLevel != 0);

  if (dangerActive && !wasDanger) {
    dangerStartedAt = now;
  }

  if (!dangerActive && wasDanger) {
    lastDangerDurationMs = (unsigned long)(now - dangerStartedAt);
    Serial.print(F("Last danger event duration: "));
    Serial.print(lastDangerDurationMs);
    Serial.println(F(" ms"));
  }

  if (levelChanged) {
    currentDangerLevel = newLevel;
    displayDangerLevel(currentDangerLevel);
  }

  digitalWrite(AUX_DANGER_PIN, dangerActive ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, dangerActive ? LOW : HIGH);

  if (!dangerActive) {
    alarmPhase = false;
    digitalWrite(RED_LED_PIN, LOW);
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  const unsigned long toggleIntervalsMs[] = {0, 400, 250, 120};

  if (!wasDanger || levelChanged) {
    alarmPhase = true;
    lastAlarmToggleAt = now;
    applyAlarmPhase(currentDangerLevel);
  } else if ((unsigned long)(now - lastAlarmToggleAt) >=
             toggleIntervalsMs[currentDangerLevel]) {
    lastAlarmToggleAt = now;
    alarmPhase = !alarmPhase;
    applyAlarmPhase(currentDangerLevel);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(VEHICLE_TRIG_PIN, OUTPUT);
  pinMode(VEHICLE_ECHO_PIN, INPUT);
  pinMode(PEDESTRIAN_TRIG_PIN, OUTPUT);
  pinMode(PEDESTRIAN_ECHO_PIN, INPUT);
  digitalWrite(VEHICLE_TRIG_PIN, LOW);
  digitalWrite(PEDESTRIAN_TRIG_PIN, LOW);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(AUX_DANGER_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(AUX_DANGER_PIN, LOW);
  noTone(BUZZER_PIN);

  pinMode(DISPLAY_CLK_PIN, OUTPUT);
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDataRelease();
  displayDangerLevel(0);

  // Allow sensor power-up stabilization without triggering either sensor.
  lastPingAt = millis();
}

void loop() {
  unsigned long now = millis();

  // Alternate sensors to reduce acoustic cross-talk. Only one bounded
  // echo measurement is performed per interval.
  if ((unsigned long)(now - lastPingAt) >= PING_INTERVAL_MS) {
    lastPingAt = now;

    if (sampleVehicleNext) {
      sampleSensor(vehicle, VEHICLE_ENTER_CM, VEHICLE_EXIT_CM);
    } else {
      sampleSensor(pedestrian, PEDESTRIAN_ENTER_CM,
                   PEDESTRIAN_EXIT_CM);
    }

    sampleVehicleNext = !sampleVehicleNext;
  }

  now = millis();
  updateWarning(calculateDangerLevel(now), now);
}