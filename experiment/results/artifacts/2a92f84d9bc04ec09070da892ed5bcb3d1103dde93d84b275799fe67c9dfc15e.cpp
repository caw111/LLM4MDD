#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>

// -------------------- Pins (authoritative) --------------------
static const uint8_t PIN_DHT_LIVING   = D7;   // D7
static const uint8_t PIN_DHT_BEDROOM  = D8;   // D8

static const uint8_t PIN_LDR_LIVING  = A0;   // A0
static const uint8_t PIN_LDR_BEDROOM = A1;   // A1

static const uint8_t PIN_TM1637_CLK   = D4;   // D4
static const uint8_t PIN_TM1637_DIO   = D5;   // D5

static const uint8_t PIN_BTN_NEXT     = D2;   // D2: button to cycle room
static const uint8_t PIN_BTN_MODE     = D3;   // D3: button to cycle display item

static const uint8_t PIN_LED_LIVING  = D12;  // D12: alarm LED for living
static const uint8_t PIN_LED_BEDROOM = D13;  // D13: alarm LED for bedroom

// -------------------- Serial --------------------
static const uint32_t SERIAL_BAUD = 9600;
static const uint32_t REPORT_INTERVAL_MS = 5000;

// -------------------- Display & UI --------------------
enum Room : uint8_t { ROOM_LIVING = 0, ROOM_BEDROOM = 1 };
enum Item : uint8_t { ITEM_TEMP = 0, ITEM_HUM = 1, ITEM_LUX = 2 };

static volatile uint8_t g_room = ROOM_LIVING;
static volatile uint8_t g_item = ITEM_TEMP;

// Display timing (rotate rooms & items via polling buttons; also rotate if desired)
static const uint32_t DISPLAY_SWITCH_INTERVAL_MS = 2000; // keep stable for "轮询展示"
static uint32_t lastDisplaySwitchMs = 0;

// -------------------- DHT --------------------
static DHT dhtLiving(PIN_DHT_LIVING, DHT11);
static DHT dhtBedroom(PIN_DHT_BEDROOM, DHT11);

// -------------------- TM1637 --------------------
static TM1637Display tm1637(PIN_TM1637_CLK, PIN_TM1637_DIO);

// -------------------- LDR Calibration/Thresholds --------------------
// NOTE: No explicit calibration constants were provided in the prompt.
// These are conservative defaults to preserve behavior and allow alarm logic.
// If your circuit provides different calibration, adjust here.
static const int LDR_ADC_THRESHOLD_STRONG_LIGHT = 700; // "光照过强" threshold (ADC units, 0-1023)

// "温度过高" threshold for alarm.
// DHT11 typical range 0-50C; set threshold around 30C.
static const float TEMP_THRESHOLD_HIGH_C = 30.0f;

// -------------------- Button handling (debounced) --------------------
struct Button {
  uint8_t pin;
  bool lastStable;
  bool lastReading;
  uint32_t lastDebounceMs;
};

static Button btnNext  = { PIN_BTN_NEXT,  true, true, 0 };
static Button btnMode  = { PIN_BTN_MODE,  true, true, 0 };

#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void initButton(Button &b);
#line 71 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static bool updateButton(Button &b);
#line 101 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readSensorsForRoom(Room r);
#line 127 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void readAllSensors();
#line 132 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateAlarms();
#line 182 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int luxFromADC(int adc);
#line 191 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void showOnTM1637(Room r, Item it);
#line 224 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static const char * roomName(Room r);
#line 228 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static const char * itemName(Item it);
#line 238 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 268 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\advanced_005\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
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

  // DHT reads can return NaN; keep valid flags.
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

static void updateAlarms() {
  bool livingAlarm = false;
  bool bedroomAlarm = false;

  if (livingData.tempValid) {
    livingAlarm = (livingData.tempC >= TEMP_THRESHOLD_HIGH_C);
  }
  // "若任一房间温度过高且光照过强"
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

  // Output room-level exception info when alarm happens
  // (Requirement: "并输出房间级异常信息")
  static bool prevLivingAlarm = false;
  static bool prevBedroomAlarm = false;

  if (livingAlarm && !prevLivingAlarm) {
    Serial.print("[ALERT] 客厅异常: ");
    Serial.print("温度=");
    if (livingData.tempValid) Serial.print(livingData.tempC, 1);
    else Serial.print("NaN");
    Serial.print("C, 光照ADC=");
    Serial.println(livingData.ldrADC);
  }
  if (bedroomAlarm && !prevBedroomAlarm) {
    Serial.print("[ALERT] 卧室异常: ");
    Serial.print("温度=");
    if (bedroomData.tempValid) Serial.print(bedroomData.tempC, 1);
    else Serial.print("NaN");
    Serial.print("C, 光照ADC=");
    Serial.println(bedroomData.ldrADC);
  }

  prevLivingAlarm = livingAlarm;
  prevBedroomAlarm = bedroomAlarm;
}

