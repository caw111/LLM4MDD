#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// Wiring: button connects D2 to GND; its internal pull-up is enabled.
// Button controls:
//   Short press in AUTO: enter MANUAL with the pump stopped.
//   Short press in MANUAL: start/restart a five-second watering cycle.
//   Long press (1 second): toggle AUTO/MANUAL without starting a manual cycle.
// Low water always inhibits watering, including manual watering.
//
// Install the "TM1637Display" library.
// Calibration values below are defaults; adjust for the actual sensors.
// Default sensor polarity: wet soil/rain -> lower ADC; more water -> higher ADC.

constexpr uint8_t SOIL_PIN   = A0;
constexpr uint8_t RAIN_PIN   = A1;
constexpr uint8_t WATER_PIN  = A2;
constexpr uint8_t RELAY_PIN  = 10;
constexpr uint8_t LED_PIN    = 13;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t CLK_PIN    = 4;
constexpr uint8_t DIO_PIN    = 5;
constexpr uint8_t BUTTON_PIN = 2;

// Change these two constants if the relay module is active HIGH.
constexpr uint8_t RELAY_ON  = LOW;
constexpr uint8_t RELAY_OFF = HIGH;

constexpr int SOIL_DRY_ADC   = 850;
constexpr int SOIL_WET_ADC   = 350;
constexpr int WATER_EMPTY_ADC = 100;
constexpr int WATER_FULL_ADC  = 850;

constexpr uint8_t SOIL_DRY_PERCENT = 35;
constexpr uint8_t SOIL_WET_PERCENT = 45;
constexpr uint8_t WATER_LOW_PERCENT = 25;
constexpr uint8_t WATER_OK_PERCENT  = 35;
constexpr int RAIN_WET_ADC = 400;
constexpr int RAIN_DRY_ADC = 500;

constexpr unsigned long SENSOR_INTERVAL_MS = 50;
constexpr unsigned long DISPLAY_INTERVAL_MS = 2000;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long LONG_PRESS_MS = 1000;
constexpr unsigned long MANUAL_RUN_MS = 5000;
constexpr unsigned long ALARM_PERIOD_MS = 1000;
constexpr unsigned long ALARM_ON_MS = 200;

TM1637Display display(CLK_PIN, DIO_PIN);

enum Mode : uint8_t { AUTO_MODE, MANUAL_MODE };
Mode mode = AUTO_MODE;

int filteredSoil;
int filteredRain;
int filteredWater;
uint8_t soilPercent = 0;
uint8_t waterPercent = 0;
bool soilDry = false;
bool raining = false;
bool waterLow = true;
bool pumpRunning = false;
bool manualRunActive = false;

unsigned long lastSensorAt = 0;
unsigned long manualStartedAt = 0;
unsigned long displayChangedAt = 0;
uint8_t displayPage = 0;

bool rawButton = HIGH;
bool stableButton = HIGH;
bool longPressHandled = false;
unsigned long rawButtonChangedAt = 0;
unsigned long buttonPressedAt = 0;

bool alarmActive = false;
bool buzzerOn = false;
unsigned long alarmStartedAt = 0;

#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t calibratedPercent(int reading, int zeroPoint, int fullPoint);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
int readAveraged(uint8_t pin);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setPump(bool on);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSensorStates(bool initialize);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSensors(unsigned long now);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void selectMode(Mode nextMode, unsigned long now);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void handleShortPress(unsigned long now);
#line 155 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updateButton(unsigned long now);
#line 180 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updatePump(unsigned long now);
#line 199 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarm(unsigned long now);
#line 216 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void showPercent(uint8_t label, uint8_t value);
#line 226 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(unsigned long now);
#line 252 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 284 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 80 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_006\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t calibratedPercent(int reading, int zeroPoint, int fullPoint) {
  if (zeroPoint == fullPoint) return 0;
  long percent = (long)(reading - zeroPoint) * 100L /
                 (fullPoint - zeroPoint);
  return (uint8_t)constrain(percent, 0L, 100L);
}

int readAveraged(uint8_t pin) {
  // Discard the first conversion after changing ADC channels.
  analogRead(pin);
  unsigned int sum = 0;
  for (uint8_t i = 0; i < 4; ++i) {
    sum += analogRead(pin);
  }
  return (sum + 2) / 4;
}

