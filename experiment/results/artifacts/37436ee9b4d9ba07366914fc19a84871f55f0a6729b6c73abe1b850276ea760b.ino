#include <Arduino.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Arduino Uno wiring:
// IR proximity sensor OUT -> D2 (active LOW)
// Solenoid valve driver   -> D10 (active HIGH)
// HC-SR04 Trig/Echo       -> D8/D9
// TM1637 CLK/DIO          -> D4/D5
// Buzzer                 -> D6
// Alarm LED              -> D13
//
// Drive the solenoid through a suitable transistor/MOSFET driver with a
// flyback diode. Never connect a solenoid directly to an Arduino output.

const uint8_t IR_PIN = 2;
const uint8_t VALVE_PIN = 10;
const uint8_t TRIG_PIN = 8;
const uint8_t ECHO_PIN = 9;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

const uint8_t IR_PRESENT_LEVEL = LOW;
const uint8_t VALVE_ON_LEVEL = HIGH;
const uint8_t VALVE_OFF_LEVEL = LOW;

const uint32_t DISPENSE_DURATION_MS = 5000UL;
const uint32_t IR_DEBOUNCE_MS = 60UL;
const uint32_t LEVEL_SAMPLE_INTERVAL_MS = 500UL;
const uint32_t DISPLAY_PAGE_INTERVAL_MS = 3000UL;
const uint32_t SERIAL_IDLE_TIMEOUT_MS = 20UL;
const uint32_t ECHO_TIMEOUT_US = 30000UL;

// Calibrate these distances for the actual tank installation.
// Distance is measured downward from the HC-SR04 transducer to the water.
const float FULL_DISTANCE_CM = 5.0f;
const float EMPTY_DISTANCE_CM = 45.0f;
const float MIN_VALID_DISTANCE_CM = 2.0f;
const float MAX_VALID_DISTANCE_CM = 400.0f;

const uint8_t LOW_WATER_PERCENT = 20;
const uint8_t RECOVERY_WATER_PERCENT = 25;
const uint8_t DISPLAY_BRIGHTNESS = 4;

const uint8_t DIGIT_SEGMENTS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

bool valveOpen = false;
uint32_t valveOpenedAt = 0;

bool levelValid = false;
bool lowWaterLockout = true;
float lastDistanceCm = 0.0f;
uint8_t waterPercent = 0;
bool levelSampleTaken = false;
uint32_t lastLevelSampleAt = 0;

bool rawPresence = false;
bool stablePresence = false;
bool approachArmed = true;
uint32_t rawPresenceChangedAt = 0;

uint32_t todayUses = 0;
uint32_t totalUses = 0;
uint32_t deniedToday = 0;
uint32_t totalDenied = 0;
uint64_t totalValveOpenMs = 0;

// Without an RTC, "today" initially means each 24-hour period since boot.
// Send TIME <unix_seconds> to synchronize calendar days. Use a timestamp
// adjusted for the desired local timezone if local midnight is required.
// Statistics are RAM-only and restart after power loss.
bool calendarSynchronized = false;
uint32_t clockSeconds = 0;
uint32_t clockRemainderMs = 0;
uint32_t clockLastMillis = 0;
uint32_t currentDay = 0;

bool countPage = false;
uint32_t displayPageChangedAt = 0;
uint32_t lastDisplayUpdateAt = 0;

bool alarmPreviouslyActive = false;
uint32_t alarmStartedAt = 0;
bool buzzerOn = false;

char serialBuffer[80];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteAt = 0;

// Minimal TM1637 implementation; no external display library is required.
void tmDelay() {
  delayMicroseconds(4);
}

void tmDataLow() {
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
}

void tmDataRelease() {
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
}

