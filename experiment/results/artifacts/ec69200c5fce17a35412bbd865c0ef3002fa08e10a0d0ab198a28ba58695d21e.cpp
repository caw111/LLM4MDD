#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

/////////////////////////////////////////////////////////////////
// Pin assignments (as required; preserve authoritative remap)
/////////////////////////////////////////////////////////////////
static const uint8_t PIN_LM35_ENV = A0;          // 环境温度
static const uint8_t PIN_LM35_EVA = A1;          // 蒸发器温度

static const uint8_t PIN_DOOR_MAG = 2;          // D2 门磁
static const uint8_t PIN_FAN = 5;               // D5 风扇
static const uint8_t PIN_DEFROST_RELAY = 10;   // D10 除霜继电器
static const uint8_t PIN_BUZZER = 6;            // D6 蜂鸣器

static const uint8_t PIN_TM_CLK = 4;            // D4 TM1637 CLK
static const uint8_t PIN_TM_DIO = 3;            // D3 TM1637 DIO

/////////////////////////////////////////////////////////////////
// TM1637
/////////////////////////////////////////////////////////////////
TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

/////////////////////////////////////////////////////////////////
// LM35 conversion
/////////////////////////////////////////////////////////////////
static const float LM35_VREF = 5.0f;     // UNO ADC ref assumed 5V
static const float LM35_SCALE = 100.0f; // 10mV/°C => (V * 100) = °C
static const uint8_t ADC_SAMPLES = 10;

#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static float readLM35C(uint8_t analogPin);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool readDoorOpenRaw();
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static const char * alarmLevelToStr(AlarmLevel lv);
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void setFan(bool on);
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void setDefrostRelay(bool on);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void buzzerBeep(uint16_t freq, uint16_t msOn, uint16_t msOff, uint8_t count);
#line 117 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void computeAlarmLevel(float envC, float evaC, bool doorOpen, uint32_t nowMs);
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void handleBuzzerAndProtection(float envC, float evaC, bool doorOpen);
#line 197 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void updateTM1637(float envC, float evaC, AlarmLevel alm, bool doorOpen, bool defrosting);
#line 221 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void doorControlAndPause(bool doorOpen, float envC, float evaC);
#line 256 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void defrostStateMachine(bool doorOpen, float evaC, float envC);
#line 313 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool doorStateStable(bool raw, bool &prevStable, uint32_t &lastChangeMs);
#line 342 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void serialStatus(float envC, float evaC, bool doorOpen, uint32_t nowMs);
#line 380 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void setup();
#line 400 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static float readLM35C(uint8_t analogPin) {
  uint32_t sum = 0;
  for (uint8_t i = 0; i < ADC_SAMPLES; i++) {
    sum += analogRead(analogPin);
    delay(5);
  }
  float avg = (float)sum / (float)ADC_SAMPLES;
  float voltage = (avg / 1023.0f) * LM35_VREF;
  return voltage * LM35_SCALE;
}

/////////////////////////////////////////////////////////////////
// Door input debouncing
/////////////////////////////////////////////////////////////////
static const uint16_t DOOR_DEBOUNCE_MS = 80;

static bool readDoorOpenRaw() {
  // Magnet wiring assumption: INPUT_PULLUP
  // door open => HIGH, door closed => LOW
  return digitalRead(PIN_DOOR_MAG) == HIGH;
}

/////////////////////////////////////////////////////////////////
// Policy parameters (defaults)
/////////////////////////////////////////////////////////////////
static const float ENV_FAN_ON_TEMP  = 4.0f;   // 环境温度达到此值开始/维持风扇
static const float ENV_FAN_OFF_TEMP = 2.0f;   // 环境温度低于此值停止风扇

static const float EVAP_DEFROST_START_TEMP = -2.0f; // 蒸发器温度低于此值，进入除霜准备
static const float EVAP_DEFROST_STOP_TEMP  = 8.0f;   // 蒸发器温度升到此值，停止除霜
static const uint32_t DEFROST_MIN_TIME_MS = 30UL * 1000UL;
static const uint32_t DEFROST_MAX_TIME_MS = 10UL * 60UL * 1000UL;

// 门长期未关
static const uint32_t DOOR_OPEN_ALARM1_MS = 60UL * 1000UL;
static const uint32_t DOOR_OPEN_ALARM2_MS = 180UL * 1000UL;
static const uint32_t DOOR_OPEN_ALARM3_MS = 300UL * 1000UL;

