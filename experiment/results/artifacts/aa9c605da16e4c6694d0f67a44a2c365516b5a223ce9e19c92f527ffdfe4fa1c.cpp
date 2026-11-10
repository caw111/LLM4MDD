#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (authoritative) --------
static const uint8_t BTN_ITEM1 = 2;    // D2
static const uint8_t BTN_ITEM2 = 3;    // D3
static const uint8_t BTN_ITEM3 = 4;    // D4
static const uint8_t BTN_CONFIRM = 7; // D7 (long-press => admin mode)

static const uint8_t LED_PIN = 13;     // D13
static const uint8_t BUZZER_PIN = 6;   // D6

static const uint8_t TM1637_CLK = 8;    // D8
static const uint8_t TM1637_DIO = 5;    // D5

// -------- TM1637 --------
TM1637Display display(TM1637_CLK, TM1637_DIO);

// -------- Application model --------
static const uint16_t MAX_PRICE_3DIG = 999;

// Prices & stock maintained by admin via serial (RAM only)
static uint16_t priceArr[3] = {125, 250, 399}; // 0..999
static uint16_t stockArr[3] = {5, 5, 5};      // stock count

// -------- Serial protocol helpers --------
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool startsWithToken(const String &cmd, const String &tok);
#line 32 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static int parseItemIndex(const String &s);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static uint16_t clamp_u16(uint32_t v, uint16_t lo, uint16_t hi);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void beep(uint16_t ms);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void showItemAndPrice(uint8_t itemIndex1to3);
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void showErrorCode(uint16_t code);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void clearSerialLine();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool readLineFromSerial(String &outLine);
#line 103 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void processAdminCommand(const String &line);
#line 240 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool updateBtnState(Button &b);
#line 259 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static void dispenseItem(uint8_t itemIndex1to3);
#line 307 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void setup();
#line 336 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
void loop();
#line 27 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\advanced_010\\repeat_1\\direct_feedback\\attempt_5\\sketch\\sketch.ino"
static bool startsWithToken(const String &cmd, const String &tok) {
  if (cmd.length() < tok.length()) return false;
  return cmd.substring(0, tok.length()) == tok;
}

static int parseItemIndex(const String &s) {
  // expects "item1"/"item2"/"item3"
  if (!startsWithToken(s, "item")) return -1;
  int idx = s.substring(4).toInt();
  if (idx >= 1 && idx <= 3) return idx;
  return -1;
}

static uint16_t clamp_u16(uint32_t v, uint16_t lo, uint16_t hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return (uint16_t)v;
}

static void beep(uint16_t ms) {
  tone(BUZZER_PIN, 2000);
  delay(ms);
  noTone(BUZZER_PIN);
}

static void showItemAndPrice(uint8_t itemIndex1to3) {
  uint8_t itemDigit = itemIndex1to3; // 1..3
  uint16_t p = priceArr[itemIndex1to3 - 1];
  if (p > MAX_PRICE_3DIG) p = MAX_PRICE_3DIG;

  // Display 4 digits: [item][price(3 digits)] => item*1000 + price
  uint16_t val = (uint16_t)itemDigit * 1000u + (uint16_t)p;
  display.showNumberDec(val, false, 4, 0);
}

static void showErrorCode(uint16_t code) {
  // Display up to 4 digits (TM1637 truncates to 4 digits)
  display.showNumberDec(code, false, 4, 0);
}

// -------- Serial line reader (idle timeout + newline) --------
static String serialLine;

static void clearSerialLine() { serialLine = ""; }

static bool readLineFromSerial(String &outLine) {
  static uint32_t lastRxMs = 0;

  while (Serial.available()) {
    char c = (char)Serial.read();
    lastRxMs = millis();
    if (c == '\n' || c == '\r') {
      if (serialLine.length() > 0) {
        outLine = serialLine;
        clearSerialLine();
        return true;
      }
    } else {
      if (c >= 32 && c <= 126) serialLine += c; // printable only
    }
  }

  if (serialLine.length() > 0 && (millis() - lastRxMs) > 20) { // idle_timeout_ms=20
    outLine = serialLine;
    clearSerialLine();
    return true;
  }
  return false;
}

