#include <Arduino.h>
#include <TM1637Display.h>

// ------------------- Pin Assignments (authoritative) -------------------
static const uint8_t PIR_PIN = 2;     // D2
static const uint8_t DOOR_PIN = 3;    // D3
static const uint8_t LDR_PIN = A0;     // A0
static const uint8_t BUZZER_PIN = 6;  // D6
static const uint8_t LED_PIN = 13;    // D13

// TM1637 (CLK=D4, DIO=D5)
static const uint8_t TM1637_CLK = 4;   // D4
static const uint8_t TM1637_DIO = 5;   // D5

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// ------------------- System Behavior -------------------
enum Mode : uint8_t {
  MODE_DISARM = 0,
  MODE_ARMED  = 1
};

// Night auto-arm when light below threshold
// Preserve behavior by using an explicit threshold and a small hysteresis.
static const int LIGHT_THRESHOLD = 500; // 光敏阈值(0~1023)，根据电路需要可在此处微调
static const int LIGHT_HYSTERESIS = 30;

// Event / alarm settings
static const uint16_t ALARM_BEEP_MS = 500;   // 报警蜂鸣时长(可多次重复)
static const uint16_t BEEP_CYCLES   = 2;     // 每次报警蜂鸣次数
static const uint16_t EVENT_DEBOUNCE_MS = 300; // 门/人体触发去抖

// Command parsing
static const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;

// ------------------- State -------------------
static Mode mode = MODE_DISARM;
static uint32_t lastSerialByteTime = 0;

static uint32_t eventCount = 0;
static bool lastPIR = false;
static bool lastDoor = false;

static bool doorTriggeredArmed = false; // 用于门触发后等待一次人体移动
static unsigned long lastDoorChangeMs = 0;
static unsigned long lastPIRChangeMs  = 0;

// For display brightness / update throttling
static unsigned long lastDisplayUpdateMs = 0;
static uint16_t displayUpdateIntervalMs = 100;

// ------------------- Helpers -------------------
static inline void buzzOnce(uint16_t ms) {
  tone(BUZZER_PIN, 2000); // 2000Hz 蜂鸣
  delay(ms);
  noTone(BUZZER_PIN);
}

static void alarmBeep() {
  for (uint16_t i = 0; i < BEEP_CYCLES; i++) {
    buzzOnce(ALARM_BEEP_MS);
    delay(50);
  }
}

static void setAlarmLED(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

static int readLight() {
  return analogRead(LDR_PIN);
}

static void updateNightAutoArm() {
  // 夜间模式：光照低于阈值自动进入（不改变手动/撤防的语义：
  // DISARM 下仍会根据夜间条件自动进入；ARM 下不退出）
  static bool nightArmedLatch = false;

  int light = readLight();

  if (!nightArmedLatch) {
    if (light < (LIGHT_THRESHOLD - LIGHT_HYSTERESIS)) {
      nightArmedLatch = true;
      mode = MODE_ARMED;
    }
  } else {
    // 若光照回升到较高阈值，可解除夜间自动布防
    if (light > (LIGHT_THRESHOLD + LIGHT_HYSTERESIS)) {
      nightArmedLatch = false;
      // 解除夜间模式时回到撤防（保持“夜间自动布防”）
      if (mode == MODE_ARMED) mode = MODE_DISARM;
    }
  }
}

static void showEventCount() {
  // 显示报警事件次数（四位）
  // 若超过9999则截断显示后四位的数值（避免TM1637溢出）
  uint16_t shown = (uint16_t)(eventCount % 10000UL);
  tm1637.showNumberDecEx(shown, 0b01000000, true); // 小数点不使用，保留亮度/格式
}

static void showStatus() {
  // DISARM: 显示 dS, 但TM1637仅能显示数字；用约定：0 表示撤防，1 表示布防
  uint16_t v = (mode == MODE_ARMED) ? 1 : 0;
  tm1637.showNumberDecEx(v, 0b00000000, true);
}

static void triggerAlarm() {
  // 报警、记录事件次数并在数码管显示
  eventCount++;
  alarmBeep();
  setAlarmLED(true);
  showEventCount();

  // 保持警示一段时间后熄灭LED（不影响后续触发）
  delay(200);
  setAlarmLED(false);
}

static void processCommand(const String &cmdRaw) {
  String cmd = cmdRaw;
  cmd.trim();
  cmd.toUpperCase();

  if (cmd.length() == 0) return;

  if (cmd == "ARM") {
    mode = MODE_ARMED;
    doorTriggeredArmed = false;
    setAlarmLED(false);
    showStatus();
    Serial.println("OK ARM");
  } else if (cmd == "DISARM") {
    mode = MODE_DISARM;
    doorTriggeredArmed = false;
    setAlarmLED(false);
    showStatus();
    Serial.println("OK DISARM");
  } else if (cmd == "RESET") {
    eventCount = 0;
    doorTriggeredArmed = false;
    setAlarmLED(false);
    showStatus();
    Serial.println("OK RESET");
  } else if (cmd == "STATUS") {
    Serial.print("MODE=");
    Serial.print((mode == MODE_ARMED) ? "ARMED" : "DISARMED");
    Serial.print(",EVENTS=");
    Serial.println(eventCount);
  } else {
    Serial.print("ERR UNKNOWN CMD: ");
    Serial.println(cmdRaw);
  }
}

// ------------------- Setup / Loop -------------------
void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(DOOR_PIN, INPUT_PULLUP); // 门磁开关常见：开关一端接GND，另一端接D3
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  setAlarmLED(false);

  // TM1637 init
  tm1637.setBrightness(7, true); // 0~7; 自动刷新

  // Initial display
  showStatus();

  Serial.begin(9600);
  lastSerialByteTime = 0;
  lastPIR = digitalRead(PIR_PIN);
  lastDoor = digitalRead(DOOR_PIN);
  lastDoorChangeMs = millis();
  lastPIRChangeMs = millis();
  doorTriggeredArmed = false;
}

