#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>
#include <string.h>

// Required libraries: "DHT sensor library" and "TM1637Display".
constexpr uint8_t ROOM_COUNT = 2;
constexpr uint8_t DHT_PINS[ROOM_COUNT] = {7, 8};
constexpr uint8_t LIGHT_PINS[ROOM_COUNT] = {A0, A1};
constexpr uint8_t ALERT_PINS[ROOM_COUNT] = {12, 13};
constexpr uint8_t ROOM_BUTTON_PIN = 2;
constexpr uint8_t ITEM_BUTTON_PIN = 3;
constexpr uint8_t DISPLAY_CLK_PIN = 4;
constexpr uint8_t DISPLAY_DIO_PIN = 5;

// Buttons connect their input pins to GND when pressed.
constexpr uint32_t DEBOUNCE_MS = 35;
constexpr uint32_t DHT_INTERVAL_MS = 2000;
constexpr uint32_t LIGHT_INTERVAL_MS = 200;
constexpr uint32_t AUTO_PAGE_INTERVAL_MS = 3000;
constexpr uint32_t ROOM_LABEL_MS = 650;
constexpr uint32_t REPORT_INTERVAL_MS = 5000;
constexpr uint32_t SERIAL_IDLE_TIMEOUT_MS = 20;

// Configurable alarm thresholds: BOTH conditions must be true.
constexpr float HIGH_TEMPERATURE_C = 30.0f;
constexpr uint8_t STRONG_LIGHT_PERCENT = 80;

// Light calibration endpoints. Reverse these values if the actual
// voltage-divider wiring produces smaller ADC readings in brighter light.
constexpr int LIGHT_DARK_ADC[ROOM_COUNT] = {0, 0};
constexpr int LIGHT_BRIGHT_ADC[ROOM_COUNT] = {1023, 1023};

DHT livingRoomDht(DHT_PINS[0], DHT11);
DHT bedroomDht(DHT_PINS[1], DHT11);
DHT *const dhtSensors[ROOM_COUNT] = {&livingRoomDht, &bedroomDht};
TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

enum DisplayItem : uint8_t {
  TEMPERATURE,
  HUMIDITY,
  LIGHT,
  ITEM_COUNT
};

struct RoomData {
  float temperature;
  float humidity;
  int lightRaw;
  uint8_t lightPercent;
  bool temperatureValid;
  bool humidityValid;
  bool alarm;
};

struct Button {
  uint8_t pin;
  uint8_t lastRaw;
  uint8_t stableState;
  uint32_t lastChangeMs;
};

RoomData rooms[ROOM_COUNT];
Button roomButton = {ROOM_BUTTON_PIN, HIGH, HIGH, 0};
Button itemButton = {ITEM_BUTTON_PIN, HIGH, HIGH, 0};

uint8_t selectedRoom = 0;
DisplayItem selectedItem = TEMPERATURE;
bool showingRoomLabel = true;
uint32_t roomLabelStartedMs = 0;
uint32_t lastDhtMs = 0;
uint32_t lastLightMs = 0;
uint32_t lastPageMs = 0;
uint32_t lastReportMs = 0;

// Optional serial commands, terminated by CR/LF or 20 ms of idle:
// R = next room, I = next item, P = full report.
char serialBuffer[16];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteMs = 0;

#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void printRoomName(uint8_t room);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
bool buttonPressed(Button &button, uint32_t now);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
uint8_t calibratedLightPercent(uint8_t room, int raw);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleLight();
#line 134 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleDht();
#line 150 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void printAlarm(uint8_t room);
#line 161 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarms();
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void printReport();
#line 234 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void showRoomLabel(uint32_t now);
#line 239 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void nextRoom(uint32_t now);
#line 245 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void nextItem(uint32_t now);
#line 251 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void advanceAutomaticPage(uint32_t now);
#line 264 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void renderDisplay(uint32_t now);
#line 335 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void processSerialCommand(uint32_t now);
#line 368 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceSerial();
#line 393 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 448 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_005\\direct\\attempt_0\\sketch\\sketch.ino"
void printRoomName(uint8_t room) {
  if (room == 0) {
    Serial.print(F("客厅"));
  } else {
    Serial.print(F("卧室"));
  }
}

