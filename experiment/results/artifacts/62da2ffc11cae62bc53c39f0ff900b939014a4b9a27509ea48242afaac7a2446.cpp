#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

TM1637Display tm1637(4, 5);

// ----- Pin mapping (authoritative assignments) -----
// Analog inputs (per request)
static const uint8_t PIN_ENV_LM35 = A0;   // 环境温度
static const uint8_t PIN_EVA_LM35 = A1;   // 蒸发器温度

// Digital inputs/outputs
static const uint8_t PIN_DOOR_MAGNET = 2;   // 门磁 D2（输入，低/高由接线决定）
static const uint8_t PIN_FAN = 5;           // 风扇 D5（原需求冲突，已按配置重映射；同时DIO给TM1637）
static const uint8_t PIN_DEFROST_RELAY = 10; // 除霜继电器 D10（输出）
static const uint8_t PIN_BUZZER = 6;         // 蜂鸣器 D6（输出；同时也标注tone_output）
static const uint8_t PIN_TM_CLK = 4;        // TM1637 CLK D4
static const uint8_t PIN_TM_DIO = 5;        // TM1637 DIO D5

// NOTE:
// D5 is shared between FAN and TM1637 DIO due to provided "resolved hardware configuration".
// This sketch preserves requested behaviors as best as possible within that constraint:
// - The TM1637 library will control D5 for display; fan control on D5 may conflict.
// - If you need strict separation, change the hardware and update pins accordingly.

// ----- Timing helpers -----
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t nowMs();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static float readLM35C(uint8_t pin);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool readDoorOpen();
#line 130 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setDefrostRelay(bool on);
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setFan(bool on);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarmLevel(float envC, float evaC);
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBuzzer();
#line 216 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplay(float envC, float evaC);
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void printDetailedStatus(float envC, float evaC);
#line 305 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 339 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_029\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint32_t nowMs() { return millis(); }

// ----- Temperature conversion (LM35) -----
// LM35 output: 10mV/°C (i.e., 0.01V/°C). Arduino UNO ADC: 5V reference, 10-bit.
static float readLM35C(uint8_t pin) {
  int raw = analogRead(pin);
  float v = (raw * 5.0f) / 1023.0f;
  float c = v / 0.01f; // 10mV per °C
  return c;
}

// ----- System behavior parameters -----
// The problem statement asks for "preserve all requirements and wiring"
// but does not provide explicit numeric thresholds/timers/calibration in text.
// So we implement a complete multi-level alarm/protection system with sensible defaults,
// while keeping logic structure required: fan control by environment temp, defrost based
// on evaporator frosting, door open pause with timing, and different alarm levels.

// These constants can be adjusted later by the user.
static const float FAN_START_ENV_C = 4.0f;    // 环境温度低于该值启动/维持制冷风机（按冷库常识可调整）
static const float FAN_STOP_ENV_C  = 2.0f;    // 环境温度高到该值停止风机（滞回）
static const uint16_t FAN_MIN_ON_MS = 10UL * 1000;   // 最小风机运行时间，避免频繁启停
static const uint16_t FAN_MIN_OFF_MS = 10UL * 1000;  // 最小风机停机时间

// Defrost decision: we don't have a direct "frost severity" sensor;
// use evaporator temperature reaching very low threshold as proxy (configurable).
static const float EVA_FROST_SEVERE_C = -3.0f; // 蒸发器温度低于该值判定结霜严重倾向
static const uint32_t EVA_SEVERE_TIME_MS = 4UL * 60UL * 1000; // 连续达到结霜严重判据的时间

// Defrost process timing
static const uint32_t DEFROST_TOTAL_MS = 8UL * 60UL * 1000; // 除霜总时长
static const float EVA_DEFROST_DONE_C = 5.0f;               // 蒸发器回升到该温度认为除霜完成
static const uint32_t DEFROST_MIN_MS = 2UL * 60UL * 1000;  // 除霜最短运行时间

// Door magnet logic and pause behavior
static const uint32_t DOOR_PAUSE_MIN_MS = 0; // 只要门打开就暂停部分控制；门关闭后恢复
static const uint32_t DOOR_MAX_OPEN_MS_LEVEL1 = 3UL * 60UL * 1000; // 门长时间未关一级报警
static const uint32_t DOOR_MAX_OPEN_MS_LEVEL2 = 8UL * 60UL * 1000; // 二级报警
static const uint32_t DOOR_MAX_OPEN_MS_LEVEL3 = 15UL * 60UL * 1000; // 三级报警

// Alarm levels
// - 环境温度超上限：一级/二级/三级根据超限程度和持续时间
static const float ENV_OVER_1_C = 8.0f;
static const float ENV_OVER_2_C = 10.0f;
static const float ENV_OVER_3_C = 12.0f;

