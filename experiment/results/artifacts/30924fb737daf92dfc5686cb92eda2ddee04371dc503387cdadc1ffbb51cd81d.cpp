#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------------------- Authoritative pin assignments (Uno: use numeric pins) --------------------
#define PIN_SENSOR           2   // D2 光电计数传感器
#define PIN_STOP_BUTTON     3   // D3 人工停线按钮
#define PIN_DISPLAY_TOGGLE  7   // D7 切换显示：目标 / 实际 / 停机次数
#define PIN_BUZZER           6   // D6 蜂鸣器
#define PIN_LED             13   // D13 LED

#define TM1637_CLK          4   // D4 CLK
#define TM1637_DIO          5   // D5 DIO

// -------------------- User-configurable parameters --------------------
static const uint32_t TARGET_PROD_PER_MIN = 300;         // 每分钟目标产量
static const uint32_t NO_PRODUCT_ALARM_INTERVAL_MS = 30000UL; // 连续30秒无产品或停线则报警
static const uint16_t DEBOUNCE_MS = 30;                  // 按键消抖
static const uint16_t TM_DISPLAY_REFRESH_MS = 200;      // 5Hz刷新
static const uint32_t SERIAL_PRINT_PERIOD_MS = 1000UL;  // 串口每秒输出统计

// 蜂鸣器节奏：1秒周期，0.2秒响、0.8秒静
static const uint16_t BUZZ_ON_MS = 200;
static const uint16_t BUZZ_PERIOD_MS = 1000;

// TM1637Display库：构造
TM1637Display tm1637(TM1637_CLK, TM1637_DIO);

// -------------------- State variables --------------------
volatile uint32_t totalCount = 0;         // 累计产量（由光电计数）
volatile uint32_t lastSensorTickMs = 0; // 上次检测到产品通过的时间戳

// 停线按钮消抖（D3）
bool stopButtonStable = false;
bool lastStopButtonReading = false;
uint32_t stopButtonLastChangeMs = 0;

// 显示切换按钮消抖（D7）
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

// “连续30秒无产品通过或人工停线”判定用状态
uint32_t noProductOrStopStartMs = 0;
bool noProductOrStopActive = false;

// 运行起始时间
uint32_t startMs = 0;

// 记录停机事件（当触发报警时记录一次/这段事件一次）
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void logEvent(uint32_t tsMs, const char* reason);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setAlarm(bool enable);
#line 74 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateBuzzer();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void tmClear();
#line 95 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void showNumberOnTM(uint16_t value);
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void showTarget();
#line 106 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void showActual();
#line 112 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void showDowntime();
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateDisplay();
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void sensorISR();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateButtons();
#line 175 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void printRealtimeStatsIfDue();
#line 207 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void updateNoProductOrStopAlarm();
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 276 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 60 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_030\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void logEvent(uint32_t tsMs, const char* reason) {
  Serial.print("[EVENT] t=");
  Serial.print(tsMs);
  Serial.print("ms reason=");
  Serial.println(reason);
}

void setAlarm(bool enable) {
  if (enable == alarmActive) return;
  alarmActive = enable;
  digitalWrite(PIN_LED, enable ? HIGH : LOW);
  if (enable) alarmStartMs = millis();
}

void updateBuzzer() {
  if (!alarmActive) {
    noTone(PIN_BUZZER);
    return;
  }
  uint32_t t = millis() - alarmStartMs;
  if (t % BUZZ_PERIOD_MS < BUZZ_ON_MS) {
    tone(PIN_BUZZER, 2000);
  } else {
    noTone(PIN_BUZZER);
  }
}

// 兼容不同TM1637Display库：clearDisplay/clear 都可能不存在
static void tmClear() {
  // TM1637Display通常提供清屏：clear(), 或直接用 showNumberDec(0,...)覆盖
  // 这里优先调用clear()；若无则用showNumberDec(0)近似清屏。
  tm1637.clear();
}

// 显示整数（四位）
void showNumberOnTM(uint16_t value) {
  tmClear();
  tm1637.showNumberDec(value, false, 4, 0);
}

