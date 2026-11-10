#include <Arduino.h>
#include <TM1637Display.h>

// -------- Pin assignments (authoritative) --------
static const uint8_t BTN_ITEM1 = 2;   // D2
static const uint8_t BTN_ITEM2 = 3;   // D3
static const uint8_t BTN_ITEM3 = 4;   // D4
static const uint8_t BTN_CONFIRM = 7; // D7 (long-press => admin mode)

static const uint8_t LED_PIN = 13;     // D13
static const uint8_t BUZZER_PIN = 6;   // D6

static const uint8_t TM1637_CLK = 8;   // D8
static const uint8_t TM1637_DIO = 5;   // D5

// -------- TM1637 --------
TM1637Display display(TM1637_CLK, TM1637_DIO);

// -------- Application model --------
// 3 products: item1=itemX? Use item1..3 with IDs 1..3
// Show:商品编号和价格
// Display formatting rules (robust):
// - In normal mode: after selecting item, show "I P" style:
//     Leftmost digit = item index (1..3) [as a single digit]
//     Other digits show price (in cents or integer as configured below).
//
// Since requirements say "商品编号和价格" but not exact formatting,
// we implement a common 4-digit convention:
//   digits: [item][price(3 digits)] => item*1000 + price (0..999)
// Price will be stored as integer (0..999).
// If price exceeds 999, it will be truncated to 3 digits to fit TM1637.
static const uint16_t PRICE[3] = {125, 250, 399}; // Example maintained prices (updateable in admin)
static const uint16_t MAX_PRICE_3DIG = 999;

// Inventory (persist only in RAM for this sketch; admin replenishes via serial)
static uint16_t stock[3] = {5, 5, 5};

// -------- Serial protocol (admin mode) --------
// On long-press D7 enters admin mode.
// Through serial, support replenishment and (optionally) price update.
//
// Simple line-based commands (9600 baud, newline or idle timeout):
//   add:item1:+N
//   add:item2:N
//   add:item3:N
//   setprice:item1:P
//   setprice:item2:P
//   setprice:item3:P
//   stock:item1:N        (alias of set stock)
//   price:item1:P        (alias of setprice)
//
// If parsing fails, reply: "error:badcmd"
// Responses are output as lines.
//
// In normal dispense mode:
//   dispense:itemX (X is 1..3)
//   error:outofstock:itemX
//
// Note: requirements only mandate dispense:itemX and error codes, not admin reply format,
// but we provide useful replies without breaking required behaviors.
static void beep(uint16_t ms) {
  tone(BUZZER_PIN, 2000);
  delay(ms);
  noTone(BUZZER_PIN);
}

// -------- Button handling (debounce + long-press) --------
struct Button {
  uint8_t pin;
  bool stableState;       // last debounced reading (pressed/unpressed)
  bool lastRead;
  uint32_t lastChangeMs; // for debounce
};

static Button btnItem[3] = {
  {BTN_ITEM1, true, true, 0},
  {BTN_ITEM2, true, true, 0},
  {BTN_ITEM3, true, true, 0}
};
static Button btnConfirm = {BTN_CONFIRM, true, true, 0};

static const uint16_t DEBOUNCE_MS = 30;
static const uint16_t LONG_PRESS_MS = 1200;

// Assume buttons are wired with INPUT_PULLUP (pressed => LOW)
static bool updateButton(Button &b) {
  bool read = (digitalRead(b.pin) == LOW);
  if (read != b.lastRead) {
    b.lastRead = read;
    b.lastChangeMs = millis();
  }
  if ((millis() - b.lastChangeMs) >= DEBOUNCE_MS) {
    b.stableState = read;
  }
  return b.stableState;
}

// Edge detection for press events
static bool risingEdgePressed(Button &b) {
  static bool prev[10]; // not used for dynamic; we'll implement per-button in caller
  // This helper is unused; we will do manual edge detection in loop.
  (void)prev;
  return false;
}

// -------- Display helpers --------
static void showItemAndPrice(uint8_t itemIndex1to3) {
  // itemIndex1to3 => 1..3
  uint8_t itemDigit = itemIndex1to3;
  uint16_t p = PRICE[itemIndex1to3 - 1];
  if (p > MAX_PRICE_3DIG) p = MAX_PRICE_3DIG;
  uint16_t val = (uint16_t)itemDigit * 1000u + p; // [item][price(3 digits)]
  display.showNumberDec(val, false, 4, 0); // no leading zeros
}

static void showErrorCode(uint16_t code) {
  // Display error code (up to 4 digits)
  display.showNumberDec(code, false, 4, 0);
}

// -------- State --------
static uint8_t selectedItem = 1;        // 1..3
static bool hasSelection = false;
static bool adminMode = false;

// For serial line reading with idle timeout
static String serialLine;

static void clearSerialLine() {
  serialLine = "";
}

static void handleNormalSerialInputNoop() {
  // In normal mode, we ignore incoming serial commands.
  while (Serial.available()) Serial.read();
}

