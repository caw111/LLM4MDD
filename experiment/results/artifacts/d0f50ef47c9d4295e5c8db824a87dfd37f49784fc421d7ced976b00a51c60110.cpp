#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring: recyclable sensor D2, other-waste sensor D3,
// TM1637 CLK D4 / DIO D5, buzzer D6, reset button D7,
// ultrasonic Trig D8 / Echo D9, alarm LED D13.
const uint8_t RECYCLABLE_PIN = 2;
const uint8_t OTHER_PIN      = 3;
const uint8_t DISPLAY_CLK    = 4;
const uint8_t DISPLAY_DIO    = 5;
const uint8_t BUZZER_PIN     = 6;
const uint8_t RESET_PIN      = 7;
const uint8_t TRIG_PIN       = 8;
const uint8_t ECHO_PIN       = 9;
const uint8_t LED_PIN        = 13;

// Sensors and reset button are active LOW.
// Connect the reset button between D7 and GND.
const uint8_t SENSOR_ACTIVE_LEVEL = LOW;
const unsigned long DEBOUNCE_MS = 30;
const unsigned long DISTANCE_INTERVAL_MS = 150;
const unsigned long ECHO_TIMEOUT_US = 30000;
const unsigned long MEASUREMENT_MAX_AGE_MS = 1000;

// Installation calibration: distance from the ultrasonic sensor
// to the contents when empty and when physically full.
const float EMPTY_DISTANCE_CM = 40.0f;
const float FULL_DISTANCE_CM  = 5.0f;
const float FULL_THRESHOLD_PERCENT = 80.0f;
const float RESET_THRESHOLD_PERCENT = 75.0f;

const unsigned long ALARM_PERIOD_MS = 1000;
const unsigned long ALARM_ON_MS = 300;
const unsigned int ALARM_FREQUENCY_HZ = 2200;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

struct DebouncedInput {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long changedAt;

  void begin(uint8_t inputPin) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = digitalRead(pin);
    stableState = rawState;
    changedAt = millis();
  }

  bool update(unsigned long now) {
    uint8_t reading = digitalRead(pin);
    if (reading != rawState) {
      rawState = reading;
      changedAt = now;
    }

    if (rawState != stableState &&
        (unsigned long)(now - changedAt) >= DEBOUNCE_MS) {
      stableState = rawState;
      return true;
    }
    return false;
  }
};

DebouncedInput recyclableSensor;
DebouncedInput otherSensor;
DebouncedInput resetButton;

uint32_t recyclableCount = 0;
uint32_t otherCount = 0;
uint32_t totalCount = 0;

bool fullLatched = false;
bool latestMeasurementValid = false;
bool haveMeasurement = false;
bool alarmOn = false;

float fullnessPercent = 0.0f;
unsigned long lastDistanceAttempt = 0;
unsigned long lastValidMeasurement = 0;
unsigned long alarmStartedAt = 0;

#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void refreshDisplay();
#line 93 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void printCounts();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void setAlarmOutput(bool enabled);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void latchFull(unsigned long now);
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void measureFullness();
#line 166 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void recordDeposit(bool recyclable);
#line 183 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void handleReset();
#line 216 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 241 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_024\\direct\\attempt_0\\sketch\\sketch.ino"
void refreshDisplay() {
  // Four digits cannot represent larger totals; retain the complete
  // counters internally and on serial while saturating the display.
  uint16_t shown = totalCount > 9999UL ? 9999 : (uint16_t)totalCount;
  display.showNumberDec(shown, false);
}

void printCounts() {
  Serial.print(F("recyclable="));
  Serial.print(recyclableCount);
  Serial.print(F(", other="));
  Serial.print(otherCount);
  Serial.print(F(", total="));
  Serial.println(totalCount);
}

void setAlarmOutput(bool enabled) {
  if (enabled == alarmOn) {
    return;
  }

  alarmOn = enabled;
  digitalWrite(LED_PIN, enabled ? HIGH : LOW);
  if (enabled) {
    tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
  } else {
    noTone(BUZZER_PIN);
  }
}

