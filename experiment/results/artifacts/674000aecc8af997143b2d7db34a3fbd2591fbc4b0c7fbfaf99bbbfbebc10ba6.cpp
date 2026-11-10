#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>

// Required libraries: "DHT sensor library" and "TM1637Display".
const uint8_t DHT_PIN         = 7;
const uint8_t LIGHT_PIN       = A0;
const uint8_t SOIL_PIN        = A1;
const uint8_t POT_PIN         = A2;
const uint8_t FAN_PIN         = 5;
const uint8_t LED_PIN         = 9;
const uint8_t PUMP_PIN        = 10;
const uint8_t BUZZER_PIN      = 6;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 8;
const uint8_t MODE_BUTTON_PIN = 2;
const uint8_t ITEM_BUTTON_PIN = 3;

// Buttons connect their pins to GND when pressed.
// D2 toggles AUTO/MANUAL. D3 selects FAN/LED/PUMP in MANUAL.
// A2 sets fan/LED PWM; for the relay, its midpoint selects OFF/ON.
const uint8_t RELAY_ON_LEVEL  = HIGH;
const uint8_t RELAY_OFF_LEVEL = LOW;

// Adjust these calibration values to match the installed sensors.
// Light: higher raw reading means brighter light.
// Soil: higher raw reading means drier soil.
const int LIGHT_DARK_RAW   = 30;
const int LIGHT_BRIGHT_RAW = 1000;
const int SOIL_DRY_RAW     = 850;
const int SOIL_WET_RAW     = 350;

// Persistent readings at ADC rails indicate a suspected wiring fault.
// Calibration endpoints deliberately remain inside these fault limits.
const int ADC_FAULT_LOW  = 2;
const int ADC_FAULT_HIGH = 1021;
const uint8_t ANALOG_BAD_SAMPLES = 4;

const float FAN_START_C = 28.0f;
const float FAN_STOP_C  = 26.0f;
const float FAN_FULL_C  = 35.0f;
const uint8_t FAN_MIN_PWM = 100;

const uint8_t LIGHT_ON_BELOW_PERCENT  = 35;
const uint8_t LIGHT_OFF_ABOVE_PERCENT = 45;
const uint8_t SOIL_PUMP_ON_PERCENT    = 30;
const uint8_t SOIL_PUMP_OFF_PERCENT   = 45;

// Relay protection: no rapid PWM switching; limit continuous watering.
const unsigned long PUMP_MAX_RUN_MS = 15000UL;
const unsigned long PUMP_REST_MS    = 30000UL;
const unsigned long DHT_INTERVAL_MS = 2000UL;
const unsigned long ANALOG_INTERVAL_MS = 250UL;
const unsigned long DISPLAY_INTERVAL_MS = 2000UL;
const unsigned long DISPLAY_REFRESH_MS  = 200UL;
const unsigned long DEBOUNCE_MS = 30UL;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum ControlItem : uint8_t { ITEM_FAN, ITEM_LED, ITEM_PUMP };
bool manualMode = false;
ControlItem selectedItem = ITEM_FAN;

// Each manually controlled output retains its setting when another is selected.
uint8_t manualFan = 0;
uint8_t manualLED = 0;
bool manualPump = false;

float temperatureC = NAN;
float humidity = NAN;
int lightRaw = 0;
int soilRaw = 0;
uint8_t lightPercent = 0;
uint8_t soilPercent = 0;

bool temperatureFault = true;
bool lightFault = true;
bool soilFault = true;
uint8_t lightBadCount = 0;
uint8_t soilBadCount = 0;

// -1 means that no initial sensor result has been reported yet.
int8_t reportedDhtFault = -1;
int8_t reportedLightFault = -1;
int8_t reportedSoilFault = -1;

bool autoFanOn = false;
bool autoLEDOn = false;
bool autoPumpOn = false;

bool pumpRunning = false;
bool pumpResting = false;
unsigned long pumpStartedAt = 0;
unsigned long pumpStoppedAt = 0;

