#include <Arduino.h>
#include <TM1637Display.h>

// Wiring (Arduino Uno)
const uint8_t TEMP_PIN    = A1;  // LM35: 10 mV / degree Celsius
const uint8_t LIGHT_PIN   = A0;
const uint8_t WATER_PIN   = A2;
const uint8_t BUTTON_PIN  = 2;   // Button between D2 and GND
const uint8_t HEATER_PIN  = 10;
const uint8_t LED_PIN     = 9;
const uint8_t PUMP_PIN    = 5;
const uint8_t BUZZER_PIN  = 6;
const uint8_t DISPLAY_CLK = 4;
const uint8_t DISPLAY_DIO = 3;

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

// Outputs are assumed active HIGH.
const uint8_t OUTPUT_ON  = HIGH;
const uint8_t OUTPUT_OFF = LOW;

// Adjust these calibration values to match measured sensor readings.
// Reversed endpoints are supported if a sensor's output decreases with level.
const float ADC_REFERENCE_VOLTS = 5.0f;
const int LIGHT_DARK_ADC   = 0;
const int LIGHT_BRIGHT_ADC = 1023;
const int WATER_EMPTY_ADC  = 0;
const int WATER_FULL_ADC   = 1023;

// Hysteresis prevents rapid switching near a threshold.
const float HEATER_ON_C  = 24.0f;
const float HEATER_OFF_C = 26.0f;
const uint8_t LIGHT_ON_PERCENT  = 30;
const uint8_t LIGHT_OFF_PERCENT = 45;
const uint8_t WATER_LOW_PERCENT = 20;
const uint8_t WATER_RECOVER_PERCENT = 30;

const unsigned long SENSOR_INTERVAL_MS = 200;
const unsigned long DISPLAY_INTERVAL_MS = 200;
const unsigned long BUTTON_DEBOUNCE_MS = 35;
const unsigned long WATER_RECOVERY_MS = 3000;
const unsigned long ALARM_PERIOD_MS = 1000;
const unsigned long ALARM_ON_MS = 250;
const unsigned int ALARM_FREQUENCY_HZ = 2200;

float temperatureC = 0.0f;
uint8_t lightPercent = 0;
uint8_t waterPercent = 0;

bool heaterOn = false;
bool lightingOn = false;
bool pumpOn = false;
bool lowWaterAlarm = true;
bool recoveryPending = false;
bool buzzerOn = false;

uint8_t displayMode = 0;  // 0: temperature, 1: light, 2: water
bool displayDirty = true;

uint8_t lastButtonReading = HIGH;
uint8_t stableButtonState = HIGH;

unsigned long lastSensorMs = 0;
unsigned long lastDisplayMs = 0;
unsigned long lastButtonChangeMs = 0;
unsigned long recoveryStartMs = 0;
unsigned long alarmStartMs = 0;

int readAveragedADC(uint8_t pin) {
  // Discard the first conversion after switching ADC channels.
  analogRead(pin);
  unsigned long sum = 0;
  for (uint8_t i = 0; i < 8; ++i) {
    sum += analogRead(pin);
  }
  return static_cast<int>((sum + 4) / 8);
}

uint8_t calibratedPercent(int reading, int zeroReading, int fullReading) {
  if (zeroReading == fullReading) {
    return 0;
  }

  long percent = (static_cast<long>(reading) - zeroReading) * 100L /
                 (fullReading - zeroReading);
  return static_cast<uint8_t>(constrain(percent, 0L, 100L));
}

void readSensors() {
  const int temperatureADC = readAveragedADC(TEMP_PIN);
  const int lightADC = readAveragedADC(LIGHT_PIN);
  const int waterADC = readAveragedADC(WATER_PIN);

  temperatureC = temperatureADC * ADC_REFERENCE_VOLTS * 100.0f / 1023.0f;
  lightPercent = calibratedPercent(lightADC, LIGHT_DARK_ADC, LIGHT_BRIGHT_ADC);
  waterPercent = calibratedPercent(waterADC, WATER_EMPTY_ADC, WATER_FULL_ADC);
}

void applyOutputs() {
  digitalWrite(HEATER_PIN, heaterOn ? OUTPUT_ON : OUTPUT_OFF);
  digitalWrite(LED_PIN, lightingOn ? OUTPUT_ON : OUTPUT_OFF);
  digitalWrite(PUMP_PIN, pumpOn ? OUTPUT_ON : OUTPUT_OFF);
}

