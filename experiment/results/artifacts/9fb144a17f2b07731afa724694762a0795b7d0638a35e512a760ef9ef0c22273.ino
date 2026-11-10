#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>

// TM1637 (CLK=D4, DIO=D5)
static const uint8_t TM1637_CLK = D4;
static const uint8_t TM1637_DIO = D5;

// DHT11 (Living: D7, Bedroom: D8)
static const uint8_t DHT_LIVING_PIN  = D7;
static const uint8_t DHT_BEDROOM_PIN = D8;
static const uint8_t DHT11_TYPE = DHT11;

DHT dhtLiving(DHT_LIVING_PIN, DHT11_TYPE);
DHT dhtBedroom(DHT_BEDROOM_PIN, DHT11_TYPE);

// Light sensors (A0, A1)
static const uint8_t LDR_LIVING_PIN  = A0;
static const uint8_t LDR_BEDROOM_PIN = A1;

// Buttons (D2, D3)
static const uint8_t BTN_NEXT_ROOM = D2;
static const uint8_t BTN_NEXT_ITEM = D3;

// Alerts (D12, D13)
static const uint8_t LED_LIVING_ALERT  = D12;
static const uint8_t LED_BEDROOM_ALERT = D13;

// Display
static const uint8_t SEG_UPDATE_MODE = 0;

// --------- Behavior requirements / thresholds (preserved, configurable via constants) ---------
const unsigned long SERIAL_REPORT_INTERVAL_MS = 5000;

// "温度过高" and "光照过强" thresholds.
// If your original circuit/spec had specific values, adjust here accordingly.
static const float TEMP_HIGH_THRESHOLD_C = 30.0;  // temperature over high
static const int   LIGHT_HIGH_THRESHOLD_RAW = 700; // light over strong (ADC raw; adjust if needed)

// --------- State ---------
enum Room : uint8_t { ROOM_LIVING = 0, ROOM_BEDROOM = 1 };
enum Item : uint8_t { ITEM_TEMPERATURE = 0, ITEM_HUMIDITY = 1, ITEM_LIGHT = 2 };

static Room currentRoom = ROOM_LIVING;
static Item currentItem = ITEM_TEMPERATURE;

// display rotation timing
static const unsigned long DISPLAY_ROTATE_INTERVAL_MS = 1500;
static unsigned long lastDisplayRotateMs = 0;

// button debounce
static const unsigned long BTN_DEBOUNCE_MS = 60;
static unsigned long lastBtnNextRoomMs = 0;
static unsigned long lastBtnNextItemMs = 0;
static bool lastBtnNextRoomState = HIGH;
static bool lastBtnNextItemState  = HIGH;

// Sensor sampling
static unsigned long lastDhtSampleMs = 0;
static const unsigned long DHT_SAMPLE_INTERVAL_MS = 2000;

static float livingTempC = NAN;
static float livingHumPct = NAN;
static float bedroomTempC = NAN;
static float bedroomHumPct = NAN;

static int livingLightRaw = 0;
static int bedroomLightRaw = 0;

// TM1637 small helper (minimal implementation without external library)
class TM1637Mini {
public:
  TM1637Mini(uint8_t clk, uint8_t dio) : _clk(clk), _dio(dio) {}

  void begin() {
    pinMode(_clk, OUTPUT);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
    setBrightness(0x0f);
  }

  void setBrightness(uint8_t brightness) {
    brightness &= 0x07; // usually 0..7
    // Command: 0x88 + brightness
    start();
    writeByte(0x88 | brightness);
    stop();
  }

  // 4-digit display: digits[0..3], each is value 0-9, or 0xFF for blank.
  // We'll display with simple mapping; sign and decimals not supported fully.
  void displayDigits(const uint8_t digits[4], bool colon = false) {
    uint8_t seg[4];
    for (int i = 0; i < 4; i++) {
      seg[i] = digitToSegment(digits[i]);
    }
    // Turn colon into 0x10 bit on second digit (common practice)
    if (colon) seg[1] |= 0x80;

    start();
    writeByte(0x40); // fixed address
    for (int i = 0; i < 4; i++) {
      writeByte(seg[i]);
    }
    stop();

    // Activate display control
    start();
    writeByte(0xC0);
    stop();
  }

