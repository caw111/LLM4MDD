#include <Arduino.h>
#include <TM1637Display.h>

#define PIN_SENSOR D2          // 光电计数传感器
#define PIN_STOP_BUTTON D3    // 人工停线按钮（按下=停线）
#define PIN_DISPLAY_TOGGLE D7 // 切换显示：目标 / 实际 / 停机次数
#define PIN_BUZZER D6          // 蜂鸣器
#define PIN_LED D13            // 指示LED

#define TM1637_CLK D4
#define TM1637_DIO D5

// -------------------- User-configurable parameters --------------------
static const uint32_t TARGET_PROD_PER_MIN = 300; // 每分钟目标产量（可按需修改）
static const uint32_t NO_PRODUCT_ALARM_INTERVAL_MS = 30000UL; // 连续30秒无产品或停线则报警
static const uint16_t DEBOUNCE_MS = 30; // 按键消抖
static const uint16_t SERIAL_IDLE_TIMEOUT_MS = 20; // 与需求一致，但串口此处无需特殊处理
// ----------------------------------------------------------------------

TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// -------------------- State variables --------------------
volatile uint32_t totalCount = 0;         // 累计产量（由光电计数）
volatile uint32_t lastSensorTickMs = 0;  // 上次检测到产品通过的时间戳

// 按钮状态/消抖
bool stopButtonStable = false;
bool lastStopButtonReading = false;
uint32_t stopButtonLastChangeMs = 0;

bool toggleButtonStable = false;
bool lastToggleButtonReading = false;
uint32_t toggleButtonLastChangeMs = 0;

enum DisplayMode : uint8_t { SHOW_TARGET = 0, SHOW_ACTUAL = 1, SHOW_DOWNTIME = 2 };
DisplayMode displayMode = SHOW_TARGET;

// 停机事件统计：停机次数
uint32_t downtimeEvents = 0;

// 报警控制
bool alarmActive = false;
uint32_t alarmStartMs = 0;

// 用于“连续30秒无产品通过或人工停线”判定的累计计时
uint32_t noProductOrStopStartMs = 0;
bool noProductOrStopActive = false;

// 记录停机事件（当触发报警时记录一次）
void logEvent(uint32_t tsMs, const char* reason) {
  Serial.print("[EVENT] t=");
  Serial.print(tsMs);
  Serial.print("ms ");
  Serial.print("reason=");
  Serial.println(reason);
}

// 如果蜂鸣器报警：保持开启（根据要求“报警并记录停机事件”）
void setAlarm(bool enable) {
  if (enable == alarmActive) return;
  alarmActive = enable;
  digitalWrite(PIN_LED, enable ? HIGH : LOW);
  if (enable) {
    alarmStartMs = millis();
  }
}

// 蜂鸣器模式：报警持续时发声（不影响其他功能）
void updateBuzzer() {
  if (!alarmActive) {
    noTone(PIN_BUZZER);
    return;
  }
  // 简单的节奏鸣叫：1秒周期，0.2秒响、0.8秒静
  uint32_t t = millis() - alarmStartMs;
  if (t % 1000UL < 200UL) {
    tone(PIN_BUZZER, 2000); // 2kHz
  } else {
    noTone(PIN_BUZZER);
  }
}

// TM1637显示帮助：显示整数（不足位数自动补0由库决定；这里限制在4位）
void showNumberOnTM(uint16_t value) {
  // TM1637Display 默认支持显示四位整数，超过四位会溢出/截断，符合实际显示用途
  tm1637.clearDisplay();
  tm1637.showNumberDec(value, false, 4, 0);
}