void latchFull(unsigned long now) {
  if (fullLatched) {
    return;
  }

  fullLatched = true;
  alarmStartedAt = now;
  setAlarmOutput(true);
  Serial.println(F("empty bin needed"));
}

void measureFullness() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  latestMeasurementValid = false;

  // A missing echo is not interpreted as an empty bin.
  if (duration == 0) {
    return;
  }

  float distanceCm = duration * 0.0343f / 2.0f;
  if (distanceCm < 2.0f || distanceCm > 400.0f) {
    return;
  }

  fullnessPercent =
      100.0f * (EMPTY_DISTANCE_CM - distanceCm) /
      (EMPTY_DISTANCE_CM - FULL_DISTANCE_CM);

  if (fullnessPercent < 0.0f) {
    fullnessPercent = 0.0f;
  } else if (fullnessPercent > 100.0f) {
    fullnessPercent = 100.0f;
  }

  latestMeasurementValid = true;
  haveMeasurement = true;
  lastValidMeasurement = millis();

  if (fullnessPercent > FULL_THRESHOLD_PERCENT) {
    latchFull(lastValidMeasurement);
  }
}

void recordDeposit(bool recyclable) {
  // Saturating counters avoid rollover after prolonged operation.
  if (totalCount == 0xFFFFFFFFUL) {
    return;
  }

  if (recyclable) {
    ++recyclableCount;
  } else {
    ++otherCount;
  }
  ++totalCount;

  refreshDisplay();
  printCounts();
}

void handleReset() {
  // Verify clearance immediately instead of relying on an old reading.
  lastDistanceAttempt = millis();
  measureFullness();

  unsigned long now = millis();
  bool cleared =
      haveMeasurement &&
      latestMeasurementValid &&
      (unsigned long)(now - lastValidMeasurement) <= MEASUREMENT_MAX_AGE_MS &&
      fullnessPercent < RESET_THRESHOLD_PERCENT;

  if (!cleared) {
    Serial.println(F("reset denied: empty bin and verify ultrasonic sensor"));
    return;
  }

  recyclableCount = 0;
  otherCount = 0;
  totalCount = 0;
  fullLatched = false;
  setAlarmOutput(false);

  // Synchronize the inputs so a held sensor cannot create a phantom
  // deposit after reset. It must be released and activated again.
  recyclableSensor.begin(RECYCLABLE_PIN);
  otherSensor.begin(OTHER_PIN);

  refreshDisplay();
  Serial.println(F("bin reset"));
  printCounts();
}

void setup() {
  Serial.begin(9600);

  recyclableSensor.begin(RECYCLABLE_PIN);
  otherSensor.begin(OTHER_PIN);
  resetButton.begin(RESET_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);

  display.setBrightness(7, true);
  refreshDisplay();

  // Establish fullness before accepting any deposits.
  measureFullness();
  lastDistanceAttempt = millis();
  printCounts();
}

void loop() {
  unsigned long now = millis();

  if ((unsigned long)(now - lastDistanceAttempt) >= DISTANCE_INTERVAL_MS) {
    lastDistanceAttempt = now;
    measureFullness();
    now = millis();
  }

  bool recyclableChanged = recyclableSensor.update(now);
  bool otherChanged = otherSensor.update(now);
  bool resetChanged = resetButton.update(now);

  bool resetPressed = resetChanged && resetButton.stableState == LOW;

  if (resetPressed) {
    handleReset();
  } else if (!fullLatched && haveMeasurement) {
    // Count one event per debounced inactive-to-active transition.
    // A continuously occupied opening cannot repeatedly increment.
    if (recyclableChanged &&
        recyclableSensor.stableState == SENSOR_ACTIVE_LEVEL) {
      recordDeposit(true);
    }

    if (otherChanged &&
        otherSensor.stableState == SENSOR_ACTIVE_LEVEL) {
      recordDeposit(false);
    }
  }

  now = millis();
  if (fullLatched) {
    unsigned long phase =
        (unsigned long)(now - alarmStartedAt) % ALARM_PERIOD_MS;
    setAlarmOutput(phase < ALARM_ON_MS);
  } else {
    setAlarmOutput(false);
  }
}
