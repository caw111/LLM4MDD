#include <Arduino.h>
#include <DHT.h>

// ---------------- Pin assignments (fixed for Arduino UNO; avoid Dx aliases) ----------------
// TM1637 (CLK=D4, DIO=D5)
static const uint8_t TM1637_CLK = 4;
static const uint8_t TM1637_DIO = 5;

// DHT11 (Living: D7, Bedroom: D8)
static const uint8_t DHT_LIVING_PIN  = 7;
static const uint8_t DHT_BEDROOM_PIN = 8;
static const uint8_t DHT11_TYPE = DHT11;

DHT dhtLiving(DHT_LIVING_PIN, DHT11_TYPE);
DHT dhtBedroom(DHT_BEDROOM_PIN, DHT11_TYPE);

// Light sensors (A0, A1)
static const uint8_t LDR_LIVING_PIN  = A0;
static const uint8_t LDR_BEDROOM_PIN = A1;

// Buttons (D2, D3)
static const uint8_t BTN_NEXT_ROOM = 2;
static const uint8_t BTN_NEXT_ITEM = 3;

// Alerts (D12, D13)
static const uint8_t LED_LIVING_ALERT  = 12;
static const uint8_t LED_BEDROOM_ALERT = 13;

// ---------------- TM1637 minimal helper (bit-banged) ----------------
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
    brightness &= 0x07;
    start();
    writeByte(0x88 | brightness);
    stop();
  }

  void displayDigits(const uint8_t digits[4], bool colon = false) {
    uint8_t seg[4];
    for (int i = 0; i < 4; i++) seg[i] = digitToSegment(digits[i]);
    if (colon) seg[1] |= 0x80;

    start();
    writeByte(0x40);
    for (int i = 0; i < 4; i++) writeByte(seg[i]);
    stop();

    start();
    writeByte(0xC0);
    stop();
  }

  void showNumberInt(int value, bool showBlankLeading = true) {
    uint8_t digits[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    bool neg = false;
    if (value < 0) {
      neg = true;
      value = -value;
    }

    int d[4] = {0, 0, 0, 0};
    d[3] = value % 10;
    d[2] = (value / 10) % 10;
    d[1] = (value / 100) % 10;
    d[0] = (value / 1000) % 10;

    bool started = !showBlankLeading ? true : (d[0] != 0);
    for (int i = 0; i < 4; i++) {
      if (!started && showBlankLeading) {
        if (i == 0) digits[i] = 0xFF;
        else if (d[i] != 0) {
          started = true;
          digits[i] = d[i];
        } else digits[i] = 0xFF;
      } else {
        digits[i] = d[i];
      }
    }

    if (neg) digits[0] = 10; // '-' special mapping
    displayDigits(digits, false);
  }

  void showCustom(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    uint8_t digits[4] = {a, b, c, d};
    displayDigits(digits, false);
  }

private:
  uint8_t _clk, _dio;

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
    digitalWrite(_clk, LOW);
    pinMode(_dio, INPUT);
    digitalWrite(_clk, HIGH);
    pinMode(_dio, OUTPUT);
    digitalWrite(_clk, LOW);
  }

  uint8_t digitToSegment(uint8_t digit) {
    if (digit == 0xFF) return 0x00;
    if (digit == 10) return 0x40; // '-'

    static const uint8_t map[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    return map[digit & 0x0F];
  }
};

TM1637Mini tm(TM1637_CLK, TM1637_DIO);

// ---------------- Requirements / thresholds / timers ----------------
const unsigned long SERIAL_REPORT_INTERVAL_MS = 5000;

static const float TEMP_HIGH_THRESHOLD_C = 30.0;   // temperature over high
static const int   LIGHT_HIGH_THRESHOLD_RAW = 700; // light over strong (ADC raw)

// "系统需要轮流显示不同房间的温度、湿度和光照"
static const unsigned long DISPLAY_ROTATE_INTERVAL_MS = 1500;
static unsigned long lastDisplayRotateMs = 0;

// debounce
static const unsigned long BTN_DEBOUNCE_MS = 60;
static unsigned long lastBtnNextRoomMs = 0;
static unsigned long lastBtnNextItemMs = 0;
static bool lastBtnNextRoomState = HIGH; // released
static bool lastBtnNextItemState  = HIGH;

// sensor sampling
static unsigned long lastDhtSampleMs = 0;
static const unsigned long DHT_SAMPLE_INTERVAL_MS = 2000;