// Convert ADC to a "lux-like" number.
// No mapping constants provided; use ADC as display proxy.
static int luxFromADC(int adc) {
  // scale to 0-999 range to fit 4-digit display with leading trim.
  // Keep monotonic behavior.
  long scaled = (long)adc * 999L / 1023L;
  if (scaled < 0) scaled = 0;
  if (scaled > 999) scaled = 999;
  return (int)scaled;
}

static void showOnTM1637(Room r, Item it) {
  RoomData &d = (r == ROOM_LIVING) ? livingData : bedroomData;

  // 4-digit display; show integer. For temperature/humidity, show one decimal via times10.
  tm1637.clear();

  if (it == ITEM_TEMP) {
    // Display temp * 10 as integer with decimal point.
    int val = 0;
    if (d.tempValid) {
      val = (int)round(d.tempC * 10.0f); // e.g., 25.3C => 253
    } else {
      // If invalid, show 0
      val = 0;
    }
    // TM1637Display::showNumberDecEx supports decimals on dots.
    // Use '.' on second digit from right to emulate one decimal.
    // Format: -99.9 to 999.9; but we'll clamp.
    if (val > 9999) val = 9999;
    if (val < -999) val = -999; // keep reasonable

    // Use leading zeros off.
    tm1637.showNumberDecEx(val, (0x40) /* show dot at position 2 from right */ , true);
  } else if (it == ITEM_HUM) {
    // Display humidity as integer percent.
    int val = d.humValid ? (int)round(d.humPercent) : 0;
    tm1637.showNumberDec(val, true);
  } else { // ITEM_LUX
    int val = luxFromADC(d.ldrADC);
    tm1637.showNumberDec(val, true);
  }
}

static const char* roomName(Room r) {
  return (r == ROOM_LIVING) ? "客厅" : "卧室";
}

static const char* itemName(Item it) {
  switch (it) {
    case ITEM_TEMP: return "温度(C)";
    case ITEM_HUM: return "湿度(%RH)";
    case ITEM_LUX: return "光照(ADC/映射)";
    default: return "";
  }
}

// -------------------- Setup/Loop --------------------
void setup() {
  Serial.begin(SERIAL_BAUD);

  // LEDs
  pinMode(PIN_LED_LIVING, OUTPUT);
  pinMode(PIN_LED_BEDROOM, OUTPUT);
  digitalWrite(PIN_LED_LIVING, LOW);
  digitalWrite(PIN_LED_BEDROOM, LOW);

  // Buttons
  initButton(btnNext);
  initButton(btnMode);

  // DHT init
  dhtLiving.begin();
  dhtBedroom.begin();

  // TM1637 brightness (0-7). Choose reasonable default.
  tm1637.setBrightness(4);

  // Prime initial readings
  readAllSensors();
  updateAlarms();

  // Show initial
  showOnTM1637((Room)g_room, (Item)g_item);

  lastDisplaySwitchMs = millis();
}

void loop() {
  // Update buttons
  if (updateButton(btnNext)) {
    // Manual room switch
    g_room = (g_room == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    // Read current room so display is immediate
    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  if (updateButton(btnMode)) {
    // Manual item switch
    g_item = (g_item == ITEM_TEMP) ? ITEM_HUM : (g_item == ITEM_HUM ? ITEM_LUX : ITEM_TEMP);
    // Read all sensors not required, but keep values fresh for the selected room
    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  // Rotating display polling (轮流显示不同房间的温度、湿度和光照)
  // Keep it independent from manual switching: periodically step through rooms/items.
  uint32_t now = millis();
  if ((now - lastDisplaySwitchMs) >= DISPLAY_SWITCH_INTERVAL_MS) {
    lastDisplaySwitchMs = now;

    // Polling behavior: iterate room then item
    // First show current room temp/hum/lux across time.
    if (g_item == ITEM_LUX) {
      g_item = ITEM_TEMP;
      g_room = (g_room == ROOM_LIVING) ? ROOM_BEDROOM : ROOM_LIVING;
    } else {
      g_item = (Item)(g_item + 1);
    }

    readSensorsForRoom((Room)g_room);
    showOnTM1637((Room)g_room, (Item)g_item);
  }

  // Periodic 5-second complete environment report via serial
  static uint32_t lastReportMs = 0;
  if ((now - lastReportMs) >= REPORT_INTERVAL_MS) {
    lastReportMs = now;

    readAllSensors();
    updateAlarms();

    // Complete environment report
    Serial.println("===== 环境报表 =====");
    // 客厅
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

    // 卧室
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

    // Also include current display selection status
    Serial.print("当前轮询显示: ");
    Serial.print(roomName((Room)g_room));
    Serial.print(" / ");
    Serial.print(itemName((Item)g_item));
    Serial.println();
    Serial.println("====================");
  }

  // Keep alarms updated even between reports when sensor changes (lightweight)
  // Read current room sensors occasionally without blocking.
  static uint32_t lastFastReadMs = 0;
  if ((now - lastFastReadMs) >= 1000) {
    lastFastReadMs = now;
    readSensorsForRoom(ROOM_LIVING);
    readSensorsForRoom(ROOM_BEDROOM);
    updateAlarms();
  }
}
