#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

/////////////////////////////////////////////////////////////////
// Pin assignments (as required)
/////////////////////////////////////////////////////////////////
static const uint8_t PIN_LM35_ENV   = A0; // 环境温度
static const uint8_t PIN_LM35_EVA   = A1; // 蒸发器温度
static const uint8_t PIN_DOOR_MAG   = D2; // 门磁
static const uint8_t PIN_FAN        = D5; // 风扇
static const uint8_t PIN_DEFROST_RELAY = D10; // 除霜继电器
static const uint8_t PIN_BUZZER     = D6; // 蜂鸣器
static const uint8_t PIN_TM_CLK     = D4; // TM1637 CLK
static const uint8_t PIN_TM_DIO     = D3; // TM1637 DIO

/////////////////////////////////////////////////////////////////
// TM1637
/////////////////////////////////////////////////////////////////
TM1637Display tm1637(PIN_TM_CLK, PIN_TM_DIO);

/////////////////////////////////////////////////////////////////
// LM35 calibration / conversion
/////////////////////////////////////////////////////////////////
static const float LM35_VREF = 5.0f;   // UNO ADC ref assumed 5V
static const float LM35_SCALE = 100.0f; // 10mV/°C => (V * 100) = °C
static const uint8_t ADC_SAMPLES = 10;

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
float readLM35C(uint8_t analogPin);
#line 44 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool readDoorOpenRaw();
#line 51 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool doorStateStable(bool raw, bool &prevStable, uint32_t &lastChangeMs);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void buzzerBeep(uint16_t freq, uint16_t msOn, uint16_t msOff, uint8_t count);
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFan(bool on);
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setDefrostRelay(bool on);
#line 154 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void clearActuatorsForSafety();
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateTM1637(float envC, float evaC, AlarmLevel alm, bool doorOpen, bool defrosting);
#line 197 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void computeAlarmLevel(float envC, float evaC, bool doorOpen, uint32_t nowMs);
#line 230 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void handleBuzzerAndProtection(float envC, float evaC, bool doorOpen);
#line 285 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void doorControlAndPause(bool doorOpen, float envC, float evaC);
#line 322 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static const char * alarmLevelToStr(AlarmLevel lv);
#line 332 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void defrostStateMachine(bool doorOpen, float evaC, float envC);
#line 395 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void serialStatus(float envC, float evaC, bool doorOpen, uint32_t nowMs);
#line 435 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 457 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
float readLM35C(uint8_t analogPin) {
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
// Inputs / debouncing
/////////////////////////////////////////////////////////////////
static const uint16_t DOOR_DEBOUNCE_MS = 80;

bool readDoorOpenRaw() {
  // Assume D2 is a simple digital input from door magnet:
  // door open => HIGH (commonly used), door closed => LOW.
  // If your wiring is inverted, swap logic in this function.
  return digitalRead(PIN_DOOR_MAG) == HIGH;
}

bool doorStateStable(bool raw, bool &prevStable, uint32_t &lastChangeMs) {
  // prevStable = last stable value
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
// System policy parameters (defaults)
/////////////////////////////////////////////////////////////////
// Note: Requirements mention "different alarm levels" and "pause some controls and time door open",
// "fan control by environment temperature", "defrost flow by evaporator temperature",
// and "detailed status outputs". However, specific thresholds/timers were not provided.
// These defaults implement a reasonable multi-level protection system while keeping full behavior structure.
/////////////////////////////////////////////////////////////////
static const float ENV_FAN_ON_TEMP  = 4.0f;   // 环境温度达到此值开始/维持风扇
static const float ENV_FAN_OFF_TEMP = 2.0f;   // 环境温度低于此值停止风扇

static const float EVAP_DEFROST_START_TEMP = -2.0f; // 蒸发器温度低于此值，进入除霜准备
static const float EVAP_DEFROST_STOP_TEMP  = 8.0f;  // 蒸发器温度升到此值，停止除霜
static const uint32_t DEFROST_MIN_TIME_MS  = 30UL * 1000UL; // 除霜最小运行时间（防抖）
static const uint32_t DEFROST_MAX_TIME_MS  = 10UL * 60UL * 1000UL; // 除霜最大保护时间

// 门长期未关
static const uint32_t DOOR_OPEN_ALARM1_MS = 60UL * 1000UL;  // 一级告警
static const uint32_t DOOR_OPEN_ALARM2_MS = 180UL * 1000UL; // 二级告警
static const uint32_t DOOR_OPEN_ALARM3_MS = 300UL * 1000UL; // 三级告警

// 蒸发器结霜严重 - 用“低温持续时间”近似
static const float EVAP_FROST_SEVERE_TEMP = -5.0f; // 蒸发器低于此温度认为结霜加重
static const uint32_t EVAP_FROST_ALARM1_MS = 120UL * 1000UL;
static const uint32_t EVAP_FROST_ALARM2_MS = 240UL * 1000UL;
static const uint32_t EVAP_FROST_ALARM3_MS = 360UL * 1000UL;

// 环境温度上限（多级告警）
static const float ENV_TEMP_UPPER1 = 10.0f; // 一级
static const float ENV_TEMP_UPPER2 = 15.0f; // 二级
static const float ENV_TEMP_UPPER3 = 20.0f; // 三级

/////////////////////////////////////////////////////////////////
// Alarm management
/////////////////////////////////////////////////////////////////
enum AlarmLevel : uint8_t { ALM_NONE=0, ALM_L1=1, ALM_L2=2, ALM_L3=3 };

static AlarmLevel alarmLevel = ALM_NONE;
static uint32_t doorOpenStartMs = 0;
static bool doorWasOpen = false;

// Frost severity timing
static bool frostBelow = false;
static uint32_t frostStartMs = 0;

// Defrost state machine
enum DefrostState : uint8_t { DEF_IDLE=0, DEF_TRIGGERED=1, DEF_RUNNING=2, DEF_WAIT_STOP=3 };
static DefrostState defrostState = DEF_IDLE;
static uint32_t defrostStartMs = 0;

// "pause部分控制并计时" when door open
// We'll: pause fan and defrost relay output, while continuing to evaluate temperatures for alarms.
// Door open will not start new defrost and will stop fan output during open.
// If door opens during defrost, we turn off defrost relay (safety) and resume after door closes (state kept).
static bool doorPaused = false;

// Timing
static uint32_t lastStatusMs = 0;
static const uint32_t STATUS_PERIOD_MS = 1000;

static void buzzerBeep(uint16_t freq, uint16_t msOn, uint16_t msOff, uint8_t count) {
  // Blocking beep for required audible alert.
  // If door open or other protections require non-blocking, this can be revised,
  // but for now keep deterministic alert behavior.
  for (uint8_t i = 0; i < count; i++) {
    tone(PIN_BUZZER, freq);
    delay(msOn);
    noTone(PIN_BUZZER);
    if (i + 1 < count) delay(msOff);
  }
}

static void setFan(bool on) {
  digitalWrite(PIN_FAN, on ? HIGH : LOW);
}

static void setDefrostRelay(bool on) {
  digitalWrite(PIN_DEFROST_RELAY, on ? HIGH : LOW);
}

static void clearActuatorsForSafety() {
  setFan(false);
  setDefrostRelay(false);
  noTone(PIN_BUZZER);
}

static void updateTM1637(float envC, float evaC, AlarmLevel alm, bool doorOpen, bool defrosting) {
  // Simple mapping:
  // - Show evaporator temperature in °C (scaled to int) with sign.
  // - Flash a segment by replacing display when alarm/defrost.
  // TM1637 library supports showNumberDec; for sign we can use showNumberDecEx.
  // We'll show two-digit-ish using 4 digits: show temperature with 1 decimal? We'll show integer.
  // If alarm >0, we show envC integer instead.
  int16_t tDisp = 0;
  if (alm != ALM_NONE) tDisp = (int16_t)round(envC);
  else tDisp = (int16_t)round(evaC);

  // Decide colon points to indicate states
  bool show = true;
  if (doorOpen) {
    // Indicate door open by flashing
    if ((millis() / 250) % 2 == 1) show = false;
  }
  if (defrosting) {
    // Indicate defrost by using same flash but different cadence
    if ((millis() / 350) % 2 == 1) show = false;
  }

  if (!show) {
    tm1637.clear();
    return;
  }

  // Display temperature
  bool neg = (tDisp < 0);
  uint16_t mag = neg ? (uint16_t)(-tDisp) : (uint16_t)tDisp;

  // showNumberDecEx: not all libs implement; TM1637Display does.
  // We'll show integer with leading zeros suppressed.
  tm1637.showNumberDecEx((int)mag, neg, false, 4);
  // Note: we don't rely on extra indicators due to unknown wiring for segments.
}

static void computeAlarmLevel(float envC, float evaC, bool doorOpen, uint32_t nowMs) {
  AlarmLevel newLevel = ALM_NONE;

  // Environment upper thresholds
  if (envC >= ENV_TEMP_UPPER3) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L3);
  else if (envC >= ENV_TEMP_UPPER2) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L2);
  else if (envC >= ENV_TEMP_UPPER1) newLevel = (AlarmLevel)max((uint8_t)newLevel, (uint8_t)ALM_L1);

  // Evaporator severe frost (time below a threshold)
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
  // Detailed state changes are output over Serial separately.
  // Here we implement alarm signaling policy without removing required behaviors.
  if (alarmLevel == ALM_NONE) {
    noTone(PIN_BUZZER);
    return;
  }

  // For increasing levels, change beep pattern.
  // Additionally, enforce more conservative actuator behavior at higher alarm.
  // Level 1: buzzer short beeps, fan may continue unless door open
  // Level 2: increase buzzer frequency; stop defrost if door open handled elsewhere
  // Level 3: strong buzzer; stop fan to reduce heat load, and stop defrost to protect (safety policy)
  if (alarmLevel == ALM_L1) {
    // non-blocking like: tone for short time periodically
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
    // Strong continuous tone bursts
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
    if (doorOpen) {
      // already paused elsewhere
    } else {
      // Stop fan at highest alarm to avoid further thermal disturbance
      setFan(false);
    }
  }
}