// Admin mode commands:
//   add:itemX:+N or add:itemX:N
//   setprice:itemX:P
//   stock:itemX:N (alias)
//   price:itemX:P (alias)
// Replies with "ok:..." or "error:badcmd"
static void processAdminCommand(const String &line) {
  String cmd = line;
  cmd.trim();
  if (cmd.length() == 0) return;

  // add:itemX:+N or add:itemX:N
  if (startsWithToken(cmd, "add:item")) {
    int afterItem = 4; // token starts at index 4 for "item"
    int nextColon = cmd.indexOf(':', afterItem);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }

    String itemStr = cmd.substring(afterItem, nextColon); // item1..item3
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }

    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();

    // Support optional leading '+', '-', but toInt handles it.
    long val = numStr.toInt();
    if (val < 0) val = 0;

    uint32_t nv = (uint32_t)stockArr[itemIdx - 1] + (uint32_t)val;
    stockArr[itemIdx - 1] = (uint16_t)clamp_u16(nv, 0, 65535);

    Serial.print("ok:add:");
    Serial.print(itemStr);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // setprice:itemX:P
  if (startsWithToken(cmd, "setprice:item")) {
    int after = (int)String("setprice:").length(); // 9
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }

    String itemStr = cmd.substring(after, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }

    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    if (val > (long)MAX_PRICE_3DIG) val = (long)MAX_PRICE_3DIG;

    priceArr[itemIdx - 1] = (uint16_t)val;

    Serial.print("ok:setprice:item");
    Serial.print(itemIdx);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // stock:itemX:N
  if (startsWithToken(cmd, "stock:item")) {
    int after = (int)String("stock:").length(); // 6
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }

    String itemStr = cmd.substring(after, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }

    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    if (val > 65535) val = 65535;

    stockArr[itemIdx - 1] = (uint16_t)val;

    Serial.print("ok:stock:item");
    Serial.print(itemIdx);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // price:itemX:P
  if (startsWithToken(cmd, "price:item")) {
    int after = (int)String("price:").length(); // 6
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }

    String itemStr = cmd.substring(after, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }

    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    if (val > (long)MAX_PRICE_3DIG) val = (long)MAX_PRICE_3DIG;

    priceArr[itemIdx - 1] = (uint16_t)val;

    Serial.print("ok:price:item");
    Serial.print(itemIdx);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  if (cmd == "status") {
    Serial.print("status:");
    for (int i = 0; i < 3; i++) {
      Serial.print(" item");
      Serial.print(i + 1);
      Serial.print("{stock=");
      Serial.print(stockArr[i]);
      Serial.print(",price=");
      Serial.print(priceArr[i]);
      Serial.print("}");
    }
    Serial.println();
    return;
  }

  Serial.println("error:badcmd");
}

// -------- Button handling (debounce + long-press) --------
struct Button {
  uint8_t pin;
  bool stableState;       // debounced pressed/unpressed (pressed => true)
  bool lastRead;          // last raw read (pressed => true)
  uint32_t lastChangeMs; // for debounce
};

static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONG_PRESS_MS = 1200;

// pressed => LOW (INPUT_PULLUP)
static bool updateBtnState(Button &b) {
  bool readPressed = (digitalRead(b.pin) == LOW);

  if (readPressed != b.lastRead) {
    b.lastRead = readPressed;
    b.lastChangeMs = millis();
  }

  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS) {
    b.stableState = readPressed;
  }
  return b.stableState;
}

// -------- Dispense logic --------
static uint8_t selectedItem = 1; // 1..3
static bool hasSelection = false;
static bool adminMode = false;

static void dispenseItem(uint8_t itemIndex1to3) {
  uint16_t &s = stockArr[itemIndex1to3 - 1];
  if (s > 0) {
    // Serial output dispense:itemX
    Serial.print("dispense:item");
    Serial.println(itemIndex1to3);

    s--;

    // 蜂鸣提示
    beep(120);
    delay(40);
    beep(120);

    // 显示剩余商品和价格
    showItemAndPrice(itemIndex1to3);

    digitalWrite(LED_PIN, HIGH);
    delay(60);
    digitalWrite(LED_PIN, LOW);
  } else {
    // 报警并显示错误代码
    Serial.print("error:outofstock:item");
    Serial.println(itemIndex1to3);

    // Error code scheme: item*100 + 0 => 100..300
    uint16_t errCode = (uint16_t)itemIndex1to3 * 100u;
    showErrorCode(errCode);

    // 报警蜂鸣模式
    for (int i = 0; i < 3; i++) {
      beep(180);
      delay(60);
    }

    digitalWrite(LED_PIN, HIGH);
    delay(120);
    digitalWrite(LED_PIN, LOW);
  }
}