void showTarget() {
  // 显示：目标产量（每分钟）
  uint32_t v = TARGET_PROD_PER_MIN;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void showActual() {
  // 显示：累计产量
  uint32_t v = totalCount;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void showDowntime() {
  // 显示：停机次数
  uint32_t v = downtimeEvents;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void updateDisplay() {
  switch (displayMode) {
    case SHOW_TARGET: showTarget(); break;
    case SHOW_ACTUAL: showActual(); break;
    case SHOW_DOWNTIME: showDowntime(); break;
  }
}

// 传感器计数：用外部中断更可靠；但需求只给 D2，UNO D2 对应 INT0
// 由于未给出传感器是高电平脉冲还是低电平脉冲，这里做为“任意边沿计数”更稳妥
// 仍保持不简化行为：每次触发即认为“产品通过一次”。
void sensorISR() {
  // 简单“闸门”避免极短抖动多计数：最小间隔 2ms
  static uint32_t lastIsrMs = 0;
  uint32_t now = millis();
  if (now - lastIsrMs < 2) return;
  lastIsrMs = now;

  totalCount++;
  lastSensorTickMs = now;
}

// 读取并消抖按钮（D3：停线，D7：切换显示）
void updateButtons() {
  // D3
  bool stopReading = digitalRead(PIN_STOP_BUTTON);
  if (stopReading != lastStopButtonReading) {
    stopButtonLastChangeMs = millis();
    lastStopButtonReading = stopReading;
  } else {
    if ((millis() - stopButtonLastChangeMs) >= DEBOUNCE_MS && stopReading != stopButtonStable) {
      stopButtonStable = stopReading;
    }
  }

  // D7
  bool togReading = digitalRead(PIN_DISPLAY_TOGGLE);
  if (togReading != lastToggleButtonReading) {
    toggleButtonLastChangeMs = millis();
    lastToggleButtonReading = togReading;
  } else {
    if ((millis() - toggleButtonLastChangeMs) >= DEBOUNCE_MS && togReading != toggleButtonStable) {
      // 只在“按下事件”上翻页：假设按下为HIGH（若按下为LOW，请在此处取反）
      // 为保持通用，这里以“稳定状态从0->1”作为按下。
      bool previous = toggleButtonStable;
      toggleButtonStable = togReading;
      if (!previous && toggleButtonStable) {
        displayMode = static_cast<DisplayMode>((displayMode + 1) % 3);
        updateDisplay();
      }
    }
  }
}

// 产量达成率：实际累计 / (目标每分钟 * 已运行分钟数)
// 需求：实时显示累计产量和达成率（这里通过串口输出“实时达成率”并保留TM1637显示模式开关）
uint32_t startMs = 0;

void printRealtimeStatsIfDue() {
  static uint32_t lastPrintMs = 0;
  const uint32_t period = 1000UL; // 每秒
  uint32_t now = millis();
  if (now - lastPrintMs < period) return;
  lastPrintMs = now;

  uint32_t elapsedMs = now - startMs;
  uint32_t elapsedMin = elapsedMs / 60000UL;

  // 若运行不足1分钟，按“目标进度随时间线性计算”：目标 = TARGET_PROD_PER_MIN * (elapsedMs/60000)
  // 为避免浮点，使用整数比例：达成率(%) = totalCount * 100 / targetSoFar
  // targetSoFar = TARGET_PROD_PER_MIN * elapsedMs / 60000
  uint64_t targetSoFar = 0;
  if (elapsedMs > 0) {
    targetSoFar = (uint64_t)TARGET_PROD_PER_MIN * (uint64_t)elapsedMs / 60000ULL;
  }

  uint32_t ratePercent = 0;
  if (targetSoFar == 0) {
    ratePercent = 0;
  } else {
    ratePercent = (uint32_t)((uint64_t)totalCount * 100ULL / targetSoFar);
    if (ratePercent > 9999) ratePercent = 9999;
  }

  Serial.print("[STAT] total=");
  Serial.print(totalCount);
  Serial.print(" rate=");
  Serial.print(ratePercent);
  Serial.println("%");
}

// 处理“连续30秒无产品通过或人工停线则报警并记录停机事件”
// 判定含义：
// - 若人工停线按钮持续触发：计时满足30秒即报警并记录（并且在人工停线期间保持报警）
// - 若传感器在30秒内没有计数：同样报警并记录
// - 一旦恢复到有产品通过且未人工停线：清除计时/停止报警
void updateNoProductOrStopAlarm() {
  bool stop = stopButtonStable;
  uint32_t now = millis();

  // 是否存在“产品通过信号最近发生”
  bool hasRecentProduct = false;
  if (lastSensorTickMs != 0) {
    hasRecentProduct = (now - lastSensorTickMs) < NO_PRODUCT_ALARM_INTERVAL_MS;
  }

  // 条件：无产品或人工停线（或两者之一）计入同一段“连续时间”
  bool conditionBad = (!hasRecentProduct) || stop;

  if (conditionBad) {
    if (!noProductOrStopActive) {
      noProductOrStopActive = true;
      noProductOrStopStartMs = now;
    }
    // 到达30秒则报警并记录停机事件（只记录一次/这段事件一次）
    if (!alarmActive && (now - noProductOrStopStartMs) >= NO_PRODUCT_ALARM_INTERVAL_MS) {
      setAlarm(true);
      downtimeEvents++;
      // 原因优先标注“人工停线”或“无产品”
      if (stop) {
        logEvent(now, "manual_stop_or_no_product_timeout(manual_stop)");
      } else {
        logEvent(now, "no_product_through_timeout");
      }
    }
  } else {
    // 恢复正常：清除报警/计时状态
    noProductOrStopActive = false;
    noProductOrStopStartMs = 0;
    setAlarm(false);
  }
}

void setup() {
  // Serial
  Serial.begin(9600);

  // I/O
  pinMode(PIN_SENSOR, INPUT_PULLUP);        // D2: 光电计数传感器（按常见用法上拉；若为开漏/有电平相反可按实际调整）
  pinMode(PIN_STOP_BUTTON, INPUT_PULLUP); // D3: 人工停线按钮
  pinMode(PIN_DISPLAY_TOGGLE, INPUT_PULLUP); // D7: 切换显示
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  // TM1637
  tm1637.setBrightness(0x0f); // 亮度最大

  // 初始化按键状态读取
  lastStopButtonReading = digitalRead(PIN_STOP_BUTTON);
  stopButtonStable = lastStopButtonReading;
  stopButtonLastChangeMs = millis();

  lastToggleButtonReading = digitalRead(PIN_DISPLAY_TOGGLE);
  toggleButtonStable = lastToggleButtonReading;
  toggleButtonLastChangeMs = millis();

  // 传感器中断：D2 对应 INT0
  // 配置为 CHANGE，确保脉冲边沿变化都可计数（与“通过计数”一致，具体由传感器输出决定）
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), sensorISR, CHANGE);

  // TM1637初始显示目标
  updateDisplay();

  startMs = millis();
  lastSensorTickMs = 0;

  Serial.println("[BOOT] Production Line Cycle Monitor Ready");
  Serial.print("[CONFIG] TARGET_PROD_PER_MIN=");
  Serial.println(TARGET_PROD_PER_MIN);
}

void loop() {
  updateButtons();

  // 达成率与累计产量：实时输出串口
  printRealtimeStatsIfDue();

  // 报警判定与停机事件记录
  updateNoProductOrStopAlarm();

  // 报警蜂鸣器更新
  updateBuzzer();

  // 若TM1637需要随模式动态更新，则在loop周期性刷新
  static uint32_t lastDispUpdateMs = 0;
  uint32_t now = millis();
  if (now - lastDispUpdateMs >= 200UL) { // 5Hz刷新
    lastDispUpdateMs = now;
    updateDisplay();
  }

  // 串口空闲超时参数在需求中给出，但此程序主要使用 line_or_idle 模式无需特别处理。
}