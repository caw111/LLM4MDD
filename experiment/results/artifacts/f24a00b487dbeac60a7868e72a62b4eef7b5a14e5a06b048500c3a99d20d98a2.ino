#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

// Wiring: button connects D2 to GND; relay is assumed active HIGH.
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t DISPLAY_DIO_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t FAN_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t DHT_PIN = 7;
constexpr uint8_t HEATER_PIN = 10;
constexpr uint8_t LED_PIN = 13;

constexpr uint8_t HEATER_ON_LEVEL = HIGH;
constexpr uint8_t HEATER_OFF_LEVEL = LOW;
constexpr uint8_t FAN_ON_LEVEL = HIGH;
constexpr uint8_t FAN_OFF_LEVEL = LOW;

// Adjustable incubation settings; temperatures are in degrees Celsius.
constexpr float LOWER_LIMIT_C = 37.0f;
constexpr float UPPER_LIMIT_C = 38.0f;
constexpr uint32_t ABNORMAL_DURATION_MS = 60000UL;
constexpr uint32_t SENSOR_INTERVAL_MS = 2000UL;
constexpr uint32_t SENSOR_STALE_MS = 6500UL;

// Run the fan for 30 seconds every 5 minutes, independently of temperature.
constexpr uint32_t VENT_INTERVAL_MS = 300000UL;
constexpr uint32_t VENT_DURATION_MS = 30000UL;
constexpr uint32_t BUTTON_DEBOUNCE_MS = 40UL;
constexpr uint32_t DISPLAY_INTERVAL_MS = 200UL;
constexpr uint32_t SETPOINT_PAGE_MS = 1500UL;

static_assert(LOWER_LIMIT_C < UPPER_LIMIT_C, "Invalid temperature limits");
static_assert(VENT_DURATION_MS < VENT_INTERVAL_MS, "Invalid ventilation timing");

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum TemperatureCondition : uint8_t {
  CONDITION_NORMAL,
  CONDITION_LOW,
  CONDITION_HIGH
};

float currentTemperatureC = NAN;
bool sensorValid = false;
bool heaterOn = false;
bool fanOn = false;
bool alarmActive = false;
bool showSetpoints = false;

TemperatureCondition temperatureCondition = CONDITION_NORMAL;
uint32_t conditionStartedAt = 0;
uint32_t lastSensorAttemptAt = 0;
uint32_t lastValidReadingAt = 0;
uint32_t lastVentStartedAt = 0;
uint32_t lastDisplayAt = 0;
uint32_t setpointModeStartedAt = 0;

uint8_t lastRawButton = HIGH;
uint8_t stableButton = HIGH;
uint32_t buttonChangedAt = 0;
bool displayDirty = true;

void setHeater(bool enabled) {
  heaterOn = enabled;
  digitalWrite(HEATER_PIN, enabled ? HEATER_ON_LEVEL : HEATER_OFF_LEVEL);
}

void setFan(bool enabled) {
  fanOn = enabled;
  digitalWrite(FAN_PIN, enabled ? FAN_ON_LEVEL : FAN_OFF_LEVEL);
}

