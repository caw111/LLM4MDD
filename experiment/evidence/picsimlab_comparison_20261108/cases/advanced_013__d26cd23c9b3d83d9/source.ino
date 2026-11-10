#include <Arduino.h>
#include <TM1637Display.h>

static const uint8_t PIN_SENSOR_L1 = 2;  // D2
static const uint8_t PIN_SENSOR_L2 = 3;  // D3
static const uint8_t PIN_SENSOR_L3 = 7;  // D7

static const uint8_t PIN_LED        = 13; // D13
static const uint8_t PIN_BUZZER     = 6;  // D6
static const uint8_t PIN_TM1637_CLK = 4;  // D4
static const uint8_t PIN_TM1637_DIO = 5;  // D5

static const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;
static const uint32_t BAUD = 9600;

static const uint32_t RESTOCK_NEEDED_MS = 5UL * 60UL * 1000UL; // 设定时间（示例：5分钟）
static const uint32_t BUZZ_LOCK_MS = 0; // 0 表示只要三层全满就锁定新增记录（不依赖时长）

// ---- TM1637 ----
TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// ---- State tracking ----
struct LayerState {
  bool occupied = false;
  bool wasOccupied = false;
  uint32_t emptySince = 0;
  bool restockNeeded = false;
};

LayerState l1, l2, l3;

static bool lockNewRecords = false;
static uint32_t lockStartMs = 0;

static uint32_t lastBuzzerMs = 0;
static bool buzzerOn = false;

// ---- Serial line buffer ----
static char serialBuf[96];
static uint8_t serialPos = 0;
static uint32_t lastRxMs = 0;

static uint8_t readOccupied(uint8_t pin) {
  // Infrared break-beam / digital slot sensor.
  // Requirement does not specify logic level. We'll treat HIGH as occupied by default.
  // If your sensor inverts, swap HIGH/LOW below.
  return digitalRead(pin) == HIGH ? 1 : 0;
}

static uint32_t nowMs() { return millis(); }

static void updateLayer(LayerState &st, bool occupiedNow) {
  st.occupied = occupiedNow;

  if (st.occupied) {
    st.emptySince = 0;
    st.restockNeeded = false;
  } else {
    if (st.emptySince == 0) st.emptySince = nowMs();
    if (!st.restockNeeded && (nowMs() - st.emptySince >= RESTOCK_NEEDED_MS)) {
      st.restockNeeded = true;
    }
  }
}

static uint8_t occupiedCount() {
  return (l1.occupied ? 1 : 0) + (l2.occupied ? 1 : 0) + (l3.occupied ? 1 : 0);
}

static void displayOccupiedCount() {
  // Show occupied layers count on TM1637 (0..3)
  uint8_t cnt = occupiedCount();
  // Use numeric display without decimals
  tm1637.showNumberDec(cnt, false, 2, 0);
}

static void buzzerTask() {
  uint8_t cnt = occupiedCount();
  bool allFull = (cnt == 3);

  // When all three full: buzz and lock新增记录
  if (allFull) {
    if (!lockNewRecords) {
      lockNewRecords = true;
      lockStartMs = nowMs();
    }
    // Buzz提示：交替蜂鸣（保持提示与锁定新增记录）
    // We'll generate audible tone on D6 while all full.
    // Use tone() for continuous alert.
    tone(PIN_BUZZER, 2000);
    buzzerOn = true;
    lastBuzzerMs = nowMs();
  } else {
    // If not all full: stop buzzer, unlock only if previously locked and allFull no longer true?
    // Requirement: "当三层全满时蜂鸣提示并锁定新增记录。"
    // It doesn't explicitly require unlocking later; we keep lockNewRecords true after full until reset via serial.
    if (buzzerOn) {
      noTone(PIN_BUZZER);
      buzzerOn = false;
    }
  }

  // Optional timeout not used; retained for structure
  if (BUZZ_LOCK_MS > 0 && lockNewRecords && (nowMs() - lockStartMs >= BUZZ_LOCK_MS)) {
    // still keep lock until explicit reset, so do nothing
  }
}

static void ledTask() {
  // LED indicates restockNeeded or lock state
  bool anyRestock = l1.restockNeeded || l2.restockNeeded || l3.restockNeeded;
  bool allFull = (occupiedCount() == 3);

  if (allFull) {
    digitalWrite(PIN_LED, HIGH);
  } else if (anyRestock) {
    // Blink faster
    static uint32_t last = 0;
    if (nowMs() - last >= 250) {
      last = nowMs();
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
    }
  } else {
    // Solid off
    digitalWrite(PIN_LED, LOW);
  }
}

static void reportStatus(Print &out) {
  out.print("OCC=");
  out.print(occupiedCount());
  out.print(" L1=");
  out.print(l1.occupied ? 1 : 0);
  out.print(l1.restockNeeded ? "(R)" : "");
  out.print(" L2=");
  out.print(l2.occupied ? 1 : 0);
  out.print(l2.restockNeeded ? "(R)" : "");
  out.print(" L3=");
  out.print(l3.occupied ? 1 : 0);
  out.print(l3.restockNeeded ? "(R)" : "");
  out.print(" LOCK="); out.print(lockNewRecords ? 1 : 0);
  out.print(" RESTOCK_MS="); out.print(RESTOCK_NEEDED_MS);
  out.println();
}

