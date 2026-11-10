#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Wiring:
// D2: PIR output, HIGH = motion.
// D3: door contact to GND, INPUT_PULLUP; LOW = closed, HIGH = open.
// A0: LDR divider; lower readings mean darker conditions.
// D6: passive buzzer. D13: alarm LED.
// TM1637: CLK = D4, DIO = D5.
const uint8_t PIR_PIN = 2;
const uint8_t DOOR_PIN = 3;
const uint8_t LIGHT_PIN = A0;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;

const uint16_t DARK_THRESHOLD = 350;
const uint16_t DAYLIGHT_THRESHOLD = 450;  // Hysteresis prevents mode chatter.
const unsigned long LIGHT_SAMPLE_MS = 50;
const unsigned long LIGHT_CONFIRM_MS = 1500;
const unsigned long DOOR_DEBOUNCE_MS = 40;
const unsigned long SERIAL_IDLE_MS = 20;
const unsigned long ALARM_PHASE_MS = 250;
const unsigned int ALARM_FREQUENCY_HZ = 2200;

enum SecurityMode {
  DISARMED,
  MANUAL_ARMED,
  NIGHT_ARMED
};

SecurityMode mode = DISARMED;
bool alarmLatched = false;
bool alarmOutputOn = false;
bool previousIntrusion = false;
uint32_t eventCount = 0;

bool doorRawOpen = false;
bool doorOpen = false;
unsigned long doorChangedAt = 0;
bool motionDetected = false;

uint16_t lightLevel = 0;
bool isDark = false;
bool candidateDark = false;
unsigned long lightCandidateSince = 0;
unsigned long lastLightSample = 0;
unsigned long lastAlarmPhase = 0;

char commandBuffer[48];
uint8_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastSerialByte = 0;

// Self-contained TM1637 driver: no external display library is required.
void displayDelay() {
  delayMicroseconds(4);
}

void displayDataLow() {
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
}

void displayDataRelease() {
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
}