void loop() {
  // ------------------- Night Auto-Arm -------------------
  updateNightAutoArm();

  // ------------------- Read Sensors -------------------
  bool pirNow = (digitalRead(PIR_PIN) == HIGH);

  // 门磁：假设闭合为LOW/断开为HIGH，且使用INPUT_PULLUP
  // 当门被打开时，通常为HIGH（取决于接线），因此这里用“变化为打开”的逻辑：
  // 以“当前为HIGH”作为“打开状态”。可根据你接线反转此判断。
  bool doorNow = (digitalRead(DOOR_PIN) == HIGH); // true: 门打开

  unsigned long nowMs = millis();

  // ------------------- Debounce / Edge Detection -------------------
  // PIR edge
  if (pirNow != lastPIR) {
    lastPIR = pirNow;
    lastPIRChangeMs = nowMs;
  }
  // Door edge
  if (doorNow != lastDoor) {
    lastDoor = doorNow;
    lastDoorChangeMs = nowMs;
  }

  // ------------------- Armed Logic -------------------
  if (mode == MODE_ARMED) {
    // 门打开 => 进入“等待一次人体移动触发”的阶段
    if (doorNow) {
      // 门触发后去抖：只在门稳定打开后标记一次
      if (!doorTriggeredArmed && (nowMs - lastDoorChangeMs) >= EVENT_DEBOUNCE_MS) {
        doorTriggeredArmed = true;
      }
    } else {
      // 门关闭，重置等待标记
      doorTriggeredArmed = false;
    }

    // 若门被打开且检测到人体移动，则触发报警
    // 需要人体移动：PIR输出为HIGH视为移动/人体存在
    if (doorTriggeredArmed && pirNow) {
      // 再做一次去抖，避免抖动导致多次计数
      static bool alarmCooldownActive = false;
      static unsigned long alarmCooldownStart = 0;
      static const unsigned long ALARM_COOLDOWN_MS = 1000;

      if (!alarmCooldownActive) {
        triggerAlarm();
        alarmCooldownActive = true;
        alarmCooldownStart = nowMs;
      } else {
        if ((nowMs - alarmCooldownStart) >= ALARM_COOLDOWN_MS) {
          alarmCooldownActive = false;
        }
      }
    }
  }

  // ------------------- Display Updates -------------------
  if ((nowMs - lastDisplayUpdateMs) >= displayUpdateIntervalMs) {
    lastDisplayUpdateMs = nowMs;
    // 持续刷新事件计数或状态，保持“在数码管显示”
    if (mode == MODE_ARMED) showEventCount();
    else showStatus();
  }

  // ------------------- Serial Command Parsing -------------------
  // 9600, idle timeout = 20ms => read line-or-idle
  static String rxBuf;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteTime = nowMs;

    // 允许包含换行/空格等，直接累积到缓冲区
    if (c == '\r' || c == '\n') {
      if (rxBuf.length() > 0) {
        processCommand(rxBuf);
        rxBuf = "";
      }
    } else {
      // 限制长度，防止异常导致内存问题
      if (rxBuf.length() < 16) rxBuf += c;
    }
  }

  // 若串口在idle_timeout_ms后无新数据，则处理当前缓冲
  if (rxBuf.length() > 0 && (nowMs - lastSerialByteTime) >= SERIAL_IDLE_TIMEOUT_MS) {
    processCommand(rxBuf);
    rxBuf = "";
  }
}