static const uint32_t OVER_TIME_MS_LEVEL1 = 2UL * 60UL * 1000;
static const uint32_t OVER_TIME_MS_LEVEL2 = 4UL * 60UL * 1000;
static const uint32_t OVER_TIME_MS_LEVEL3 = 7UL * 60UL * 1000;

// 风险：结霜严重持续时间 -> 报警层级
static const uint32_t FROST_TIME_LEVEL1 = EVA_SEVERE_TIME_MS; // 4min
static const uint32_t FROST_TIME_LEVEL2 = 2UL * EVA_SEVERE_TIME_MS;
static const uint32_t FROST_TIME_LEVEL3 = 3UL * EVA_SEVERE_TIME_MS;

// Buzzer patterns
static const uint32_t BUZZ_OFF_MS = 200;
static const uint32_t BUZZ_ON_MS_L1 = 300;
static const uint32_t BUZZ_ON_MS_L2 = 600;
static const uint32_t BUZZ_ON_MS_L3 = 900;

// ----- State variables -----
enum AlarmLevel : uint8_t { ALM_NONE=0, ALM_L1=1, ALM_L2=2, ALM_L3=3 };

static AlarmLevel alarmLevel = ALM_NONE;

// Door
static bool doorOpen = false;
static uint32_t doorOpenedAt = 0;
static uint32_t doorOpenDurationMs = 0;

// Fan control timing
static bool fanState = false;
static uint32_t fanLastToggleMs = 0;

// Defrost
static bool defrosting = false;
static uint32_t defrostStartMs = 0;
static uint32_t defrostLastActionMs = 0;

// Frost severity timer
static bool frostSevereCond = false;
static uint32_t frostSevereStartMs = 0;

// Over-temp timer
static bool overCond1 = false, overCond2 = false, overCond3 = false;
static uint32_t overStartMs = 0;

// Display refresh
static uint32_t lastDisplayMs = 0;

// Buzzer state
static uint32_t lastBuzzerToggleMs = 0;
static bool buzzerOn = false;

// ----- Utility: door open detection -----
// Unknown polarity: Assume door magnet switch pulls LOW when door open (common).
static bool readDoorOpen() {
  int v = digitalRead(PIN_DOOR_MAGNET);
  // If wiring is opposite, user must invert here.
  // Current assumption: door open => LOW.
  return (v == LOW);
}

static void setDefrostRelay(bool on) {
  digitalWrite(PIN_DEFROST_RELAY, on ? HIGH : LOW);
}

static void setFan(bool on) {
  // D5 shared with TM1637 DIO; we still attempt to control it.
  // If TM1637 updates, it may override.
  fanState = on;
  digitalWrite(PIN_FAN, on ? HIGH : LOW);
}

static void updateAlarmLevel(float envC, float evaC) {
  // Door duration-based
  doorOpenDurationMs = doorOpen ? (nowMs() - doorOpenedAt) : 0;

  AlarmLevel lvlDoor = ALM_NONE;
  if (doorOpenDurationMs >= DOOR_MAX_OPEN_MS_LEVEL3) lvlDoor = ALM_L3;
  else if (doorOpenDurationMs >= DOOR_MAX_OPEN_MS_LEVEL2) lvlDoor = ALM_L2;
  else if (doorOpenDurationMs >= DOOR_MAX_OPEN_MS_LEVEL1) lvlDoor = ALM_L1;

  // Over-temp duration-based
  bool c1 = (envC >= ENV_OVER_1_C);
  bool c2 = (envC >= ENV_OVER_2_C);
  bool c3 = (envC >= ENV_OVER_3_C);

  AlarmLevel lvlOver = ALM_NONE;

  if (c1 || c2 || c3) {
    if (overStartMs == 0) overStartMs = nowMs();
    uint32_t dt = nowMs() - overStartMs;

    if (c3 && dt >= OVER_TIME_MS_LEVEL3) lvlOver = ALM_L3;
    else if (c2 && dt >= OVER_TIME_MS_LEVEL2) lvlOver = ALM_L2;
    else if (c1 && dt >= OVER_TIME_MS_LEVEL1) lvlOver = ALM_L1;
    else {
      // If over condition is present but not long enough, keep none.
      // Alternatively, you can set provisional level; choose strict as required for "different级别告警".
      lvlOver = ALM_NONE;
    }
  } else {
    overStartMs = 0;
  }

  // Frost severity based on evaporator temperature duration
  AlarmLevel lvlFrost = ALM_NONE;
  if (frostSevereCond) {
    uint32_t dt = nowMs() - frostSevereStartMs;
    if (dt >= FROST_TIME_LEVEL3) lvlFrost = ALM_L3;
    else if (dt >= FROST_TIME_LEVEL2) lvlFrost = ALM_L2;
    else if (dt >= FROST_TIME_LEVEL1) lvlFrost = ALM_L1;
  }

  // Combine: take max level
  AlarmLevel maxLvl = ALM_NONE;
  maxLvl = (lvlDoor > maxLvl) ? lvlDoor : maxLvl;
  maxLvl = (lvlOver > maxLvl) ? lvlOver : maxLvl;
  maxLvl = (lvlFrost > maxLvl) ? lvlFrost : maxLvl;

  alarmLevel = maxLvl;
}