static bool readLineFromSerial(String &outLine) {
  // Reads a line if newline received OR if idle timeout occurs via available polling.
  // We implement by accumulating characters and checking idle timeout from last received time.
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
      // ignore non-printable control chars
      if (c >= 32 && c <= 126) serialLine += c;
    }
  }

  if (serialLine.length() > 0 && (millis() - lastRxMs) > 20) { // idle_timeout_ms=20
    outLine = serialLine;
    clearSerialLine();
    return true;
  }
  return false;
}

static int parseItemIndex(const String &s) {
  // expects "item1"/"item2"/"item3"
  int idx = -1;
  if (s.startsWith("item")) {
    idx = s.substring(4).toInt();
    if (idx >= 1 && idx <= 3) return idx;
  }
  return -1;
}

static bool startsWithToken(const String &cmd, const String &tok) {
  if (cmd.length() < tok.length()) return false;
  return cmd.substring(0, tok.length()) == tok;
}

static void processAdminCommand(const String &line) {
  // Normalize spaces (trim)
  String cmd = line;
  cmd.trim();
  if (cmd.length() == 0) return;

  // dispense mode commands are not expected; only admin in this mode
  // add:itemX:+N  or add:itemX:N
  // setprice:itemX:P
  // stock:itemX:N
  // price:itemX:P
  int itemIdx = -1;
  long val = 0;

  auto findAfterColon = [&](const String &prefix)->String{
    int p = cmd.indexOf(prefix);
    if (p != 0) return String("");
    int cpos = cmd.indexOf(':', prefix.length());
    if (cpos < 0) return String("");
    return cmd.substring(cpos + 1);
  };

  if (startsWithToken(cmd, "add:item")) {
    // Format: add:itemX:+N
    // Extract "itemX" then value after second ':'
    int firstColon = cmd.indexOf(':'); // after "add"
    (void)firstColon;
    // cmd = "add:item1:+5"
    int secondColon = cmd.indexOf(':', 4); // after "add:" => start at 4
    // Actually "add:item" length 8; but robust:
    // We'll locate item part between "add:" and next ':'
    int afterAdd = 4; // position after "add:"
    int nextColon = cmd.indexOf(':', afterAdd);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }

    String itemStr = cmd.substring(afterAdd, nextColon); // item1
    itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }

    String numStr = cmd.substring(nextColon + 1); // +N or N
    numStr.trim();
    val = numStr.toInt();
    if (val < 0) val = 0; // prevent negative add (keep simple)
    stock[itemIdx - 1] = (uint16_t)min<long>(65535, (long)stock[itemIdx - 1] + val);
    Serial.print("ok:add:");
    Serial.print(itemStr);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  if (startsWithToken(cmd, "setprice:item")) {
    int after = 11; // "setprice:" length
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }
    String itemStr = cmd.substring(after, nextColon);
    itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }
    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    val = numStr.toInt();
    if (val < 0) val = 0;
    // Update PRICE[]: since it's const in this sketch, we can't.
    // So provide behavior: price updates are not supported if PRICE is const.
    // To preserve "maintained prices", we implement mutable prices via a separate array.
  }

  Serial.println("error:badcmd");
}

// To truly "maintain商品价格", implement mutable prices (not const).
static uint16_t price[3] = {125, 250, 399};

static void showItemAndPriceUsingMutable(uint8_t itemIndex1to3) {
  uint8_t itemDigit = itemIndex1to3;
  uint16_t p = price[itemIndex1to3 - 1];
  if (p > MAX_PRICE_3DIG) p = MAX_PRICE_3DIG;
  uint16_t val = (uint16_t)itemDigit * 1000u + p;
  display.showNumberDec(val, false, 4, 0);
}

static void processAdminCommand2(const String &line) {
  String cmd = line;
  cmd.trim();
  if (cmd.length() == 0) return;

  // add:itemX:+N
  if (startsWithToken(cmd, "add:item")) {
    int afterAdd = 4; // after "add:"
    int nextColon = cmd.indexOf(':', afterAdd);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }
    String itemStr = cmd.substring(afterAdd, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }
    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    stock[itemIdx - 1] = (uint16_t)min<long>(65535, (long)stock[itemIdx - 1] + val);
    Serial.print("ok:add:");
    Serial.print(itemStr);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // setprice:itemX:P
  if (startsWithToken(cmd, "setprice:item")) {
    int after = String("setprice:").length(); // 9
    // find next ':' after itemX
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }
    String itemStr = cmd.substring(after, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }
    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    val = min<long>(MAX_PRICE_3DIG, val);
    price[itemIdx - 1] = (uint16_t)val;
    Serial.print("ok:setprice:item");
    Serial.print(itemIdx);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // stock:itemX:N (alias)
  if (startsWithToken(cmd, "stock:item")) {
    int after = String("stock:").length(); // 6
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
    stock[itemIdx - 1] = (uint16_t)val;
    Serial.print("ok:stock:item");
    Serial.print(itemIdx);
    Serial.print(":");
    Serial.println(val);
    return;
  }

  // price:itemX:P (alias)
  if (startsWithToken(cmd, "price:item")) {
    int after = String("price:").length(); // 6
    int nextColon = cmd.indexOf(':', after);
    if (nextColon < 0) { Serial.println("error:badcmd"); return; }
    String itemStr = cmd.substring(after, nextColon);
    int itemIdx = parseItemIndex(itemStr);
    if (itemIdx < 0) { Serial.println("error:badcmd"); return; }
    String numStr = cmd.substring(nextColon + 1);
    numStr.trim();
    long val = numStr.toInt();
    if (val < 0) val = 0;
    val = min<long>(MAX_PRICE_3DIG, val);
    price[itemIdx - 1] = (uint16_t)val;
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
      Serial.print(stock[i]);
      Serial.print(",price=");
      Serial.print(price[i]);
      Serial.print("}");
    }
    Serial.println();
    return;
  }

  Serial.println("error:badcmd");
}