unsigned long lastDhtRead = 0;
unsigned long lastAnalogRead = 0;
unsigned long lastDisplayPage = 0;
unsigned long lastDisplayRefresh = 0;
uint8_t displayPage = 0;

bool alarmActive = false;
bool buzzerOn = false;
unsigned long alarmStartedAt = 0;

struct Button {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long changedAt;
};

Button modeButton = {MODE_BUTTON_PIN, HIGH, HIGH, 0};
Button itemButton = {ITEM_BUTTON_PIN, HIGH, HIGH, 0};

#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
bool pressed(Button &button, unsigned long now);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t calibratedPercent(int raw, int zeroRaw, int fullRaw);
#line 139 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
int readAveraged(uint8_t pin);
#line 149 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void reportFault(bool fault, int8_t &previous, const __FlashStringHelper *name, const __FlashStringHelper *reason);
#line 168 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAnalogFault(int raw, uint8_t &badCount, bool &fault);
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void readSensors(unsigned long now);
#line 233 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void printSelection();
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtons(unsigned long now);
#line 276 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void updatePump(bool requested, unsigned long now);
#line 301 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void updateOutputs(unsigned long now);
#line 354 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarm(unsigned long now);
#line 374 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(unsigned long now);
#line 423 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 458 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_001\\direct\\attempt_0\\sketch\\sketch.ino"
bool pressed(Button &button, unsigned long now) {
  uint8_t reading = digitalRead(button.pin);

  if (reading != button.rawState) {
    button.rawState = reading;
    button.changedAt = now;
  }

  if (reading != button.stableState &&
      now - button.changedAt >= DEBOUNCE_MS) {
    button.stableState = reading;
    return reading == LOW;
  }
  return false;
}