static void updateBuzzer() {
  if (alarmLevel == ALM_NONE) {
    buzzerOn = false;
    digitalWrite(PIN_BUZZER, LOW);
    return;
  }

  uint32_t onMs = (alarmLevel == ALM_L1) ? BUZZ_ON_MS_L1 :
                   (alarmLevel == ALM_L2) ? BUZZ_ON_MS_L2 : BUZZ_ON_MS_L3;

  if (buzzerOn) {
    if (nowMs() - lastBuzzerToggleMs >= onMs) {
      buzzerOn = false;
      lastBuzzerToggleMs = nowMs();
      digitalWrite(PIN_BUZZER, LOW);
    }
  } else {
    if (nowMs() - lastBuzzerToggleMs >= BUZZ_OFF_MS) {
      buzzerOn = true;
      lastBuzzerToggleMs = nowMs();
      digitalWrite(PIN_BUZZER, HIGH);
    }
  }
}

static void updateDisplay(float envC, float evaC) {
  // Show alarm level and current mode code
  // 4-digit display (TM1637): [L][mode][temp tens][temp ones] simplistic
  // Also keep "详细状态" on serial as requirement.
  // Mode codes:
  // 0 idle, 1 fan paused(door open), 2 defrosting, 3 alarm
  uint8_t mode = 0;
  if (defrosting) mode = 2;
  else if (doorOpen) mode = 1;
  else mode = 0;
  if (alarmLevel != ALM_NONE) mode = 3;

  int t = (int)round(envC); // show env temp rounded
  if (t < -99) t = -99;
  if (t > 99) t = 99;

  // Encode: "Lx" and 2 digits of env temp magnitude
  // We'll use:
  // digit0: blank/negative not supported well; show mode and alarm
  // Display example: mode(1 digit) + alarm(1 digit) + temp(2 digits)
  // Using TM1637Display expects setSegments or showNumberDecEx;
  // We'll show 4 digits as: [mode][alarm][abs(temp)/10][abs(temp)%10] (ignore sign).
  int at = abs(t);
  uint8_t d1 = mode;
  uint8_t d2 = (uint8_t)alarmLevel;
  uint8_t d3 = (at / 10) % 10;
  uint8_t d4 = at % 10;

  uint8_t digits[4] = { d1, d2, d3, d4 };
  tm1637.setBrightness(7);

  // TM1637Display has showSegments; but internal.
  // We'll use showNumberDecEx to display as 4 digits number: mode*1000 + alarm*100 + d3*10 + d4
  int val = (int)d1 * 1000 + (int)d2 * 100 + (int)d3 * 10 + (int)d4;
  // Keep leading zeros (we want fixed 4 digits)
  tm1637.showNumberDecEx(val, 0b01000000, true); // no dots
}

// Serial detailed status (throttled)
static uint32_t lastSerialMs = 0;

static void printDetailedStatus(float envC, float evaC) {
  if (nowMs() - lastSerialMs < 500) return;
  lastSerialMs = nowMs();

  Serial.print("T_env(C)="); Serial.print(envC, 2);
  Serial.print(", T_eva(C)="); Serial.print(evaC, 2);

  Serial.print(", Door="); Serial.print(doorOpen ? "OPEN" : "CLOSED");
  Serial.print(" (");
  Serial.print(doorOpenDurationMs);
  Serial.print("ms)");

  Serial.print(", Defrost=");
  Serial.print(defrosting ? "ON" : "OFF");
  Serial.print(" (start=");
  if (defrosting) Serial.print(nowMs() - defrostStartMs);
  else Serial.print(0);
  Serial.print("ms)");

  Serial.print(", FrostCond=");
  Serial.print(frostSevereCond ? "SEVERE" : "NORMAL");
  Serial.print(" (");
  if (frostSevereCond) Serial.print(nowMs() - frostSevereStartMs);
  else Serial.print(0);
  Serial.print("ms)");

  Serial.print(", OverCondStart=");
  if (overStartMs != 0) Serial.print(nowMs() - overStartMs);
  else Serial.print(0);

  Serial.print("ms, AlarmLevel=");
  Serial.print((int)alarmLevel);

  Serial.print(", Fan=");
  Serial.print(fanState ? "ON" : "OFF");

  Serial.print(", Relay(D10)=");
  Serial.print(digitalRead(PIN_DEFROST_RELAY) ? 1 : 0);

  Serial.print(", RelayFan(D5 written)=");
  Serial.print(digitalRead(PIN_FAN) ? 1 : 0);

  Serial.print(", Buzzer=");
  Serial.print(buzzerOn ? "ON" : "OFF");

  Serial.println();
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_DOOR_MAGNET, INPUT_PULLUP);
  pinMode(PIN_DEFROST_RELAY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  pinMode(PIN_FAN, OUTPUT); // D5 shared with TM1637 DIO

  // Start states
  setDefrostRelay(false);
  setFan(false);
  digitalWrite(PIN_BUZZER, LOW);

  // TM1637 init
  tm1637.setBrightness(7);
  tm1637.clear();

  // Initialize timers
  doorOpen = readDoorOpen();
  if (doorOpen) doorOpenedAt = nowMs();
  frostSevereCond = false;
  frostSevereStartMs = 0;
  overStartMs = 0;

  defrosting = false;
  defrostStartMs = 0;
  defrostLastActionMs = 0;

  lastBuzzerToggleMs = nowMs();
  lastDisplayMs = nowMs();
  lastSerialMs = 0;
}