bool buttonPressed(Button &button, uint32_t now) {
  const uint8_t raw = digitalRead(button.pin);

  if (raw != button.lastRaw) {
    button.lastRaw = raw;
    button.lastChangeMs = now;
  }

  if (raw != button.stableState &&
      uint32_t(now - button.lastChangeMs) >= DEBOUNCE_MS) {
    button.stableState = raw;
    return raw == LOW;
  }
  return false;
}

uint8_t calibratedLightPercent(uint8_t room, int raw) {
  const long span = long(LIGHT_BRIGHT_ADC[room]) - LIGHT_DARK_ADC[room];
  if (span == 0) {
    return 0;
  }

  long percent = (long(raw) - LIGHT_DARK_ADC[room]) * 100L / span;
  percent = constrain(percent, 0L, 100L);
  return uint8_t(percent);
}

void sampleLight() {
  for (uint8_t room = 0; room < ROOM_COUNT; ++room) {
    // Discard the first reading after switching ADC channels.
    analogRead(LIGHT_PINS[room]);
    long total = 0;
    for (uint8_t sample = 0; sample < 8; ++sample) {
      total += analogRead(LIGHT_PINS[room]);
    }

    rooms[room].lightRaw = int(total / 8);
    rooms[room].lightPercent =
        calibratedLightPercent(room, rooms[room].lightRaw);
  }
}

void sampleDht() {
  for (uint8_t room = 0; room < ROOM_COUNT; ++room) {
    const float humidity = dhtSensors[room]->readHumidity();
    const float temperature = dhtSensors[room]->readTemperature();

    rooms[room].temperatureValid =
        !isnan(temperature) && temperature >= -20.0f && temperature <= 80.0f;
    rooms[room].humidityValid =
        !isnan(humidity) && humidity >= 0.0f && humidity <= 100.0f;

    // Do not silently reuse stale readings following a sensor failure.
    rooms[room].temperature = temperature;
    rooms[room].humidity = humidity;
  }
}

void printAlarm(uint8_t room) {
  Serial.print(F("[异常] "));
  printRoomName(room);
  Serial.print(F(": 温度过高且光照过强, 温度="));
  Serial.print(rooms[room].temperature, 1);
  Serial.print(F(" C, 光照="));
  Serial.print(rooms[room].lightPercent);
  Serial.print(F("%, ADC="));
  Serial.println(rooms[room].lightRaw);
}

void updateAlarms() {
  for (uint8_t room = 0; room < ROOM_COUNT; ++room) {
    const bool alarm =
        rooms[room].temperatureValid &&
        rooms[room].temperature > HIGH_TEMPERATURE_C &&
        rooms[room].lightPercent > STRONG_LIGHT_PERCENT;

    digitalWrite(ALERT_PINS[room], alarm ? HIGH : LOW);

    if (alarm && !rooms[room].alarm) {
      printAlarm(room);
    } else if (!alarm && rooms[room].alarm) {
      Serial.print(F("[告警解除] "));
      printRoomName(room);
      if (!rooms[room].temperatureValid) {
        Serial.println(F(": 温度无效，无法继续判定温度告警"));
      } else {
        Serial.println(F(": 不再同时满足高温和强光条件"));
      }
    }

    rooms[room].alarm = alarm;
  }
}