void tmStart() {
  tmDataRelease();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();
  tmDataLow();
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void tmStop() {
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  tmDataLow();
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();
  tmDataRelease();
  tmDelay();
}

void tmWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    digitalWrite(DISPLAY_CLK_PIN, LOW);
    if (value & 0x01) {
      tmDataRelease();
    } else {
      tmDataLow();
    }
    tmDelay();
    digitalWrite(DISPLAY_CLK_PIN, HIGH);
    tmDelay();
    value >>= 1;
  }

  digitalWrite(DISPLAY_CLK_PIN, LOW);
  tmDataRelease();
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();  // ACK clock; absence of a display must not block control.
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void tmShow(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40);  // Automatic address increment.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    tmWriteByte(segments[i]);
  }
  tmStop();

  tmStart();
  tmWriteByte(0x88 | (DISPLAY_BRIGHTNESS & 0x07));
  tmStop();
}

void closeValve(uint32_t now) {
  digitalWrite(VALVE_PIN, VALVE_OFF_LEVEL);
  if (valveOpen) {
    totalValveOpenMs += (uint32_t)(now - valveOpenedAt);
    valveOpen = false;
  }
}

void startDispense(uint32_t now) {
  if (valveOpen || !levelValid || lowWaterLockout) {
    return;
  }

  valveOpenedAt = now;
  valveOpen = true;
  digitalWrite(VALVE_PIN, VALVE_ON_LEVEL);
  ++todayUses;
  ++totalUses;
}

void updateClock(uint32_t now) {
  uint32_t elapsed = now - clockLastMillis;
  clockLastMillis = now;

  uint64_t accumulated = (uint64_t)clockRemainderMs + elapsed;
  clockSeconds += (uint32_t)(accumulated / 1000ULL);
  clockRemainderMs = (uint32_t)(accumulated % 1000ULL);

  uint32_t day = clockSeconds / 86400UL;
  if (day != currentDay) {
    currentDay = day;
    todayUses = 0;
    deniedToday = 0;
  }
}

void updateValveTimer(uint32_t now) {
  if (valveOpen &&
      (uint32_t)(now - valveOpenedAt) >= DISPENSE_DURATION_MS) {
    closeValve(now);
  }
}

void sampleLevel(uint32_t now) {
  if (levelSampleTaken &&
      (uint32_t)(now - lastLevelSampleAt) < LEVEL_SAMPLE_INTERVAL_MS) {
    return;
  }

  // Avoid a blocking echo measurement near the scheduled valve shutoff.
  if (valveOpen &&
      (uint32_t)(now - valveOpenedAt) >=
          DISPENSE_DURATION_MS - (ECHO_TIMEOUT_US / 1000UL + 5UL)) {
    return;
  }

  lastLevelSampleAt = now;
  levelSampleTaken = true;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long echoUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  float distance = echoUs * 0.0343f / 2.0f;

  levelValid = echoUs != 0 &&
               distance >= MIN_VALID_DISTANCE_CM &&
               distance <= MAX_VALID_DISTANCE_CM;

  if (!levelValid) {
    lowWaterLockout = true;
    closeValve(millis());
    return;
  }

  lastDistanceCm = distance;
  float percent = 100.0f * (EMPTY_DISTANCE_CM - distance) /
                  (EMPTY_DISTANCE_CM - FULL_DISTANCE_CM);

  if (percent < 0.0f) percent = 0.0f;
  if (percent > 100.0f) percent = 100.0f;
  waterPercent = (uint8_t)(percent + 0.5f);

  if (percent <= LOW_WATER_PERCENT) {
    lowWaterLockout = true;
  } else if (percent >= RECOVERY_WATER_PERCENT) {
    lowWaterLockout = false;
  }

  // Safety overrides the normal fixed dispensing duration.
  if (lowWaterLockout) {
    closeValve(millis());
  }
}

void updatePresence(uint32_t now) {
  bool reading = digitalRead(IR_PIN) == IR_PRESENT_LEVEL;

  if (reading != rawPresence) {
    rawPresence = reading;
    rawPresenceChangedAt = now;
  }

  if (stablePresence != rawPresence &&
      (uint32_t)(now - rawPresenceChangedAt) >= IR_DEBOUNCE_MS) {
    stablePresence = rawPresence;

    if (!stablePresence) {
      approachArmed = true;
    } else if (approachArmed) {
      // One fixed-duration cycle per approach. Remaining near the sensor
      // cannot repeatedly restart or extend dispensing.
      approachArmed = false;

      if (!levelValid || lowWaterLockout) {
        ++deniedToday;
        ++totalDenied;
      } else if (!valveOpen) {
        startDispense(now);
      }
    }
  }
}