static void doorControlAndPause(bool doorOpen, float envC, float evaC) {
  // pause部分控制并计时: when door open => pause fan and defrost relay
  uint32_t nowMs = millis();

  if (doorOpen && !doorWasOpen) {
    doorWasOpen = true;
    doorOpenStartMs = nowMs;
    doorPaused = true;
    // Turn off fan/defrost during door open
    setFan(false);
    setDefrostRelay(false);
  } else if (!doorOpen && doorWasOpen) {
    doorWasOpen = false;
    doorPaused = false;
    // Restore fan based on env temp (unless alarms stop it)
    // Defrost resume logic handled by defrost state machine below.
  }

  // If door open, we do not actuate fan and defrost relay.
  if (doorOpen) {
    // ensure outputs remain paused
    setFan(false);
    setDefrostRelay(false);
  } else {
    // Fan control by environment temperature
    // If alarm level 3 already stopped fan, handleBuzzerAndProtection may have setFan(false)
    // We'll still apply normal control but do not override if alarm3 forced off.
    if (alarmLevel == ALM_L3) {
      // keep off
    } else {
      if (envC >= ENV_FAN_ON_TEMP) setFan(true);
      else if (envC <= ENV_FAN_OFF_TEMP) setFan(false);
      // between thresholds: keep previous state (hysteresis handled by not changing)
    }
  }
}