// 蒸发器结霜严重 - 低温持续时间
static const float EVAP_FROST_SEVERE_TEMP = -5.0f;
static const uint32_t EVAP_FROST_ALARM1_MS = 120UL * 1000UL;
static const uint32_t EVAP_FROST_ALARM2_MS = 240UL * 1000UL;
static const uint32_t EVAP_FROST_ALARM3_MS = 360UL * 1000UL;

// 环境温度上限（多级告警）
static const float ENV_TEMP_UPPER1 = 10.0f;
static const float ENV_TEMP_UPPER2 = 15.0f;
static const float ENV_TEMP_UPPER3 = 20.0f;

/////////////////////////////////////////////////////////////////
// Alarm management
/////////////////////////////////////////////////////////////////
enum AlarmLevel : uint8_t { ALM_NONE = 0, ALM_L1 = 1, ALM_L2 = 2, ALM_L3 = 3 };

static AlarmLevel alarmLevel = ALM_NONE;
static uint32_t doorOpenStartMs = 0;
static bool doorWasOpen = false;

// Frost severity timing
static bool frostBelow = false;
static uint32_t frostStartMs = 0;

static const char* alarmLevelToStr(AlarmLevel lv) {
  switch (lv) {
    case ALM_NONE: return "NONE";
    case ALM_L1: return "LEVEL1";
    case ALM_L2: return "LEVEL2";
    case ALM_L3: return "LEVEL3";
    default: return "UNKNOWN";
  }
}

/////////////////////////////////////////////////////////////////
// Actuators helpers
/////////////////////////////////////////////////////////////////
static void setFan(bool on) { digitalWrite(PIN_FAN, on ? HIGH : LOW); }
static void setDefrostRelay(bool on) { digitalWrite(PIN_DEFROST_RELAY, on ? HIGH : LOW); }

static void buzzerBeep(uint16_t freq, uint16_t msOn, uint16_t msOff, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    tone(PIN_BUZZER, freq);
    delay(msOn);
    noTone(PIN_BUZZER);
    if (i + 1 < count) delay(msOff);
  }
}

static void computeAlarmLevel(float envC, float evaC, bool doorOpen, uint32_t nowMs) {
  AlarmLevel newLevel = ALM_NONE;

  // Environment upper thresholds
  if (envC >= ENV_TEMP_UPPER3) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L3);
  else if (envC >= ENV_TEMP_UPPER2) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L2);
  else if (envC >= ENV_TEMP_UPPER1) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L1);

  // Evaporator severe frost based on continuous low temp time
  if (evaC <= EVAP_FROST_SEVERE_TEMP) {
    if (!frostBelow) {
      frostBelow = true;
      frostStartMs = nowMs;
    }
    uint32_t dur = nowMs - frostStartMs;
    if (dur >= EVAP_FROST_ALARM3_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L3);
    else if (dur >= EVAP_FROST_ALARM2_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L2);
    else if (dur >= EVAP_FROST_ALARM1_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L1);
  } else {
    frostBelow = false;
  }

  // Door open duration alarms
  if (doorOpen) {
    uint32_t dur = nowMs - doorOpenStartMs;
    if (dur >= DOOR_OPEN_ALARM3_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L3);
    else if (dur >= DOOR_OPEN_ALARM2_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L2);
    else if (dur >= DOOR_OPEN_ALARM1_MS) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L1);
  }

  alarmLevel = newLevel;
}