// -------- Global buttons --------
static Button btnItem1{BTN_ITEM1, false, true, 0};
static Button btnItem2{BTN_ITEM2, false, true, 0};
static Button btnItem3{BTN_ITEM3, false, true, 0};
static Button btnConfirm{BTN_CONFIRM, false, true, 0};

// -------- Setup & Loop --------
void setup() {
  pinMode(BTN_ITEM1, INPUT_PULLUP);
  pinMode(BTN_ITEM2, INPUT_PULLUP);
  pinMode(BTN_ITEM3, INPUT_PULLUP);
  pinMode(BTN_CONFIRM, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);

  Serial.begin(9600);

  display.setBrightness(0x0f, true); // max brightness
  display.clear();

  // Initialize stable states
  btnItem1.stableState = false; btnItem1.lastRead = true;
  btnItem2.stableState = false; btnItem2.lastRead = true;
  btnItem3.stableState = false; btnItem3.lastRead = true;
  btnConfirm.stableState = false; btnConfirm.lastRead = true;

  selectedItem = 1;
  hasSelection = false;
  showItemAndPrice(selectedItem);
  clearSerialLine();
}

void loop() {
  bool i1 = updateBtnState(btnItem1);
  bool i2 = updateBtnState(btnItem2);
  bool i3 = updateBtnState(btnItem3);
  bool conf = updateBtnState(btnConfirm);

  static bool prevI1 = false, prevI2 = false, prevI3 = false;
  static bool prevConf = false;

  if (!adminMode) {
    // Normal selection presses
    if (i1 && !prevI1) {
      selectedItem = 1;
      hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(60);
    } else if (i2 && !prevI2) {
      selectedItem = 2;
      hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(60);
    } else if (i3 && !prevI3) {
      selectedItem = 3;
      hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(60);
    }

    // Long press D7 enters admin mode
    static uint32_t confirmPressStart = 0;
    static bool confirmLongTriggered = false;

    if (conf && !prevConf) {
      confirmPressStart = millis();
      confirmLongTriggered = false;
    }

    if (conf && !confirmLongTriggered) {
      if ((millis() - confirmPressStart) >= LONG_PRESS_MS) {
        adminMode = true;
        confirmLongTriggered = true;

        display.showNumberDec(8888, false, 4, 0);
        digitalWrite(LED_PIN, HIGH);
        beep(150);
        delay(80);
        beep(150);
        digitalWrite(LED_PIN, LOW);

        clearSerialLine();
      }
    }

    // Normal confirm (short press): on release
    if (!conf && prevConf) {
      if (hasSelection) {
        dispenseItem(selectedItem);
        hasSelection = false;
      }
    }

    // Ignore incoming serial commands in normal mode
    while (Serial.available()) Serial.read();

  } else {
    // Admin mode
    // Short release of D7 exits admin mode (dur < LONG_PRESS_MS)
    static uint32_t confirmPressStart2 = 0;
    static bool confPressed2 = false;

    if (conf && !prevConf) {
      confirmPressStart2 = millis();
      confPressed2 = true;
    }
    if (!conf && prevConf) {
      if (confPressed2) {
        uint32_t dur = millis() - confirmPressStart2;
        confPressed2 = false;

        if (dur < LONG_PRESS_MS) {
          adminMode = false;
          showItemAndPrice(selectedItem);
          digitalWrite(LED_PIN, LOW);
          clearSerialLine();
          beep(80);
        }
      }
    }

    // Keep display consistent with selection updates in admin
    if (i1 && !prevI1) {
      selectedItem = 1; hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(50);
    } else if (i2 && !prevI2) {
      selectedItem = 2; hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(50);
    } else if (i3 && !prevI3) {
      selectedItem = 3; hasSelection = true;
      showItemAndPrice(selectedItem);
      beep(50);
    }

    // Process admin serial commands
    String line;
    if (readLineFromSerial(line)) {
      processAdminCommand(line);
    }

    // Periodic refresh
    static uint32_t lastRefresh = 0;
    if (millis() - lastRefresh > 200) {
      lastRefresh = millis();
      showItemAndPrice(selectedItem);
    }
  }

  prevI1 = i1;
  prevI2 = i2;
  prevI3 = i3;
  prevConf = conf;
}
