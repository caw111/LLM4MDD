#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Wiring:
// PIR OUT -> D2; reed switch -> D3/GND (LOW = door closed).
// TM1637 CLK -> D4, DIO -> D5; passive buzzer -> D6.
// Release button -> D7/GND; auxiliary occupancy output -> D8.
// Green LED -> D12; red LED -> D13 (use suitable series resistors).
//
// Serial, 9600 baud; newline or 20 ms idle terminates a command:
//   TIME 08:30:00          Set the software clock.
//   RESERVE 09:00 10:00    Book a room; overnight bookings are supported.
//   09:00 10:00            Same as RESERVE.
//   RELEASE                Same as pressing D7: clear occupancy and booking.
//   CANCEL                 Cancel booking without clearing occupancy.
//   STATUS
//   HELP
//
// There is no RTC: the clock starts at 00:00:00 after power-up.
// A PIR cannot reliably determine whether a motionless person has left.
// Consequently occupancy is latched until manually released.
// TM1637 cannot render literal text faithfully; it scrolls an approximate
// "OCCUPIEd", while Serial prints the exact word "occupied".

const uint8_t PIR_PIN = 2;
const uint8_t DOOR_PIN = 3;
const uint8_t TM_CLK = 4;
const uint8_t TM_DIO = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t RELEASE_PIN = 7;
const uint8_t AUX_OCCUPIED_PIN = 8;
const uint8_t GREEN_PIN = 12;
const uint8_t RED_PIN = 13;

const uint32_t DAY_SECONDS = 86400UL;
const uint32_t SERIAL_IDLE_MS = 20;
const uint32_t DEBOUNCE_MS = 35;

struct DebouncedInput {
  uint8_t pin;
  bool raw;
  bool stable;
  uint32_t changedAt;

  void begin(uint8_t p, uint32_t now) {
    pin = p;
    pinMode(pin, INPUT_PULLUP);
    raw = stable = digitalRead(pin);
    changedAt = now;
  }

  bool update(uint32_t now) {
    bool value = digitalRead(pin);
    if (value != raw) {
      raw = value;
      changedAt = now;
    }
    if (stable != raw && (uint32_t)(now - changedAt) >= DEBOUNCE_MS) {
      stable = raw;
      return true;
    }
    return false;
  }
};

DebouncedInput door;
DebouncedInput releaseButton;

uint32_t clockSeconds = 0;
uint32_t clockTickMs = 0;
uint32_t reservationStart = 0;
uint32_t reservationEnd = 0;
bool reservationValid = false;
bool occupied = false;
bool overtime = false;
bool suppressPir = false;
bool pirLowTiming = false;
uint32_t pirLowSince = 0;
bool reminderWasActive = false;
bool buzzerOn = false;
uint32_t overtimeStartedMs = 0;
uint32_t overtimeReportMs = 0;

char serialBuffer[96];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteMs = 0;

uint8_t displayCache[4] = {0xFF, 0xFF, 0xFF, 0xFF};
uint8_t scrollPosition = 0;
uint32_t scrollTickMs = 0;
bool displayWasOccupied = false;

const uint8_t DIGITS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

// Four leading and trailing blanks allow a complete scrolling word.
const uint8_t OCCUPIED_TEXT[] = {
  0, 0, 0, 0,
  0x3F, 0x39, 0x39, 0x3E, 0x73, 0x06, 0x79, 0x5E,
  0, 0, 0, 0
};

void tmDelay() {
  delayMicroseconds(5);
}

void tmStart() {
  digitalWrite(TM_DIO, HIGH);
  digitalWrite(TM_CLK, HIGH);
  tmDelay();
  digitalWrite(TM_DIO, LOW);
  tmDelay();
  digitalWrite(TM_CLK, LOW);
}

void tmStop() {
  digitalWrite(TM_CLK, LOW);
  digitalWrite(TM_DIO, LOW);
  tmDelay();
  digitalWrite(TM_CLK, HIGH);
  tmDelay();
  digitalWrite(TM_DIO, HIGH);
  tmDelay();
}