static const char* alarmLevelToStr(AlarmLevel lv) {
  switch (lv) {
    case ALM_NONE: return "NONE";
    case ALM_L1: return "LEVEL1";
    case ALM_L2: return "LEVEL2";
    case ALM_L3: return "LEVEL3";
    default: return "UNKNOWN";
  }
}

static void defrostStateMachine(bool doorOpen, float evaC, float envC) {
  (void)envC;
  // Door open pauses certain controls; defrost relay off.
  if (doorOpen) {
    // Keep state but ensure relay off. When door closes, resume.
    setDefrostRelay(false);
    return;
  }

  uint32_t nowMs = millis();

  switch (defrostState) {
    case DEF_IDLE: {
      setDefrostRelay(false);

      // Enter defrost when evaporator temperature is low enough.
      if (evaC <= EVAP_DEFROST_START_TEMP) {
        defrostState = DEF_TRIGGERED;
      }
      break;
    }
    case DEF_TRIGGERED: {
      // Start defrost relay.
      defrostStartMs = nowMs;
      defrostState = DEF_RUNNING;
      setDefrostRelay(true);
      break;
    }
    case DEF_RUNNING: {
      // Defrost relay ON
      setDefrostRelay(true);

      bool minTimeMet = (nowMs - defrostStartMs) >= DEFROST_MIN_TIME_MS;
      bool stopByTemp = evaC >= EVAP_DEFROST_STOP_TEMP;
      bool maxTimeExceeded = (nowMs - defrostStartMs) >= DEFROST_MAX_TIME_MS;

      if ((minTimeMet && stopByTemp) || maxTimeExceeded) {
        setDefrostRelay(false);
        defrostState = DEF_WAIT_STOP;
        // In WAIT_STOP, we keep relay off until evaporator cools somewhat to avoid oscillation
        defrostStartMs = nowMs;
      }
      break;
    }
    case DEF_WAIT_STOP: {
      // Relay OFF
      setDefrostRelay(false);

      // Wait for evaporator to cool below start temp minus hysteresis or time limit
      bool coolEnough = evaC <= (EVAP_DEFROST_START_TEMP + 1.0f); // hysteresis
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

static void serialStatus(float envC, float evaC, bool doorOpen, uint32_t nowMs) {
  if (nowMs - lastStatusMs < STATUS_PERIOD_MS) return;
  lastStatusMs = nowMs;

  // Determine flags
  bool defrosting = (defrostState == DEF_TRIGGERED || defrostState == DEF_RUNNING);

  // Detailed status
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
  pinMode(PIN_DOOR_MAG, INPUT_PULLUP); // use pullup to avoid floating; assumes door magnet closes to GND for closed
  pinMode(PIN_FAN, OUTPUT);
  pinMode(PIN_DEFROST_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initial outputs
  setFan(false);
  setDefrostRelay(false);
  noTone(PIN_BUZZER);

  Serial.begin(9600);

  // TM1637
  tm1637.setBrightness(0x0f, false);
  tm1637.clear();

  // Door input stable init
  bool rawInit = readDoorOpenRaw();
  (void)rawInit;
}

void loop() {
  uint32_t nowMs = millis();

  // Read door with debounce
  static bool stableDoor = false;
  static uint32_t lastDoorChange = 0;
  bool rawDoor = readDoorOpenRaw();

  // If using INPUT_PULLUP, typical magnet wiring is:
  // door closed => magnet pressed => D2 reads LOW, door open => D2 reads HIGH.
  // We used INPUT_PULLUP and rawDoor assumes HIGH means open; keep as is.
  doorStateStable(rawDoor, stableDoor, lastDoorChange);
  bool doorOpen = stableDoor;

  // Read sensors
  float envC = readLM35C(PIN_LM35_ENV);
  float evaC = readLM35C(PIN_LM35_EVA);

  // Update alarm level based on current conditions
  computeAlarmLevel(envC, evaC, doorOpen, nowMs);

  // Alarm signaling and protective actions
  handleBuzzerAndProtection(envC, evaC, doorOpen);

  // Door pause + fan control by environment temp
  doorControlAndPause(doorOpen, envC, evaC);

  // Defrost flow based on evaporator temp (door open pauses some controls)
  defrostStateMachine(doorOpen, evaC, envC);

  // TM1637 display
  bool defrosting = (defrostState == DEF_TRIGGERED || defrostState == DEF_RUNNING);
  updateTM1637(envC, evaC, alarmLevel, doorOpen, defrosting);

  // Detailed serial output
  serialStatus(envC, evaC, doorOpen, nowMs);
}