static void handleBuzzerAndProtection(float envC, float evaC, bool doorOpen) {
  (void)envC;
  (void)evaC;

  if (alarmLevel == ALM_NONE) {
    noTone(PIN_BUZZER);
    return;
  }

  if (alarmLevel == ALM_L1) {
    static uint32_t lastToggle = 0;
    static bool on = false;
    const uint32_t period = 1200;
    if (millis() - lastToggle >= (on ? 150 : (period - 150))) {
      lastToggle = millis();
      on = !on;
      if (on) tone(PIN_BUZZER, 2000);
      else noTone(PIN_BUZZER);
    }
  } else if (alarmLevel == ALM_L2) {
    static uint32_t lastToggle2 = 0;
    static bool on2 = false;
    const uint32_t period = 900;
    if (millis() - lastToggle2 >= (on2 ? 220 : (period - 220))) {
      lastToggle2 = millis();
      on2 = !on2;
      if (on2) tone(PIN_BUZZER, 2400);
      else noTone(PIN_BUZZER);
    }
  } else { // ALM_L3
    static uint32_t lastToggle3 = 0;
    static bool on3 = false;
    const uint32_t period = 600;
    if (millis() - lastToggle3 >= (on3 ? 300 : (period - 300))) {
      lastToggle3 = millis();
      on3 = !on3;
      if (on3) tone(PIN_BUZZER, 3000);
      else noTone(PIN_BUZZER);
    }

    // Additional protection actions
    if (!doorOpen) {
      setFan(false);
    }
  }
}

static void updateTM1637(float envC, float evaC, AlarmLevel alm, bool doorOpen, bool defrosting) {
  int16_t tDisp = 0;
  if (alm != ALM_NONE) tDisp = (int16_t)round(envC);
  else tDisp = (int16_t)round(evaC);

  bool show = true;
  if (doorOpen) {
    if ((millis() / 250) % 2 == 1) show = false;
  }
  if (defrosting) {
    if ((millis() / 350) % 2 == 1) show = false;
  }

  if (!show) {
    tm1637.clear();
    return;
  }

  bool neg = (tDisp < 0);
  uint16_t mag = neg ? (uint16_t)(-tDisp) : (uint16_t)tDisp;

  tm1637.showNumberDecEx((int)mag, neg, false, 4);
}

static void doorControlAndPause(bool doorOpen, float envC, float evaC) {
  (void)evaC;
  (void)envC;

  // If door opens: pause fan & defrost relay and remember open time
  if (doorOpen && !doorWasOpen) {
    doorWasOpen = true;
    doorOpenStartMs = millis();
    setFan(false);
    setDefrostRelay(false);
  } else if (!doorOpen && doorWasOpen) {
    doorWasOpen = false;
  }

  if (doorOpen) {
    setFan(false);
    setDefrostRelay(false);
  } else {
    // Fan control by environment temperature, with alarm3 override
    if (alarmLevel == ALM_L3) {
      // keep off
    } else {
      if (envC >= ENV_FAN_ON_TEMP) setFan(true);
      else if (envC <= ENV_FAN_OFF_TEMP) setFan(false);
    }
  }
}

/////////////////////////////////////////////////////////////////
// Defrost state machine
/////////////////////////////////////////////////////////////////
enum DefrostState : uint8_t { DEF_IDLE = 0, DEF_TRIGGERED = 1, DEF_RUNNING = 2, DEF_WAIT_STOP = 3 };
static DefrostState defrostState = DEF_IDLE;
static uint32_t defrostStartMs = 0;

static void defrostStateMachine(bool doorOpen, float evaC, float envC) {
  (void)envC;
  if (doorOpen) {
    setDefrostRelay(false);
    return;
  }

  uint32_t nowMs = millis();

  switch (defrostState) {
    case DEF_IDLE: {
      setDefrostRelay(false);
      if (evaC <= EVAP_DEFROST_START_TEMP) {
        defrostState = DEF_TRIGGERED;
      }
      break;
    }
    case DEF_TRIGGERED: {
      defrostStartMs = nowMs;
      defrostState = DEF_RUNNING;
      setDefrostRelay(true);
      break;
    }
    case DEF_RUNNING: {
      setDefrostRelay(true);

      bool minTimeMet = (nowMs - defrostStartMs) >= DEFROST_MIN_TIME_MS;
      bool stopByTemp = evaC >= EVAP_DEFROST_STOP_TEMP;
      bool maxTimeExceeded = (nowMs - defrostStartMs) >= DEFROST_MAX_TIME_MS;

      if ((minTimeMet && stopByTemp) || maxTimeExceeded) {
        setDefrostRelay(false);
        defrostState = DEF_WAIT_STOP;
        defrostStartMs = nowMs;
      }
      break;
    }
    case DEF_WAIT_STOP: {
      setDefrostRelay(false);

      bool coolEnough = evaC <= (EVAP_DEFROST_START_TEMP + 1.0f);
      bool waitLong = (nowMs - defrostStartMs) >= 30UL * 1000UL;
      if (coolEnough || waitLong) {
        defrostState = DEF_IDLE;
      }
      break;
    }
    default:
      defrostState = DEF_IDLE;
      setDefrostRelay(false);
      break;
  }
}

