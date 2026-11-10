#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// -------------------- Pins (Arduino UNO authoritative numeric pins) --------------------
// D7 -> 7, D8 -> 8, D4 -> 4, D5 -> 5, D2 -> 2, D3 -> 3, D12 -> 12, D13 -> 13
static const uint8_t PIN_DHT_LIVING   = 7;   // 客厅传感器 D7
static const uint8_t PIN_DHT_BEDROOM  = 8;   // 卧室传感器 D8

static const uint8_t PIN_LDR_LIVING  = A0;  // 两个光敏电阻 A0/A1
static const uint8_t PIN_LDR_BEDROOM = A1;

static const uint8_t PIN_TM1637_CLK   = 4;   // CLK=D4
static const uint8_t PIN_TM1637_DIO   = 5;   // DIO=D5

static const uint8_t PIN_BTN_NEXT     = 2;   // 按钮 D2
static const uint8_t PIN_BTN_MODE     = 3;   // 按钮 D3

static const uint8_t PIN_LED_LIVING  = 12;  // D12: 客厅告警LED
static const uint8_t PIN_LED_BEDROOM = 13;  // D13: 卧室告警LED

// -------------------- Serial --------------------
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t REPORT_INTERVAL_MS = 5000;

// -------------------- Display & UI --------------------
enum Room : uint8_t { ROOM_LIVING = 0, ROOM_BEDROOM = 1 };
enum Item : uint8_t { ITEM_TEMP = 0, ITEM_HUM = 1, ITEM_LUX = 2 };

static volatile uint8_t g_room = ROOM_LIVING;
static volatile uint8_t g_item = ITEM_TEMP;

// 轮询展示（房间/项目轮流显示）
static const uint32_t DISPLAY_SWITCH_INTERVAL_MS = 2000;
static uint32_t lastDisplaySwitchMs = 0;

// -------------------- DHT --------------------
static DHT dhtLiving(PIN_DHT_LIVING, DHT11);
static DHT dhtBedroom(PIN_DHT_BEDROOM, DHT11);

// -------------------- TM1637 --------------------
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- LDR Thresholds --------------------
// "光照过强" threshold (ADC units, 0-1023)
static const int LDR_ADC_THRESHOLD_STRONG_LIGHT = 700;

// "温度过高" threshold (C)
static const float TEMP_THRESHOLD_HIGH_C = 30.0f;

// -------------------- Button handling (debounced) --------------------
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastReading;
  uint32_t lastDebounceMs;
};

static Button btnNext = { PIN_BTN_NEXT, true, true, 0 };
static Button btnMode = { PIN_BTN_MODE, true, true, 0 };

#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void initButton(Button &b);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static bool updateButton(Button &b);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readSensorsForRoom(Room r);
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void readAllSensors();
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static int luxFromADC(int adc);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void showOnTM1637(Room r, Item it);
#line 163 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void updateAlarms();
#line 210 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void setup();
#line 233 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
void loop();
#line 62 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct_feedback\\attempt_1\\sketch\\sketch.ino"
static void initButton(Button &b) {
  pinMode(b.pin, INPUT_PULLUP);
  b.lastStable = digitalRead(b.pin);
  b.lastReading = b.lastStable;
  b.lastDebounceMs = millis();
}

static bool updateButton(Button &b) {
  bool reading = digitalRead(b.pin);
  if (reading != b.lastReading) {
    b.lastDebounceMs = millis();
    b.lastReading = reading;
  }

  if ((millis() - b.lastDebounceMs) > 25) {
    if (reading != b.lastStable) {
      b.lastStable = reading;
      // Active LOW press
      if (b.lastStable == false) return true;
    }
  }
  return false;
}

// -------------------- Sensor reading buffers --------------------
struct RoomData {
  float tempC;
  float humPercent;
  int ldrADC;
  bool tempValid;
  bool humValid;
};

