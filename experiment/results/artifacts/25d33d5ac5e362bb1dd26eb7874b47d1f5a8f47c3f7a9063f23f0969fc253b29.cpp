#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// 接线：三层传感器 D2/D3/D7，TM1637 CLK=D4、DIO=D5，
// 蜂鸣器 D6，LED D13。串口：9600 baud。
// 默认传感器占用时输出 LOW；若使用高电平有效传感器，修改此常量。
const uint8_t OCCUPIED_LEVEL = LOW;
const uint8_t SENSOR_PINS[3] = {2, 3, 7};
const uint8_t TM_CLK = 4;
const uint8_t TM_DIO = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

const unsigned long DEBOUNCE_MS = 50;
const unsigned long SERIAL_IDLE_MS = 20;
unsigned long emptyTimeoutMs = 60000UL;  // 默认连续空置 60 秒后报警。

struct Shelf {
  bool occupied;
  bool candidateOccupied;
  unsigned long candidateSince;
  unsigned long emptySince;
  bool restockReported;
  unsigned long additions;
};

Shelf shelves[3];
uint8_t occupiedCount = 0;
bool recordsLocked = false;

bool fullBeepActive = false;
uint8_t fullBeepPhase = 0;
unsigned long fullBeepPhaseSince = 0;
const unsigned long BEEP_ON_MS = 180;
const unsigned long BEEP_OFF_MS = 150;

char commandBuffer[80];
uint8_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastSerialByteAt = 0;

// TM1637 驱动，无需外部库。
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStop();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void tmWriteByte(uint8_t value);
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void displayCount(uint8_t count);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
bool readOccupied(uint8_t index);
#line 111 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void startFullBeep(unsigned long now);
#line 118 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void stopFullBeep();
#line 124 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFullBeep(unsigned long now);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFullState(unsigned long now);
#line 172 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void updateSensors(unsigned long now);
#line 212 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void checkRestock(unsigned long now);
#line 224 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus(unsigned long now);
#line 248 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void printHelp();
#line 257 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseLayer(const char *text, uint8_t &index);
#line 265 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseTimeout(const char *text, unsigned long &value);
#line 279 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void handleCommand(char *line, unsigned long now);
#line 340 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void finishCommand();
#line 351 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void pollSerial();
#line 376 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 409 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_013\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart() {
  digitalWrite(TM_DIO, HIGH);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(TM_DIO, LOW);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, LOW);
}

void tmStop() {
  digitalWrite(TM_CLK, LOW);
  digitalWrite(TM_DIO, LOW);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(TM_DIO, HIGH);
  delayMicroseconds(3);
}

void tmWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    digitalWrite(TM_CLK, LOW);
    digitalWrite(TM_DIO, (value & 1) ? HIGH : LOW);
    delayMicroseconds(3);
    digitalWrite(TM_CLK, HIGH);
    delayMicroseconds(3);
    value >>= 1;
  }

  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, INPUT_PULLUP);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, LOW);
  digitalWrite(TM_DIO, LOW);
  pinMode(TM_DIO, OUTPUT);
}

void displayCount(uint8_t count) {
  static const uint8_t digits[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
  };

  tmStart();
  tmWriteByte(0x40);  // 自动地址递增。
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  tmWriteByte(0x00);
  tmWriteByte(0x00);
  tmWriteByte(0x00);
  tmWriteByte(digits[count]);
  tmStop();

  tmStart();
  tmWriteByte(0x8A);  // 开启显示，亮度 2/7。
  tmStop();
}

bool readOccupied(uint8_t index) {
  return digitalRead(SENSOR_PINS[index]) == OCCUPIED_LEVEL;
}

void startFullBeep(unsigned long now) {
  fullBeepActive = true;
  fullBeepPhase = 0;
  fullBeepPhaseSince = now;
  tone(BUZZER_PIN, 2200);
}

