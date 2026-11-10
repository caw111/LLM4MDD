#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Arduino Uno wiring:
// LM35: OUT=A1, VCC=5V, GND=GND.
// Vibration sensor: signal=D2, active LOW.
// Door contact: D3 to GND when closed; open contact means door open.
// TM1637: CLK=D4, DIO=D5.
// Passive buzzer: D6. Status/alarm LED: D13.
//
// Serial GPS simulation at 9600 baud:
//   GPS,31.230400,121.473700
// or:
//   31.230400,121.473700
// Frames end at CR/LF or after 20 ms without another byte.
// START / STOP control transport monitoring; transport starts enabled.
// Temperature events count separate excursions above 8 C.

const uint8_t TEMP_PIN = A1;
const uint8_t VIBRATION_PIN = 2;
const uint8_t DOOR_PIN = 3;
const uint8_t DISPLAY_CLK_PIN = 4;
const uint8_t DISPLAY_DIO_PIN = 5;
const uint8_t BUZZER_PIN = 6;
const uint8_t LED_PIN = 13;

const uint8_t VIBRATION_ACTIVE_LEVEL = LOW;
const uint8_t DOOR_OPEN_LEVEL = HIGH;

const float ADC_REFERENCE_VOLTS = 5.0f; // Calibrate to measured Uno supply.
const float LM35_OFFSET_C = 0.0f;
const float LM35_SCALE = 1.0f;
const float TEMP_UPPER_LIMIT_C = 8.0f;
const float TEMP_RESET_HYSTERESIS_C = 0.5f;

const uint32_t TEMP_SAMPLE_MS = 100;
const uint32_t DOOR_DEBOUNCE_MS = 25;
const uint32_t VIBRATION_QUIET_US = 150000UL;
const uint32_t DISPLAY_PAGE_MS = 2000;
const uint32_t DISPLAY_LABEL_MS = 450;
const uint32_t SERIAL_IDLE_MS = 20;
const uint32_t GPS_STALE_MS = 30000;

uint32_t temperatureEvents = 0;
uint32_t doorEvents = 0;
uint32_t vibrationEvents = 0;

float temperatureC = 0.0f;
bool temperatureExcursion = false;
bool transportActive = true;
bool doorOpen = false;
bool doorCandidateOpen = false;
bool alarmActive = false;

uint32_t doorCandidateSince = 0;
uint32_t lastTemperatureSample = 0;
uint32_t pageSince = 0;
uint32_t lastDisplayUpdate = 0;
uint8_t displayPage = 0;

bool gpsValid = false;
float latitude = 0.0f;
float longitude = 0.0f;
uint32_t lastGpsUpdate = 0;

volatile uint32_t pendingVibrations = 0;
volatile uint32_t lastVibrationEdgeUs = 0;
volatile bool vibrationSeen = false;

char serialBuffer[96];
uint8_t serialLength = 0;
bool serialOverflow = false;
uint32_t lastSerialByte = 0;

// TM1637 driver; no external library required.
// Segment order: bit 0=a, 1=b, ..., 6=g, 7=decimal point.
const uint8_t DIGITS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay();
#line 88 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStart();
#line 98 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void tmStop();
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void tmWriteByte(uint8_t value);
#line 129 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void displaySegments(const uint8_t segments[4]);
#line 146 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void displayCount(uint32_t count);
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void displayTemperature();
#line 206 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void incrementCounter(uint32_t &counter);
#line 212 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void updateAlarm();
#line 228 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void logEvent(const __FlashStringHelper *event);
#line 262 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleTemperature();
#line 289 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void pollDoor(uint32_t now);
#line 314 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void vibrationISR();
#line 327 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void collectVibrations();
#line 345 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
char * skipSpaces(char *text);
#line 352 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
bool parseCoordinates(char *text, float &lat, float &lon);
#line 393 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void processSerialFrame();
#line 437 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void pollSerial();
#line 462 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void updateDisplay(uint32_t now);
#line 492 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 535 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 84 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_023\\direct\\attempt_0\\sketch\\sketch.ino"
void tmDelay() {
  delayMicroseconds(4);
}

void tmStart() {
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  digitalWrite(DISPLAY_DIO_PIN, HIGH);
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
}

void tmStop() {
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  digitalWrite(DISPLAY_DIO_PIN, LOW);
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(DISPLAY_DIO_PIN, HIGH);
  tmDelay();
}