void tmWriteByte(uint8_t value) {
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(TM_CLK, LOW);
    digitalWrite(TM_DIO, (value & 1) ? HIGH : LOW);
    tmDelay();
    digitalWrite(TM_CLK, HIGH);
    tmDelay();
    value >>= 1;
  }

  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, INPUT_PULLUP);
  tmDelay();
  digitalWrite(TM_CLK, HIGH);
  tmDelay();  // ACK clock; display acknowledgement is not required here.
  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, OUTPUT);
  digitalWrite(TM_DIO, LOW);
}

void showSegments(const uint8_t segments[4]) {
  if (memcmp(displayCache, segments, 4) == 0) return;
  memcpy(displayCache, segments, 4);

  tmStart();
  tmWriteByte(0x40);  // Automatic address increment.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) tmWriteByte(segments[i]);
  tmStop();

  tmStart();
  tmWriteByte(0x8A);  // Display on, moderate brightness.
  tmStop();
}

void updateClock(uint32_t now) {
  uint32_t elapsed = now - clockTickMs;
  uint32_t seconds = elapsed / 1000UL;
  if (seconds) {
    clockSeconds += seconds;
    clockTickMs += seconds * 1000UL;
  }
}

void printTwo(uint8_t value) {
  if (value < 10) Serial.print('0');
  Serial.print(value);
}

void printTime(uint32_t seconds) {
  seconds %= DAY_SECONDS;
  printTwo(seconds / 3600UL);
  Serial.print(':');
  printTwo((seconds / 60UL) % 60);
  Serial.print(':');
  printTwo(seconds % 60);
}

void stopOvertime() {
  overtime = false;
  noTone(BUZZER_PIN);
  buzzerOn = false;
}

void releaseRoom() {
  occupied = false;
  reservationValid = false;
  reminderWasActive = false;
  stopOvertime();

  // Do not immediately relatch on the same PIR pulse after release.
  // Once PIR has been LOW for 500 ms, new motion can occupy the room again.
  suppressPir = true;
  pirLowTiming = false;
  Serial.println(F("Room released; reservation cleared."));
}

void markOccupied() {
  if (occupied) return;
  occupied = true;
  Serial.println(F("occupied"));
}

bool parseTime(const char *text, uint32_t &result) {
  if (!text) return false;
  size_t length = strlen(text);
  if (length != 5 && length != 8) return false;
  if (text[2] != ':' || (length == 8 && text[5] != ':')) return false;

  for (size_t i = 0; i < length; ++i) {
    if (i == 2 || i == 5) continue;
    if (!isdigit((unsigned char)text[i])) return false;
  }

  uint8_t hours = (text[0] - '0') * 10 + text[1] - '0';
  uint8_t minutes = (text[3] - '0') * 10 + text[4] - '0';
  uint8_t seconds = length == 8
                      ? (text[6] - '0') * 10 + text[7] - '0'
                      : 0;
  if (hours > 23 || minutes > 59 || seconds > 59) return false;
  result = hours * 3600UL + minutes * 60UL + seconds;
  return true;
}

void printStatus() {
  Serial.print(F("Time "));
  printTime(clockSeconds);
  Serial.print(F("; room "));
  Serial.print(occupied ? F("occupied") : F("free"));
  Serial.print(F("; door "));
  Serial.print(door.stable == LOW ? F("closed") : F("open"));
  if (reservationValid) {
    Serial.print(F("; reservation "));
    printTime(reservationStart);
    Serial.print(F(" -> "));
    printTime(reservationEnd);
    if (reservationStart / DAY_SECONDS != reservationEnd / DAY_SECONDS) {
      Serial.print(F(" (overnight)"));
    }
    if (overtime) Serial.print(F("; overtime use"));
  } else {
    Serial.print(F("; no reservation"));
  }
  Serial.println();
}