void stopFullBeep() {
  fullBeepActive = false;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void updateFullBeep(unsigned long now) {
  if (!fullBeepActive) return;

  unsigned long duration =
      (fullBeepPhase % 2 == 0) ? BEEP_ON_MS : BEEP_OFF_MS;

  if (now - fullBeepPhaseSince < duration) return;

  fullBeepPhaseSince = now;
  ++fullBeepPhase;

  // 三次短鸣；保持满架时不重复触发。
  if (fullBeepPhase >= 5) {
    stopFullBeep();
  } else if (fullBeepPhase % 2 == 0) {
    tone(BUZZER_PIN, 2200);
  } else {
    noTone(BUZZER_PIN);
  }
}

void updateFullState(unsigned long now) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < 3; ++i) {
    if (shelves[i].occupied) ++count;
  }

  if (count != occupiedCount) {
    occupiedCount = count;
    displayCount(occupiedCount);
    Serial.print(F("Occupied layers: "));
    Serial.println(occupiedCount);
  }

  bool full = (occupiedCount == 3);
  digitalWrite(LED_PIN, full ? HIGH : LOW);

  if (full && !recordsLocked) {
    recordsLocked = true;
    Serial.println(F("FULL: new records LOCKED"));
    startFullBeep(now);
  } else if (!full && recordsLocked) {
    recordsLocked = false;
    stopFullBeep();
    Serial.println(F("Space available: new records UNLOCKED"));
  }
}

void updateSensors(unsigned long now) {
  for (uint8_t i = 0; i < 3; ++i) {
    Shelf &s = shelves[i];
    bool reading = readOccupied(i);

    if (reading != s.candidateOccupied) {
      s.candidateOccupied = reading;
      s.candidateSince = now;
    }

    if (s.candidateOccupied == s.occupied ||
        now - s.candidateSince < DEBOUNCE_MS) {
      continue;
    }

    s.occupied = s.candidateOccupied;
    s.restockReported = false;

    Serial.print(F("Layer "));
    Serial.print(i + 1);
    Serial.println(s.occupied ? F(": OCCUPIED") : F(": EMPTY"));

    if (s.occupied) {
      // 将使货架达到满架的入库记录仍可保存；
      // 满架锁定后，传感器监测与空置计时继续运行。
      if (!recordsLocked) {
        ++s.additions;
        Serial.print(F("New record saved: layer "));
        Serial.println(i + 1);
      } else {
        Serial.println(F("New record rejected: FULL"));
      }
    } else {
      s.emptySince = now;
    }

    updateFullState(now);
  }
}

void checkRestock(unsigned long now) {
  for (uint8_t i = 0; i < 3; ++i) {
    Shelf &s = shelves[i];
    if (!s.occupied && !s.restockReported &&
        now - s.emptySince >= emptyTimeoutMs) {
      s.restockReported = true;
      Serial.print(F("restock needed: layer "));
      Serial.println(i + 1);
    }
  }
}

void printStatus(unsigned long now) {
  Serial.print(F("Occupied layers: "));
  Serial.print(occupiedCount);
  Serial.println(F("/3"));

  Serial.print(F("New records: "));
  Serial.println(recordsLocked ? F("LOCKED") : F("UNLOCKED"));

  Serial.print(F("Empty timeout (ms): "));
  Serial.println(emptyTimeoutMs);

  for (uint8_t i = 0; i < 3; ++i) {
    Serial.print(F("Layer "));
    Serial.print(i + 1);
    Serial.print(shelves[i].occupied ? F(": OCCUPIED") : F(": EMPTY"));
    Serial.print(F(", empty_ms="));
    Serial.print(shelves[i].occupied ? 0UL : now - shelves[i].emptySince);
    Serial.print(F(", restock="));
    Serial.print(shelves[i].restockReported ? F("YES") : F("NO"));
    Serial.print(F(", records="));
    Serial.println(shelves[i].additions);
  }
}

void printHelp() {
  Serial.println(F("Commands (case insensitive):"));
  Serial.println(F("STATUS or ?       - query all layers"));
  Serial.println(F("RESET [ALL|1|2|3] - reset empty timers, not records"));
  Serial.println(F("TIMEOUT <ms>      - set timeout 1..2147483647 ms"));
  Serial.println(F("ADD <1|2|3>       - save occupied-layer record unless FULL"));
  Serial.println(F("HELP              - show commands"));
}

bool parseLayer(const char *text, uint8_t &index) {
  if (!text || text[0] < '1' || text[0] > '3' || text[1] != '\0') {
    return false;
  }
  index = text[0] - '1';
  return true;
}

