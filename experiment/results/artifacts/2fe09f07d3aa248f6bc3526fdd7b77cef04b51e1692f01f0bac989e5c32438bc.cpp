#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// ---------- Authoritative pin assignments ----------
static const uint8_t PIN_BTN_ITEM1  = 2;   // D2
static const uint8_t PIN_BTN_ITEM2  = 3;   // D3
static const uint8_t PIN_BTN_ITEM3  = 4;   // D4 (CLK冲突由硬件决定；代码保持原行为语义)
static const uint8_t PIN_BTN_ADMIN  = 7;   // D7

static const uint8_t PIN_LED_STATUS = 13;  // D13
static const uint8_t PIN_BUZZER     = 6;   // D6

static const uint8_t PIN_TM1637_CLK  = 4;  // CLK=D4
static const uint8_t PIN_TM1637_DIO  = 5;  // DIO=D5

static const uint32_t SERIAL_BAUD = 9600;

// ---------- TM1637 ----------
TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

// --------- 商品配置：价格(分)与库存 ----------
static const uint8_t NUM_ITEMS = 3;
static int itemPrice[NUM_ITEMS] = { 500, 800, 1200 };
static int itemStock[NUM_ITEMS] = { 5, 3, 2 };

// ---------- 显示：商品编号和价格 ----------
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void showItem(int itemIndex);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void showErrorCode(uint8_t code);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool readButtonRaw(const Button &b);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void updateButton(Button &btn, unsigned long now);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool checkPressedEvent(Button &btn);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool checkReleasedEvent(Button &btn);
#line 94 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool checkLongPressEvent(Button &btn, unsigned long now);
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void playBeep(uint16_t freq, uint16_t durationMs, uint8_t times);
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void confirmDispense();
#line 156 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool readLineWithTimeout(String &out, uint16_t timeoutMs);
#line 173 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static int parseItemIndexFromString(const String &s);
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static int parseStockFromString(const String &s);
#line 222 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static int parseItemBySimpleFormat(const String &s);
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void applyAdminRestock(int item, int stockAdd);
#line 253 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void adminModeLoop();
#line 300 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void setup();
#line 351 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void showItem(int itemIndex) {
  int idx = constrain(itemIndex, 1, 3);
  int price = itemPrice[idx - 1];
  int lastTwo = abs(price) % 100;

  int toShow = idx * 100 + lastTwo; // “编号 + 价格后两位”
  display.showNumberDecEx(toShow, 0b01000000, true);
}

// 错误显示：E1表示库存不足（用数字编码显示）
static void showErrorCode(uint8_t code) {
  int toShow = (code % 100);
  display.showNumberDecEx(toShow, 0b01000000, false);
}

// --------- 按键消抖与事件检测 ----------
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastRaw;
  unsigned long lastChangeMs;
  unsigned long lastPressMs;
  bool pressedEventFired;
};

static inline bool readButtonRaw(const Button &b) {
  return (digitalRead(b.pin) == LOW); // INPUT_PULLUP: 低电平表示按下
}

static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONGPRESS_MS = 1500;

static void updateButton(Button &btn, unsigned long now) {
  bool raw = readButtonRaw(btn);

  if (raw != btn.lastRaw) {
    btn.lastRaw = raw;
    btn.lastChangeMs = now;
  }

  if ((now - btn.lastChangeMs) >= DEBOUNCE_MS) {
    if (raw != btn.lastStable) {
      btn.lastStable = raw;
      if (btn.lastStable) {
        btn.lastPressMs = now;
      }
    }
  }
}

static bool checkPressedEvent(Button &btn) {
  if (btn.lastStable && !btn.pressedEventFired) {
    btn.pressedEventFired = true;
    return true;
  }
  return false;
}

static bool checkReleasedEvent(Button &btn) {
  if (!btn.lastStable && btn.pressedEventFired) {
    btn.pressedEventFired = false;
    return true;
  }
  return false;
}

