#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <TM1637Display.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Wiring:
// D2: photoelectric sensor, active LOW (one FALLING edge per product).
// D3: manual-stop button to GND; hold to stop, release to resume.
// D7: display-selection button to GND.
// D6: passive buzzer; D13: alarm LED.
// TM1637: CLK=D4, DIO=D5. All devices must share GND.
//
// Display modes, selected with D7:
// TARGET -> ACTUAL -> STOPS -> TARGET ...
// ACTUAL alternates cumulative count ("Cnt") and achievement percent ("Pct").
// Values above the display's range alternate between 9999 and "OFLO".
// Achievement = total products / elapsed planned production * 100%.
// Planned production includes downtime; target changes apply prospectively.
//
// Serial, 9600 baud, newline OR 20 ms idle terminated:
//   TARGET 60   Set target to 1..9999 products/minute.
//   STATUS      Report complete status.
//   HELP        List commands.
// Product counts remain active during manual stops. An alarm clears only when
// the manual button is released AND the no-product timeout is no longer active.

constexpr uint8_t SENSOR_PIN = 2;
constexpr uint8_t MANUAL_PIN = 3;
constexpr uint8_t CLK_PIN = 4;
constexpr uint8_t DIO_PIN = 5;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t SELECT_PIN = 7;
constexpr uint8_t LED_PIN = 13;

constexpr uint32_t NO_PRODUCT_MS = 30000UL;
constexpr uint32_t DEBOUNCE_MS = 30UL;
constexpr uint32_t SERIAL_IDLE_MS = 20UL;
constexpr uint32_t REPORT_MS = 1000UL;
constexpr uint32_t DISPLAY_MS = 100UL;

TM1637Display display(CLK_PIN, DIO_PIN);

volatile uint32_t sensorPulseCount = 0;
volatile uint32_t sensorLastMs = 0;

uint32_t previousPulseCount = 0;
uint32_t previousLoopMs = 0;
uint32_t lastReportMs = 0;
uint32_t lastDisplayMs = 0;
uint32_t displayModeStartMs = 0;
uint32_t alarmStartMs = 0;

uint64_t totalProducts = 0;
uint64_t uptimeMs = 0;
uint64_t noProductMs = 0;
uint64_t downtimeMs = 0;
uint64_t stopStartedUptimeMs = 0;
uint64_t stopCount = 0;

uint16_t targetPerMinute = 60;
double plannedProducts = 0.0;

enum DisplayMode : uint8_t {
  SHOW_TARGET,
  SHOW_ACTUAL,
  SHOW_STOPS
};
DisplayMode displayMode = SHOW_ACTUAL;

enum AlarmReason : uint8_t {
  REASON_NONE = 0,
  REASON_IDLE = 1,
  REASON_MANUAL = 2
};
uint8_t alarmReasons = REASON_NONE;
bool buzzerOn = false;

struct DebouncedButton {
  uint8_t pin;
  bool candidate;
  bool stable;
  uint32_t changedMs;

  void begin(uint8_t inputPin, uint32_t now) {
    pin = inputPin;
    pinMode(pin, INPUT_PULLUP);
    stable = HIGH;
    candidate = digitalRead(pin);
    changedMs = now;
  }

  bool update(uint32_t now) {
    bool raw = digitalRead(pin);
    if (raw != candidate) {
      candidate = raw;
      changedMs = now;
    }
    if (stable != candidate &&
        (uint32_t)(now - changedMs) >= DEBOUNCE_MS) {
      stable = candidate;
      return true;
    }
    return false;
  }

  bool pressed() const {
    return stable == LOW;
  }
};

DebouncedButton manualButton;
DebouncedButton selectButton;

char serialBuffer[64];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByteMs = 0;