void tmWriteByte(uint8_t value) {
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  for (uint8_t i = 0; i < 8; ++i) {
    digitalWrite(DISPLAY_CLK_PIN, LOW);
    digitalWrite(DISPLAY_DIO_PIN, (value & 1) ? HIGH : LOW);
    tmDelay();
    digitalWrite(DISPLAY_CLK_PIN, HIGH);
    tmDelay();
    value >>= 1;
  }

  digitalWrite(DISPLAY_CLK_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, INPUT_PULLUP);
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  tmDelay();
  digitalWrite(DISPLAY_CLK_PIN, LOW);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
}

void displaySegments(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40); // Auto-increment addressing.
  tmStop();

  tmStart();
  tmWriteByte(0xC0);
  for (uint8_t i = 0; i < 4; ++i) {
    tmWriteByte(segments[i]);
  }
  tmStop();

  tmStart();
  tmWriteByte(0x8B); // Display on, moderate brightness.
  tmStop();
}

void displayCount(uint32_t count) {
  uint8_t segments[4] = {0, 0, 0, 0};

  if (count > 9999UL) {
    // Saturate visually; the full 32-bit counter is retained in serial logs.
    count = 9999;
  }

  for (int8_t i = 3; i >= 0; --i) {
    segments[i] = DIGITS[count % 10];
    count /= 10;
    if (count == 0) {
      break;
    }
  }
  displaySegments(segments);
}

void displayTemperature() {
  // Three numeric positions plus C. Negative temperatures include a sign.
  uint8_t segments[4] = {0, 0, 0, 0x39};
  int value = (int)(temperatureC * 10.0f +
                    (temperatureC >= 0.0f ? 0.5f : -0.5f));

  if (value >= 0 && value <= 999) {
    if (value >= 100) {
      segments[0] = DIGITS[value / 100];
    }
    segments[1] = DIGITS[(value / 10) % 10] | 0x80;
    segments[2] = DIGITS[value % 10];
  } else if (value < 0 && value >= -99) {
    value = -value;
    segments[0] = 0x40;
    segments[1] = DIGITS[value / 10] | 0x80;
    segments[2] = DIGITS[value % 10];
  } else {
    // Fall back to whole degrees when the decimal format cannot fit.
    int whole = (int)temperatureC;
    bool negative = whole < 0;
    unsigned int magnitude = negative ? -whole : whole;

    if ((!negative && magnitude > 999) ||
        (negative && magnitude > 99)) {
      segments[0] = segments[1] = segments[2] = 0x40;
    } else {
      segments[2] = DIGITS[magnitude % 10];
      magnitude /= 10;
      if (magnitude || negative) {
        segments[1] = DIGITS[magnitude % 10];
      }
      if (negative) {
        segments[0] = 0x40;
      } else if (magnitude >= 10) {
        segments[0] = DIGITS[(magnitude / 10) % 10];
      }
    }
  }
  displaySegments(segments);
}

void incrementCounter(uint32_t &counter) {
  if (counter != 0xFFFFFFFFUL) {
    ++counter;
  }
}

void updateAlarm() {
  bool requested = temperatureC > TEMP_UPPER_LIMIT_C ||
                   (transportActive && doorOpen);
  if (requested == alarmActive) {
    return;
  }

  alarmActive = requested;
  digitalWrite(LED_PIN, requested ? HIGH : LOW);
  if (requested) {
    tone(BUZZER_PIN, 2400);
  } else {
    noTone(BUZZER_PIN);
  }
}

void logEvent(const __FlashStringHelper *event) {
  Serial.print(F("EVENT,ms="));
  Serial.print(millis());
  Serial.print(F(",type="));
  Serial.print(event);
  Serial.print(F(",tempC="));
  Serial.print(temperatureC, 2);
  Serial.print(F(",transport="));
  Serial.print(transportActive ? 1 : 0);
  Serial.print(F(",doorOpen="));
  Serial.print(doorOpen ? 1 : 0);

  if (gpsValid) {
    Serial.print(F(",lat="));
    Serial.print(latitude, 6);
    Serial.print(F(",lon="));
    Serial.print(longitude, 6);
    Serial.print(F(",gpsAgeMs="));
    uint32_t age = millis() - lastGpsUpdate;
    Serial.print(age);
    Serial.print(F(",gps="));
    Serial.print(age > GPS_STALE_MS ? F("STALE") : F("OK"));
  } else {
    Serial.print(F(",lat=UNKNOWN,lon=UNKNOWN,gps=NO_FIX"));
  }

  Serial.print(F(",tempEvents="));
  Serial.print(temperatureEvents);
  Serial.print(F(",doorEvents="));
  Serial.print(doorEvents);
  Serial.print(F(",vibrationEvents="));
  Serial.println(vibrationEvents);
}