/////////////////////////////////////////////////////////////////
// Door state stable helper (kept after computeAlarm dependencies compile)
/////////////////////////////////////////////////////////////////
static bool doorStateStable(bool raw, bool &prevStable, uint32_t &lastChangeMs) {
  static bool lastRaw = false;
  static bool init = true;

  if (init) {
    lastRaw = raw;
    prevStable = raw;
    lastChangeMs = millis();
    init = false;
  }

  uint32_t now = millis();
  if (raw != lastRaw) {
    lastRaw = raw;
    lastChangeMs = now;
  }

  if ((now - lastChangeMs) >= DOOR_DEBOUNCE_MS) {
    if (prevStable != raw) prevStable = raw;
  }
  return prevStable;
}

/////////////////////////////////////////////////////////////////
// Serial status
/////////////////////////////////////////////////////////////////
static uint32_t lastStatusMs = 0;
static const uint32_t STATUS_PERIOD_MS = 1000;

static void serialStatus(float envC, float evaC, bool doorOpen, uint32_t nowMs) {
  if (nowMs - lastStatusMs < STATUS_PERIOD_MS) return;
  lastStatusMs = nowMs;

  bool defrosting = (defrostState == DEF_TRIGGERED || defrostState == DEF_RUNNING);

  Serial.print("[STAT] ");
  Serial.print("ENV=");
  Serial.print(envC, 2);
  Serial.print("C, EVA=");
  Serial.print(evaC, 2);
  Serial.print("C, Door=");
  Serial.print(doorOpen ? "OPEN" : "CLOSE");
  Serial.print(", DefrostState=");
  switch (defrostState) {
    case DEF_IDLE: Serial.print("IDLE"); break;
    case DEF_TRIGGERED: Serial.print("TRIGGERED"); break;
    case DEF_RUNNING: Serial.print("RUNNING"); break;
    case DEF_WAIT_STOP: Serial.print("WAIT_STOP"); break;
    default: Serial.print("UNK"); break;
  }
  Serial.print(", Fan=");
  Serial.print(digitalRead(PIN_FAN) == HIGH ? "ON" : "OFF");
  Serial.print(", Relay=");
  Serial.print(digitalRead(PIN_DEFROST_RELAY) == HIGH ? "ON" : "OFF");
  Serial.print(", Alarm=");
  Serial.print(alarmLevelToStr(alarmLevel));
  Serial.print(", frostBelow=");
  Serial.print(frostBelow ? "YES" : "NO");
  Serial.print(", doorDurMs=");
  if (doorOpen) Serial.print(nowMs - doorOpenStartMs);
  else Serial.print(0);
  Serial.println();
}

/////////////////////////////////////////////////////////////////
// setup / loop
/////////////////////////////////////////////////////////////////
void setup() {
  pinMode(PIN_DOOR_MAG, INPUT_PULLUP);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_DEFROST_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  setFan(false);
  setDefrostRelay(false);
  noTone(PIN_BUZZER);

  Serial.begin(9600);

  tm1637.setBrightness(0x0f, false);
  tm1637.clear();

  // init stable door state
  bool rawInit = readDoorOpenRaw();
  (void)rawInit;
}

void loop() {
  uint32_t nowMs = millis();

  static bool stableDoor = false;
  static uint32_t lastDoorChange = 0;
  bool rawDoor = readDoorOpenRaw();

  doorStateStable(rawDoor, stableDoor, lastDoorChange);
  bool doorOpen = stableDoor;

  float envC = readLM35C(PIN_LM35_ENV);
  float evaC = readLM35C(PIN_LM35_EVA);

  computeAlarmLevel(envC, evaC, doorOpen, nowMs);

  handleBuzzerAndProtection(envC, evaC, doorOpen);

  doorControlAndPause(doorOpen, envC, evaC);

  defrostStateMachine(doorOpen, evaC, envC);

  bool defrosting = (defrostState == DEF_TRIGGERED || defrostState == DEF_RUNNING);
  updateTM1637(envC, evaC, alarmLevel, doorOpen, defrosting);

  serialStatus(envC, evaC, doorOpen, nowMs);
}