void setPump(bool on) {
  pumpRunning = on;
  digitalWrite(RELAY_PIN, on ? RELAY_ON : RELAY_OFF);
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

void updateSensorStates(bool initialize) {
  soilPercent = calibratedPercent(filteredSoil, SOIL_DRY_ADC, SOIL_WET_ADC);
  waterPercent = calibratedPercent(filteredWater,
                                   WATER_EMPTY_ADC, WATER_FULL_ADC);

  if (initialize) {
    soilDry = soilPercent <= SOIL_DRY_PERCENT;
    raining = filteredRain <= RAIN_WET_ADC;
    // On startup, require the safe recovery threshold before allowing pumping.
    waterLow = waterPercent < WATER_OK_PERCENT;
    return;
  }

  if (soilPercent <= SOIL_DRY_PERCENT) soilDry = true;
  else if (soilPercent >= SOIL_WET_PERCENT) soilDry = false;

  if (filteredRain <= RAIN_WET_ADC) raining = true;
  else if (filteredRain >= RAIN_DRY_ADC) raining = false;

  if (waterPercent <= WATER_LOW_PERCENT) waterLow = true;
  else if (waterPercent >= WATER_OK_PERCENT) waterLow = false;
}

void updateSensors(unsigned long now) {
  if (now - lastSensorAt < SENSOR_INTERVAL_MS) return;
  lastSensorAt = now;

  filteredSoil = (3L * filteredSoil + readAveraged(SOIL_PIN) + 2) / 4;
  filteredRain = (3L * filteredRain + readAveraged(RAIN_PIN) + 2) / 4;
  filteredWater = (3L * filteredWater + readAveraged(WATER_PIN) + 2) / 4;
  updateSensorStates(false);
}

void selectMode(Mode nextMode, unsigned long now) {
  mode = nextMode;
  manualRunActive = false;
  setPump(false);
  displayPage = 2;
  displayChangedAt = now;
}

void handleShortPress(unsigned long now) {
  if (mode == AUTO_MODE) {
    selectMode(MANUAL_MODE, now);
  } else if (!waterLow) {
    manualRunActive = true;
    manualStartedAt = now;
  } else {
    manualRunActive = false;
  }
}

void updateButton(unsigned long now) {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != rawButton) {
    rawButton = reading;
    rawButtonChangedAt = now;
  }

  if (now - rawButtonChangedAt >= DEBOUNCE_MS &&
      stableButton != rawButton) {
    stableButton = rawButton;
    if (stableButton == LOW) {
      buttonPressedAt = now;
      longPressHandled = false;
    } else if (!longPressHandled) {
      handleShortPress(now);
    }
  }

  if (stableButton == LOW && rawButton == LOW &&
      !longPressHandled && now - buttonPressedAt >= LONG_PRESS_MS) {
    longPressHandled = true;
    selectMode(mode == AUTO_MODE ? MANUAL_MODE : AUTO_MODE, now);
  }
}

void updatePump(unsigned long now) {
  if (waterLow) {
    // Cancel rather than pause: refilling must not restart a manual cycle.
    manualRunActive = false;
    setPump(false);
    return;
  }

  if (mode == AUTO_MODE) {
    setPump(soilDry && !raining);
  } else {
    if (manualRunActive && now - manualStartedAt >= MANUAL_RUN_MS) {
      manualRunActive = false;
    }
    // Manual mode overrides soil and rain, but never the low-water interlock.
    setPump(manualRunActive);
  }
}

void updateAlarm(unsigned long now) {
  if (waterLow && !alarmActive) {
    alarmActive = true;
    alarmStartedAt = now;
  } else if (!waterLow) {
    alarmActive = false;
  }

  bool shouldSound = alarmActive &&
                     ((now - alarmStartedAt) % ALARM_PERIOD_MS < ALARM_ON_MS);
  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) tone(BUZZER_PIN, 2200);
    else noTone(BUZZER_PIN);
  }
}

void showPercent(uint8_t label, uint8_t value) {
  uint8_t segments[4] = {
    label,
    value >= 100 ? display.encodeDigit(value / 100) : (uint8_t)0,
    value >= 10 ? display.encodeDigit((value / 10) % 10) : (uint8_t)0,
    display.encodeDigit(value % 10)
  };
  display.setSegments(segments);
}

void updateDisplay(unsigned long now) {
  if (now - displayChangedAt >= DISPLAY_INTERVAL_MS) {
    displayChangedAt = now;
    displayPage = (displayPage + 1) % 3;
  }

  // Only write when the page or its displayed value changes.
  static uint8_t previousPage = 255;
  static uint8_t previousValue = 255;
  uint8_t value = displayPage == 0 ? soilPercent :
                  displayPage == 1 ? waterPercent : (uint8_t)mode;
  if (displayPage == previousPage && value == previousValue) return;
  previousPage = displayPage;
  previousValue = value;

  if (displayPage == 0) {
    showPercent(0x6D, soilPercent);  // S: soil moisture, percent
  } else if (displayPage == 1) {
    showPercent(0x38, waterPercent); // L: water level, percent
  } else {
    const uint8_t autoText[4] = {0x77, 0x3E, 0x78, 0x3F}; // AUto
    const uint8_t manualText[4] = {0x76, 0x77, 0x54, 0x5E}; // HAnd
    display.setSegments(mode == AUTO_MODE ? autoText : manualText);
  }
}

void setup() {
  // Preload the OFF level before enabling the relay output.
  digitalWrite(RELAY_PIN, RELAY_OFF);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(WATER_PIN, INPUT);
  noTone(BUZZER_PIN);

  display.setBrightness(5);
  display.clear();

  filteredSoil = readAveraged(SOIL_PIN);
  filteredRain = readAveraged(RAIN_PIN);
  filteredWater = readAveraged(WATER_PIN);
  updateSensorStates(true);

  unsigned long now = millis();
  rawButton = stableButton = digitalRead(BUTTON_PIN);
  rawButtonChangedAt = buttonPressedAt = now;
  lastSensorAt = displayChangedAt = now;

  updatePump(now);
  updateAlarm(now);
  updateDisplay(now);
}

void loop() {
  unsigned long now = millis();
  updateSensors(now);
  updateButton(now);
  updatePump(now);
  updateAlarm(now);
  updateDisplay(now);
}
