#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// ==== Pin assignments (authoritative) ====
// DHT11 (D7)
static const uint8_t PIN_DHT = 7;
// Light sensor (LDR) (A0)
static const uint8_t PIN_LDR = A0;
// Soil moisture sensor (A1)
static const uint8_t PIN_SOIL = A1;
// PWM fan output (D5)
static const uint8_t PIN_FAN_PWM = 5;
// Supplement light LED (D9)
static const uint8_t PIN_LED = 9;
// Water pump relay (D10)
static const uint8_t PIN_PUMP_RELAY = 10;
// Alarm buzzer (D6)
static const uint8_t PIN_BUZZER = 6;
// Buttons: D2, D3
static const uint8_t PIN_BTN_MODE = 2;     // mode: auto/manual (toggle or set)
static const uint8_t PIN_BTN_NEXT = 3;     // manual: select controllable object
// Potentiometer output (A2) to adjust manual output
static const uint8_t PIN_POT = A2;

// TM1637 CLK=D4, DIO=D8
static const uint8_t PIN_TM1637_CLK = 4;
static const uint8_t PIN_TM1637_DIO = 8;

// ==== Sensors / displays ====
// DHT type: DHT11
static DHT dht(PIN_DHT, DHT11);

// TM1637
static TM1637Display tmDisplay(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ==== Behavior parameters (knobs) ====
// Fan mapping in auto mode
static const float TEMP_FAN_ON_C = 28.0f;     // below => fan off
static const float TEMP_FAN_FULL_C = 35.0f;   // above => fan full
static const uint8_t FAN_PWM_MIN = 0;
static const uint8_t FAN_PWM_MAX = 255;

// LED control in auto mode with hysteresis (using digital state as memory)
static const int LDR_DARK_ON = 600;  // ADC >= => dark enough to turn on LED
static const int LDR_DARK_OFF = 450; // ADC <= => bright enough to turn off LED

// Pump control in auto mode with hysteresis (using relay state as memory)
static const int SOIL_DRY_ON = 650;   // ADC >= => soil too dry => pump on
static const int SOIL_DRY_OFF = 520; // ADC <= => soil OK => pump off

// Display rotation: temperature, light, soil
static const uint16_t DISPLAY_ROTATE_MS = 1500;

// Alarm timing: if any sensor read异常则报警
static const uint16_t ALARM_BUZZ_MS = 120;
static const uint16_t ALARM_PAUSE_MS = 200;

// Sensor validity checks
static const int ANALOG_MIN_VALID = 20;
static const int ANALOG_MAX_VALID = 1010;

// Serial
static const uint32_t SERIAL_BAUD = 9600;

// Manual mapping via potentiometer A2
static const int POT_DEADZONE = 0;

// Manual select enumeration
enum ControlTarget : uint8_t {
  TARGET_FAN = 0,
  TARGET_LED = 1,
  TARGET_PUMP = 2
};

enum SystemMode : uint8_t {
  MODE_AUTO = 0,
  MODE_MANUAL = 1
};

// ==== State variables ====
static SystemMode systemMode = MODE_AUTO;
static ControlTarget manualTarget = TARGET_FAN;

// Display rotation variables
static uint32_t lastDisplayMs = 0;
static uint8_t displayIndex = 0;

// Cached sensor values
static float lastTempC = NAN;
static int lastLdr = -1;
static int lastSoil = -1;

// Alarm state
static bool alarmActive = false;
static uint32_t lastAlarmToggleMs = 0;
static bool buzzerOn = false;

// ==== Helper functions ====
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isAnalogValid(int v);
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setFanPWM(uint8_t duty);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setLED(bool on);
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void setPump(bool on);
#line 116 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void handleAlarm(const char* reason);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void clearAlarmIfHealthy();
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readSensorsAndUpdate();
#line 178 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void autoControl();
#line 227 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static uint8_t potToPWM(int potRaw);
#line 234 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void manualControl();
#line 253 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void displayOff();
#line 261 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showValueOnTM1637(uint8_t idx);
#line 287 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool readButton(uint8_t pin);
#line 291 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool edgePressed(uint8_t pin);
#line 313 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 342 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool isAnalogValid(int v) {
  return (v >= ANALOG_MIN_VALID && v <= ANALOG_MAX_VALID);
}

static void setFanPWM(uint8_t duty) {
  analogWrite(PIN_FAN_PWM, duty);
}

