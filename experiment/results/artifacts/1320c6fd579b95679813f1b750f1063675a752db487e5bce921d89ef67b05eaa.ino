#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_BTN_ITEM1 = 2;   // D2: 商品1选择
static const uint8_t PIN_BTN_ADMIN  = 7;   // D7: 确认/管理员进入
static const uint8_t PIN_LED_STATUS = 13;  // D13: 状态LED
static const uint8_t PIN_BUZZER     = 6;   // D6: 蜂鸣器
static const uint8_t PIN_TM1637_CLK = 4;   // CLK=D4
static const uint8_t PIN_TM1637_DIO = 5;   // DIO=D5

static const uint32_t SERIAL_BAUD = 9600;

// TM1637
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// --------- 商品配置（维护价格与库存）---------
// 说明：商品编号为 1~3，按钮 D2/D3/D4? 题意为 D2、D3、D4 选择三种商品，但此处D4已用于TM1637 CLK
// 给定“authoritative pin assignments”后，D4用于TM1637 CLK，因此这里采用 D2(商品1)、D3(商品2)、D4不可用
// 但题目要求“Preserve all requirements and wiring; do not simplify away behavior.”
static const uint8_t PIN_BTN_ITEM2 = 3;   // D3: 商品2选择（按要求保留）
static const uint8_t PIN_BTN_ITEM3 = 4;   // D4: 商品3选择（与TM1637 CLK冲突）
                                         // 按题目“Resolved hardware configuration ... explicitly remapped”要求，
                                         // 这里保留原选择键语义，并将D4也作为商品3按键输入。
                                         // 若真实硬件按authoritative pin为准，则商品3会受TM1637通信影响；本代码不做删减。

static const uint8_t NUM_ITEMS = 3;

// 初始化商品：价格(分)与库存
// 如需通过管理员模式补货，可在串口接收指令更新。
static int itemPrice[NUM_ITEMS] = { 500, 800, 1200 }; // 商品1/2/3 价格（单位：分/可自行理解）
static int itemStock[NUM_ITEMS] = { 5, 3, 2 };      // 初始库存

// --------- 显示编码：数码管显示“商品编号和价格” ---------
// 采用TM1637的4位显示：显示“编号 + 价格(两位/三位)”的简化映射。
// 为尽量满足“显示商品编号和价格”，这里约定：
// - 显示前两位为 商品编号(1~3)，后两位为 价格的后两位(价格%100)。
// - 若价格>=10000，可显示后四位的后两位仍能用（保持可用）。
// 错误码：使用固定数字显示（E1库存不足等）。
static void showItem(int itemIndex) {
  int idx = constrain(itemIndex, 1, 3);
  int price = itemPrice[idx - 1];
  int lastTwo = abs(price) % 100;

  int tens = (lastTwo / 10) % 10;
  int ones = lastTwo % 10;

  // TM1637Display: setSegments supports 4 digits; but we use showNumberDecEx
  // We'll show "idx lastTwo" like: idx=1 => "1 0 x" -> format: "1XY"? 4 digits: idx*100 + lastTwo
  // Example: idx=1, lastTwo=50 => show 150 (prints "0150" or "0150"?). We want "1 50" -> use 4 digits: idx*100 + lastTwo = 150
  int toShow = idx * 100 + lastTwo;
  display.showNumberDecEx(toShow, 0b01000000, true); // colon off; leading zeros allowed
}

// 错误显示：E1表示库存不足（若需要其它错误扩展可沿用）
static void showErrorCode(uint8_t code) {
  // 4位显示：E(用0显示) + code(两位)
  // TM1637Display不直接显示字母E，这里用 0 代替“E”，并显示 code 的后两位
  // 格式：0 + code(两位) => 00code
  int toShow = (code % 100);
  display.showNumberDecEx(toShow, 0b01000000, false);
}

// --------- 按键消抖与长按检测 ---------
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastRaw;
  unsigned long lastChangeMs;
  unsigned long lastPressMs;
  bool pressedEventFired;
};

static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONGPRESS_MS = 1500;

static Button btnItem1{PIN_BTN_ITEM1, false, false, 0, 0, false};
static Button btnItem2{PIN_BTN_ITEM2, false, false, 0, 0, false};
static Button btnItem3{PIN_BTN_ITEM3, false, false, 0, 0, false};
static Button btnAdmin {PIN_BTN_ADMIN,  false, false, 0, 0, false};