// 只触发一次：当持续按下超过阈值且还未触发过
static bool checkLongPressEvent(Button &btn, unsigned long now) {
  if (btn.lastStable) {
    bool clickedOrFired = btn.pressedEventFired;
    if (!clickedOrFired) {
      if ((now - btn.lastPressMs) >= LONGPRESS_MS) {
        btn.pressedEventFired = true; // 锁定，防止再次触发
        return true;
      }
    }
  }
  return false;
}

// 按键实例
static Button btnItem1 { PIN_BTN_ITEM1, false, false, 0, 0, false };
static Button btnItem2 { PIN_BTN_ITEM2, false, false, 0, 0, false };
static Button btnItem3 { PIN_BTN_ITEM3, false, false, 0, 0, false };
static Button btnAdmin { PIN_BTN_ADMIN, false, false, 0, 0, false };

// --------- 蜂鸣 ----------
static void playBeep(uint16_t freq, uint16_t durationMs, uint8_t times) {
  for (uint8_t i = 0; i < times; i++) {
    tone(PIN_BUZZER, freq, durationMs);
    delay(durationMs + 30);
  }
  noTone(PIN_BUZZER);
}

// --------- 确认售货逻辑 ----------
static int selectedItem = 1;

static void confirmDispense() {
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

    showItem(idx);
  } else {
    Serial.print("error:stock_not_enough:item");
    Serial.println(idx);

    digitalWrite(PIN_LED_STATUS, HIGH);
    playBeep(2500, 80, 6);
    digitalWrite(PIN_LED_STATUS, LOW);

    showErrorCode(1);
  }
}

// --------- 管理模式：长按 D7 进入，通过串口补货 ----------
static bool adminModeActive = false;

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
  int p = s.indexOf("item");
  if (p >= 0 && p + 4 < (int)s.length()) {
    char c = s[p + 4];
    if (c >= '1' && c <= '9') return c - '0';
  }
  return -1;
}