void updateAlarm(uint32_t now) {
  bool alarmActive = !levelValid || lowWaterLockout;

  if (alarmActive && !alarmPreviouslyActive) {
    alarmStartedAt = now;
  }
  alarmPreviouslyActive = alarmActive;

  uint32_t phase = now - alarmStartedAt;
  digitalWrite(LED_PIN,
               alarmActive && ((phase / 250UL) % 2UL == 0) ? HIGH : LOW);

  // Two short beeps every two seconds while water is low or sensing fails.
  uint16_t beepPhase = phase % 2000UL;
  bool shouldBeep = alarmActive &&
                   (beepPhase < 150 ||
                    (beepPhase >= 300 && beepPhase < 450));

  if (shouldBeep != buzzerOn) {
    buzzerOn = shouldBeep;
    if (buzzerOn) {
      tone(BUZZER_PIN, levelValid ? 2200 : 1200);
    } else {
      noTone(BUZZER_PIN);
    }
  }
}

void updateDisplay(uint32_t now) {
  if ((uint32_t)(now - displayPageChangedAt) >=
      DISPLAY_PAGE_INTERVAL_MS) {
    displayPageChangedAt = now;
    countPage = !countPage;
  }

  if ((uint32_t)(now - lastDisplayUpdateAt) < 100UL) {
    return;
  }
  lastDisplayUpdateAt = now;

  uint8_t segments[4] = {0, 0, 0, 0};

  if (!countPage) {
    if (!levelValid) {
      for (uint8_t i = 0; i < 4; ++i) segments[i] = 0x40; // "----"
    } else {
      segments[0] = 0x73; // P: remaining water percentage, P 85 / P100.
      segments[1] = waterPercent >= 100 ? DIGIT_SEGMENTS[1] : 0;
      segments[2] = waterPercent >= 10
                      ? DIGIT_SEGMENTS[(waterPercent / 10) % 10] : 0;
      segments[3] = DIGIT_SEGMENTS[waterPercent % 10];
    }
  } else {
    // Four digits show today's valve-opening count. Saturate at 9999;
    // serial statistics retain the complete count.
    uint16_t value = todayUses > 9999UL ? 9999 : (uint16_t)todayUses;
    for (int8_t i = 3; i >= 0; --i) {
      segments[i] = DIGIT_SEGMENTS[value % 10];
      value /= 10;
    }
  }

  tmShow(segments);
}

void printUint64(uint64_t value) {
  char digits[21];
  uint8_t length = 0;
  do {
    digits[length++] = '0' + value % 10;
    value /= 10;
  } while (value);

  while (length) {
    Serial.write(digits[--length]);
  }
}

void printStatistics() {
  Serial.print(F("level="));
  if (levelValid) {
    Serial.print(waterPercent);
    Serial.print(F("% distance_cm="));
    Serial.print(lastDistanceCm, 1);
  } else {
    Serial.print(F("SENSOR_FAULT"));
  }

  Serial.print(F(" lockout="));
  Serial.print(lowWaterLockout ? F("YES") : F("NO"));
  Serial.print(F(" valve="));
  Serial.print(valveOpen ? F("OPEN") : F("CLOSED"));
  Serial.print(F(" today_uses="));
  Serial.print(todayUses);
  Serial.print(F(" total_uses="));
  Serial.print(totalUses);
  Serial.print(F(" denied_today="));
  Serial.print(deniedToday);
  Serial.print(F(" denied_total="));
  Serial.print(totalDenied);

  uint64_t openMs = totalValveOpenMs;
  if (valveOpen) openMs += (uint32_t)(millis() - valveOpenedAt);
  Serial.print(F(" valve_open_ms="));
  printUint64(openMs);

  Serial.print(F(" day_basis="));
  Serial.print(calendarSynchronized ? F("SYNCED_TIMESTAMP") : F("BOOT_24H"));
  Serial.print(F(" day_index="));
  Serial.println(currentDay);
}