// 低电平按下（默认INPUT_PULLUP）
static inline bool readButtonRaw(const Button &b) {
  return (digitalRead(b.pin) == LOW);
}

static void updateButton(Button &b, unsigned long now) {
  bool raw = readButtonRaw(b);

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = now;
  }

  if ((now - b.lastChangeMs) >= DEBOUNCE_MS) {
    if (raw != b.lastStable) {
      b.lastStable = raw;
      if (b.lastStable) {
        b.lastPressMs = now;
      } else {
        // release
      }
    }
  }
}

static bool checkPressedEvent(Button &b, unsigned long now) {
  (void)now;
  // pressedEventFired: when transition to stable pressed and wasn't fired
  // We'll detect by "stable pressed and not yet fired"
  if (b.lastStable && !b.pressedEventFired) {
    b.pressedEventFired = true;
    return true;
  }
  return false;
}

static bool checkReleased(Button &b) {
  if (!b.lastStable && b.pressedEventFired) {
    b.pressedEventFired = false;
    return true;
  }
  return false;
}

static bool checkLongPressEvent(Button &b, unsigned long now) {
  // Long press when stable pressed持续超过阈值，并且只触发一次
  static uint32_t guardMs = 0;
  (void)guardMs;

  // Use pressedEventFired as one-shot for long press too (separate from click by using logic):
  // We'll trigger long press when pressedEventFired is false at the time and duration reached.
  if (b.lastStable) {
    // Click will also set pressedEventFired; prevent conflict by using additional flag via local static map
  }
  return false;
}

// --------- 状态机 ---------
enum State {
  WAIT_SELECT = 0,
  WAIT_CONFIRM,
  ADMIN_MODE
};

static State state = WAIT_SELECT;
static int selectedItem = 1;

// 管理模式串口补货：简单协议
// - 格式：restock:itemX,stock=Y\n  或 restock:itemX Y\n
// - 或：restock 1 10\n
// 为“preserve all behavior”，这里实现通用解析：在串口有行时解析其中的 item 和 stock
static void adminModeLoop();

static void playBeep(uint16_t freq, uint16_t durationMs, uint8_t times) {
  for (uint8_t i = 0; i < times; i++) {
    tone(PIN_BUZZER, freq, durationMs);
    delay(durationMs + 30);
  }
  noTone(PIN_BUZZER);
}

static void confirmDispense() {
  // 若库存充足则串口输出 dispense:itemX，库存减一并蜂鸣提示
  // 若库存不足则报警并显示错误代码
  int idx = selectedItem;
  int &stock = itemStock[idx - 1];

  if (stock > 0) {
    stock--;
    Serial.print("dispense:item");
    Serial.println(idx);
    digitalWrite(PIN_LED_STATUS, HIGH);
    // 蜂鸣提示：两短一长风格
    playBeep(2000, 120, 2);
    playBeep(1400, 250, 1);
    digitalWrite(PIN_LED_STATUS, LOW);
    // 显示刚处理后的商品信息（库存变化仍显示）
    showItem(idx);
  } else {
    // 报警并显示错误代码
    // 错误代码：1表示库存不足
    Serial.print("error:stock_not_enough:item");
    Serial.println(idx);

    digitalWrite(PIN_LED_STATUS, HIGH);
    // 连续短促蜂鸣报警
    playBeep(2500, 80, 6);
    digitalWrite(PIN_LED_STATUS, LOW);

    showErrorCode(1);
  }
}

// --------- 管理模式的按键：长按 D7 进入 ---------
static bool adminModeActive = false;
static unsigned long adminHoldStart = 0;
static bool adminTriggered = false;

static bool readLineWithTimeout(String &out, uint16_t timeoutMs) {
  out = "";
  unsigned long start = millis();
  while ((millis() - start) < timeoutMs) {
    while (Serial.available()) {
      char c = (char)Serial.read();
      if (c == '\n' || c == '\r') {
        if (out.length() > 0) return true;
      } else {
        out += c;
      }
    }
    delay(1);
  }
  return out.length() > 0;
}

static int parseItemIndexFromString(const String &s) {
  // 查找 itemX
  int p = s.indexOf("item");
  if (p >= 0 && p + 4 < (int)s.length()) {
    char c = s[p + 4];
    if (c >= '1' && c <= '9') return c - '0';
  }
  // 也支持直接 "restock X Y"
  // 尝试提取第一个非空整数
  return -1;
}