void printHelp() {
  Serial.println(F("TIME HH:MM[:SS]"));
  Serial.println(F("RESERVE HH:MM[:SS] HH:MM[:SS]"));
  Serial.println(F("Or enter the two times directly."));
  Serial.println(F("RELEASE | CANCEL | STATUS | HELP"));
  Serial.println(F("TIME changes the clock only when no booking exists."));
}

void processCommand(char *line) {
  for (char *p = line; *p; ++p) {
    *p = toupper((unsigned char)*p);
  }

  char *first = strtok(line, " \t,-");
  if (!first) return;
  char *second = strtok(NULL, " \t,-");
  char *third = strtok(NULL, " \t,-");
  char *extra = strtok(NULL, " \t,-");

  if (!strcmp(first, "HELP") && !second) {
    printHelp();
    return;
  }
  if (!strcmp(first, "STATUS") && !second) {
    printStatus();
    return;
  }
  if (!strcmp(first, "RELEASE") && !second) {
    releaseRoom();
    return;
  }
  if (!strcmp(first, "CANCEL") && !second) {
    reservationValid = false;
    reminderWasActive = false;
    stopOvertime();
    Serial.println(F("Reservation cancelled; occupancy unchanged."));
    return;
  }
  if (!strcmp(first, "TIME")) {
    uint32_t time;
    if (!second || third || !parseTime(second, time)) {
      Serial.println(F("Error: TIME HH:MM[:SS]"));
      return;
    }
    if (reservationValid) {
      Serial.println(F("Error: CANCEL booking before changing TIME."));
      return;
    }
    clockSeconds = time;
    clockTickMs = millis();
    Serial.print(F("Clock set: "));
    printTime(clockSeconds);
    Serial.println();
    return;
  }

  const char *startText;
  const char *endText;
  if (!strcmp(first, "RESERVE") || !strcmp(first, "BOOK")) {
    startText = second;
    endText = third;
    if (extra) {
      Serial.println(F("Error: too many arguments."));
      return;
    }
  } else {
    startText = first;
    endText = second;
    if (third) {
      Serial.println(F("Error: unknown command or extra arguments."));
      return;
    }
  }

  uint32_t startTime, endTime;
  if (!parseTime(startText, startTime) ||
      !parseTime(endText, endTime) || startTime == endTime) {
    Serial.println(F("Error: enter distinct start/end times, e.g. RESERVE 09:00 10:00"));
    return;
  }

  uint32_t day = clockSeconds - clockSeconds % DAY_SECONDS;
  uint32_t timeOfDay = clockSeconds % DAY_SECONDS;
  reservationStart = day + startTime;
  reservationEnd = day + endTime;

  if (endTime < startTime) {
    if (timeOfDay < endTime && day >= DAY_SECONDS) {
      reservationStart -= DAY_SECONDS;  // Currently inside yesterday's booking.
    } else {
      reservationEnd += DAY_SECONDS;
    }
  } else if (clockSeconds >= reservationEnd) {
    // A fully elapsed daytime interval refers to the next day.
    reservationStart += DAY_SECONDS;
    reservationEnd += DAY_SECONDS;
  }

  reservationValid = true;
  reminderWasActive = false;
  stopOvertime();
  Serial.println(F("Reservation accepted."));
  printStatus();
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("Error: command too long."));
  } else if (serialLength) {
    serialBuffer[serialLength] = '\0';
    processCommand(serialBuffer);
  }
  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial(uint32_t now) {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteMs = millis();
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
      (uint32_t)(now - lastSerialByteMs) < 0x80000000UL &&
      (uint32_t)(now - lastSerialByteMs) >= SERIAL_IDLE_MS) {
    finishSerialFrame();
  }
}