void displayStart() {
  displayDataRelease();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  displayDataLow();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void displayStop() {
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  displayDataLow();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  displayDataRelease();
  displayDelay();
}

void displayWriteByte(uint8_t value) {
  for (uint8_t bit = 0; bit < 8; ++bit) {
    digitalWrite(DISPLAY_CLK_PIN, LOW);
    if (value & 0x01) {
      displayDataRelease();
    } else {
      displayDataLow();
    }
    displayDelay();
    digitalWrite(DISPLAY_CLK_PIN, HIGH);
    displayDelay();
    value >>= 1;
  }

  // Release DIO during the TM1637 acknowledgment clock.
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  displayDataRelease();
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  displayDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void updateDisplay() {
  static const uint8_t digitSegments[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
  };

  // The display shows the last four digits; STATUS reports the full count.
  uint16_t number = eventCount % 10000UL;
  uint8_t digits[4];
  for (int8_t i = 3; i >= 0; --i) {
    digits[i] = digitSegments[number % 10];
    number /= 10;
  }

  displayStart();
  displayWriteByte(0x40);  // Automatic address increment.
  displayStop();

  displayStart();
  displayWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    displayWriteByte(digits[i]);
  }
  displayStop();

  displayStart();
  displayWriteByte(0x8B);  // Display enabled, brightness 3/7.
  displayStop();
}

bool isArmed() {
  return mode != DISARMED;
}

void printMode() {
  switch (mode) {
    case MANUAL_ARMED:
      Serial.print(F("ARMED"));
      break;
    case NIGHT_ARMED:
      Serial.print(F("NIGHT_ARMED"));
      break;
    default:
      Serial.print(F("DISARMED"));
      break;
  }
}

void printStatus() {
  Serial.print(F("MODE="));
  printMode();
  Serial.print(F(" ALARM="));
  Serial.print(alarmLatched ? F("ON") : F("OFF"));
  Serial.print(F(" EVENTS="));
  Serial.print(eventCount);
  Serial.print(F(" DOOR="));
  Serial.print(doorOpen ? F("OPEN") : F("CLOSED"));
  Serial.print(F(" PIR="));
  Serial.print(motionDetected ? 1 : 0);
  Serial.print(F(" LIGHT="));
  Serial.print(lightLevel);
  Serial.print(F(" DARK="));
  Serial.println(isDark ? 1 : 0);
}

void clearAlarm() {
  alarmLatched = false;
  alarmOutputOn = false;
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void updateDoor(unsigned long now) {
  bool reading = digitalRead(DOOR_PIN) == HIGH;
  if (reading != doorRawOpen) {
    doorRawOpen = reading;
    doorChangedAt = now;
  }
  if (now - doorChangedAt >= DOOR_DEBOUNCE_MS) {
    doorOpen = doorRawOpen;
  }
}

void updateLight(unsigned long now) {
  if (now - lastLightSample < LIGHT_SAMPLE_MS) {
    return;
  }
  lastLightSample = now;

  uint16_t sample = analogRead(LIGHT_PIN);
  lightLevel = (uint16_t)((3UL * lightLevel + sample + 2) / 4);

  bool requestedDark = isDark;
  if (!isDark && lightLevel < DARK_THRESHOLD) {
    requestedDark = true;
  } else if (isDark && lightLevel > DAYLIGHT_THRESHOLD) {
    requestedDark = false;
  }

  if (requestedDark != candidateDark) {
    candidateDark = requestedDark;
    lightCandidateSince = now;
  }

  if (candidateDark == isDark ||
      now - lightCandidateSince < LIGHT_CONFIRM_MS) {
    return;
  }

  isDark = candidateDark;

  // Explicit DISARM remains effective during the current night.
  // Automatic arming resumes on the next daylight-to-dark transition.
  // Manual ARM remains armed regardless of illumination.
  if (isDark && mode == DISARMED) {
    mode = NIGHT_ARMED;
    previousIntrusion = false;
    Serial.println(F("AUTO: NIGHT_ARMED"));
  } else if (!isDark && mode == NIGHT_ARMED) {
    mode = DISARMED;
    previousIntrusion = false;
    clearAlarm();
    Serial.println(F("AUTO: DAYLIGHT, DISARMED"));
  }
}

void triggerEvent(unsigned long now) {
  if (eventCount != UINT32_MAX) {
    ++eventCount;
  }
  updateDisplay();

  alarmLatched = true;
  alarmOutputOn = true;
  lastAlarmPhase = now;
  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);

  Serial.print(F("EVENT: DOOR_OPEN_AND_MOTION COUNT="));
  Serial.println(eventCount);
}

void updateSecurity(unsigned long now) {
  bool intrusion = isArmed() && doorOpen && motionDetected;

  // Count once per continuous overlapping door-open/motion episode.
  // Further distinct episodes are counted even while the alarm is latched.
  if (intrusion && !previousIntrusion) {
    triggerEvent(now);
  }
  previousIntrusion = intrusion;

  if (!alarmLatched) {
    return;
  }

  if (now - lastAlarmPhase >= ALARM_PHASE_MS) {
    lastAlarmPhase = now;
    alarmOutputOn = !alarmOutputOn;
    digitalWrite(LED_PIN, alarmOutputOn ? HIGH : LOW);
    if (alarmOutputOn) {
      tone(BUZZER_PIN, ALARM_FREQUENCY_HZ);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void executeCommand() {
  if (commandOverflow) {
    Serial.println(F("ERROR: COMMAND TOO LONG"));
  } else if (commandLength > 0) {
    commandBuffer[commandLength] = '\0';

    char *command = commandBuffer;
    while (*command && isspace((unsigned char)*command)) {
      ++command;
    }

    char *end = command + strlen(command);
    while (end > command && isspace((unsigned char)*(end - 1))) {
      --end;
    }
    *end = '\0';

    for (char *p = command; *p; ++p) {
      *p = (char)toupper((unsigned char)*p);
    }

    if (strcmp(command, "ARM") == 0) {
      if (!isArmed()) {
        previousIntrusion = false;
      }
      mode = MANUAL_ARMED;
      Serial.println(F("OK: ARMED"));
    } else if (strcmp(command, "DISARM") == 0) {
      mode = DISARMED;
      previousIntrusion = false;
      clearAlarm();
      Serial.println(F("OK: DISARMED"));
    } else if (strcmp(command, "STATUS") == 0) {
      printStatus();
    } else if (strcmp(command, "RESET") == 0) {
      // Preserve the selected mode; acknowledge the current episode so
      // a continuously active input does not immediately retrigger.
      eventCount = 0;
      clearAlarm();
      previousIntrusion = isArmed() && doorOpen && motionDetected;
      updateDisplay();
      Serial.println(F("OK: EVENTS AND ALARM RESET; MODE UNCHANGED"));
    } else if (*command) {
      Serial.println(F("ERROR: USE ARM, DISARM, STATUS, RESET"));
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void serviceSerial() {
  // Bounded work per loop keeps sensing and alarm timing responsive.
  uint8_t budget = 32;
  while (budget-- && Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByte = millis();

    if (c == '\r' || c == '\n') {
      executeCommand();
    } else if (!commandOverflow) {
      if (commandLength < sizeof(commandBuffer) - 1) {
        commandBuffer[commandLength++] = c;
      } else {
        commandOverflow = true;
      }
    }
  }

  if ((commandLength > 0 || commandOverflow) &&
      Serial.available() == 0 &&
      millis() - lastSerialByte >= SERIAL_IDLE_MS) {
    executeCommand();
  }
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  pinMode(DISPLAY_CLK_PIN, OUTPUT);
  displayDataRelease();

  Serial.begin(9600);

  unsigned long now = millis();
  doorRawOpen = digitalRead(DOOR_PIN) == HIGH;
  doorOpen = doorRawOpen;
  doorChangedAt = now;
  motionDetected = digitalRead(PIR_PIN) == HIGH;

  lightLevel = analogRead(LIGHT_PIN);
  isDark = lightLevel < DARK_THRESHOLD;
  candidateDark = isDark;
  lightCandidateSince = now;
  lastLightSample = now;

  // At power-up, start disarmed in daylight or automatically armed at night.
  mode = isDark ? NIGHT_ARMED : DISARMED;

  updateDisplay();
  Serial.println(F("HOME SECURITY READY: ARM, DISARM, STATUS, RESET"));
  printStatus();
}

void loop() {
  unsigned long now = millis();

  updateDoor(now);
  motionDetected = digitalRead(PIR_PIN) == HIGH;
  updateLight(now);
  serviceSerial();
  updateSecurity(millis());
}