void printReport() {
  Serial.println(F("========== 完整环境报表 =========="));
  Serial.print(F("运行时间: "));
  Serial.print(millis() / 1000UL);
  Serial.println(F(" s"));

  for (uint8_t room = 0; room < ROOM_COUNT; ++room) {
    printRoomName(room);
    Serial.print(F(" | 温度: "));
    if (rooms[room].temperatureValid) {
      Serial.print(rooms[room].temperature, 1);
      Serial.print(F(" C"));
    } else {
      Serial.print(F("读取失败"));
    }

    Serial.print(F(" | 湿度: "));
    if (rooms[room].humidityValid) {
      Serial.print(rooms[room].humidity, 1);
      Serial.print(F(" %RH"));
    } else {
      Serial.print(F("读取失败"));
    }

    Serial.print(F(" | 光照: "));
    Serial.print(rooms[room].lightPercent);
    Serial.print(F("% (ADC="));
    Serial.print(rooms[room].lightRaw);
    Serial.print(F(") | 告警: "));
    Serial.println(rooms[room].alarm ? F("是") : F("否"));

    if (rooms[room].alarm) {
      printAlarm(room);
    }
  }

  Serial.print(F("当前显示: "));
  printRoomName(selectedRoom);
  Serial.print(F(" / "));
  switch (selectedItem) {
    case TEMPERATURE: Serial.println(F("温度")); break;
    case HUMIDITY: Serial.println(F("湿度")); break;
    case LIGHT: Serial.println(F("光照")); break;
    default: break;
  }
  Serial.println(F("=================================="));
}

void showRoomLabel(uint32_t now) {
  showingRoomLabel = true;
  roomLabelStartedMs = now;
}

void nextRoom(uint32_t now) {
  selectedRoom = (selectedRoom + 1) % ROOM_COUNT;
  lastPageMs = now;
  showRoomLabel(now);
}

void nextItem(uint32_t now) {
  selectedItem = DisplayItem((uint8_t(selectedItem) + 1) % ITEM_COUNT);
  showingRoomLabel = false;
  lastPageMs = now;
}

void advanceAutomaticPage(uint32_t now) {
  const uint8_t next = uint8_t(selectedItem) + 1;
  if (next >= ITEM_COUNT) {
    selectedItem = TEMPERATURE;
    selectedRoom = (selectedRoom + 1) % ROOM_COUNT;
    showRoomLabel(now);
  } else {
    selectedItem = DisplayItem(next);
    showingRoomLabel = false;
  }
  lastPageMs = now;
}

void renderDisplay(uint32_t now) {
  static uint8_t previousSegments[4] = {0xFF, 0xFF, 0xFF, 0xFF};
  uint8_t segments[4] = {0, 0, 0, 0};

  if (showingRoomLabel &&
      uint32_t(now - roomLabelStartedMs) >= ROOM_LABEL_MS) {
    showingRoomLabel = false;
  }

  if (showingRoomLabel) {
    // "L- 1" = living room; "b- 2" = bedroom.
    segments[0] = selectedRoom == 0 ? 0x38 : 0x7C;
    segments[1] = 0x40;
    segments[3] = display.encodeDigit(selectedRoom + 1);
  } else {
    const RoomData &room = rooms[selectedRoom];
    bool valid = true;
    int value = 0;

    switch (selectedItem) {
      case TEMPERATURE:
        valid = room.temperatureValid;
        if (valid) {
          value = int(round(room.temperature));
        }
        segments[3] = 0x39;  // C, degrees Celsius
        break;

      case HUMIDITY:
        valid = room.humidityValid;
        if (valid) {
          value = int(round(room.humidity));
        }
        segments[3] = 0x76;  // H, relative humidity %
        break;

      case LIGHT:
        value = room.lightPercent;
        segments[3] = 0x38;  // L, calibrated light %
        break;

      default:
        valid = false;
        break;
    }

    if (!valid) {
      segments[0] = segments[1] = segments[2] = 0x40;
    } else {
      const bool negative = value < 0;
      const unsigned int magnitude = negative ? -value : value;

      segments[2] = display.encodeDigit(magnitude % 10);
      if (magnitude >= 10) {
        segments[1] = display.encodeDigit((magnitude / 10) % 10);
      }
      if (magnitude >= 100) {
        segments[0] = display.encodeDigit((magnitude / 100) % 10);
      }
      if (negative) {
        segments[magnitude >= 10 ? 0 : 1] = 0x40;
      }
    }
  }

  if (memcmp(previousSegments, segments, sizeof(segments)) != 0) {
    display.setSegments(segments);
    memcpy(previousSegments, segments, sizeof(segments));
  }
}

