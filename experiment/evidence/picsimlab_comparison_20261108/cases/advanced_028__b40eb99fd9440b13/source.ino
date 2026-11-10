#include <Arduino.h>
#include <TM1637Display.h>

#define BTN1_PIN 2   // 床位1按钮 D2
#define BTN2_PIN 3   // 床位2按钮 D3
#define BTN3_PIN 4   // 床位3按钮 D4（与要求冲突已按权威分配保留）
#define ACK_PIN  7   // 护士确认按钮 D7

// D4 被用于 TM1637 CLK（权威分配：CLK=D4），因此床位3按钮若实际仍连在D4，需硬件已按题意“显式重映射”完成。
// 代码中仍按“床位按钮 D2、D3、D4 对应三张床”的逻辑处理床位3为D4。
// 同时 TM1637 使用 D4/D5；若硬件已完成重映射，请保证床位3不再占用D4，或保持题面所述映射一致。
#define BUZZER_PIN 6  // 蜂鸣器 D6

#define LED_PIN 13    // LED D13

#define TM_CLK_PIN 4  // TM1637 CLK = D4（权威）
#define TM_DIO_PIN 5  // TM1637 DIO = D5（权威）

TM1637Display tm1637(TM_CLK_PIN, TM_DIO_PIN);

static const uint8_t BED_COUNT = 3;
static const uint8_t bedPins[BED_COUNT] = {BTN1_PIN, BTN2_PIN, BTN3_PIN}; // Bed 1..3

// 简单按键消抖参数
static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONG_PRESS_MS = 0; // 不需要长按，只保留占位（不改变行为）

struct CallInfo {
  bool active = false;           // 是否存在未被确认的呼叫
  unsigned long t_call_ms = 0;  // 呼叫时刻
  unsigned long seq = 0;        // 呼叫序号（记录顺序）
  unsigned long t_last_seen_ms = 0; // 可选：用于避免重复
};

CallInfo calls[BED_COUNT];
unsigned long seqCounter = 1;

// 当前报警/提示策略
static const uint16_t BUZZ_ALARM_MS = 150;   // 报警响一下的持续
static const uint16_t BUZZ_PERIOD_MS = 500;  // 报警周期
static const uint16_t BUZZ_FREQ_HZ = 2000;

unsigned long lastBuzzToggleMs = 0;
bool buzzOn = false;

// 等待时间显示：显示当前“等待最久”的床位编号和秒数
// 由于TM1637通常两位/四位，这里用4位显示：床位(1位)+秒(3位)或床位(1位)+秒(2位)；根据显示库由数字写入决定。
// 为稳妥，使用自定义显示：将“bed编号”放百位前一格，秒放后两到三位。
// 这里采用：显示 [bed][sec(0-99)]，若秒>=100则显示秒截断到99（符合常见两位/两位显示）。
static const uint8_t SECONDS_MAX_DISPLAY = 99;

// 按键消抖状态
struct KeyState {
  bool stable = false;
  bool lastRaw = false;
  unsigned long lastChangeMs = 0;
};
KeyState bedKeyState[BED_COUNT];
KeyState ackKeyState;

static unsigned long nowMs() { return millis(); }

static bool readButtonPressed(uint8_t pin) {
  // 采用内部上拉：按下为LOW
  return digitalRead(pin) == LOW;
}

static bool updateDebounced(KeyState &ks, uint8_t pin) {
  bool raw = readButtonPressed(pin);
  if (raw != ks.lastRaw) {
    ks.lastChangeMs = nowMs();
    ks.lastRaw = raw;
  }
  if ((nowMs() - ks.lastChangeMs) >= DEBOUNCE_MS) {
    ks.stable = raw;
  }
  return ks.stable;
}