#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void productISR();
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void printUInt64(uint64_t value);
#line 136 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
double achievementPercent();
#line 143 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void printReasons(uint8_t reasons);
#line 159 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus();
#line 181 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void printHelp();
#line 187 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void processCommand(char *command);
#line 242 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void finishSerialFrame();
#line 253 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceSerial();
#line 282 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarm(uint32_t now);
#line 331 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarmOutputs(uint32_t now);
#line 346 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void showLabel(DisplayMode mode, bool percent);
#line 381 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void showValue(uint64_t value, uint32_t now);
#line 395 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint32_t now);
#line 418 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 446 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 120 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_030\\direct\\attempt_0\\sketch\\sketch.ino"
void productISR() {
  ++sensorPulseCount;
  sensorLastMs = millis();
}

void printUInt64(uint64_t value) {
  char digits[21];
  uint8_t index = sizeof(digits) - 1;
  digits[index] = '\0';
  do {
    digits[--index] = char('0' + value % 10);
    value /= 10;
  } while (value);
  Serial.print(&digits[index]);
}

double achievementPercent() {
  if (plannedProducts <= 0.0) {
    return 0.0;
  }
  return ((double)totalProducts / plannedProducts) * 100.0;
}

void printReasons(uint8_t reasons) {
  if (!reasons) {
    Serial.print(F("NONE"));
    return;
  }
  if (reasons & REASON_IDLE) {
    Serial.print(F("NO_PRODUCT_30S"));
  }
  if (reasons & REASON_MANUAL) {
    if (reasons & REASON_IDLE) {
      Serial.print('+');
    }
    Serial.print(F("MANUAL"));
  }
}

void printStatus() {
  Serial.print(F("STATUS uptime_ms="));
  printUInt64(uptimeMs);
  Serial.print(F(" target_per_min="));
  Serial.print(targetPerMinute);
  Serial.print(F(" total="));
  printUInt64(totalProducts);
  Serial.print(F(" planned="));
  Serial.print(plannedProducts, 2);
  Serial.print(F(" achievement_pct="));
  Serial.print(achievementPercent(), 1);
  Serial.print(F(" stops="));
  printUInt64(stopCount);
  Serial.print(F(" downtime_ms="));
  printUInt64(downtimeMs);
  Serial.print(F(" no_product_ms="));
  printUInt64(noProductMs);
  Serial.print(F(" alarm="));
  printReasons(alarmReasons);
  Serial.println();
}

void printHelp() {
  Serial.println(F("Commands: TARGET <1..9999>, STATUS, HELP"));
  Serial.println(F("D3: hold=manual stop; release=resume if idle alarm cleared."));
  Serial.println(F("D7: TARGET / ACTUAL(count and percent) / STOPS."));
}

void processCommand(char *command) {
  while (isspace((unsigned char)*command)) {
    ++command;
  }
  char *end = command + strlen(command);
  while (end > command && isspace((unsigned char)end[-1])) {
    *--end = '\0';
  }
  if (!*command) {
    return;
  }

  for (char *p = command; *p; ++p) {
    *p = toupper((unsigned char)*p);
  }

  if (strcmp(command, "STATUS") == 0) {
    printStatus();
  } else if (strcmp(command, "HELP") == 0) {
    printHelp();
  } else if (strncmp(command, "TARGET", 6) == 0 &&
             isspace((unsigned char)command[6])) {
    char *argument = command + 6;
    while (isspace((unsigned char)*argument)) {
      ++argument;
    }

    // Parse with an explicit range check, preventing integer overflow.
    uint32_t value = 0;
    bool valid = (*argument >= '0' && *argument <= '9');
    while (*argument >= '0' && *argument <= '9') {
      value = value * 10UL + (uint8_t)(*argument - '0');
      if (value > 9999UL) {
        valid = false;
        break;
      }
      ++argument;
    }
    while (isspace((unsigned char)*argument)) {
      ++argument;
    }

    if (!valid || *argument != '\0' || value == 0 || value > 9999UL) {
      Serial.println(F("ERROR TARGET requires an integer from 1 to 9999"));
      return;
    }

    targetPerMinute = (uint16_t)value;
    Serial.print(F("TARGET_UPDATED target_per_min="));
    Serial.println(targetPerMinute);
  } else {
    Serial.println(F("ERROR unknown command; send HELP"));
  }
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("ERROR command too long"));
  } else if (serialLength) {
    serialBuffer[serialLength] = '\0';
    processCommand(serialBuffer);
  }
  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  // Limit work per loop so continuous serial traffic cannot starve monitoring.
  uint8_t budget = 32;
  while (budget-- && Serial.available() > 0) {
    uint32_t now = millis();
    if ((serialLength || serialOverflow) &&
        (uint32_t)(now - lastSerialByteMs) >= SERIAL_IDLE_MS) {
      finishSerialFrame();
    }

    char c = (char)Serial.read();
    lastSerialByteMs = now;
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

  if (Serial.available() == 0 && (serialLength || serialOverflow) &&
      (uint32_t)(millis() - lastSerialByteMs) >= SERIAL_IDLE_MS) {
    finishSerialFrame();
  }
}

