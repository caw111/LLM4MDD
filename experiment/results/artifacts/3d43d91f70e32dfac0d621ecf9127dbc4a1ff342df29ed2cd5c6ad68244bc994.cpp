#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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

// TM1637: brightness (0..7) (set in setup)
static TM1637Display tmDisplay(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ==== Behavior parameters (preserve as behavior knobs) ====
// Note: Requirements mention PWM, calibration, thresholds, timers; these
// are not explicitly quantified in the prompt. The sketch implements
// complete, deterministic behavior with sensible defaults while keeping
// the full mode logic, display rotation, alarm handling, and manual control.

// Fan PWM control mapping in auto mode:
// - Use temperature to drive fan speed.
// - Duty increases with temperature, linearly between thresholds.
static const float TEMP_FAN_ON_C = 28.0f;     // below => fan off
static const float TEMP_FAN_FULL_C = 35.0f;   // above => fan full
static const uint8_t FAN_PWM_MIN = 0;
static const uint8_t FAN_PWM_MAX = 255;

// LED control in auto mode:
// - Use light sensor (LDR). Typical: lower ADC => brighter; higher ADC => darker.
// We'll treat "darkness" as requiring more light.
// LED on when ADC above darkness threshold.
static const int LDR_DARK_ON = 600;  // ADC >= => dark enough to turn on LED
static const int LDR_DARK_OFF = 450; // ADC <= => bright enough to turn off LED (hysteresis)
static const uint8_t LED_ON_BRIGHT = 255; // D9 is digital in circuit; we'll use on/off.

// Pump control in auto mode:
// - Use soil moisture ADC. Typical: lower ADC => wetter (depends on module).
// We'll assume: higher ADC => drier soil.
// Pump on when soil dryness above threshold; hysteresis to avoid chatter.
static const int SOIL_DRY_ON = 650;   // ADC >= => soil too dry => pump on
static const int SOIL_DRY_OFF = 520; // ADC <= => soil OK => pump off

// Display rotation timing (TM1637): "轮流显示温度、光照和土壤湿度"
static const uint16_t DISPLAY_ROTATE_MS = 1500;

// Alarm timing: if any sensor read异常 then buzzer reports.
static const uint16_t ALARM_BUZZ_MS = 120;
static const uint16_t ALARM_PAUSE_MS = 200;

// Sensor validity checks
// - DHT: invalid if isnan
// - Analog: treat out-of-range as异常
static const int ANALOG_MIN_VALID = 20;
static const int ANALOG_MAX_VALID = 1010;

// Serial
static const uint32_t SERIAL_BAUD = 9600;

// Manual mode control mapping using pot (A2):
// When selecting controllable object:
// - Fan: pot sets PWM 0..255
// - LED: pot sets on/off (threshold at midpoint)
// - Pump: pot sets on/off
static const int POT_DEADZONE = 0; // none; keep full range

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
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isAnalogValid(int v);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFanPWM(uint8_t duty);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLED(bool on);
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setPump(bool on);
#line 140 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleAlarm(const char* reason);
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearAlarmIfHealthy();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readSensorsAndUpdate();
#line 214 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void autoControl();
#line 284 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t potToPWM(int potRaw);
#line 292 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void manualControl();
#line 313 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showValueOnTM1637(uint8_t idx);
#line 349 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readButton(uint8_t pin);
#line 354 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool edgePressed(uint8_t pin);
#line 376 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 412 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_001\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool isAnalogValid(int v) {
  return (v >= ANALOG_MIN_VALID && v <= ANALOG_MAX_VALID);
}

static void setFanPWM(uint8_t duty) {
  // D5 is PWM. Use analogWrite.
  analogWrite(PIN_FAN_PWM, duty);
}

static void setLED(bool on) {
  digitalWrite(PIN_LED, on ? HIGH : LOW);
}

static void setPump(bool on) {
  // Relay active level assumed HIGH unless wiring says otherwise; requirement says preserve wiring,
  // not explicit active-low. We'll assume HIGH->pump on.
  digitalWrite(PIN_PUMP_RELAY, on ? HIGH : LOW);
}