// ---------------- State ----------------
enum Room : uint8_t { ROOM_LIVING = 0, ROOM_BEDROOM = 1 };
enum Item : uint8_t { ITEM_TEMPERATURE = 0, ITEM_HUMIDITY = 1, ITEM_LIGHT = 2 };

static Room currentRoom = ROOM_LIVING;
static Item currentItem = ITEM_TEMPERATURE;

static float livingTempC = NAN;
static float livingHumPct = NAN;
static float bedroomTempC = NAN;
static float bedroomHumPct = NAN;

static int livingLightRaw = 0;
static int bedroomLightRaw = 0;

// ---------------- Button handling ----------------
static inline bool readButtonPressed(uint8_t pin) {
  // Active LOW: pressed => LOW
  return digitalRead(pin) == LOW;
}

static void handleButtons() {
  unsigned long now = millis();

  // BTN_NEXT_ROOM: toggle room
  {
    bool currentPressed = readButtonPressed(BTN_NEXT_ROOM);
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
    bool currentPressed = readButtonPressed(BTN_NEXT_ITEM);
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

// ---------------- Display logic ----------------
static void displayCurrent() {
  int shown = 0;
  bool valid = true;

  if (currentRoom == ROOM_LIVING) {
    if (currentItem == ITEM_TEMPERATURE) {
      if (isnan(livingTempC)) valid = false;
      else shown = (int)lround(livingTempC);
    } else if (currentItem == ITEM_HUMIDITY) {
      if (isnan(livingHumPct)) valid = false;
      else shown = (int)lround(livingHumPct);
    } else {
      shown = livingLightRaw;
    }
  } else { // ROOM_BEDROOM
    if (currentItem == ITEM_TEMPERATURE) {
      if (isnan(bedroomTempC)) valid = false;
      else shown = (int)lround(bedroomTempC);
    } else if (currentItem == ITEM_HUMIDITY) {
      if (isnan(bedroomHumPct)) valid = false;
      else shown = (int)lround(bedroomHumPct);
    } else {
      shown = bedroomLightRaw;
    }
  }

  if (currentItem == ITEM_LIGHT) {
    if (shown < 0) shown = 0;
    if (shown > 999) shown = 999;
  }

  if (!valid) {
    tm.showCustom(10, 10, 10, 10); // "----" style (minus segments)
    return;
  }

  tm.setBrightness(0x06);
  tm.showNumberInt(shown, true);
}

static void rotateDisplayByTimerIfNeeded() {
  unsigned long now = millis();
  if (now - lastDisplayRotateMs >= DISPLAY_ROTATE_INTERVAL_MS) {
    lastDisplayRotateMs = now;
    currentItem = static_cast<Item>((static_cast<uint8_t>(currentItem) + 1) % 3);
    if (currentItem == ITEM_TEMPERATURE) {
      currentRoom = (currentRoom == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    }
  }
}

// ---------------- Sensor sampling ----------------
static void sampleSensors() {
  unsigned long now = millis();
  if (now - lastDhtSampleMs < DHT_SAMPLE_INTERVAL_MS) return;

  lastDhtSampleMs = now;

  float tL = dhtLiving.readTemperature();
  float hL = dhtLiving.readHumidity();
  float tB = dhtBedroom.readTemperature();
  float hB = dhtBedroom.readHumidity();

  if (!isnan(tL)) livingTempC = tL;
  if (!isnan(hL)) livingHumPct = hL;
  if (!isnan(tB)) bedroomTempC = tB;
  if (!isnan(hB)) bedroomHumPct = hB;

  livingLightRaw = analogRead(LDR_LIVING_PIN);
  bedroomLightRaw = analogRead(LDR_BEDROOM_PIN);
}

// ---------------- Alerts & exception output ----------------
static void updateAlertsAndReportRoomException() {
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

// ---------------- 5s full serial report ----------------
static void printFullReport() {
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

static unsigned long lastSerialReportMs = 0;

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

  sampleSensors();
  updateAlertsAndReportRoomException();
  displayCurrent();
}

void loop() {
  handleButtons();

  sampleSensors();
  updateAlertsAndReportRoomException();

  rotateDisplayByTimerIfNeeded();
  displayCurrent();

  unsigned long now = millis();
  if (now - lastSerialReportMs >= SERIAL_REPORT_INTERVAL_MS) {
    lastSerialReportMs = now;
    printFullReport();
  }
}