static bool detectPressedEdge(KeyState &ks, uint8_t pin) {
  // 返回“从未按到按”的上升沿（按下事件）
  bool rawNow = readButtonPressed(pin);
  // 更新消抖
  bool stableNow = updateDebounced(ks, pin);
  (void)rawNow;
  // 检测从stable=false到stable=true
  static bool prevStable[10];
  // 由于数组大小未知，这里不用静态prevStable；改为在外部用一次比较
  // 本函数将改为：通过传入ks并在外部记录上次事件更可靠。
  return stableNow;
}

static uint8_t getOldestBedIndex(bool &found) {
  found = false;
  unsigned long oldestT = 0;
  uint8_t oldestIdx = 0;
  for (uint8_t i = 0; i < BED_COUNT; i++) {
    if (!calls[i].active) continue;
    if (!found || calls[i].t_call_ms < oldestT) {
      oldestT = calls[i].t_call_ms;
      oldestIdx = i;
      found = true;
    }
  }
  return oldestIdx;
}

static void showBedAndSeconds(uint8_t bedIndex1Based, unsigned long seconds) {
  uint8_t bed = bedIndex1Based; // 1..3
  uint8_t sec = (seconds > SECONDS_MAX_DISPLAY) ? SECONDS_MAX_DISPLAY : (uint8_t)seconds;

  // 使用两位加一位展示：bed*100 + sec 需要三位；TM1637Display支持显示整数（按位数会截断）
  // 我们显示为三位数：如床1秒5 => "1 05"（显示整数 105）
  uint16_t value = (uint16_t)bed * 100u + (uint16_t)sec; // 范围 100..305
  tm1637.showNumberDec(value, false, 3, 0);
}

static void alarmBeepStartStop(bool enable) {
  // 蜂鸣器使用 tone，不用停止方式控制在loop里节拍
  if (!enable) {
    noTone(BUZZER_PIN);
    buzzOn = false;
  }
}

static bool allCallsClear() {
  for (uint8_t i = 0; i < BED_COUNT; i++) {
    if (calls[i].active) return false;
  }
  return true;
}

static uint8_t clearHighestPriorityCall() {
  // 需求：确认按钮按下后清除当前最高优先级呼叫
  // “最高优先级”依据：等待时间最长（最久未确认） -> 即t_call最早。
  bool found = false;
  uint8_t idx = getOldestBedIndex(found);
  if (!found) return 0xFF;

  calls[idx].active = false;
  return idx;
}

static void logCall(uint8_t bedIndex1Based, unsigned long callTimeMs) {
  // 输出处理日志：呼叫顺序和等待时间并报警提示
  // 此处呼叫时仅记录“呼叫顺序”，等待时间在确认时计算。
  Serial.print(F("[CALL] Bed "));
  Serial.print(bedIndex1Based);
  Serial.print(F(" Seq "));
  Serial.println(calls[bedIndex1Based - 1].seq);
  (void)callTimeMs;
}

static void logAck(uint8_t bedIndex1Based, unsigned long waitMs, unsigned long seq) {
  Serial.print(F("[ACK ] Bed "));
  Serial.print(bedIndex1Based);
  Serial.print(F(" Seq "));
  Serial.print(seq);
  Serial.print(F(" WaitSec "));
  Serial.println(waitMs / 1000UL);
}