void showTarget() {
  uint32_t v = TARGET_PROD_PER_MIN;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void showActual() {
  uint32_t v = totalCount;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void showDowntime() {
  uint32_t v = downtimeEvents;
  if (v > 9999) v = 9999;
  showNumberOnTM((uint16_t)v);
}

void updateDisplay() {
  switch (displayMode) {
    case SHOW_TARGET:  showTarget(); break;
    case SHOW_ACTUAL:   showActual(); break;
    case SHOW_DOWNTIME: showDowntime(); break;
  }
}

// 传感器中断：D2(UNO INT0) “任意边沿计数”
void sensorISR() {
  static uint32_t lastIsrMs = 0;
  uint32_t now = millis();
  if (now - lastIsrMs < 2) return; // 最小间隔2ms避免极短抖动多计数
  lastIsrMs = now;

  totalCount++;
  lastSensorTickMs = now;
}

// 读取并消抖按钮
void updateButtons() {
  // D3 停线：按下=停线（按常见：INPUT_PULLUP，按下为LOW）
  bool stopReading = digitalRead(PIN_STOP_BUTTON);
  // 将“按下=停线”映射为 stable=true
  // 若硬件相反，可在此处调整：这里按“按下为LOW”处理
  bool stopPressed = (stopReading == LOW);

  if (stopPressed != lastStopButtonReading) {
    stopButtonLastChangeMs = millis();
    lastStopButtonReading = stopPressed;
  } else {
    if ((millis() - stopButtonLastChangeMs) >= DEBOUNCE_MS && stopPressed != stopButtonStable) {
      stopButtonStable = stopPressed;
    }
  }

  // D7 显示切换：按下翻页（按下为LOW）
  bool togReading = digitalRead(PIN_DISPLAY_TOGGLE);
  bool togPressed = (togReading == LOW);

  if (togPressed != lastToggleButtonReading) {
    toggleButtonLastChangeMs = millis();
    lastToggleButtonReading = togPressed;
  } else {
    if ((millis() - toggleButtonLastChangeMs) >= DEBOUNCE_MS && togPressed != toggleButtonStable) {
      bool previous = toggleButtonStable;
      toggleButtonStable = togPressed;
      // 只在“按下事件”上翻页：previous=false -> stable=true
      if (!previous && toggleButtonStable) {
        displayMode = (DisplayMode)((displayMode + 1) % 3);
        updateDisplay();
      }
    }
  }
}

// 计算并串口输出实时达成率
void printRealtimeStatsIfDue() {
  static uint32_t lastPrintMs = 0;
  uint32_t now = millis();
  if (now - lastPrintMs < SERIAL_PRINT_PERIOD_MS) return;
  lastPrintMs = now;

  uint32_t elapsedMs = now - startMs;
  uint32_t elapsedMin = elapsedMs / 60000UL;
  (void)elapsedMin; // 保留变量以匹配原逻辑意图

  uint64_t targetSoFar = 0;
  if (elapsedMs > 0) {
    targetSoFar = (uint64_t)TARGET_PROD_PER_MIN * (uint64_t)elapsedMs / 60000ULL;
  }

  uint32_t ratePercent = 0;
  uint32_t countCopy = totalCount; // 原子性不强制，但UNO 32-bit读基本OK；如需可关中断
  if (targetSoFar == 0) {
    ratePercent = 0;
  } else {
    ratePercent = (uint32_t)((uint64_t)countCopy * 100ULL / targetSoFar);
    if (ratePercent > 9999) ratePercent = 9999;
  }

  Serial.print("[STAT] total=");
  Serial.print(countCopy);
  Serial.print(" rate=");
  Serial.print(ratePercent);
  Serial.println("%");
}

// 处理“连续30秒无产品通过或人工停线则报警并记录停机事件”
void updateNoProductOrStopAlarm() {
  bool stop = stopButtonStable;
  uint32_t now = millis();

  bool hasRecentProduct = false;
  if (lastSensorTickMs != 0) {
    hasRecentProduct = (now - lastSensorTickMs) < NO_PRODUCT_ALARM_INTERVAL_MS;
  }

  bool conditionBad = (!hasRecentProduct) || stop;

  if (conditionBad) {
    if (!noProductOrStopActive) {
      noProductOrStopActive = true;
      noProductOrStopStartMs = now;
    }
    if (!alarmActive && (now - noProductOrStopStartMs) >= NO_PRODUCT_ALARM_INTERVAL_MS) {
      setAlarm(true);
      downtimeEvents++;

      if (stop) {
        logEvent(now, "manual_stop_or_no_product_timeout(manual_stop)");
      } else {
        logEvent(now, "no_product_through_timeout");
      }
    }
  } else {
    noProductOrStopActive = false;
    noProductOrStopStartMs = 0;
    setAlarm(false);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(PIN_SENSOR, INPUT_PULLUP);         // D2
  pinMode(PIN_STOP_BUTTON, INPUT_PULLUP);  // D3
  pinMode(PIN_DISPLAY_TOGGLE, INPUT_PULLUP);// D7
  pinMode(PIN_BUZZER, OUTPUT);              // D6
  pinMode(PIN_LED, OUTPUT);                // D13

  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  tm1637.setBrightness(0x0f);

  // 初始化按钮状态
  lastStopButtonReading = (digitalRead(PIN_STOP_BUTTON) == LOW);
  stopButtonStable = lastStopButtonReading;
  stopButtonLastChangeMs = millis();

  lastToggleButtonReading = (digitalRead(PIN_DISPLAY_TOGGLE) == LOW);
  toggleButtonStable = lastToggleButtonReading;
  toggleButtonLastChangeMs = millis();

  // 传感器中断：D2 对应 INT0
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), sensorISR, CHANGE);

  updateDisplay();

  startMs = millis();
  lastSensorTickMs = 0;

  Serial.println("[BOOT] Production Line Cycle Monitor Ready");
  Serial.print("[CONFIG] TARGET_PROD_PER_MIN=");
  Serial.println(TARGET_PROD_PER_MIN);
}

void loop() {
  updateButtons();

  printRealtimeStatsIfDue();

  updateNoProductOrStopAlarm();

  updateBuzzer();

  static uint32_t lastDispUpdateMs = 0;
  uint32_t now = millis();
  if (now - lastDispUpdateMs >= TM_DISPLAY_REFRESH_MS) {
    lastDispUpdateMs = now;
    updateDisplay();
  }
}