void printHelp() {
  Serial.println(F("STATUS / STATS / ? : query statistics"));
  Serial.println(F("TIME <unix_seconds> : synchronize day boundary"));
  Serial.println(F("RESETTODAY : clear today's counts only"));
  Serial.println(F("HELP : show commands"));
  Serial.println(F("Display alternates Pxxx water % and today's use count."));
  Serial.println(F("No RTC: days start at boot until TIME is supplied."));
}

void handleCommand(char *line) {
  while (isspace((unsigned char)*line)) ++line;
  size_t length = strlen(line);
  while (length && isspace((unsigned char)line[length - 1])) {
    line[--length] = '\0';
  }
  if (!length) return;

  for (char *p = line; *p; ++p) {
    *p = toupper((unsigned char)*p);
  }

  if (!strcmp(line, "STATUS") || !strcmp(line, "STATS") ||
      !strcmp(line, "?")) {
    printStatistics();
  } else if (!strcmp(line, "HELP")) {
    printHelp();
  } else if (!strcmp(line, "RESETTODAY")) {
    todayUses = 0;
    deniedToday = 0;
    Serial.println(F("OK today's counters cleared"));
  } else if (!strncmp(line, "TIME ", 5)) {
    char *argument = line + 5;
    while (*argument == ' ') ++argument;

    uint32_t timestamp = 0;
    bool valid = *argument != '\0';
    for (char *p = argument; *p; ++p) {
      if (*p < '0' || *p > '9') {
        valid = false;
        break;
      }
      uint8_t digit = *p - '0';
      if (timestamp > (0xFFFFFFFFUL - digit) / 10UL) {
        valid = false;
        break;
      }
      timestamp = timestamp * 10UL + digit;
    }

    if (!valid) {
      Serial.println(F("ERR use TIME <unsigned 32-bit unix_seconds>"));
      return;
    }

    uint32_t newDay = timestamp / 86400UL;
    // First synchronization retains the current day's accumulated uses.
    if (calendarSynchronized && newDay != currentDay) {
      todayUses = 0;
      deniedToday = 0;
    }
    clockSeconds = timestamp;
    clockRemainderMs = 0;
    clockLastMillis = millis();
    currentDay = newDay;
    calendarSynchronized = true;
    Serial.println(F("OK clock synchronized"));
  } else {
    Serial.println(F("ERR unknown command; send HELP"));
  }
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("ERR command too long"));
  } else if (serialLength) {
    serialBuffer[serialLength] = '\0';
    handleCommand(serialBuffer);
  }
  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  // Bound serial work so continuous incoming data cannot starve control.
  uint8_t budget = 32;
  while (budget-- && Serial.available()) {
    char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      finishSerialFrame();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength || serialOverflow) &&
      !Serial.available() &&
      (uint32_t)(millis() - lastSerialByteAt) >= SERIAL_IDLE_TIMEOUT_MS) {
    finishSerialFrame();
  }
}

void setup() {
  // Establish a safe valve level before making the pin an output.
  digitalWrite(VALVE_PIN, VALVE_OFF_LEVEL);
  pinMode(VALVE_PIN, OUTPUT);

  pinMode(IR_PIN, INPUT_PULLUP);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  pinMode(DISPLAY_CLK_PIN, OUTPUT);
  tmDataRelease();

  Serial.begin(9600);
  clockLastMillis = millis();
  rawPresence = digitalRead(IR_PIN) == IR_PRESENT_LEVEL;
  rawPresenceChangedAt = millis();
  displayPageChangedAt = millis();

  const uint8_t initialDisplay[4] = {0x40, 0x40, 0x40, 0x40};
  tmShow(initialDisplay);

  Serial.println(F("Water-saving controller ready; valve defaults CLOSED."));
  printHelp();
}

void loop() {
  uint32_t now = millis();
  updateClock(now);
  updateValveTimer(now);

  sampleLevel(now);

  now = millis();
  updateClock(now);
  updateValveTimer(now);
  updatePresence(now);
  updateAlarm(now);
  updateDisplay(now);

  // Query replies can take time at 9600 baud, so close the valve first
  // whenever its fixed interval has elapsed.
  updateValveTimer(millis());
  serviceSerial();
  updateValveTimer(millis());
}