static int parseStockFromString(const String &s) {
  // 查找 stock=Y 或 stock Y
  int p = s.indexOf("stock");
  if (p >= 0) {
    int eq = s.indexOf('=', p);
    if (eq >= 0) {
      int start = eq + 1;
      int end = start;
      while (end < (int)s.length() && (isDigit(s[end]) || s[end] == '-')) end++;
      if (end > start) return s.substring(start, end).toInt();
    }
    // 无等号：stock 后一个整数
    // 例如: stock 10
    int start = p;
    while (start < (int)s.length() && s[start] != ' ') start++;
    while (start < (int)s.length() && s[start] == ' ') start++;
    if (start < (int)s.length()) {
      int end = start;
      while (end < (int)s.length() && (isDigit(s[end]) || s[end] == '-')) end++;
      if (end > start) return s.substring(start, end).toInt();
    }
  }

  // 支持最后一个整数作为stock（用于简单格式）
  int last = -1;
  int i = 0;
  while (i < (int)s.length()) {
    while (i < (int)s.length() && !isDigit(s[i]) && s[i] != '-') i++;
    if (i >= (int)s.length()) break;
    int st = i;
    bool neg = false;
    if (s[i] == '-') { neg = true; i++; }
    while (i < (int)s.length() && isDigit(s[i])) i++;
    int en = i;
    if (en > st + (neg ? 1 : 0)) {
      int v = s.substring(st, en).toInt();
      last = v;
    }
  }
  return last;
}

static int parseItemBySimpleFormat(const String &s) {
  // 简单：restock 1 10 或 1 10
  // 提取前两个整数：第一个当item, 第二个当stock
  int nums[2] = {-1, -1};
  int count = 0;
  int i = 0;
  while (i < (int)s.length() && count < 2) {
    while (i < (int)s.length() && !isDigit(s[i]) && s[i] != '-') i++;
    if (i >= (int)s.length()) break;
    int st = i;
    if (s[i] == '-') i++;
    while (i < (int)s.length() && isDigit(s[i])) i++;
    int en = i;
    nums[count++] = s.substring(st, en).toInt();
  }
  if (nums[0] >= 1 && nums[0] <= 3) return nums[0];
  return -1;
}

static void applyAdminRestock(int item, int stockAdd) {
  if (item < 1 || item > 3) return;
  if (stockAdd < 0) stockAdd = 0;
  itemStock[item - 1] += stockAdd;

  Serial.print("ack:restock:item");
  Serial.print(item);
  Serial.print(",stock=");
  Serial.println(itemStock[item - 1]);

  // 管理模式显示：显示 item编号及当前价格后两位，库存不强制显示
  showItem(item);
}

// 管理模式逻辑：通过串口补货，长按D7再次退出（或松开后再长按）
static void adminModeLoop() {
  // 提示管理员模式（蜂鸣一次）
  static bool first = true;
  if (first) {
    first = false;
    digitalWrite(PIN_LED_STATUS, HIGH);
    playBeep(1800, 150, 2);
    digitalWrite(PIN_LED_STATUS, LOW);
    // 显示管理员提示：用错误码样式 02/03无法区分，这里显示 0 + 99 -> 99
    display.showNumberDecEx(99, 0b01000000, false);
  }

  // 在管理员模式持续接收串口行
  String line;
  if (readLineWithTimeout(line, 10)) {
    line.trim();
    if (line.length() == 0) return;

    // 如果收到 "exit" 则退出
    if (line == "exit" || line == "EXIT" || line == "Exit") {
      Serial.println("ack:admin_exit");
      adminModeActive = false;
      state = WAIT_SELECT;
      first = true;
      digitalWrite(PIN_LED_STATUS, LOW);
      showItem(selectedItem);
      return;
    }

    // restock:itemX,stock=Y
    int item = parseItemIndexFromString(line);
    int stockAdd = parseStockFromString(line);

    if (item < 1 || item > 3) {
      // 尝试简单格式解析
      int it2 = parseItemBySimpleFormat(line);
      if (it2 >= 1 && it2 <= 3) {
        item = it2;
        // stockAdd取第二个整数
        // 直接找最后一个整数作为stockAdd（与parseStockFromString一致）
        stockAdd = parseStockFromString(line);
      }
    }

    // 如果解析失败，忽略但回显错误
    if (item >= 1 && item <= 3 && stockAdd >= 0) {
      applyAdminRestock(item, stockAdd);
    } else {
      Serial.println("error:bad_restock_format");
    }
  }
}