void processSerialCommand(uint32_t now) {
  if (serialOverflow) {
    Serial.println(F("ERR: command too long"));
  } else if (serialLength > 0) {
    serialBuffer[serialLength] = '\0';

    if (serialLength == 1) {
      switch (serialBuffer[0]) {
        case 'R':
        case 'r':
          nextRoom(now);
          break;
        case 'I':
        case 'i':
          nextItem(now);
          break;
        case 'P':
        case 'p':
          printReport();
          break;
        default:
          Serial.println(F("Commands: R=room, I=item, P=report"));
          break;
      }
    } else {
      Serial.println(F("Commands: R=room, I=item, P=report"));
    }
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  uint32_t now = millis();

  if ((serialLength > 0 || serialOverflow) &&
      uint32_t(now - lastSerialByteMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    processSerialCommand(now);
  }

  while (Serial.available() > 0) {
    const char character = char(Serial.read());
    now = millis();

    if (character == '\r' || character == '\n') {
      processSerialCommand(now);
    } else {
      lastSerialByteMs = now;
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = character;
      } else {
        serialOverflow = true;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ROOM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ITEM_BUTTON_PIN, INPUT_PULLUP);

  for (uint8_t room = 0; room < ROOM_COUNT; ++room) {
    pinMode(ALERT_PINS[room], OUTPUT);
    digitalWrite(ALERT_PINS[room], LOW);
    pinMode(LIGHT_PINS[room], INPUT);
    rooms[room].temperature = NAN;
    rooms[room].humidity = NAN;
    dhtSensors[room]->begin();
  }

  display.setBrightness(5, true);
  display.clear();

  Serial.println(F("多房间环境监测启动"));
  Serial.println(F("D2: 切换房间; D3: 切换项目; 自动轮询每3秒"));
  Serial.println(F("显示后缀: C=温度, H=湿度%, L=光照%"));
  Serial.println(F("房间标签: L-1=客厅, b-2=卧室"));
  Serial.print(F("告警条件: 温度 > "));
  Serial.print(HIGH_TEMPERATURE_C, 1);
  Serial.print(F(" C 且光照 > "));
  Serial.print(STRONG_LIGHT_PERCENT);
  Serial.println(F("%"));
  Serial.println(F("串口命令: R=房间, I=项目, P=报表"));

  // Let both DHT11 sensors stabilize before their first measurement.
  showRoomLabel(millis());
  renderDisplay(millis());
  delay(2000);

  sampleLight();
  sampleDht();
  updateAlarms();

  const uint32_t now = millis();
  lastDhtMs = now;
  lastLightMs = now;
  lastPageMs = now;
  lastReportMs = now;
  showRoomLabel(now);

  roomButton.lastRaw = roomButton.stableState =
      digitalRead(ROOM_BUTTON_PIN);
  itemButton.lastRaw = itemButton.stableState =
      digitalRead(ITEM_BUTTON_PIN);
  roomButton.lastChangeMs = itemButton.lastChangeMs = now;

  printReport();
  renderDisplay(now);
}

void loop() {
  uint32_t now = millis();

  if (buttonPressed(roomButton, now)) {
    nextRoom(now);
  }
  if (buttonPressed(itemButton, now)) {
    nextItem(now);
  }

  serviceSerial();
  now = millis();

  if (uint32_t(now - lastLightMs) >= LIGHT_INTERVAL_MS) {
    lastLightMs = now;
    sampleLight();
    updateAlarms();
  }

  if (uint32_t(now - lastDhtMs) >= DHT_INTERVAL_MS) {
    lastDhtMs = now;
    sampleDht();
    updateAlarms();
  }

  now = millis();

  if (uint32_t(now - lastPageMs) >= AUTO_PAGE_INTERVAL_MS) {
    advanceAutomaticPage(now);
  }

  if (uint32_t(now - lastReportMs) >= REPORT_INTERVAL_MS) {
    lastReportMs = now;
    printReport();
  }

  renderDisplay(millis());
}