void setup() {
  Serial.begin(9600);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(ACK_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  tm1637.setBrightness(7, true);
  tm1637.showNumberDec(0, false, 3, 0);

  // init call state
  for (uint8_t i = 0; i < BED_COUNT; i++) {
    calls[i].active = false;
    calls[i].t_call_ms = 0;
    calls[i].seq = 0;
    calls[i].t_last_seen_ms = 0;
    bedKeyState[i].stable = readButtonPressed(bedPins[i]);
    bedKeyState[i].lastRaw = readButtonPressed(bedPins[i]);
    bedKeyState[i].lastChangeMs = nowMs();
  }
  ackKeyState.stable = readButtonPressed(ACK_PIN);
  ackKeyState.lastRaw = readButtonPressed(ACK_PIN);
  ackKeyState.lastChangeMs = nowMs();
}

void loop() {
  // 更新按键消抖并检测边沿
  // 为检测边沿，我们保存上一次stable状态
  static bool lastBedStable[BED_COUNT] = {false, false, false};
  static bool lastAckStable = false;

  for (uint8_t i = 0; i < BED_COUNT; i++) {
    bool stableNow = updateDebounced(bedKeyState[i], bedPins[i]);
    if (!lastBedStable[i] && stableNow) {
      // 床位按下事件
      unsigned long t = nowMs();
      if (!calls[i].active) {
        calls[i].active = true;
        calls[i].t_call_ms = t;
        calls[i].seq = seqCounter++;
        calls[i].t_last_seen_ms = t;

        // 呼叫顺序和报警提示
        logCall(i + 1, t);
      } else {
        // 若已呼叫仍按下：不重复创建呼叫（避免顺序乱序），但可保持行为不简化
        // 这里不做额外日志以免干扰“记录呼叫顺序”
      }

      // 报警提示：点亮LED + 蜂鸣提示（在节拍里持续）
      digitalWrite(LED_PIN, HIGH);
    }
    lastBedStable[i] = stableNow;
  }

  bool ackStableNow = updateDebounced(ackKeyState, ACK_PIN);
  static bool lastAckReported = false;
  if (!lastAckReported && !lastAckStable && ackStableNow) {
    // 护士确认按下事件
    bool found = false;
    uint8_t oldestIdx = getOldestBedIndex(found);

    if (found) {
      unsigned long t = nowMs();
      unsigned long waitMs = t - calls[oldestIdx].t_call_ms;
      unsigned long seq = calls[oldestIdx].seq;

      // 清除最高优先级呼叫
      calls[oldestIdx].active = false;

      // 处理日志
      logAck(oldestIdx + 1, waitMs, seq);

      // 若清空所有则停止报警提示/关闭LED；否则继续提示
      if (allCallsClear()) {
        digitalWrite(LED_PIN, LOW);
        alarmBeepStartStop(false);
        tm1637.showNumberDec(0, false, 3, 0);
      } else {
        digitalWrite(LED_PIN, HIGH);
      }
    } else {
      // 没有呼叫也按了确认：不做任何处理日志（保持系统行为合理）
    }
  }
  lastAckReported = ackStableNow;
  lastAckStable = ackStableNow;

  // 显示当前等待最久床位编号和秒数
  bool haveAny = false;
  uint8_t oldest = getOldestBedIndex(haveAny);
  if (haveAny) {
    unsigned long waitedMs = nowMs() - calls[oldest].t_call_ms;
    unsigned long waitedSec = waitedMs / 1000UL;
    showBedAndSeconds(oldest + 1, waitedSec);
  } else {
    // 没有呼叫
    // 保持显示为0
    // tm1637.showNumberDec(0, false, 3, 0); // 在确认清空时已处理，这里也确保
    tm1637.showNumberDec(0, false, 3, 0);
  }

  // 报警提示：任一床位按下后系统报警
  if (!allCallsClear()) {
    unsigned long t = nowMs();
    if (t - lastBuzzToggleMs >= BUZZ_PERIOD_MS) {
      lastBuzzToggleMs = t;
      // 翻转节拍：每个period里先响一段
      buzzOn = true;
      tone(BUZZER_PIN, BUZZ_FREQ_HZ);
    }
    // 在每个period内响的持续时间到就停
    // 这里用简单方法：当到期后下一次period会再启动，因此需要额外用一个phase判定更准确
    // 采用：记录响开始时间并在持续时间后停
    static unsigned long buzzStartMs = 0;
    if (buzzOn && buzzStartMs == 0) buzzStartMs = t;
    if (buzzOn && (t - buzzStartMs >= BUZZ_ALARM_MS)) {
      noTone(BUZZER_PIN);
      buzzOn = false;
      buzzStartMs = 0;
    }
  } else {
    noTone(BUZZER_PIN);
  }
}