  void showNumberInt(int value, bool showBlankLeading = true) {
    // displays value on 4 digits, no decimals
    uint8_t digits[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    bool neg = false;
    if (value < 0) { neg = true; value = -value; }

    int d[4] = {0,0,0,0};
    d[3] = value % 10;
    d[2] = (value / 10) % 10;
    d[1] = (value / 100) % 10;
    d[0] = (value / 1000) % 10;

    // place digits with blank leading
    bool started = !showBlankLeading ? true : (d[0] != 0);
    for (int i = 0; i < 4; i++) {
      if (!started && showBlankLeading) {
        if (i == 0) {
          // leading blank
          digits[i] = 0xFF;
        } else if (d[i] != 0) {
          started = true;
          digits[i] = d[i];
        } else {
          digits[i] = 0xFF;
        }
      } else {
        digits[i] = d[i];
      }
    }

    // sign: put '-' in digit 0 if negative
    if (neg) {
      // If digit 0 is blank, show '-' by segment 0x40
      digits[0] = 10; // special mapping to '-'
    }

    displayDigits(digits, false);
  }

  void showCustom(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    uint8_t digits[4] = {a,b,c,d};
    displayDigits(digits, false);
  }

private:
  uint8_t _clk, _dio;

  // start/stop and write bit-banged protocol
  void start() {
    digitalWrite(_dio, HIGH);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, LOW);
  }
  void stop() {
    digitalWrite(_clk, LOW);
    digitalWrite(_dio, LOW);
    digitalWrite(_clk, HIGH);
    digitalWrite(_dio, HIGH);
  }
  void writeByte(uint8_t b) {
    for (int i = 0; i < 8; i++) {
      digitalWrite(_clk, LOW);
      digitalWrite(_dio, (b & 0x01) ? HIGH : LOW);
      b >>= 1;
      digitalWrite(_clk, HIGH);
    }
    // ack
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    digitalWrite(_clk, HIGH);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
  }