void updateControl(unsigned long now) {
  if (waterPercent <= WATER_LOW_PERCENT) {
    if (!lowWaterAlarm) {
      alarmStartMs = now;
    }
    lowWaterAlarm = true;
    recoveryPending = false;
  } else if (lowWaterAlarm) {
    if (waterPercent >= WATER_RECOVER_PERCENT) {
      if (!recoveryPending) {
        recoveryPending = true;
        recoveryStartMs = now;
      } else if (now - recoveryStartMs >= WATER_RECOVERY_MS) {
        lowWaterAlarm = false;
        recoveryPending = false;
      }
    } else {
      recoveryPending = false;
    }
  }

  if (lowWaterAlarm) {
    // Low-water protection overrides every normal operating mode.
    heaterOn = false;
    lightingOn = false;
    pumpOn = false;
  } else {
    if (temperatureC <= HEATER_ON_C) {
      heaterOn = true;
    } else if (temperatureC >= HEATER_OFF_C) {
      heaterOn = false;
    }

    if (lightPercent <= LIGHT_ON_PERCENT) {
      lightingOn = true;
    } else if (lightPercent >= LIGHT_OFF_PERCENT) {
      lightingOn = false;
    }

    // Continuous circulation/aeration while water level is safe.
    pumpOn = true;
  }

  applyOutputs();
}

void updateAlarm(unsigned long now) {
  const bool shouldSound =
      lowWaterAlarm &&
      ((now - alarmStartMs) % ALARM_PERIOD_MS < ALARM_ON_MS);

  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) {
      tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void updateButton(unsigned long now) {
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastButtonReading = reading;
    lastButtonChangeMs = now;
  }

  if (now - lastButtonChangeMs >= BUTTON_DEBOUNCE_MS &&
      reading != stableButtonState) {
    stableButtonState = reading;

    if (stableButtonState == LOW) {
      displayMode = (displayMode + 1) % 3;
      displayDirty = true;
    }
  }
}

void updateDisplay(unsigned long now) {
  if (!displayDirty && now - lastDisplayMs < DISPLAY_INTERVAL_MS) {
    return;
  }

  lastDisplayMs = now;
  displayDirty = false;

  uint8_t segments[4] = {0, 0, 0, 0};

  if (displayMode == 0) {
    // Example: "25.3C"; decimal point belongs to digit 1.
    const int tenths = static_cast<int>(temperatureC * 10.0f + 0.5f);

    if (tenths >= 0 && tenths <= 999) {
      segments[0] = tenths >= 100
                        ? display.encodeDigit(tenths / 100)
                        : 0;
      segments[1] = display.encodeDigit((tenths / 10) % 10) | 0x80;
      segments[2] = display.encodeDigit(tenths % 10);
      segments[3] = SEG_A | SEG_D | SEG_E | SEG_F;  // C
    } else {
      segments[0] = SEG_B | SEG_C | SEG_E | SEG_F | SEG_G;  // H
      segments[1] = SEG_B | SEG_C;                          // I
      segments[3] = SEG_A | SEG_D | SEG_E | SEG_F;          // C
    }
  } else {
    // L000..L100: light percentage; U000..U100: water percentage.
    const uint8_t value = (displayMode == 1) ? lightPercent : waterPercent;
    segments[0] = (displayMode == 1)
                      ? (SEG_D | SEG_E | SEG_F)                   // L
                      : (SEG_B | SEG_C | SEG_D | SEG_E | SEG_F); // U
    segments[1] = display.encodeDigit(value / 100);
    segments[2] = display.encodeDigit((value / 10) % 10);
    segments[3] = display.encodeDigit(value % 10);
  }

  display.setSegments(segments);
}

void setup() {
  // Set safe output latches before enabling the output drivers.
  digitalWrite(HEATER_PIN, OUTPUT_OFF);
  digitalWrite(LED_PIN, OUTPUT_OFF);
  digitalWrite(PUMP_PIN, OUTPUT_OFF);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(TEMP_PIN, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(WATER_PIN, INPUT);
  analogReference(DEFAULT);

  noTone(BUZZER_PIN);
  display.setBrightness(4, true);
  display.clear();

  readSensors();

  const unsigned long now = millis();
  lastSensorMs = now;
  lastButtonChangeMs = now;
  lastButtonReading = digitalRead(BUTTON_PIN);
  stableButtonState = lastButtonReading;
  alarmStartMs = now;

  // Require a clearly safe water level before starting equipment.
  lowWaterAlarm = waterPercent < WATER_RECOVER_PERCENT;

  updateControl(now);
  updateAlarm(now);
  updateDisplay(now);
}

void loop() {
  const unsigned long now = millis();

  updateButton(now);

  if (now - lastSensorMs >= SENSOR_INTERVAL_MS) {
    lastSensorMs = now;
    readSensors();
    updateControl(now);
  }

  updateAlarm(now);
  updateDisplay(now);
}