void loop() {
  float envC = readLM35C(PIN_ENV_LM35);
  float evaC = readLM35C(PIN_EVA_LM35);

  // Door state
  bool doorNowOpen = readDoorOpen();
  if (doorNowOpen != doorOpen) {
    doorOpen = doorNowOpen;
    if (doorOpen) {
      doorOpenedAt = nowMs();
      // When door opens: pause部分控制（具体为：暂停风机控制；除霜继电器仍按温度流程执行）
      // Requirement says "门打开时暂停部分控制并计时"
      // We'll pause fan control only.
      // (Do not stop defrosting automatically.)
    } else {
      // Door closed: reset duration timer handled via doorOpenedAt update logic
      doorOpenedAt = 0;
    }
  }
  doorOpenDurationMs = doorOpen ? (nowMs() - doorOpenedAt) : 0;

  // Frost severity condition tracking (evaporator temperature proxy)
  bool severeCondNow = (evaC <= EVA_FROST_SEVERE_C);
  if (severeCondNow) {
    if (!frostSevereCond) {
      frostSevereCond = true;
      frostSevereStartMs = nowMs();
    }
  } else {
    frostSevereCond = false;
    frostSevereStartMs = 0;
  }

  // Update fan control based on environment temperature, paused when door is open
  if (doorOpen) {
    // Pause fan control when door open
    // Keep last fan state? Requirement: "暂停部分控制"
    // We'll turn fan OFF while door open to reduce heat ingress.
    if (fanState) {
      setFan(false);
    }
  } else {
    // Fan hysteresis with min on/off timing
    uint32_t t = nowMs();
    bool canTurnOn = true;
    bool canTurnOff = true;

    if (fanState) {
      if (t - fanLastToggleMs < FAN_MIN_ON_MS) canTurnOff = false;
    } else {
      if (t - fanLastToggleMs < FAN_MIN_OFF_MS) canTurnOn = false;
    }

    if (!fanState && envC <= FAN_START_ENV_C && canTurnOn) {
      setFan(true);
      fanLastToggleMs = t;
    } else if (fanState && envC >= FAN_STOP_ENV_C && canTurnOff) {
      setFan(false);
      fanLastToggleMs = t;
    }
  }

  // Defrost process logic:
  // - "根据蒸发器温度判断是否进入除霜流程"
  // - If frost severe persists, start defrost.
  if (!defrosting) {
    if (frostSevereCond && (nowMs() - frostSevereStartMs) >= EVA_SEVERE_TIME_MS) {
      // Start defrost
      defrosting = true;
      defrostStartMs = nowMs();
      defrostLastActionMs = nowMs();
      setDefrostRelay(true);
    }
  } else {
    // In defrosting mode, stop conditions:
    // - After minimum time and either evaporator reaches done temperature
    // - Or total time exceeded
    uint32_t dt = nowMs() - defrostStartMs;

    bool doneByTemp = (dt >= DEFROST_MIN_MS) && (evaC >= EVA_DEFROST_DONE_C);
    bool doneByTime = (dt >= DEFROST_TOTAL_MS);

    if (doneByTemp || doneByTime) {
      defrosting = false;
      setDefrostRelay(false);
      // After defrost, clear frost condition timer so it doesn't immediately re-trigger
      frostSevereCond = false;
      frostSevereStartMs = 0;
    }
  }

  // Update alarm level based on all conditions
  updateAlarmLevel(envC, evaC);

  // Update buzzer pattern
  updateBuzzer();

  // Update display periodically
  if (nowMs() - lastDisplayMs >= 250) {
    lastDisplayMs = nowMs();
    updateDisplay(envC, evaC);
  }

  // Detailed serial status
  printDetailedStatus(envC, evaC);

  // Small delay to reduce serial flooding; still responsive
  delay(10);
}