void updateAlarm(uint32_t now) {
  uint8_t newReasons = REASON_NONE;
  if (noProductMs >= NO_PRODUCT_MS) {
    newReasons |= REASON_IDLE;
  }
  if (manualButton.pressed()) {
    newReasons |= REASON_MANUAL;
  }

  if (newReasons == alarmReasons) {
    return;
  }

  if (alarmReasons == REASON_NONE && newReasons != REASON_NONE) {
    ++stopCount;
    stopStartedUptimeMs = uptimeMs;
    alarmStartMs = now;

    Serial.print(F("STOP_START event="));
    printUInt64(stopCount);
    Serial.print(F(" uptime_ms="));
    printUInt64(uptimeMs);
    Serial.print(F(" total="));
    printUInt64(totalProducts);
    Serial.print(F(" reason="));
    printReasons(newReasons);
    Serial.println();
  } else if (newReasons == REASON_NONE) {
    Serial.print(F("STOP_END event="));
    printUInt64(stopCount);
    Serial.print(F(" uptime_ms="));
    printUInt64(uptimeMs);
    Serial.print(F(" duration_ms="));
    printUInt64(uptimeMs - stopStartedUptimeMs);
    Serial.println();
  } else {
    // Overlapping reasons belong to one continuous downtime episode.
    Serial.print(F("STOP_REASON_CHANGE event="));
    printUInt64(stopCount);
    Serial.print(F(" uptime_ms="));
    printUInt64(uptimeMs);
    Serial.print(F(" reason="));
    printReasons(newReasons);
    Serial.println();
  }

  alarmReasons = newReasons;
}

void updateAlarmOutputs(uint32_t now) {
  bool on = alarmReasons != REASON_NONE &&
            ((uint32_t)(now - alarmStartMs) % 500UL < 250UL);

  digitalWrite(LED_PIN, on ? HIGH : LOW);
  if (on != buzzerOn) {
    buzzerOn = on;
    if (on) {
      tone(BUZZER_PIN, 2000);
    } else {
      noTone(BUZZER_PIN);
    }
  }
}

void showLabel(DisplayMode mode, bool percent) {
  const uint8_t targetLabel[] = {
    SEG_D | SEG_E | SEG_F | SEG_G,               // t
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G, // A
    SEG_E | SEG_G,                             // r
    0
  };
  const uint8_t countLabel[] = {
    SEG_A | SEG_D | SEG_E | SEG_F,              // C
    SEG_C | SEG_E | SEG_G,                     // n
    SEG_D | SEG_E | SEG_F | SEG_G,              // t
    0
  };
  const uint8_t percentLabel[] = {
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G,       // P
    SEG_D | SEG_E | SEG_G,                     // c
    SEG_D | SEG_E | SEG_F | SEG_G,              // t
    0
  };
  const uint8_t stopsLabel[] = {
    SEG_A | SEG_C | SEG_D | SEG_F | SEG_G,       // S
    SEG_D | SEG_E | SEG_F | SEG_G,              // t
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
    SEG_A | SEG_B | SEG_E | SEG_F | SEG_G        // P
  };

  if (mode == SHOW_TARGET) {
    display.setSegments(targetLabel);
  } else if (mode == SHOW_STOPS) {
    display.setSegments(stopsLabel);
  } else {
    display.setSegments(percent ? percentLabel : countLabel);
  }
}