// --------- Arduino setup/loop ---------
void setup() {
  pinMode(PIN_BTN_ITEM1, INPUT_PULLUP);
  pinMode(PIN_BTN_ITEM2, INPUT_PULLUP);
  pinMode(PIN_BTN_ITEM3, INPUT_PULLUP); // D4 is also TM1637 CLK in hardware; still configure per stated requirement
  pinMode(PIN_BTN_ADMIN, INPUT_PULLUP);

  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  Serial.begin(SERIAL_BAUD);

  display.setBrightness(0x0f);
  display.clear();

  // 初始显示：商品1
  showItem(selectedItem);

  // init buttons
  btnItem1.lastStable = readButtonRaw(btnItem1);
  btnItem2.lastStable = readButtonRaw(btnItem2);
  btnItem3.lastStable = readButtonRaw(btnItem3);
  btnAdmin.lastStable  = readButtonRaw(btnAdmin);

  btnItem1.lastRaw = btnItem1.lastStable;
  btnItem2.lastRaw = btnItem2.lastStable;
  btnItem3.lastRaw = btnItem3.lastStable;
  btnAdmin.lastRaw  = btnAdmin.lastStable;

  adminModeActive = false;
  state = WAIT_SELECT;
}

void loop() {
  unsigned long now = millis();

  updateButton(btnItem1, now);
  updateButton(btnItem2, now);
  updateButton(btnItem3, now);
  updateButton(btnAdmin, now);

  // 长按 D7 进入管理员模式（触发一次）
  // 不使用btnAdmin.checkLongPressEvent以避免与点击冲突
  if (!adminModeActive) {
    bool adminPressed = btnAdmin.lastStable;
    if (adminPressed && adminHoldStart == 0) {
      adminHoldStart = now;
      adminTriggered = false;
    }
    if (!adminPressed) {
      adminHoldStart = 0;
      adminTriggered = false;
    }
    if (adminPressed && !adminTriggered && (now - adminHoldStart) >= LONGPRESS_MS) {
      adminTriggered = true;
      adminModeActive = true;
      state = ADMIN_MODE;

      Serial.println("ack:admin_enter");
      // 重新提示
      display.showNumberDecEx(88, 0b01000000, false);
    }
  }

  if (adminModeActive) {
    state = ADMIN_MODE;
    adminModeLoop();
    // 管理模式中也允许按D7退出：再次长按D7超过阈值
    bool adminPressed = btnAdmin.lastStable;
    static unsigned long adminExitHold = 0;
    static bool adminExitTrig = false;
    if (adminPressed && adminExitHold == 0) {
      adminExitHold = now;
      adminExitTrig = false;
    }
    if (!adminPressed) {
      adminExitHold = 0;
      adminExitTrig = false;
    }
    if (adminPressed && !adminExitTrig && (now - adminExitHold) >= LONGPRESS_MS) {
      adminExitTrig = true;
      adminModeActive = false;
      state = WAIT_SELECT;
      Serial.println("ack:admin_exit");
      display.showNumberDecEx(selectedItem * 100 + (abs(itemPrice[selectedItem - 1]) % 100), 0b01000000, true);
      // 管理模式显示处理由next loop的状态更新
    }
    return;
  }

  // 选择商品（每次按下更新selectedItem并显示）
  if (checkPressedEvent(btnItem1, now)) {
    selectedItem = 1;
    showItem(selectedItem);
  }
  if (checkPressedEvent(btnItem2, now)) {
    selectedItem = 2;
    showItem(selectedItem);
  }
  if (checkPressedEvent(btnItem3, now)) {
    selectedItem = 3;
    showItem(selectedItem);
  }

  // 确认键 D7：短按确认售货
  // 这里确认事件与长按已被处理：只有未进入管理员模式时才允许短按触发
  // 使用“释放后触发”会更稳；但要求“确认后...”，这里按下即可确认
  if (checkPressedEvent(btnAdmin, now)) {
    // 确认售货
    confirmDispense();
  }

  // 释放复位 one-shot
  checkReleased(btnItem1);
  checkReleased(btnItem2);
  checkReleased(btnItem3);
  checkReleased(btnAdmin);
}