static RoomData livingData = {0, 0, 0, false, false};
static RoomData bedroomData = {0, 0, 0, false, false};

// -------------------- Helpers --------------------
static void readSensorsForRoom(Room r) {
  RoomData &d = (r == ROOM_LIVING) ? livingData : bedroomData;

  float h = NAN;
  float t = NAN;

  if (r == ROOM_LIVING) {
    h = dhtLiving.readHumidity();
    t = dhtLiving.readTemperature(); // Celsius
  } else {
    h = dhtBedroom.readHumidity();
    t = dhtBedroom.readTemperature(); // Celsius
  }

  bool okH = !isnan(h);
  bool okT = !isnan(t);

  d.humPercent = okH ? h : d.humPercent;
  d.tempC = okT ? t : d.tempC;
  d.humValid = okH;
  d.tempValid = okT;

  d.ldrADC = (r == ROOM_LIVING) ? analogRead(PIN_LDR_LIVING) : analogRead(PIN_LDR_BEDROOM);
}

static void readAllSensors() {
  readSensorsForRoom(ROOM_LIVING);
  readSensorsForRoom(ROOM_BEDROOM);
}

static int luxFromADC(int adc) {
  long scaled = (long)adc * 999L / 1023L;
  if (scaled < 0) scaled = 0;
  if (scaled > 999) scaled = 999;
  return (int)scaled;
}

static void showOnTM1637(Room r, Item it) {
  RoomData &d = (r == ROOM_LIVING) ? livingData : bedroomData;

  tm1637.clear();

  if (it == ITEM_TEMP) {
    int val = 0;
    if (d.tempValid) {
      val = (int)round(d.tempC * 10.0f); // one decimal (x10)
    } else {
      val = 0;
    }

    if (val > 9999) val = 9999;
    if (val < -999) val = -999;

    // Use dot at position 2 from right to emulate one decimal
    tm1637.showNumberDecEx(val, (0x40), true);
  } else if (it == ITEM_HUM) {
    int val = d.humValid ? (int)round(d.humPercent) : 0;
    tm1637.showNumberDec(val, true);
  } else { // ITEM_LUX
    int val = luxFromADC(d.ldrADC);
    tm1637.showNumberDec(val, true);
  }
}

static void updateAlarms() {
  bool livingAlarm = false;
  bool bedroomAlarm = false;

  if (livingData.tempValid) {
    livingAlarm = (livingData.tempC >= TEMP_THRESHOLD_HIGH_C);
  }
  // 若任一房间温度过高且光照过强
  if (livingAlarm) {
    livingAlarm = (livingData.ldrADC >= LDR_ADC_THRESHOLD_STRONG_LIGHT);
  }

  if (bedroomData.tempValid) {
    bedroomAlarm = (bedroomData.tempC >= TEMP_THRESHOLD_HIGH_C);
  }
  if (bedroomAlarm) {
    bedroomAlarm = (bedroomData.ldrADC >= LDR_ADC_THRESHOLD_STRONG_LIGHT);
  }

  digitalWrite(PIN_LED_LIVING, livingAlarm ? HIGH : LOW);
  digitalWrite(PIN_LED_BEDROOM, bedroomAlarm ? HIGH : LOW);

  // 输出房间级异常信息（警报从低到高时输出一次）
  static bool prevLivingAlarm = false;
  static bool prevBedroomAlarm = false;

  if (livingAlarm && !prevLivingAlarm) {
    Serial.print("[ALERT] 客厅异常: 温度=");
    if (livingData.tempValid) Serial.print(livingData.tempC, 1);
    else Serial.print("NaN");
    Serial.print("C, 光照ADC=");
    Serial.println(livingData.ldrADC);
  }

  if (bedroomAlarm && !prevBedroomAlarm) {
    Serial.print("[ALERT] 卧室异常: 温度=");
    if (bedroomData.tempValid) Serial.print(bedroomData.tempC, 1);
    else Serial.print("NaN");
    Serial.print("C, 光照ADC=");
    Serial.println(bedroomData.ldrADC);
  }

  prevLivingAlarm = livingAlarm;
  prevBedroomAlarm = bedroomAlarm;
}