static void setLED(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static void setPump(bool on) {
  digitalWrite(PIN_PUMP_RELAY, on ? HIGH : LOW);
}

static void handleAlarm(const char* reason) {
  alarmActive = true;
  Serial.println(reason);

  uint32_t now = millis();
  if (buzzerOn) {
    if (now - lastAlarmToggleMs >= ALARM_BUZZ_MS) {
      buzzerOn = false;
      digitalWrite(PIN_BUZZER, LOW);
      lastAlarmToggleMs = now;
    }
  } else {
    if (now - lastAlarmToggleMs >= ALARM_PAUSE_MS) {
      buzzerOn = true;
      digitalWrite(PIN_BUZZER, HIGH);
      lastAlarmToggleMs = now;
    }
  }
}

static void clearAlarmIfHealthy() {
  if (!alarmActive) return;

  bool tempOk = !isnan(lastTempC);
  bool ldrOk = isAnalogValid(lastLdr);
  bool soilOk = isAnalogValid(lastSoil);

  if (tempOk && ldrOk && soilOk) {
    alarmActive = false;
    digitalWrite(PIN_BUZZER, LOW);
    buzzerOn = false;
  }
}

static void readSensorsAndUpdate() {
  float t = dht.readTemperature(); // Celsius
  int ldr = analogRead(PIN_LDR);
  int soil = analogRead(PIN_SOIL);

  bool tempOk = !isnan(t);
  bool ldrOk = isAnalogValid(ldr);
  bool soilOk = isAnalogValid(soil);

  lastTempC = t;
  lastLdr = ldr;
  lastSoil = soil;

  if (!tempOk || !ldrOk || !soilOk) {
    if (!tempOk) {
      handleAlarm("[FAULT] DHT11 temperature read abnormal (NaN).");
    } else if (!ldrOk) {
      handleAlarm("[FAULT] LDR analog read abnormal (out of valid range).");
    } else if (!soilOk) {
      handleAlarm("[FAULT] Soil moisture analog read abnormal (out of valid range).");
    } else {
      handleAlarm("[FAULT] Unknown sensor anomaly.");
    }
  } else {
    clearAlarmIfHealthy();
  }
}

static void autoControl() {
  // Fan duty based on temperature
  uint8_t fanDuty = 0;
  if (!isnan(lastTempC)) {
    if (lastTempC <= TEMP_FAN_ON_C) {
      fanDuty = FAN_PWM_MIN;
    } else if (lastTempC >= TEMP_FAN_FULL_C) {
      fanDuty = FAN_PWM_MAX;
    } else {
      float frac = (lastTempC - TEMP_FAN_ON_C) / (TEMP_FAN_FULL_C - TEMP_FAN_ON_C);
      if (frac < 0) frac = 0;
      if (frac > 1) frac = 1;
      fanDuty = (uint8_t)round((float)(FAN_PWM_MIN) + frac * (float)(FAN_PWM_MAX - FAN_PWM_MIN));
    }
  }

  // LED hysteresis with LDR and stored digital state
  bool ledOn = false;
  if (isAnalogValid(lastLdr)) {
    bool curLed = digitalRead(PIN_LED) == HIGH;
    if (!curLed) {
      ledOn = (lastLdr >= LDR_DARK_ON);
    } else {
      ledOn = !(lastLdr <= LDR_DARK_OFF);
    }
  }

  // Pump hysteresis with relay state
  bool pumpOn = false;
  if (isAnalogValid(lastSoil)) {
    bool curPump = digitalRead(PIN_PUMP_RELAY) == HIGH;
    if (!curPump) {
      pumpOn = (lastSoil >= SOIL_DRY_ON);
    } else {
      pumpOn = !(lastSoil <= SOIL_DRY_OFF);
    }
  }

  // Apply controls
  if (isnan(lastTempC)) setFanPWM(0);
  else setFanPWM(fanDuty);

  if (!isAnalogValid(lastLdr)) setLED(false);
  else setLED(ledOn);

  if (!isAnalogValid(lastSoil)) setPump(false);
  else setPump(pumpOn);
}

static uint8_t potToPWM(int potRaw) {
  int v = potRaw;
  if (v < POT_DEADZONE) v = 0;
  if (v > 1023) v = 1023;
  return (uint8_t)map(v, 0, 1023, 0, 255);
}

static void manualControl() {
  int potRaw = analogRead(PIN_POT);

  switch (manualTarget) {
    case TARGET_FAN: {
      uint8_t duty = potToPWM(potRaw);
      setFanPWM(duty);
    } break;
    case TARGET_LED: {
      bool on = (potRaw >= 512);
      setLED(on);
    } break;
    case TARGET_PUMP: {
      bool on = (potRaw >= 512);
      setPump(on);
    } break;
  }
}

static void displayOff() {
  // TM1637Display library used here may not have clearDisplay().
  // Use showNumberDec with 0.. and turn off colon/segments via leading zeros flag.
  // Easiest: show blank by writing 0 and then setting all digits? Not available.
  // We'll simply show 0 with leading zeros off; this avoids compilation errors.
  tmDisplay.showNumberDec(0, true, 2, 0);
}

static void showValueOnTM1637(uint8_t idx) {
  // Library variant: no clearDisplay(). We'll overwrite each update.
  if (idx == 0) { // temperature
    if (!isnan(lastTempC)) {
      int tInt = (int)round(lastTempC);
      tmDisplay.showNumberDec(tInt, true, 2, 0);
    } else {
      tmDisplay.showNumberDec(-999, true, 3, 0);
    }
  } else if (idx == 1) { // light
    if (isAnalogValid(lastLdr)) {
      tmDisplay.showNumberDec(lastLdr, false, 4, 0);
    } else {
      tmDisplay.showNumberDec(-1, true, 2, 0);
    }
  } else { // soil
    if (isAnalogValid(lastSoil)) {
      tmDisplay.showNumberDec(lastSoil, false, 4, 0);
    } else {
      tmDisplay.showNumberDec(-1, true, 2, 0);
    }
  }
  tmDisplay.setBrightness(6);
}

// ==== Buttons handling ====
static bool readButton(uint8_t pin) {
  return digitalRead(pin) == LOW; // pressed = LOW (INPUT_PULLUP)
}

static bool edgePressed(uint8_t pin) {
  static uint32_t lastMs[3] = {0, 0, 0};
  static bool lastState[3] = {false, false, false};

  uint8_t idx;
  if (pin == PIN_BTN_MODE) idx = 0;
  else if (pin == PIN_BTN_NEXT) idx = 1;
  else idx = 2;

  bool cur = readButton(pin);
  bool ret = false;
  uint32_t now = millis();

  if (cur && !lastState[idx] && (now - lastMs[idx] > 200)) {
    ret = true;
    lastMs[idx] = now;
  }
  lastState[idx] = cur;
  return ret;
}

// ==== setup/loop ====
void setup() {
  Serial.begin(SERIAL_BAUD);

  dht.begin();

  tmDisplay.setBrightness(6);
  displayOff();

  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_PUMP_RELAY, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  analogWrite(PIN_FAN_PWM, 0);

  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);

  readSensorsAndUpdate();

  lastDisplayMs = millis();
  displayIndex = 0;

  Serial.println(systemMode == MODE_AUTO ? "[INFO] Mode: AUTO" : "[INFO] Mode: MANUAL");
}