uint8_t calibratedPercent(int raw, int zeroRaw, int fullRaw) {
  long value = map(raw, zeroRaw, fullRaw, 0, 100);
  return (uint8_t)constrain(value, 0L, 100L);
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

void reportFault(bool fault, int8_t &previous,
                 const __FlashStringHelper *name,
                 const __FlashStringHelper *reason) {
  if (previous == (int8_t)fault) {
    return;
  }

  previous = fault;
  if (fault) {
    Serial.print(F("FAULT: "));
    Serial.print(name);
    Serial.print(F(" - "));
    Serial.println(reason);
  } else {
    Serial.print(F("OK: "));
    Serial.println(name);
  }
}

void updateAnalogFault(int raw, uint8_t &badCount, bool &fault) {
  if (raw <= ADC_FAULT_LOW || raw >= ADC_FAULT_HIGH) {
    if (badCount < ANALOG_BAD_SAMPLES) {
      ++badCount;
    }
    if (badCount >= ANALOG_BAD_SAMPLES) {
      fault = true;
    }
  } else {
    badCount = 0;
    fault = false;
  }
}

void readSensors(unsigned long now) {
  if (now - lastAnalogRead >= ANALOG_INTERVAL_MS) {
    lastAnalogRead = now;

    lightRaw = readAveraged(LIGHT_PIN);
    soilRaw = readAveraged(SOIL_PIN);

    updateAnalogFault(lightRaw, lightBadCount, lightFault);
    updateAnalogFault(soilRaw, soilBadCount, soilFault);

    if (!lightFault) {
      lightPercent = calibratedPercent(
          lightRaw, LIGHT_DARK_RAW, LIGHT_BRIGHT_RAW);
    }
    if (!soilFault) {
      soilPercent = calibratedPercent(
          soilRaw, SOIL_DRY_RAW, SOIL_WET_RAW);
    }

    // Wait for confirmation before reporting an initial rail fault.
    if (!lightFault || lightBadCount >= ANALOG_BAD_SAMPLES) {
      reportFault(lightFault, reportedLightFault, F("LIGHT A0"),
                  F("persistent ADC rail reading; check sensor/wiring"));
    }
    if (!soilFault || soilBadCount >= ANALOG_BAD_SAMPLES) {
      reportFault(soilFault, reportedSoilFault, F("SOIL A1"),
                  F("persistent ADC rail reading; check sensor/wiring"));
    }
  }

  if (now - lastDhtRead >= DHT_INTERVAL_MS) {
    lastDhtRead = now;

    float newHumidity = dht.readHumidity();
    float newTemperature = dht.readTemperature();

    temperatureFault =
        isnan(newTemperature) || isnan(newHumidity) ||
        newTemperature < 0.0f || newTemperature > 50.0f ||
        newHumidity < 0.0f || newHumidity > 100.0f;

    if (!temperatureFault) {
      temperatureC = newTemperature;
      humidity = newHumidity;
    }

    reportFault(temperatureFault, reportedDhtFault, F("DHT11 D7"),
                F("read failed or value outside DHT11 range"));
  }
}

void printSelection() {
  Serial.print(F("MANUAL target: "));
  switch (selectedItem) {
    case ITEM_FAN:  Serial.println(F("FAN PWM")); break;
    case ITEM_LED:  Serial.println(F("LED PWM")); break;
    case ITEM_PUMP: Serial.println(F("PUMP relay")); break;
  }
}

void handleButtons(unsigned long now) {
  if (pressed(modeButton, now)) {
    manualMode = !manualMode;
    Serial.println(manualMode ? F("MODE: MANUAL") : F("MODE: AUTO"));
    if (manualMode) {
      printSelection();
    }
  }

  if (pressed(itemButton, now) && manualMode) {
    selectedItem = (ControlItem)((selectedItem + 1) % 3);
    printSelection();
  }

  if (manualMode) {
    int pot = readAveraged(POT_PIN);
    uint8_t output = (uint8_t)map(pot, 0, 1023, 0, 255);

    switch (selectedItem) {
      case ITEM_FAN:
        manualFan = output;
        break;
      case ITEM_LED:
        manualLED = output;
        break;
      case ITEM_PUMP:
        // Hysteresis prevents relay chatter near the midpoint.
        if (pot >= 560) manualPump = true;
        if (pot <= 460) manualPump = false;
        break;
    }
  }
}

void updatePump(bool requested, unsigned long now) {
  if (pumpRunning &&
      (!requested || now - pumpStartedAt >= PUMP_MAX_RUN_MS)) {
    bool timedOut = requested;
    digitalWrite(PUMP_PIN, RELAY_OFF_LEVEL);
    pumpRunning = false;
    pumpResting = true;
    pumpStoppedAt = now;

    if (timedOut) {
      Serial.println(F("PUMP: maximum run reached; cooling/rest interval"));
    }
  }

  if (pumpResting && now - pumpStoppedAt >= PUMP_REST_MS) {
    pumpResting = false;
  }

  if (requested && !pumpRunning && !pumpResting) {
    pumpRunning = true;
    pumpStartedAt = now;
    digitalWrite(PUMP_PIN, RELAY_ON_LEVEL);
  }
}

void updateOutputs(unsigned long now) {
  uint8_t automaticFanPWM = 0;
  uint8_t automaticLEDPWM = 0;

  if (temperatureFault) {
    autoFanOn = false;
  } else {
    if (temperatureC >= FAN_START_C) autoFanOn = true;
    if (temperatureC <= FAN_STOP_C) autoFanOn = false;

    if (autoFanOn) {
      float fraction =
          (temperatureC - FAN_STOP_C) / (FAN_FULL_C - FAN_STOP_C);
      fraction = constrain(fraction, 0.0f, 1.0f);
      automaticFanPWM =
          FAN_MIN_PWM + (uint8_t)((255 - FAN_MIN_PWM) * fraction);
    }
  }

  if (lightFault) {
    autoLEDOn = false;
  } else {
    if (lightPercent <= LIGHT_ON_BELOW_PERCENT) autoLEDOn = true;
    if (lightPercent >= LIGHT_OFF_ABOVE_PERCENT) autoLEDOn = false;

    if (autoLEDOn) {
      automaticLEDPWM = (uint8_t)map(
          lightPercent, 0, LIGHT_OFF_ABOVE_PERCENT, 255, 80);
    }
  }

  if (soilFault) {
    autoPumpOn = false;
  } else {
    if (soilPercent <= SOIL_PUMP_ON_PERCENT) autoPumpOn = true;
    if (soilPercent >= SOIL_PUMP_OFF_PERCENT) autoPumpOn = false;
  }

  uint8_t fanPWM = manualMode ? manualFan : automaticFanPWM;
  uint8_t ledPWM = manualMode ? manualLED : automaticLEDPWM;
  bool pumpRequest = manualMode ? manualPump : autoPumpOn;

  // Failed sensor channels force their corresponding actuators OFF,
  // including in manual mode. Other healthy channels remain operational.
  if (temperatureFault) fanPWM = 0;
  if (lightFault) ledPWM = 0;
  if (soilFault) pumpRequest = false;

  analogWrite(FAN_PIN, fanPWM);
  analogWrite(LED_PIN, ledPWM);
  updatePump(pumpRequest, now);
}

void updateAlarm(unsigned long now) {
  bool fault = temperatureFault || lightFault || soilFault;

  if (fault && !alarmActive) {
    alarmStartedAt = now;
  }
  alarmActive = fault;

  // Nonblocking 200 ms beep once per second while any fault persists.
  bool shouldSound = fault && ((now - alarmStartedAt) % 1000UL < 200UL);
  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) {
      tone(BUZZER_PIN, 2200);
    } else {
      noTone(BUZZER_PIN);
    }
  }
}

