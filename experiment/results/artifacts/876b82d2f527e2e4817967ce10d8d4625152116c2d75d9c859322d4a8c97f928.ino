#include <Arduino.h>
#include <TM1637Display.h>
#include <string.h>

// Buttons connect their assigned pins to GND when pressed.
const uint8_t BED_PINS[3] = {2, 3, 4};
const uint8_t ACK_PIN = 7;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;
const uint8_t DISPLAY_CLK_PIN = 8;
const uint8_t DISPLAY_DIO_PIN = 5;

const unsigned long DEBOUNCE_MS = 30;
const unsigned long DISPLAY_UPDATE_MS = 100;
const unsigned long ALARM_PERIOD_MS = 1000;
const unsigned long ALARM_ON_MS = 200;
const unsigned long LED_HALF_PERIOD_MS = 250;
const unsigned long SERIAL_IDLE_MS = 20;

TM1637Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

struct DebouncedButton {
  uint8_t pin;
  uint8_t rawState;
  uint8_t stableState;
  unsigned long changedAt;

  void begin(uint8_t assignedPin, unsigned long now) {
    pin = assignedPin;
    pinMode(pin, INPUT_PULLUP);
    rawState = HIGH;
    stableState = HIGH;
    changedAt = now;
  }

  bool pressed(unsigned long now) {
    const uint8_t reading = digitalRead(pin);

    if (reading != rawState) {
      rawState = reading;
      changedAt = now;
    }

    if (rawState != stableState &&
        (unsigned long)(now - changedAt) >= DEBOUNCE_MS) {
      stableState = rawState;
      return stableState == LOW;
    }

    return false;
  }
};

struct CallRecord {
  uint8_t bed;
  unsigned long sequence;
  unsigned long requestedAt;
};

DebouncedButton bedButtons[3];
DebouncedButton ackButton;

// One outstanding call per bed; repeated presses never reset its waiting time.
// FIFO order gives highest priority to the longest-waiting bed.
// Simultaneous debounced presses are ordered by bed number.
CallRecord calls[3];
uint8_t callCount = 0;
unsigned long nextSequence = 1;

unsigned long alarmStartedAt = 0;
unsigned long lastDisplayAt = 0;
bool displayDirty = true;
bool buzzerOn = false;

char serialBuffer[48];
uint8_t serialLength = 0;
bool serialOverflow = false;
unsigned long lastSerialByteAt = 0;

void printWait(unsigned long elapsed) {
  Serial.print(elapsed / 1000UL);
  Serial.print('.');
  const unsigned int fraction = elapsed % 1000UL;
  if (fraction < 100) Serial.print('0');
  if (fraction < 10) Serial.print('0');
  Serial.print(fraction);
  Serial.print(F(" s"));
}

void printStatus(unsigned long now) {
  Serial.print(F("STATUS pending="));
  Serial.println(callCount);

  for (uint8_t i = 0; i < callCount; ++i) {
    Serial.print(F("  priority="));
    Serial.print(i + 1);
    Serial.print(F(" bed="));
    Serial.print(calls[i].bed);
    Serial.print(F(" order="));
    Serial.print(calls[i].sequence);
    Serial.print(F(" wait="));
    printWait((unsigned long)(now - calls[i].requestedAt));
    Serial.println();
  }
}

void addCall(uint8_t bed, unsigned long now) {
  for (uint8_t i = 0; i < callCount; ++i) {
    if (calls[i].bed == bed) {
      Serial.print(F("REPEAT bed="));
      Serial.print(bed);
      Serial.print(F(" order="));
      Serial.print(calls[i].sequence);
      Serial.print(F(" wait="));
      printWait((unsigned long)(now - calls[i].requestedAt));
      Serial.println(F(" (original priority retained)"));
      return;
    }
  }

  if (callCount >= 3) return;

  const bool wasEmpty = (callCount == 0);
  CallRecord &record = calls[callCount++];
  record.bed = bed;
  record.sequence = nextSequence++;
  record.requestedAt = now;

  if (wasEmpty) alarmStartedAt = now;
  displayDirty = true;

  Serial.print(F("CALL order="));
  Serial.print(record.sequence);
  Serial.print(F(" bed="));
  Serial.print(record.bed);
  Serial.print(F(" at_ms="));
  Serial.print(record.requestedAt);
  Serial.print(F(" pending="));
  Serial.println(callCount);
}

void acknowledgeCall(unsigned long now) {
  if (callCount == 0) {
    Serial.println(F("ACK ignored: no pending calls"));
    return;
  }

  const CallRecord handled = calls[0];

  for (uint8_t i = 1; i < callCount; ++i) {
    calls[i - 1] = calls[i];
  }
  --callCount;
  displayDirty = true;

  Serial.print(F("HANDLED order="));
  Serial.print(handled.sequence);
  Serial.print(F(" bed="));
  Serial.print(handled.bed);
  Serial.print(F(" called_at_ms="));
  Serial.print(handled.requestedAt);
  Serial.print(F(" acknowledged_at_ms="));
  Serial.print(now);
  Serial.print(F(" wait="));
  printWait((unsigned long)(now - handled.requestedAt));
  Serial.print(F(" remaining="));
  Serial.println(callCount);

  if (callCount > 0) {
    Serial.print(F("NEXT bed="));
    Serial.print(calls[0].bed);
    Serial.print(F(" order="));
    Serial.print(calls[0].sequence);
    Serial.print(F(" wait="));
    printWait((unsigned long)(now - calls[0].requestedAt));
    Serial.println();
  }
}