void loop() {
  readSensorsAndUpdate();

  // D2: switch auto/manual
  if (edgePressed(PIN_BTN_MODE)) {
    systemMode = (systemMode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
    Serial.println(systemMode == MODE_AUTO ? "[INFO] Mode: AUTO" : "[INFO] Mode: MANUAL");

    if (systemMode == MODE_MANUAL) {
      manualTarget = TARGET_FAN;
      lastDisplayMs = millis();
    }
  }

  // D3: cycle manual control target
  if (systemMode == MODE_MANUAL && edgePressed(PIN_BTN_NEXT)) {
    manualTarget = (ControlTarget)(((uint8_t)manualTarget + 1) % 3);
    Serial.print("[INFO] Manual target: ");
    if (manualTarget == TARGET_FAN) Serial.println("FAN (PWM D5)");
    else if (manualTarget == TARGET_LED) Serial.println("LED (D9)");
    else Serial.println("PUMP (D10)");
  }

  if (systemMode == MODE_AUTO) autoControl();
  else manualControl();

  uint32_t now = millis();
  if (now - lastDisplayMs >= DISPLAY_ROTATE_MS) {
    lastDisplayMs = now;
    displayIndex = (displayIndex + 1) % 3;
  }
  showValueOnTM1637(displayIndex);
}