void updateDisplay(uint32_t now) {
  uint8_t segments[4];

  if (occupied) {
    if (!displayWasOccupied) {
      scrollPosition = 0;
      scrollTickMs = now;
    } else if ((uint32_t)(now - scrollTickMs) >= 350) {
      scrollTickMs = now;
      scrollPosition = (scrollPosition + 1) %
                       (sizeof(OCCUPIED_TEXT) - 3);
    }
    for (uint8_t i = 0; i < 4; ++i) {
      segments[i] = OCCUPIED_TEXT[scrollPosition + i];
    }
  } else {
    // Show upcoming booking start; otherwise show the current clock.
    uint32_t shown = reservationValid && clockSeconds < reservationStart
                       ? reservationStart : clockSeconds;
    uint16_t minutes = (shown % DAY_SECONDS) / 60UL;
    uint8_t hours = minutes / 60;
    uint8_t mins = minutes % 60;
    segments[0] = DIGITS[hours / 10];
    segments[1] = DIGITS[hours % 10] | 0x80;  // Center colon.
    segments[2] = DIGITS[mins / 10];
    segments[3] = DIGITS[mins % 10];
  }

  displayWasOccupied = occupied;
  showSegments(segments);
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(AUX_OCCUPIED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(AUX_OCCUPIED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(TM_CLK, OUTPUT);
  pinMode(TM_DIO, OUTPUT);
  digitalWrite(TM_CLK, HIGH);
  digitalWrite(TM_DIO, HIGH);

  uint32_t now = millis();
  clockTickMs = now;
  door.begin(DOOR_PIN, now);
  releaseButton.begin(RELEASE_PIN, now);

  Serial.begin(9600);
  Serial.println(F("Meeting room system ready. Software clock starts at 00:00:00."));
  Serial.println(F("Set TIME, then RESERVE. D7 manually releases the room."));
  printHelp();
  updateDisplay(now);
}

void loop() {
  uint32_t now = millis();
  updateClock(now);
  serviceSerial(now);
  now = millis();
  updateClock(now);

  bool doorChanged = door.update(now);
  if (doorChanged) {
    Serial.println(door.stable == LOW ? F("Door closed") : F("Door open"));
  }

  if (releaseButton.update(now) && releaseButton.stable == LOW) {
    releaseRoom();
  }

  bool motion = digitalRead(PIR_PIN) == HIGH;
  if (suppressPir) {
    if (motion) {
      pirLowTiming = false;
    } else if (!pirLowTiming) {
      pirLowTiming = true;
      pirLowSince = now;
    } else if ((uint32_t)(now - pirLowSince) >= 500) {
      suppressPir = false;
      pirLowTiming = false;
    }
  }

  // Motion proves presence, even if the door was already open or closed.
  // Door transitions alone cannot establish entry versus departure.
  // Holding the release button keeps the room released.
  if (!suppressPir && releaseButton.stable == HIGH && motion) {
    markOccupied();
  }

  bool reminder = reservationValid &&
                  clockSeconds < reservationStart &&
                  reservationStart - clockSeconds <= 60UL;

  if (reminder && !reminderWasActive) {
    Serial.println(F("Reservation starts within one minute."));
  }
  reminderWasActive = reminder;

  if (reservationValid && clockSeconds >= reservationEnd) {
    if (occupied) {
      if (!overtime) {
        overtime = true;
        overtimeStartedMs = now;
        overtimeReportMs = now;
        Serial.println(F("overtime use"));
      } else if ((uint32_t)(now - overtimeReportMs) >= 30000UL) {
        overtimeReportMs = now;
        Serial.println(F("overtime use"));
      }
    } else {
      reservationValid = false;
      stopOvertime();
      Serial.println(F("Reservation ended; room free."));
    }
  }

  bool shouldBuzz = overtime &&
                    ((uint32_t)(now - overtimeStartedMs) % 2000UL < 250UL);
  if (shouldBuzz != buzzerOn) {
    buzzerOn = shouldBuzz;
    if (buzzerOn) tone(BUZZER_PIN, 2200);
    else noTone(BUZZER_PIN);
  }

  // The reminder flashes green even if occupied; red always shows occupancy.
  digitalWrite(GREEN_PIN, reminder ? ((now / 500UL) % 2 == 0)
                                  : !occupied);
  digitalWrite(RED_PIN, occupied ? HIGH : LOW);
  digitalWrite(AUX_OCCUPIED_PIN, occupied ? HIGH : LOW);

  updateDisplay(now);
}