void updateAlarm(unsigned long now) {
  bool shouldSound = false;
  bool shouldLight = false;

  if (callCount > 0) {
    const unsigned long elapsed = now - alarmStartedAt;
    shouldSound = (elapsed % ALARM_PERIOD_MS) < ALARM_ON_MS;
    shouldLight = ((elapsed / LED_HALF_PERIOD_MS) & 1UL) == 0;
  }

  digitalWrite(LED_PIN, shouldLight ? HIGH : LOW);

  if (shouldSound != buzzerOn) {
    buzzerOn = shouldSound;
    if (buzzerOn) {
      tone(BUZZER_PIN, 2200);
    } else {
      noTone(BUZZER_PIN);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void updateDisplay(unsigned long now) {
  if (!displayDirty &&
      (unsigned long)(now - lastDisplayAt) < DISPLAY_UPDATE_MS) {
    return;
  }

  lastDisplayAt = now;
  displayDirty = false;
  uint8_t segments[4];

  if (callCount == 0) {
    for (uint8_t i = 0; i < 4; ++i) {
      segments[i] = SEG_G;
    }
  } else {
    // Format: BSSS, where B is the bed and SSS is waiting seconds.
    // Display saturates at 999 seconds; serial logs retain full waiting time.
    unsigned long seconds =
        (unsigned long)(now - calls[0].requestedAt) / 1000UL;
    if (seconds > 999UL) seconds = 999UL;

    segments[0] = display.encodeDigit(calls[0].bed);
    segments[1] = display.encodeDigit(seconds / 100UL);
    segments[2] = display.encodeDigit((seconds / 10UL) % 10UL);
    segments[3] = display.encodeDigit(seconds % 10UL);
  }

  display.setSegments(segments);
}

void processSerialFrame(unsigned long now) {
  if (serialOverflow) {
    Serial.println(F("ERROR: serial command too long"));
  } else if (serialLength > 0) {
    serialBuffer[serialLength] = '\0';

    char *command = serialBuffer;
    while (*command == ' ' || *command == '\t') ++command;

    char *end = command + strlen(command);
    while (end > command && (end[-1] == ' ' || end[-1] == '\t')) {
      *--end = '\0';
    }

    for (char *p = command; *p; ++p) {
      if (*p >= 'a' && *p <= 'z') *p -= ('a' - 'A');
    }

    if (strcmp(command, "STATUS") == 0) {
      printStatus(now);
    } else if (strcmp(command, "HELP") == 0) {
      Serial.println(F("Commands: STATUS, HELP. Nurse acknowledgment uses D7."));
    } else if (*command != '\0') {
      Serial.println(F("ERROR: unknown command; use STATUS or HELP"));
    }
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial(unsigned long now) {
  // Accept CR/LF-terminated frames or a frame terminated by 20 ms idle.
  // Bound work per loop so serial traffic cannot starve button scanning.
  uint8_t budget = 32;

  while (budget-- > 0 && Serial.available() > 0) {
    const char c = (char)Serial.read();
    lastSerialByteAt = millis();

    if (c == '\r' || c == '\n') {
      processSerialFrame(lastSerialByteAt);
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  now = millis();
  if ((serialLength > 0 || serialOverflow) &&
      Serial.available() == 0 &&
      (unsigned long)(now - lastSerialByteAt) >= SERIAL_IDLE_MS) {
    processSerialFrame(now);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  noTone(BUZZER_PIN);

  Serial.begin(9600);

  const unsigned long now = millis();
  for (uint8_t i = 0; i < 3; ++i) {
    bedButtons[i].begin(BED_PINS[i], now);
  }
  ackButton.begin(ACK_PIN, now);

  display.setBrightness(5, true);
  updateDisplay(now);

  Serial.println(F("Ward call system ready. Buttons are active LOW."));
  Serial.println(F("Priority: oldest call first; one pending call per bed."));
  Serial.println(F("Display: bed + 3-digit seconds (maximum 999)."));
  Serial.println(F("Serial: 9600 baud; CR/LF or 20 ms idle; STATUS / HELP."));
}

void loop() {
  unsigned long now = millis();

  // Sample all buttons before logging, so simultaneous events share a timestamp.
  bool bedPressed[3];
  for (uint8_t i = 0; i < 3; ++i) {
    bedPressed[i] = bedButtons[i].pressed(now);
  }
  const bool acknowledged = ackButton.pressed(now);

  for (uint8_t i = 0; i < 3; ++i) {
    if (bedPressed[i]) addCall(i + 1, now);
  }
  if (acknowledged) acknowledgeCall(now);

  now = millis();
  updateAlarm(now);
  updateDisplay(now);
  serviceSerial(now);
}