// -------------------- Setup/Loop --------------------
void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(PIN_LED_LIVING, OUTPUT);
  pinMode(PIN_LED_BEDROOM, OUTPUT);
  digitalWrite(PIN_LED_LIVING, LOW);
  digitalWrite(PIN_LED_BEDROOM, LOW);

  initButton(btnNext);
  initButton(btnMode);

  dhtLiving.begin();
  dhtBedroom.begin();

  tm1637.setBrightness(4);

  readAllSensors();
  updateAlarms();

  showOnTM1637((Room)g_room, (Item)g_item);
  lastDisplaySwitchMs = millis();
}

void loop() {
  // 手动切换房间/显示项目
  if (updateButton(btnNext)) {
    g_room = (g_room == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  if (updateButton(btnMode)) {
    if (g_item == ITEM_TEMP) g_item = ITEM_HUM;
    else if (g_item == ITEM_HUM) g_item = ITEM_LUX;
    else g_item = ITEM_TEMP;

    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  // 轮询展示：每 DISPLAY_SWITCH_INTERVAL_MS 切换一次（按原逻辑迭代房间+项目）
  uint32_t now = millis();
  if ((now - lastDisplaySwitchMs) >= DISPLAY_SWITCH_INTERVAL_MS) {
    lastDisplaySwitchMs = now;

    if (g_item == ITEM_LUX) {
      g_item = ITEM_TEMP;
      g_room = (g_room == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    } else {
      g_item = (Item)(g_item + 1);
    }

    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  // 每 5 秒通过串口输出完整环境报表，并包含轮询显示状态
  static uint32_t lastReportMs = 0;
  if ((now - lastReportMs) >= REPORT_INTERVAL_MS) {
    lastReportMs = now;

    readAllSensors();
    updateAlarms();

    Serial.println("===== 环境报表 =====");

    Serial.print("[客厅] 温度=");
    if (livingData.tempValid) {
      Serial.print(livingData.tempC, 1);
      Serial.print("C");
    } else {
      Serial.print("NaN");
    }
    Serial.print(", 湿度=");
    if (livingData.humValid) {
      Serial.print(livingData.humPercent, 1);
      Serial.print("%RH");
    } else {
      Serial.print("NaN");
    }
    Serial.print(", 光照ADC=");
    Serial.println(livingData.ldrADC);

    Serial.print("[卧室] 温度=");
    if (bedroomData.tempValid) {
      Serial.print(bedroomData.tempC, 1);
      Serial.print("C");
    } else {
      Serial.print("NaN");
    }
    Serial.print(", 湿度=");
    if (bedroomData.humValid) {
      Serial.print(bedroomData.humPercent, 1);
      Serial.print("%RH");
    } else {
      Serial.print("NaN");
    }
    Serial.print(", 光照ADC=");
    Serial.println(bedroomData.ldrADC);

    Serial.print("当前轮询显示: ");
    Serial.print((g_room == ROOM_LIVING) ? "客厅" : "卧室");
    Serial.print(" / ");
    if (g_item == ITEM_TEMP) Serial.print("温度(C)");
    else if (g_item == ITEM_HUM) Serial.print("湿度(%RH)");
    else Serial.print("光照(ADC/映射)");
    Serial.println();

    Serial.println("====================");
  }

  // 保持报警逻辑在两次报表之间更新（原逻辑：每 1 秒快速读并更新）
  static uint32_t lastFastReadMs = 0;
  if ((now - lastFastReadMs) >= 1000) {
    lastFastReadMs = now;
    readSensorsForRoom(ROOM_LIVING);
    readSensorsForRoom(ROOM_BEDROOM);
    updateAlarms();
  }
}