void showValue(uint64_t value, uint32_t now) {
  if (value > 9999 && ((now / 1000UL) & 1UL)) {
    const uint8_t overflowLabel[] = {
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F, // O
      SEG_A | SEG_E | SEG_F | SEG_G,                // F
      SEG_D | SEG_E | SEG_F,                        // L
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F  // O
    };
    display.setSegments(overflowLabel);
  } else {
    display.showNumberDec(value > 9999 ? 9999 : (int)value, false);
  }
}

void updateDisplay(uint32_t now) {
  uint32_t elapsed = now - displayModeStartMs;
  bool percent = displayMode == SHOW_ACTUAL &&
                 ((elapsed / 2000UL) & 1UL);

  if (elapsed % 2000UL < 250UL) {
    showLabel(displayMode, percent);
    return;
  }

  if (displayMode == SHOW_TARGET) {
    showValue(targetPerMinute, now);
  } else if (displayMode == SHOW_STOPS) {
    showValue(stopCount, now);
  } else if (!percent) {
    showValue(totalProducts, now);
  } else {
    double rate = achievementPercent();
    // Whole-percent display; serial retains one decimal place.
    showValue(rate >= 9999.5 ? 10000UL : (uint64_t)(rate + 0.5), now);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  display.setBrightness(5);
  display.clear();

  uint32_t now = millis();
  manualButton.begin(MANUAL_PIN, now);
  selectButton.begin(SELECT_PIN, now);
  previousLoopMs = now;
  displayModeStartMs = now;
  lastDisplayMs = now;
  lastReportMs = now;
  sensorLastMs = now;

  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), productISR, FALLING);

  Serial.println(F("Production pace monitor ready; default target=60/min."));
  printHelp();
  printStatus();
  updateDisplay(now);
}

void loop() {
  uint32_t pulseSnapshot;
  uint32_t lastProductSnapshot;
  uint32_t now;

  noInterrupts();
  pulseSnapshot = sensorPulseCount;
  lastProductSnapshot = sensorLastMs;
  now = millis();
  interrupts();

  uint32_t elapsed = now - previousLoopMs;
  previousLoopMs = now;
  uptimeMs += elapsed;
  plannedProducts += ((double)targetPerMinute * elapsed) / 60000.0;

  if (alarmReasons != REASON_NONE) {
    downtimeMs += elapsed;
  }

  uint32_t newProducts = pulseSnapshot - previousPulseCount;
  previousPulseCount = pulseSnapshot;
  totalProducts += newProducts;

  if (newProducts) {
    noProductMs = (uint32_t)(now - lastProductSnapshot);
  } else {
    noProductMs += elapsed;
  }

  manualButton.update(now);
  if (selectButton.update(now) && selectButton.pressed()) {
    displayMode = (DisplayMode)((displayMode + 1) % 3);
    displayModeStartMs = now;
    lastDisplayMs = now;
    updateDisplay(now);

    Serial.print(F("DISPLAY "));
    if (displayMode == SHOW_TARGET) {
      Serial.println(F("TARGET"));
    } else if (displayMode == SHOW_ACTUAL) {
      Serial.println(F("ACTUAL_COUNT_AND_PERCENT"));
    } else {
      Serial.println(F("STOP_COUNT"));
    }
  }

  updateAlarm(now);
  updateAlarmOutputs(now);

  if ((uint32_t)(now - lastDisplayMs) >= DISPLAY_MS) {
    lastDisplayMs = now;
    updateDisplay(now);
  }

  if ((uint32_t)(now - lastReportMs) >= REPORT_MS) {
    lastReportMs = now;
    printStatus();
  }

  serviceSerial();
}