// -------- Dispense logic --------
static void dispenseItem(uint8_t itemIndex1to3) {
  uint16_t &s = stock[itemIndex1to3 - 1];
  if (s > 0) {
    // Serial output dispense:itemX
    Serial.print("dispense:item");
    Serial.println(itemIndex1to3);

    s--;

    // beep to confirm
    beep(120);
    delay(40);
    beep(120);

    // Update display to show remaining item and price
    showItemAndPriceUsingMutable(itemIndex1to3);
    digitalWrite(LED_PIN, HIGH);
    delay(60);
    digitalWrite(LED_PIN, LOW);
  } else {
    //报警并显示错误代码
    Serial.print("error:outofstock:item");
    Serial.println(itemIndex1to3);

    // Error code scheme: item*100 + 0 => e.g., 100..300
    uint16_t errCode = (uint16_t)itemIndex1to3 * 100u; // 100,200,300
    showErrorCode(errCode);

    // Alarm beep pattern
    for (int i = 0; i < 3; i++) {
      beep(180);
      delay(60);
    }
    digitalWrite(LED_PIN, HIGH);
    delay(120);
    digitalWrite(LED_PIN, LOW);
  }
}

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

  // Initial display: show item1 and its price
  hasSelection = false;
  selectedItem = 1;
  showItemAndPriceUsingMutable(selectedItem);
  clearSerialLine();
}

void loop() {
  // Update buttons
  bool i1 = updateButton(btnItem[0]);
  bool i2 = updateButton(btnItem[1]);
  bool i3 = updateButton(btnItem[2]);
  bool conf = updateButton(btnConfirm);

  static bool prevI1 = false, prevI2 = false, prevI3 = false;
  static bool prevConf = false;

  // Selection presses
  if (!adminMode) {
    if (i1 && !prevI1) {
      selectedItem = 1;
      hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
      beep(60);
    } else if (i2 && !prevI2) {
      selectedItem = 2;
      hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
      beep(60);
    } else if (i3 && !prevI3) {
      selectedItem = 3;
      hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
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
      if (millis() - confirmPressStart >= LONG_PRESS_MS) {
        adminMode = true;
        confirmLongTriggered = true;

        // Admin entry indication
        display.showNumberDec(8888, false, 4, 0);
        digitalWrite(LED_PIN, HIGH);
        beep(150);
        delay(80);
        beep(150);
        digitalWrite(LED_PIN, LOW);

        clearSerialLine();
      }
    }

    // Normal confirm (short press)
    if (!conf && prevConf) {
      // released
      // If not in admin mode, treat as confirm if currently selected
      if (!adminMode && hasSelection) {
        dispenseItem(selectedItem);
      }
    }

    // Ignore normal serial input
    handleNormalSerialInputNoop();

  } else {
    // Admin mode: keep displaying current selection and allow selection buttons
    // (spec not required but doesn't conflict; we keep showing selected item+price)
    // Also exit admin mode on long-press again? Not specified.
    // We'll keep admin mode until reset; but allow it to be ended by short confirm? Not specified.
    // We'll exit admin mode when D7 is pressed and released quickly (short press).
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

        // Short release => exit admin mode
        if (dur < LONG_PRESS_MS) {
          adminMode = false;
          display.showNumberDec(selectedItem * 1000u + (price[selectedItem - 1] % 1000u), false, 4, 0);
          digitalWrite(LED_PIN, LOW);
          clearSerialLine();
          beep(80);
        }
      }
    }

    // Allow item selection updates in admin mode
    if (i1 && !prevI1) {
      selectedItem = 1; hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
      beep(50);
    } else if (i2 && !prevI2) {
      selectedItem = 2; hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
      beep(50);
    } else if (i3 && !prevI3) {
      selectedItem = 3; hasSelection = true;
      showItemAndPriceUsingMutable(selectedItem);
      beep(50);
    }

    // Process admin serial commands
    String line;
    if (readLineFromSerial(line)) {
      processAdminCommand2(line);
    }

    // Also keep display refreshed (optional)
    static uint32_t lastRefresh = 0;
    if (millis() - lastRefresh > 200) {
      lastRefresh = millis();
      showItemAndPriceUsingMutable(selectedItem);
    }
  }

  prevI1 = i1;
  prevI2 = i2;
  prevI3 = i3;
  prevConf = conf;
}