void sampleTemperature() {
  int adc = analogRead(TEMP_PIN);
  temperatureC = ((adc * ADC_REFERENCE_VOLTS / 1023.0f) * 100.0f)
                 * LM35_SCALE + LM35_OFFSET_C;

  bool newExcursion = false;
  bool recovered = false;

  if (!temperatureExcursion && temperatureC > TEMP_UPPER_LIMIT_C) {
    temperatureExcursion = true;
    incrementCounter(temperatureEvents);
    newExcursion = true;
  } else if (temperatureExcursion &&
             temperatureC <= TEMP_UPPER_LIMIT_C - TEMP_RESET_HYSTERESIS_C) {
    temperatureExcursion = false;
    recovered = true;
  }

  // Apply alarm before potentially blocking serial output.
  updateAlarm();
  if (newExcursion) {
    logEvent(F("TEMP_HIGH"));
  } else if (recovered) {
    logEvent(F("TEMP_RECOVERED"));
  }
}

void pollDoor(uint32_t now) {
  bool rawOpen = digitalRead(DOOR_PIN) == DOOR_OPEN_LEVEL;

  if (rawOpen != doorCandidateOpen) {
    doorCandidateOpen = rawOpen;
    doorCandidateSince = now;
  }

  if (doorOpen != doorCandidateOpen &&
      (uint32_t)(now - doorCandidateSince) >= DOOR_DEBOUNCE_MS) {
    doorOpen = doorCandidateOpen;
    if (doorOpen) {
      incrementCounter(doorEvents);
    }

    updateAlarm();
    if (doorOpen) {
      logEvent(transportActive ? F("DOOR_OPEN_IN_TRANSIT")
                               : F("DOOR_OPEN"));
    } else {
      logEvent(F("DOOR_CLOSED"));
    }
  }
}

void vibrationISR() {
  uint32_t now = micros();
  if (!vibrationSeen ||
      (uint32_t)(now - lastVibrationEdgeUs) >= VIBRATION_QUIET_US) {
    if (pendingVibrations != 0xFFFFFFFFUL) {
      ++pendingVibrations;
    }
  }
  // A burst of closely spaced edges is one severe-vibration incident.
  lastVibrationEdgeUs = now;
  vibrationSeen = true;
}

void collectVibrations() {
  noInterrupts();
  uint32_t count = pendingVibrations;
  pendingVibrations = 0;
  interrupts();

  if (count == 0) {
    return;
  }

  if (0xFFFFFFFFUL - vibrationEvents < count) {
    vibrationEvents = 0xFFFFFFFFUL;
  } else {
    vibrationEvents += count;
  }
  logEvent(F("SEVERE_VIBRATION"));
}

char *skipSpaces(char *text) {
  while (*text && isspace((unsigned char)*text)) {
    ++text;
  }
  return text;
}

bool parseCoordinates(char *text, float &lat, float &lon) {
  char *end;
  text = skipSpaces(text);

  // Reject empty fields, NAN, INF and other nonnumeric forms.
  if (!isdigit((unsigned char)*text) &&
      *text != '-' && *text != '+' && *text != '.') {
    return false;
  }

  double parsedLat = strtod(text, &end);
  if (end == text) {
    return false;
  }

  end = skipSpaces(end);
  if (*end != ',') {
    return false;
  }
  text = skipSpaces(end + 1);

  if (!isdigit((unsigned char)*text) &&
      *text != '-' && *text != '+' && *text != '.') {
    return false;
  }

  double parsedLon = strtod(text, &end);
  if (end == text || *skipSpaces(end) != '\0') {
    return false;
  }

  if (!(parsedLat >= -90.0 && parsedLat <= 90.0 &&
        parsedLon >= -180.0 && parsedLon <= 180.0)) {
    return false;
  }

  lat = (float)parsedLat;
  lon = (float)parsedLon;
  return true;
}

void processSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("ERROR,SERIAL_FRAME_TOO_LONG"));
  } else if (serialLength != 0) {
    serialBuffer[serialLength] = '\0';
    char *frame = skipSpaces(serialBuffer);
    char *end = frame + strlen(frame);
    while (end > frame && isspace((unsigned char)end[-1])) {
      *--end = '\0';
    }

    if (strcmp(frame, "START") == 0) {
      bool startingWithOpenDoor = !transportActive && doorOpen;
      transportActive = true;
      updateAlarm();
      logEvent(F("TRANSPORT_START"));
      if (startingWithOpenDoor) {
        logEvent(F("DOOR_OPEN_IN_TRANSIT"));
      }
    } else if (strcmp(frame, "STOP") == 0) {
      transportActive = false;
      updateAlarm();
      logEvent(F("TRANSPORT_STOP"));
    } else if (*frame) {
      if (strncmp(frame, "GPS,", 4) == 0) {
        frame += 4;
      }
      float lat, lon;
      if (parseCoordinates(frame, lat, lon)) {
        latitude = lat;
        longitude = lon;
        gpsValid = true;
        lastGpsUpdate = millis();
        Serial.println(F("OK,GPS"));
      } else {
        Serial.println(F("ERROR,USE_GPS_lat_lon_OR_START_OR_STOP"));
      }
    }
  }

  serialLength = 0;
  serialOverflow = false;
}

void pollSerial() {
  // Bound work per loop so serial traffic cannot starve sensors.
  uint8_t budget = 24;
  while (budget-- && Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByte = millis();

    if (c == '\r' || c == '\n') {
      processSerialFrame();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength != 0 || serialOverflow) &&
      Serial.available() == 0 &&
      (uint32_t)(millis() - lastSerialByte) >= SERIAL_IDLE_MS) {
    processSerialFrame();
  }
}

void updateDisplay(uint32_t now) {
  if ((uint32_t)(now - pageSince) >= DISPLAY_PAGE_MS) {
    pageSince = now;
    displayPage = (displayPage + 1) % 4;
  }

  if ((uint32_t)(now - lastDisplayUpdate) < 100) {
    return;
  }
  lastDisplayUpdate = now;

  if (displayPage == 0) {
    displayTemperature();
    return;
  }

  if ((uint32_t)(now - pageSince) < DISPLAY_LABEL_MS) {
    // Labels: t-HI (temperature), door, SHOC (shock).
    const uint8_t labels[3][4] = {
      {0x78, 0x40, 0x76, 0x06},
      {0x5E, 0x5C, 0x5C, 0x50},
      {0x6D, 0x76, 0x3F, 0x39}
    };
    displaySegments(labels[displayPage - 1]);
  } else {
    displayCount(displayPage == 1 ? temperatureEvents :
                 displayPage == 2 ? doorEvents : vibrationEvents);
  }
}

void setup() {
  pinMode(TEMP_PIN, INPUT);
  analogReference(DEFAULT);

  pinMode(VIBRATION_PIN, INPUT_PULLUP);
  pinMode(DOOR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  pinMode(DISPLAY_CLK_PIN, OUTPUT);
  pinMode(DISPLAY_DIO_PIN, OUTPUT);
  digitalWrite(DISPLAY_CLK_PIN, HIGH);
  digitalWrite(DISPLAY_DIO_PIN, HIGH);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(VIBRATION_PIN),
                  vibrationISR,
                  VIBRATION_ACTIVE_LEVEL == LOW ? FALLING : RISING);

  // Initialize the actual door state, including an already-open box at boot.
  doorOpen = digitalRead(DOOR_PIN) == DOOR_OPEN_LEVEL;
  doorCandidateOpen = doorOpen;
  doorCandidateSince = millis();
  if (doorOpen) {
    incrementCounter(doorEvents);
  }

  sampleTemperature();
  lastTemperatureSample = millis();
  updateAlarm();

  logEvent(F("SYSTEM_START"));
  if (doorOpen) {
    logEvent(F("DOOR_OPEN_IN_TRANSIT"));
  }

  pageSince = millis();
  displayTemperature();
}

void loop() {
  uint32_t now = millis();

  pollDoor(now);

  if ((uint32_t)(now - lastTemperatureSample) >= TEMP_SAMPLE_MS) {
    lastTemperatureSample = now;
    sampleTemperature();
  }

  collectVibrations();
  pollSerial();
  updateDisplay(millis());
}