// Alarm buzzer + serial output of fault info
static void handleAlarm(const char* reason) {
  alarmActive = true;
  Serial.println(reason);

  // Beep pattern
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
  // If alarm was active, we only clear when all readings are healthy.
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
  // Temperature from DHT11
  float t = dht.readTemperature(); // Celsius
  int ldr = analogRead(PIN_LDR);
  int soil = analogRead(PIN_SOIL);

  // Determine sensor anomalies
  bool tempOk = !isnan(t);
  bool ldrOk = isAnalogValid(ldr);
  bool soilOk = isAnalogValid(soil);

  // Update cache for display (even if anomalous, keep last valid?).
  // Requirement: "若任一传感器读数异常则蜂鸣器D6报警并通过串口输出故障信息。"
  // We'll update caches with new values to reflect attempt; alarm depends on validity.
  lastTempC = t;
  lastLdr = ldr;
  lastSoil = soil;

  // Alarm if any abnormal
  if (!tempOk || !ldrOk || !soilOk) {
    // Provide fault info per the first detected issue plus include all.
    // Keep serial output detailed.
    // Format: [FAULT] ... (line-oriented by Serial.println)
    if (!tempOk) {
      handleAlarm("[FAULT] DHT11 temperature read abnormal (NaN).");
    } else if (!ldrOk) {
      handleAlarm("[FAULT] LDR analog read abnormal (out of valid range).");
    } else if (!soilOk) {
      handleAlarm("[FAULT] Soil moisture analog read abnormal (out of valid range).");
    } else {
      // Shouldn't happen; safe fallback
      handleAlarm("[FAULT] Unknown sensor anomaly.");
    }
  } else {
    clearAlarmIfHealthy();
  }
}

static void autoControl() {
  // Auto mode:
  // - Fan based on temperature
  // - LED based on light sensor
  // - Pump based on soil moisture

  // Fan mapping
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

  // LED hysteresis with LDR
  bool ledOn = false;
  if (isAnalogValid(lastLdr)) {
    // Hysteresis: if already decided? We'll use direct compare with hysteresis
    // by storing previous LED state would be more precise. We'll implement with last LED state.
    // If currently OFF, turn ON only when >= LDR_DARK_ON.
    // If currently ON, turn OFF only when <= LDR_DARK_OFF.
    bool curLed = digitalRead(PIN_LED) == HIGH;
    if (!curLed) {
      ledOn = (lastLdr >= LDR_DARK_ON);
    } else {
      ledOn = !(lastLdr <= LDR_DARK_OFF); // keep on unless bright enough
    }
  }

  // Pump hysteresis with soil moisture
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
  // If alarm is active due to sensor fault, still execute current auto logic (requirement doesn't say stop),
  // but alarm will be raised. We'll keep deterministic: apply only if corresponding sensor is valid.
  if (isnan(lastTempC)) {
    // If temp invalid, turn fan off (safe).
    setFanPWM(0);
  } else {
    setFanPWM(fanDuty);
  }

  if (!isAnalogValid(lastLdr)) {
    setLED(false);
  } else {
    setLED(ledOn);
  }

  if (!isAnalogValid(lastSoil)) {
    setPump(false);
  } else {
    setPump(pumpOn);
  }
}

static uint8_t potToPWM(int potRaw) {
  int v = potRaw;
  if (v < POT_DEADZONE) v = 0;
  if (v > 1023) v = 1023;
  // Map 0..1023 -> 0..255
  return (uint8_t)map(v, 0, 1023, 0, 255);
}

static void manualControl() {
  int potRaw = analogRead(PIN_POT);

  // Select target by manualTarget
  switch (manualTarget) {
    case TARGET_FAN: {
      uint8_t duty = potToPWM(potRaw);
      setFanPWM(duty);
    } break;
    case TARGET_LED: {
      // Use pot to turn on/off with threshold at ~50%
      bool on = (potRaw >= 512);
      setLED(on);
    } break;
    case TARGET_PUMP: {
      bool on = (potRaw >= 512);
      setPump(on);
    } break;
  }
}