void setAlarm(bool enabled) {
  if (enabled == alarmActive) {
    return;
  }

  alarmActive = enabled;
  digitalWrite(LED_PIN, enabled ? HIGH : LOW);

  if (enabled) {
    tone(BUZZER_PIN, 2000);
    // One line per continuous abnormal-temperature episode.
    Serial.println(F("incubation abnormal"));
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void resetTemperatureCondition() {
  temperatureCondition = CONDITION_NORMAL;
  conditionStartedAt = 0;
  setAlarm(false);
}

void updateButton(uint32_t now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastRawButton) {
    lastRawButton = reading;
    buttonChangedAt = now;
  }

  if (reading != stableButton &&
      (uint32_t)(now - buttonChangedAt) >= BUTTON_DEBOUNCE_MS) {
    stableButton = reading;
    if (stableButton == LOW) {
      showSetpoints = !showSetpoints;
      setpointModeStartedAt = now;
      displayDirty = true;
    }
  }
}

void updateSensor(uint32_t now) {
  if ((uint32_t)(now - lastSensorAttemptAt) < SENSOR_INTERVAL_MS) {
    return;
  }

  lastSensorAttemptAt = now;
  const float temperature = dht.readTemperature();

  if (isnan(temperature) || temperature < 0.0f || temperature > 50.0f) {
    // An invalid measurement cannot establish continuous over/undertemperature.
    sensorValid = false;
    currentTemperatureC = NAN;
    setHeater(false);
    resetTemperatureCondition();
  } else {
    currentTemperatureC = temperature;
    sensorValid = true;
    lastValidReadingAt = millis();
  }

  displayDirty = true;
}

void updateTemperatureControl(uint32_t now) {
  if (!sensorValid ||
      (uint32_t)(now - lastValidReadingAt) >= SENSOR_STALE_MS) {
    // Fail safe: never heat using missing or stale sensor data.
    sensorValid = false;
    setHeater(false);
    resetTemperatureCondition();
    return;
  }

  // Hysteresis: retain the previous heater state between the two limits.
  if (currentTemperatureC <= LOWER_LIMIT_C) {
    setHeater(true);
  } else if (currentTemperatureC >= UPPER_LIMIT_C) {
    setHeater(false);
  }

  TemperatureCondition nextCondition = CONDITION_NORMAL;
  if (currentTemperatureC < LOWER_LIMIT_C) {
    nextCondition = CONDITION_LOW;
  } else if (currentTemperatureC > UPPER_LIMIT_C) {
    nextCondition = CONDITION_HIGH;
  }

  if (nextCondition != temperatureCondition) {
    temperatureCondition = nextCondition;
    conditionStartedAt = now;
    setAlarm(false);
  }

  if (temperatureCondition == CONDITION_NORMAL) {
    setAlarm(false);
  } else if ((uint32_t)(now - conditionStartedAt) >= ABNORMAL_DURATION_MS) {
    setAlarm(true);
  }
}

void updateVentilation(uint32_t now) {
  if (fanOn &&
      (uint32_t)(now - lastVentStartedAt) >= VENT_DURATION_MS) {
    setFan(false);
  }

  if (!fanOn &&
      (uint32_t)(now - lastVentStartedAt) >= VENT_INTERVAL_MS) {
    lastVentStartedAt = now;
    setFan(true);
  }
}

void displayTemperature(float temperature) {
  // Four digits: "37.5C", using a decimal point after the second digit.
  const int16_t tenths = (int16_t)roundf(temperature * 10.0f);
  uint8_t segments[4] = {0, 0, 0, SEG_A | SEG_D | SEG_E | SEG_F};

  if (tenths < 0 || tenths > 999) {
    const uint8_t errorSegments[4] = {SEG_G, SEG_G, SEG_G, SEG_G};
    display.setSegments(errorSegments);
    return;
  }

  if (tenths >= 100) {
    segments[0] = display.encodeDigit((tenths / 100) % 10);
  }
  segments[1] = display.encodeDigit((tenths / 10) % 10) | SEG_DP;
  segments[2] = display.encodeDigit(tenths % 10);
  display.setSegments(segments);
}

void updateDisplay(uint32_t now) {
  if (!displayDirty &&
      (uint32_t)(now - lastDisplayAt) < DISPLAY_INTERVAL_MS) {
    return;
  }

  displayDirty = false;
  lastDisplayAt = now;

  if (showSetpoints) {
    // Cycle: "Lo", lower limit, "Hi", upper limit.
    const uint8_t page =
        ((uint32_t)(now - setpointModeStartedAt) / SETPOINT_PAGE_MS) % 4;

    if (page == 0) {
      const uint8_t label[4] = {
        SEG_D | SEG_E | SEG_F,
        SEG_C | SEG_D | SEG_E | SEG_G,
        0,
        0
      };
      display.setSegments(label);
    } else if (page == 1) {
      displayTemperature(LOWER_LIMIT_C);
    } else if (page == 2) {
      const uint8_t label[4] = {
        SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
        SEG_E | SEG_F,
        0,
        0
      };
      display.setSegments(label);
    } else {
      displayTemperature(UPPER_LIMIT_C);
    }
  } else if (sensorValid) {
    displayTemperature(currentTemperatureC);
  } else {
    const uint8_t unavailable[4] = {SEG_G, SEG_G, SEG_G, SEG_G};
    display.setSegments(unavailable);
  }
}

void setup() {
  // Establish safe output levels before enabling output drivers.
  digitalWrite(HEATER_PIN, HEATER_OFF_LEVEL);
  digitalWrite(FAN_PIN, FAN_OFF_LEVEL);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.setTimeout(20);  // Line/idle interface idle timeout, if input is used.

  dht.begin();
  display.setBrightness(5, true);

  const uint32_t now = millis();
  lastSensorAttemptAt = now;  // Allow DHT11 to settle before the first reading.
  lastVentStartedAt = now;
  buttonChangedAt = now;
  lastRawButton = digitalRead(BUTTON_PIN);
  stableButton = HIGH;

  setHeater(false);
  setFan(false);
  updateDisplay(now);
}

void loop() {
  updateButton(millis());
  updateSensor(millis());

  // Refresh time after the DHT transaction.
  const uint32_t now = millis();
  updateTemperatureControl(now);
  updateVentilation(now);
  updateDisplay(now);
}