bool parseTimeout(const char *text, unsigned long &value) {
  if (!text || !*text) return false;
  value = 0;

  for (const char *p = text; *p; ++p) {
    if (*p < '0' || *p > '9') return false;
    uint8_t digit = *p - '0';
    if (value > (2147483647UL - digit) / 10UL) return false;
    value = value * 10UL + digit;
  }

  return value > 0;
}

void handleCommand(char *line, unsigned long now) {
  for (char *p = line; *p; ++p) {
    *p = (char)toupper((unsigned char)*p);
  }

  char *cmd = strtok(line, " \t");
  if (!cmd) return;
  char *arg = strtok(NULL, " \t");
  char *extra = strtok(NULL, " \t");

  if (extra) {
    Serial.println(F("ERROR: too many arguments"));
    return;
  }

  if ((!strcmp(cmd, "STATUS") || !strcmp(cmd, "?")) && !arg) {
    printStatus(now);
  } else if (!strcmp(cmd, "HELP") && !arg) {
    printHelp();
  } else if (!strcmp(cmd, "RESET")) {
    uint8_t index = 0;
    bool all = !arg || !strcmp(arg, "ALL");
    if (!all && !parseLayer(arg, index)) {
      Serial.println(F("ERROR: RESET [ALL|1|2|3]"));
      return;
    }

    for (uint8_t i = 0; i < 3; ++i) {
      if (all || i == index) {
        shelves[i].emptySince = now;
        shelves[i].restockReported = false;
      }
    }
    Serial.println(F("Empty timers reset; occupancy and lock unchanged"));
  } else if (!strcmp(cmd, "TIMEOUT")) {
    unsigned long value;
    if (!parseTimeout(arg, value)) {
      Serial.println(F("ERROR: TIMEOUT range is 1..2147483647 ms"));
      return;
    }
    emptyTimeoutMs = value;
    Serial.print(F("Empty timeout (ms): "));
    Serial.println(emptyTimeoutMs);
  } else if (!strcmp(cmd, "ADD")) {
    uint8_t index;
    if (!parseLayer(arg, index)) {
      Serial.println(F("ERROR: ADD <1|2|3>"));
    } else if (recordsLocked) {
      Serial.println(F("New record rejected: FULL"));
    } else if (!shelves[index].occupied) {
      Serial.println(F("New record rejected: layer is EMPTY"));
    } else {
      ++shelves[index].additions;
      Serial.print(F("New record saved: layer "));
      Serial.println(index + 1);
    }
  } else {
    Serial.println(F("ERROR: unknown command or arguments; use HELP"));
  }
}

void finishCommand() {
  if (commandOverflow) {
    Serial.println(F("ERROR: command too long"));
  } else if (commandLength > 0) {
    commandBuffer[commandLength] = '\0';
    handleCommand(commandBuffer, millis());
  }
  commandLength = 0;
  commandOverflow = false;
}

void pollSerial() {
  // 限制每轮处理字节数，持续串口输入也不会阻塞传感器监测。
  uint8_t budget = 64;
  while (budget-- && Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      finishCommand();
    } else if (!commandOverflow) {
      if (commandLength < sizeof(commandBuffer) - 1) {
        commandBuffer[commandLength++] = c;
      } else {
        commandOverflow = true;
      }
    }
  }

  if ((commandLength || commandOverflow) &&
      Serial.available() == 0 &&
      millis() - lastSerialByteAt >= SERIAL_IDLE_MS) {
    finishCommand();
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(TM_CLK, OUTPUT);
  pinMode(TM_DIO, OUTPUT);
  digitalWrite(TM_CLK, HIGH);
  digitalWrite(TM_DIO, HIGH);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  unsigned long now = millis();

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(SENSOR_PINS[i], INPUT_PULLUP);
    bool occupied = readOccupied(i);
    shelves[i].occupied = occupied;
    shelves[i].candidateOccupied = occupied;
    shelves[i].candidateSince = now;
    shelves[i].emptySince = now;
    shelves[i].restockReported = false;
    shelves[i].additions = 0;
  }

  Serial.println(F("Warehouse shelf alarm ready"));
  updateFullState(now);
  displayCount(occupiedCount);
  printHelp();
  printStatus(now);
}

void loop() {
  unsigned long now = millis();
  updateSensors(now);
  updateFullState(now);
  checkRestock(now);
  updateFullBeep(now);
  pollSerial();
}