  uint8_t digitToSegment(uint8_t digit) {
    // digit: 0-9, 0xFF blank, 10 '-' (minus)
    if (digit == 0xFF) return 0x00;
    if (digit == 10) return 0x40; // minus

    // segments: a b c d e f g (common). TM1637 typical encoding:
    // 0: 0x3F, 1:0x06, 2:0x5B, 3:0x4F, 4:0x66, 5:0x6D, 6:0x7D, 7:0x07, 8:0x7F, 9:0x6F
    static const uint8_t map[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
    return map[digit & 0x0F];
  }
};

TM1637Mini tm(TM1637_CLK, TM1637_DIO);

// ---------- Button handling ----------
bool readButtonPressed(uint8_t pin) {
  // Active LOW expected: press => LOW
  return digitalRead(pin) == LOW;
}

void handleButtons() {
  unsigned long now = millis();

  // BTN_NEXT_ROOM: toggle room
  {
    bool st = readButtonPressed(BTN_NEXT_ROOM);
    // Debounce based on raw transitions
    // If pressed newly (transition from not pressed to pressed) and debounce elapsed:
    if (st != !lastBtnNextRoomState) {
      // no-op: keep it simple with debounce timing below
    }
    bool currentPressed = st;
    bool lastPressed = (lastBtnNextRoomState == LOW);

    if (currentPressed != lastPressed && (now - lastBtnNextRoomMs) > BTN_DEBOUNCE_MS) {
      lastBtnNextRoomMs = now;
      if (currentPressed) {
        currentRoom = (currentRoom == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
      }
      lastBtnNextRoomState = currentPressed ? LOW : HIGH;
    } else {
      lastBtnNextRoomState = currentPressed ? LOW : HIGH;
    }
  }

  // BTN_NEXT_ITEM: cycle item
  {
    bool st = readButtonPressed(BTN_NEXT_ITEM);
    bool currentPressed = st;
    bool lastPressed = (lastBtnNextItemState == LOW);

    if (currentPressed != lastPressed && (now - lastBtnNextItemMs) > BTN_DEBOUNCE_MS) {
      lastBtnNextItemMs = now;
      if (currentPressed) {
        currentItem = static_cast<Item>((static_cast<uint8_t>(currentItem) + 1) % 3);
      }
      lastBtnNextItemState = currentPressed ? LOW : HIGH;
    } else {
      lastBtnNextItemState = currentPressed ? LOW : HIGH;
    }
  }
}

// ---------- Display logic ----------
void displayCurrent() {
  // Show values for current room & current item.
  // 4-digit display:
  // - Temperature: show integer part (°C) e.g., 25 -> "  25"
  // - Humidity: show integer part (%) e.g., 60 -> "  60"
  // - Light: show raw mapped to 0..999 for fit: show "  xxx"
  int shown = 0;

  bool valid = true;

  if (currentRoom == ROOM_LIVING) {
    if (currentItem == ITEM_TEMPERATURE) {
      if (isnan(livingTempC)) valid = false;
      else shown = (int)round(livingTempC);
    } else if (currentItem == ITEM_HUMIDITY) {
      if (isnan(livingHumPct)) valid = false;
      else shown = (int)round(livingHumPct);
    } else { // light
      shown = livingLightRaw;
    }
  } else { // BEDROOM
    if (currentItem == ITEM_TEMPERATURE) {
      if (isnan(bedroomTempC)) valid = false;
      else shown = (int)round(bedroomTempC);
    } else if (currentItem == ITEM_HUMIDITY) {
      if (isnan(bedroomHumPct)) valid = false;
      else shown = (int)round(bedroomHumPct);
    } else {
      shown = bedroomLightRaw;
    }
  }

  // Fit light raw into 4 digits (0..1023 -> ok). For >999, clamp.
  if (currentItem == ITEM_LIGHT) {
    if (shown < 0) shown = 0;
    if (shown > 999) shown = 999;
  }

  if (!valid) {
    // show "----"
    tm.showCustom(10,10,10,10);
    return;
  }

  // Optionally update TM1637 brightness
  tm.setBrightness(0x06);

  // For temperature/humidity show integer directly
  tm.showNumberInt(shown, true);
}

void rotateDisplayByTimerIfNeeded() {
  // "系统需要轮流显示不同房间的温度、湿度和光照"
  // We implement round-robin when user not pressing buttons:
  // Every interval: cycle item first, then room when item wraps.
  unsigned long now = millis();
  if (now - lastDisplayRotateMs >= DISPLAY_ROTATE_INTERVAL_MS) {
    lastDisplayRotateMs = now;
    currentItem = static_cast<Item>((static_cast<uint8_t>(currentItem) + 1) % 3);
    if (currentItem == ITEM_TEMPERATURE) {
      // wrapped -> advance room
      currentRoom = (currentRoom == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    }
  }
}

// ---------- Sensor sampling ----------
void sampleSensors() {
  unsigned long now = millis();
  if (now - lastDhtSampleMs < DHT_SAMPLE_INTERVAL_MS) return;

  lastDhtSampleMs = now;

  // DHT11: reads
  float tL = dhtLiving.readTemperature();
  float hL = dhtLiving.readHumidity();
  float tB = dhtBedroom.readTemperature();
  float hB = dhtBedroom.readHumidity();

  if (!isnan(tL)) livingTempC = tL;
  if (!isnan(hL)) livingHumPct = hL;
  if (!isnan(tB)) bedroomTempC = tB;
  if (!isnan(hB)) bedroomHumPct = hB;

  // LDR
  livingLightRaw = analogRead(LDR_LIVING_PIN);
  bedroomLightRaw = analogRead(LDR_BEDROOM_PIN);
}

// ---------- Alerts ----------
void updateAlertsAndReportRoomException() {
  // "若任一房间温度过高且光照过强，则点亮对应告警 LED（D12 或 D13）并输出房间级异常信息。"
  // We'll print immediately when condition is true (and when it changes to true) to avoid spamming.
  static bool livingAlarmPrev = false;
  static bool bedroomAlarmPrev = false;

  bool livingTempHigh = (!isnan(livingTempC)) && (livingTempC > TEMP_HIGH_THRESHOLD_C);
  bool livingLightHigh = (livingLightRaw > LIGHT_HIGH_THRESHOLD_RAW);
  bool livingAlarm = livingTempHigh && livingLightHigh;

  bool bedroomTempHigh = (!isnan(bedroomTempC)) && (bedroomTempC > TEMP_HIGH_THRESHOLD_C);
  bool bedroomLightHigh = (bedroomLightRaw > LIGHT_HIGH_THRESHOLD_RAW);
  bool bedroomAlarm = bedroomTempHigh && bedroomLightHigh;

  digitalWrite(LED_LIVING_ALERT, livingAlarm ? HIGH : LOW);
  digitalWrite(LED_BEDROOM_ALERT, bedroomAlarm ? HIGH : LOW);

  // Print exception info when transitions to alarm state, but still required to "输出房间级异常信息"
  // We'll print while alarm is active every 5s report too (handled separately).
  if (livingAlarm && !livingAlarmPrev) {
    Serial.print("[ALERT] 客厅异常: 温度过高且光照过强. TempC=");
    if (isnan(livingTempC)) Serial.print("nan");
    else Serial.print(livingTempC, 1);
    Serial.print("C, LightRaw=");
    Serial.print(livingLightRaw);
    Serial.print(" (阈值LightRaw>");
    Serial.print(LIGHT_HIGH_THRESHOLD_RAW);
    Serial.println(")");
  }
  if (bedroomAlarm && !bedroomAlarmPrev) {
    Serial.print("[ALERT] 卧室异常: 温度过高且光照过强. TempC=");
    if (isnan(bedroomTempC)) Serial.print("nan");
    else Serial.print(bedroomTempC, 1);
    Serial.print("C, LightRaw=");
    Serial.print(bedroomLightRaw);
    Serial.print(" (阈值LightRaw>");
    Serial.print(LIGHT_HIGH_THRESHOLD_RAW);
    Serial.println(")");
  }

  livingAlarmPrev = livingAlarm;
  bedroomAlarmPrev = bedroomAlarm;
}

// ---------- 5s Serial full report ----------
void printFullReport() {
  // "每 5 秒通过串口输出完整环境报表"
  // Include both rooms, all 3 items, and whether alarm is active + exception info if needed.
  static bool livingAlarmPrevPrint = false;
  static bool bedroomAlarmPrevPrint = false;

  // evaluate alarms for report
  bool livingAlarm = (!isnan(livingTempC)) && (livingTempC > TEMP_HIGH_THRESHOLD_C) && (livingLightRaw > LIGHT_HIGH_THRESHOLD_RAW);
  bool bedroomAlarm = (!isnan(bedroomTempC)) && (bedroomTempC > TEMP_HIGH_THRESHOLD_C) && (bedroomLightRaw > LIGHT_HIGH_THRESHOLD_RAW);

  Serial.println("----- 环境报表 (Living / Bedroom) -----");

  Serial.print("客厅: ");
  Serial.print("温度=");
  if (isnan(livingTempC)) Serial.print("nan");
  else Serial.print(livingTempC, 1);
  Serial.print("C, 湿度=");
  if (isnan(livingHumPct)) Serial.print("nan");
  else Serial.print(livingHumPct, 1);
  Serial.print("%, 光照Raw=");
  Serial.print(livingLightRaw);
  Serial.print(", 异常=");
  Serial.println(livingAlarm ? "YES" : "NO");

  Serial.print("卧室: ");
  Serial.print("温度=");
  if (isnan(bedroomTempC)) Serial.print("nan");
  else Serial.print(bedroomTempC, 1);
  Serial.print("C, 湿度=");
  if (isnan(bedroomHumPct)) Serial.print("nan");
  else Serial.print(bedroomHumPct, 1);
  Serial.print("%, 光照Raw=");
  Serial.print(bedroomLightRaw);
  Serial.print(", 异常=");
  Serial.println(bedroomAlarm ? "YES" : "NO");

  // Explicit exception info output when alarm is active
  if (livingAlarm) {
    Serial.print("[EXCEPTION] 客厅: TempC=");
    Serial.print(isnan(livingTempC) ? 0 : livingTempC, 1);
    Serial.print("C (> ");
    Serial.print(TEMP_HIGH_THRESHOLD_C, 1);
    Serial.print("), LightRaw=");
    Serial.print(livingLightRaw);
    Serial.print(" (> ");
    Serial.print(LIGHT_HIGH_THRESHOLD_RAW);
    Serial.println(")");
  }
  if (bedroomAlarm) {
    Serial.print("[EXCEPTION] 卧室: TempC=");
    Serial.print(isnan(bedroomTempC) ? 0 : bedroomTempC, 1);
    Serial.print("C (> ");
    Serial.print(TEMP_HIGH_THRESHOLD_C, 1);
    Serial.print("), LightRaw=");
    Serial.print(bedroomLightRaw);
    Serial.print(" (> ");
    Serial.print(LIGHT_HIGH_THRESHOLD_RAW);
    Serial.println(")");
  }

  Serial.println("----------------------------------------");
}

// ---------- Timing for serial reports ----------
unsigned long lastSerialReportMs = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BTN_NEXT_ROOM, INPUT_PULLUP);
  pinMode(BTN_NEXT_ITEM, INPUT_PULLUP);

  pinMode(LED_LIVING_ALERT, OUTPUT);
  pinMode(LED_BEDROOM_ALERT, OUTPUT);
  digitalWrite(LED_LIVING_ALERT, LOW);
  digitalWrite(LED_BEDROOM_ALERT, LOW);

  dhtLiving.begin();
  dhtBedroom.begin();

  tm.begin();

  lastDisplayRotateMs = millis();
  lastSerialReportMs = millis();
  lastDhtSampleMs = 0;

  // initial reads
  sampleSensors();
  updateAlertsAndReportRoomException();
  displayCurrent();
}

void loop() {
  handleButtons();

  // Periodically sample sensors
  sampleSensors();

  // Update alerts based on latest readings
  updateAlertsAndReportRoomException();

  // Rotate display on timer
  rotateDisplayByTimerIfNeeded();

  // Update display for current state
  displayCurrent();

  // Serial full report every 5s
  unsigned long now = millis();
  if (now - lastSerialReportMs >= SERIAL_REPORT_INTERVAL_MS) {
    lastSerialReportMs = now;
    printFullReport();
  }
}