static void handleCommand(const char *cmdLine, Print &out) {
  // Supported commands (best-effort):
  // ? or "STATUS" -> query each layer state and count
  // "RESET" -> reset timers (clear restockNeeded flags) and unlock if requested
  // "RESET_TIMERS" -> clear emptySince and restockNeeded
  // "RESET_ALL" -> clear timers and unlock lockNewRecords
  // "SET_RESTOCK=<ms>" -> set restock threshold (optional; doesn't contradict requirements)
  // Ensure no behavior removed; only adds serial command handling as requested.

  // Trim spaces
  while (*cmdLine == ' ') cmdLine++;

  if (cmdLine[0] == '\0') return;

  if (strcmp(cmdLine, "?") == 0 || strcasecmp(cmdLine, "STATUS") == 0) {
    reportStatus(out);
    return;
  }

  if (strcasecmp(cmdLine, "GET") == 0) {
    reportStatus(out);
    return;
  }

  if (strcasecmp(cmdLine, "RESET") == 0) {
    // reset timers (reinitialize empty tracking) and restock flags
    l1.emptySince = 0; l1.restockNeeded = false;
    l2.emptySince = 0; l2.restockNeeded = false;
    l3.emptySince = 0; l3.restockNeeded = false;
    // lock remains unless explicitly reset
    out.println("OK RESET_TIMERS");
    return;
  }

  if (strcasecmp(cmdLine, "RESET_TIMERS") == 0) {
    l1.emptySince = 0; l1.restockNeeded = false;
    l2.emptySince = 0; l2.restockNeeded = false;
    l3.emptySince = 0; l3.restockNeeded = false;
    out.println("OK RESET_TIMERS");
    return;
  }

  if (strcasecmp(cmdLine, "RESET_ALL") == 0) {
    l1.emptySince = 0; l1.restockNeeded = false;
    l2.emptySince = 0; l2.restockNeeded = false;
    l3.emptySince = 0; l3.restockNeeded = false;
    lockNewRecords = false;
    lockStartMs = 0;
    out.println("OK RESET_ALL");
    return;
  }

  // Optional: SET_RESTOCK=<ms>
  if (strncasecmp(cmdLine, "SET_RESTOCK=", 12) == 0) {
    const char *p = cmdLine + 12;
    long v = strtol(p, nullptr, 10);
    if (v > 0) {
      // RESTOCK_NEEDED_MS is const in this sketch; to preserve requirement behavior,
      // we won't change it at runtime. Report error.
      out.println("ERR RESTOCK_MS_FIXED");
    } else {
      out.println("ERR INVALID_VALUE");
    }
    return;
  }

  out.print("ERR UNKNOWN_CMD: ");
  out.println(cmdLine);
}

void setup() {
  pinMode(PIN_SENSOR_L1, INPUT);
  pinMode(PIN_SENSOR_L2, INPUT);
  pinMode(PIN_SENSOR_L3, INPUT);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);

  tm1637.setBrightness(0x0f);
  tm1637.clear();

  Serial.begin(BAUD);

  // Initialize states
  l1.occupied = readOccupied(PIN_SENSOR_L1);
  l2.occupied = readOccupied(PIN_SENSOR_L2);
  l3.occupied = readOccupied(PIN_SENSOR_L3);

  l1.emptySince = l1.occupied ? 0 : nowMs();
  l2.emptySince = l2.occupied ? 0 : nowMs();
  l3.emptySince = l3.occupied ? 0 : nowMs();

  l1.restockNeeded = false;
  l2.restockNeeded = false;
  l3.restockNeeded = false;

  displayOccupiedCount();
}

void loop() {
  // ---- Read sensors and update timers ----
  bool occ1 = readOccupied(PIN_SENSOR_L1);
  bool occ2 = readOccupied(PIN_SENSOR_L2);
  bool occ3 = readOccupied(PIN_SENSOR_L3);

  updateLayer(l1, occ1);
  updateLayer(l2, occ2);
  updateLayer(l3, occ3);

  // If any layer empty long enough, output restock needed.
  // "输出 restock needed" => send notification and keep status via restockNeeded flag.
  // To preserve behavior (no simplification), we emit once per activation event.
  static bool prevR1 = false, prevR2 = false, prevR3 = false;
  if (!prevR1 && l1.restockNeeded) {
    Serial.println("restock needed: L1");
  }
  if (!prevR2 && l2.restockNeeded) {
    Serial.println("restock needed: L2");
  }
  if (!prevR3 && l3.restockNeeded) {
    Serial.println("restock needed: L3");
  }
  prevR1 = l1.restockNeeded;
  prevR2 = l2.restockNeeded;
  prevR3 = l3.restockNeeded;

  // If three layers all full, buzz and lock新增记录.
  // Lock behavior: once locked, any potential "新增记录" action should be prevented.
  // Since no explicit add-record input is specified, we enforce by reporting status only and locking remains until reset.
  // (User can query via serial.)

  // ---- Update display, buzzer, LED ----
  displayOccupiedCount();
  buzzerTask();
  ledTask();

  // ---- Serial: line_or_idle with idle_timeout_ms ----
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastRxMs = nowMs();

    if (c == '\r') continue;
    if (c == '\n' || c == ';') {
      serialBuf[serialPos] = '\0';
      if (serialPos > 0) handleCommand(serialBuf, Serial);
      serialPos = 0;
    } else {
      if (serialPos < sizeof(serialBuf) - 1) {
        serialBuf[serialPos++] = c;
      }
    }
  }

  // Handle idle line completion
  if (serialPos > 0 && (nowMs() - lastRxMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    serialBuf[serialPos] = '\0';
    handleCommand(serialBuf, Serial);
    serialPos = 0;
  }

  // ---- Prevent busy tone if lock not all full handled in buzzerTask ----
  // Also ensure new records are considered locked: here we just keep flag; no further actions defined.

  // Small delay for stability
  delay(20);
}