void updateDisplay(unsigned long now) {
  if (now - lastDisplayPage >= DISPLAY_INTERVAL_MS) {
    lastDisplayPage = now;
    displayPage = (displayPage + 1) % 3;
  }

  if (now - lastDisplayRefresh < DISPLAY_REFRESH_MS) {
    return;
  }
  lastDisplayRefresh = now;

  uint8_t segments[4] = {0, 0, 0, 0};
  bool fault;
  unsigned int value;

  switch (displayPage) {
    case 0:
      segments[0] = 0x78; // t: temperature, e.g. t25.0
      fault = temperatureFault;
      value = fault ? 0 : (unsigned int)(temperatureC * 10.0f + 0.5f);
      break;
    case 1:
      segments[0] = 0x38; // L: calibrated light percentage
      fault = lightFault;
      value = lightPercent;
      break;
    default:
      segments[0] = 0x6D; // S: calibrated soil moisture percentage
      fault = soilFault;
      value = soilPercent;
      break;
  }

  if (fault) {
    segments[1] = segments[2] = segments[3] = 0x40; // ---
  } else {
    segments[1] = value >= 100 ? display.encodeDigit(value / 100) : 0;
    segments[2] = (value >= 10 || displayPage == 0)
                      ? display.encodeDigit((value / 10) % 10) : 0;
    segments[3] = display.encodeDigit(value % 10);

    if (displayPage == 0) {
      segments[2] |= 0x80; // Decimal point: tenths of a degree.
    }
  }

  display.setSegments(segments);
}

void setup() {
  // Set safe latch levels before enabling actuator outputs.
  digitalWrite(PUMP_PIN, RELAY_OFF_LEVEL);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ITEM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(SOIL_PIN, INPUT);
  pinMode(POT_PIN, INPUT);

  Serial.begin(9600);
  dht.begin();
  display.setBrightness(4);
  display.clear();

  unsigned long now = millis();
  lastDhtRead = now; // Allow DHT11 to stabilize before its first reading.
  lastAnalogRead = now - ANALOG_INTERVAL_MS;
  lastDisplayPage = now;
  lastDisplayRefresh = now - DISPLAY_REFRESH_MS;

  Serial.println(F("Greenhouse controller ready. MODE: AUTO"));
  Serial.println(F("D2: AUTO/MANUAL; D3: manual target; A2: output"));
  Serial.println(F("Display: t=temperature C, L=light %, S=soil moisture %"));
  Serial.println(F("Sensor initialization: outputs disabled until valid."));
}

void loop() {
  unsigned long now = millis();
  handleButtons(now);
  readSensors(now);
  updateOutputs(now);
  updateAlarm(now);
  updateDisplay(now);
}