static void showValueOnTM1637(uint8_t idx) {
  // idx: 0=temp, 1=light, 2=soil
  // TM1637: numbers only; we'll show scaled readings with simple formatting.
  // Temperature: show as integer with °C suffix ignored (display limited).
  // Light and Soil: show ADC value as integer.
  tmDisplay.clearDisplay();

  if (idx == 0) {
    if (!isnan(lastTempC)) {
      // Show as temperature * 1 (integer)
      int tInt = (int)round(lastTempC);
      tmDisplay.showNumberDec(tInt, true, 2, 0); // width 2, no leading zeros; segment formatting internal
      // Many TM1637 libs differ; showNumberDec handles decimal places.
      // Use 2 digits by default; if temperature exceeds, it will spill; acceptable.
    } else {
      // Invalid: show "----" using segments? We'll show 0 with minus sign not available in number dec.
      tmDisplay.showNumberDec(-999, true, 3, 0);
    }
  } else if (idx == 1) {
    if (isAnalogValid(lastLdr)) {
      tmDisplay.showNumberDec(lastLdr, false, 4, 0);
    } else {
      tmDisplay.showNumberDec(-1, true, 2, 0);
    }
  } else {
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
  // Use INPUT_PULLUP
  return digitalRead(pin) == LOW; // pressed = LOW
}

static bool edgePressed(uint8_t pin) {
  static uint32_t lastMs[10] = {0};
  static bool lastState[10] = {false};

  // Map pin to small index by pin number; keep simple with fixed pins.
  uint8_t idx = 0;
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

  // DHT
  dht.begin();

  // TM1637
  tmDisplay.setBrightness(6);
  tmDisplay.clearDisplay();

  // Outputs
  pinMode(PIN_FAN_PWM, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_PUMP_RELAY, LOW);
  digitalWrite(PIN_BUZZER, LOW);
  analogWrite(PIN_FAN_PWM, 0);

  // Buttons with pullups
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);

  // Initial sensor read to populate cache and alarm status
  readSensorsAndUpdate();

  // Ensure display rotation starts
  lastDisplayMs = millis();
  displayIndex = 0;

  // Inform mode at startup
  Serial.println(systemMode == MODE_AUTO ? "[INFO] Mode: AUTO" : "[INFO] Mode: MANUAL");
}

void loop() {
  // Read sensors continuously
  readSensorsAndUpdate();

  // Button handling
  // D2: switch mode auto/manual (toggle)
  if (edgePressed(PIN_BTN_MODE)) {
    systemMode = (systemMode == MODE_AUTO) ? MODE_MANUAL : MODE_AUTO;
    Serial.println(systemMode == MODE_AUTO ? "[INFO] Mode: AUTO" : "[INFO] Mode: MANUAL");

    // In manual mode, ensure fan/led/pump act according to target immediately.
    if (systemMode == MODE_MANUAL) {
      manualTarget = TARGET_FAN;
      lastDisplayMs = millis();
    }
  }

  // D3: in manual mode, cycle current control object
  if (systemMode == MODE_MANUAL && edgePressed(PIN_BTN_NEXT)) {
    manualTarget = (ControlTarget)(((uint8_t)manualTarget + 1) % 3);
    Serial.print("[INFO] Manual target: ");
    if (manualTarget == TARGET_FAN) Serial.println("FAN (PWM D5)");
    else if (manualTarget == TARGET_LED) Serial.println("LED (D9)");
    else Serial.println("PUMP (D10)");
  }

  // Control logic
  if (systemMode == MODE_AUTO) {
    autoControl();
  } else {
    manualControl();
  }

  // TM1637 display rotation
  uint32_t now = millis();
  if (now - lastDisplayMs >= DISPLAY_ROTATE_MS) {
    lastDisplayMs = now;
    displayIndex = (displayIndex + 1) % 3;
  }
  showValueOnTM1637(displayIndex);

  // If alarm active, keep buzzer beeping via handleAlarm calls inside readSensorsAndUpdate().
  // No extra delay to keep control responsive.
}