static int parseStockFromString(const String &s) {
  int p = s.indexOf("stock");
  if (p >= 0) {
    int eq = s.indexOf('=', p);
    if (eq >= 0) {
      int start = eq + 1;
      int end = start;
      while (end < (int)s.length() && (isDigit(s[end]) || s[end] == '-')) end++;
      if (end > start) return s.substring(start, end).toInt();
    }

    int start = p;
    while (start < (int)s.length() && s[start] != ' ') start++;
    while (start < (int)s.length() && s[start] == ' ') start++;
    if (start < (int)s.length()) {
      int end = start;
      while (end < (int)s.length() && (isDigit(s[end]) || s[end] == '-')) end++;
      if (end > start) return s.substring(start, end).toInt();
    }
  }

  // 兜底：最后一个整数
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
  int nums[2] = { -1, -1 };
  int count = 0;
  int i = 0;
  while (i < (int)s.length() && count < 2) {
    while (i < (int)s.length() && !isDigit(s[i]) && s[i] != '-') i++;
    if (i >= (int)s.length()) break;
    if (s[i] == '-') i++;
    int st = i;
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

  showItem(item);
}

static void adminModeLoop() {
  static bool first = true;
  if (first) {
    first = false;
    digitalWrite(PIN_LED_STATUS, HIGH);
    playBeep(1800, 150, 2);
    digitalWrite(PIN_LED_STATUS, LOW);

    // 管理提示：显示“99”
    display.showNumberDecEx(99, 0b01000000, false);
  }

  String line;
  if (readLineWithTimeout(line, 10)) {
    line.trim();
    if (line.length() == 0) return;

    if (line == "exit" || line == "EXIT" || line == "Exit") {
      Serial.println("ack:admin_exit");
      adminModeActive = false;
      first = true;

      digitalWrite(PIN_LED_STATUS, LOW);
      showItem(selectedItem);
      return;
    }

    int item = parseItemIndexFromString(line);
    int stockAdd = parseStockFromString(line);

    if (item < 1 || item > 3) {
      int it2 = parseItemBySimpleFormat(line);
      if (it2 >= 1 && it2 <= 3) {
        item = it2;
        stockAdd = parseStockFromString(line);
      }
    }

    if (item >= 1 && item <= 3 && stockAdd >= 0) {
      applyAdminRestock(item, stockAdd);
    } else {
      Serial.println("error:bad_restock_format");
    }
  }
}

// --------- setup/loop ----------
void setup() {
  pinMode(PIN_BTN_ITEM1, INPUT_PULLUP);
  pinMode(PIN_BTN_ITEM2, INPUT_PULLUP);
  pinMode(PIN_BTN_ITEM3, INPUT_PULLUP); // 按要求配置：与TM1637 CLK冲突由硬件决定

  pinMode(PIN_BTN_ADMIN, INPUT_PULLUP);

  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);

  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);

  Serial.begin(SERIAL_BAUD);

  display.setBrightness(0x0f);
  display.clear();

  selectedItem = 1;
  showItem(selectedItem);

  unsigned long now = millis();

  btnItem1.lastStable = readButtonRaw(btnItem1);
  btnItem2.lastStable = readButtonRaw(btnItem2);
  btnItem3.lastStable = readButtonRaw(btnItem3);
  btnAdmin.lastStable  = readButtonRaw(btnAdmin);

  btnItem1.lastRaw = btnItem1.lastStable;
  btnItem2.lastRaw = btnItem2.lastStable;
  btnItem3.lastRaw = btnItem3.lastStable;
  btnAdmin.lastRaw  = btnAdmin.lastStable;

  btnItem1.lastChangeMs = now;
  btnItem2.lastChangeMs = now;
  btnItem3.lastChangeMs = now;
  btnAdmin.lastChangeMs  = now;

  btnItem1.lastPressMs = 0;
  btnItem2.lastPressMs = 0;
  btnItem3.lastPressMs = 0;
  btnAdmin.lastPressMs  = 0;

  btnItem1.pressedEventFired = false;
  btnItem2.pressedEventFired = false;
  btnItem3.pressedEventFired = false;
  btnAdmin.pressedEventFired  = false;

  adminModeActive = false;
}

void loop() {
  unsigned long now = millis();

  updateButton(btnItem1, now);
  updateButton(btnItem2, now);
  updateButton(btnItem3, now);
  updateButton(btnAdmin, now);

  // 管理模式进入：长按 D7
  if (!adminModeActive) {
    if (checkLongPressEvent(btnAdmin, now)) {
      adminModeActive = true;
      Serial.println("ack:admin_enter");
      display.showNumberDecEx(88, 0b01000000, false);
      return;
    }
  }

  if (adminModeActive) {
    adminModeLoop();

    // 管理模式中也允许按 D7 长按退出（再次长按）
    if (checkLongPressEvent(btnAdmin, now)) {
      adminModeActive = false;
      Serial.println("ack:admin_exit");
      int priceLastTwo = abs(itemPrice[selectedItem - 1]) % 100;
      display.showNumberDecEx(selectedItem * 100 + priceLastTwo, 0b01000000, true);

      digitalWrite(PIN_LED_STATUS, LOW);
    }
    return;
  }

  // 选择商品（短按）
  if (checkPressedEvent(btnItem1)) { selectedItem = 1; showItem(selectedItem); }
  if (checkPressedEvent(btnItem2)) { selectedItem = 2; showItem(selectedItem); }
  if (checkPressedEvent(btnItem3)) { selectedItem = 3; showItem(selectedItem); }

  // 确认键 D7：短按确认售货
  if (checkPressedEvent(btnAdmin)) {
    confirmDispense();
  }

  // 释放复位 one-shot
  (void)checkReleasedEvent(btnItem1);
  (void)checkReleasedEvent(btnItem2);
  (void)checkReleasedEvent(btnItem3);
  (void)checkReleasedEvent(btnAdmin);
}
