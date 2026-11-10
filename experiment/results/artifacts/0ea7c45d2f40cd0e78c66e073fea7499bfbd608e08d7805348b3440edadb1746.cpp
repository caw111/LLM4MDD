#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Required libraries: "DHT sensor library" and "TM1637Display".
constexpr uint8_t ENTRY_TRIG_PIN = 8;
constexpr uint8_t ENTRY_ECHO_PIN = 9;
constexpr uint8_t EXIT_TRIG_PIN  = 10;
constexpr uint8_t EXIT_ECHO_PIN  = 11;
constexpr uint8_t DHT_PIN        = 7;
constexpr uint8_t FAN_PWM_PIN    = 5;
constexpr uint8_t DISPLAY_CLK    = 4;
constexpr uint8_t DISPLAY_DIO    = 3;
constexpr uint8_t BUTTON_PIN     = 2;

// Button: D2 to GND, using the internal pull-up.
// The fan must use a suitable transistor/MOSFET driver and flyback protection.
constexpr float DETECT_DISTANCE_CM = 60.0f;
constexpr float RELEASE_DISTANCE_CM = 70.0f;
constexpr unsigned long ECHO_TIMEOUT_US = 18000UL;
constexpr unsigned long PING_INTERVAL_MS = 35UL;
constexpr unsigned long SEQUENCE_TIMEOUT_MS = 2500UL;
constexpr unsigned long CLEAR_REARM_MS = 250UL;
constexpr unsigned long BUTTON_DEBOUNCE_MS = 35UL;
constexpr unsigned long DHT_INTERVAL_MS = 2000UL;
constexpr unsigned long TEMPERATURE_STALE_MS = 10000UL;
constexpr unsigned long DISPLAY_INTERVAL_MS = 200UL;
constexpr unsigned long STATUS_INTERVAL_MS = 3000UL;
constexpr unsigned long SERIAL_IDLE_TIMEOUT_MS = 20UL;
constexpr uint16_t MAX_OCCUPANCY = 9999;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);

struct UltrasonicSensor {
  uint8_t trig;
  uint8_t echo;
  float distanceCm;
  bool blocked;
  uint8_t changeSamples;
};

UltrasonicSensor sensors[2] = {
  {ENTRY_TRIG_PIN, ENTRY_ECHO_PIN, -1.0f, false, 0},
  {EXIT_TRIG_PIN, EXIT_ECHO_PIN, -1.0f, false, 0}
};

enum PassageState : uint8_t {
  READY,
  ENTRY_FIRST,
  EXIT_FIRST,
  WAIT_CLEAR
};

PassageState passageState = READY;
uint16_t occupancy = 0;
uint8_t fanPwm = 0;
bool showTemperature = false;

float temperatureC = NAN;
bool haveTemperature = false;
unsigned long lastGoodTemperatureMs = 0;
unsigned long lastDhtMs = 0;
unsigned long lastPingMs = 0;
unsigned long sequenceStartedMs = 0;
unsigned long clearStartedMs = 0;
bool clearTimerRunning = false;
uint8_t nextSensor = 0;

bool buttonRaw = HIGH;
bool buttonStable = HIGH;
unsigned long buttonChangedMs = 0;

unsigned long lastDisplayMs = 0;
unsigned long lastStatusMs = 0;
bool displayDirty = true;

char serialBuffer[48];
uint8_t serialLength = 0;
bool serialOverflow = false;
unsigned long lastSerialByteMs = 0;

#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
bool temperatureUsable(unsigned long now);
#line 92 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void printStatus();
#line 114 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void updateFan(unsigned long now);
#line 144 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void finishPassage(bool entering, unsigned long now);
#line 160 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void handleSensorRise(uint8_t index, unsigned long now);
#line 186 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void servicePassageTimeouts(unsigned long now);
#line 209 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void sampleUltrasonic(uint8_t index);
#line 246 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceUltrasonics(unsigned long now);
#line 255 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceTemperature(unsigned long now);
#line 271 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceButton(unsigned long now);
#line 289 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceDisplay(unsigned long now);
#line 319 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void processSerialCommand();
#line 357 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void serviceSerial();
#line 379 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 408 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\advanced_009\\direct\\attempt_0\\sketch\\sketch.ino"
bool temperatureUsable(unsigned long now) {
  return haveTemperature &&
         (unsigned long)(now - lastGoodTemperatureMs) <= TEMPERATURE_STALE_MS;
}

void printStatus() {
  const unsigned long now = millis();
  Serial.print(F("COUNT="));
  Serial.print(occupancy);
  Serial.print(F(" TEMP_C="));
  if (temperatureUsable(now)) {
    Serial.print(temperatureC, 1);
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.print(F(" FAN_PWM="));
  Serial.print(fanPwm);
  Serial.print(F(" MODE="));
  Serial.print(occupancy == 0 ? F("ENERGY_SAVE") : F("ACTIVE"));
  Serial.print(F(" DISPLAY="));
  Serial.print(showTemperature ? F("TEMP") : F("COUNT"));
  Serial.print(F(" ENTRY_CM="));
  Serial.print(sensors[0].distanceCm, 1);
  Serial.print(F(" EXIT_CM="));
  Serial.println(sensors[1].distanceCm, 1);
}

void updateFan(unsigned long now) {
  uint8_t requestedPwm = 0;

  if (occupancy != 0) {
    if (!temperatureUsable(now)) {
      // With occupants present, use fail-safe ventilation on DHT failure.
      requestedPwm = 255;
    } else {
      // Temperature contribution: 0 at <=18 C, 140 at >=32 C.
      // Occupancy contribution: 4 PWM units/person, capped at 20 people.
      // A minimum of 80 maintains airflow whenever the room is occupied.
      float limitedTemperature = temperatureC;
      if (limitedTemperature < 18.0f) limitedTemperature = 18.0f;
      if (limitedTemperature > 32.0f) limitedTemperature = 32.0f;

      uint16_t peopleForControl = occupancy > 20 ? 20 : occupancy;
      int requested = 80 +
                      (int)((limitedTemperature - 18.0f) * 10.0f + 0.5f) +
                      peopleForControl * 4;
      if (requested > 255) requested = 255;
      requestedPwm = (uint8_t)requested;
    }
  }

  if (requestedPwm != fanPwm) {
    fanPwm = requestedPwm;
    analogWrite(FAN_PWM_PIN, fanPwm);
  }
}

void finishPassage(bool entering, unsigned long now) {
  if (entering) {
    if (occupancy < MAX_OCCUPANCY) ++occupancy;
    Serial.print(F("EVENT=ENTER COUNT="));
  } else {
    if (occupancy > 0) --occupancy;
    Serial.print(F("EVENT=EXIT COUNT="));
  }
  Serial.println(occupancy);

  passageState = WAIT_CLEAR;
  clearTimerRunning = false;
  displayDirty = true;
  updateFan(now);
}

void handleSensorRise(uint8_t index, unsigned long now) {
  switch (passageState) {
    case READY:
      // If both beams are already blocked, direction is ambiguous.
      if (sensors[1 - index].blocked) {
        passageState = WAIT_CLEAR;
        clearTimerRunning = false;
      } else {
        passageState = index == 0 ? ENTRY_FIRST : EXIT_FIRST;
        sequenceStartedMs = now;
      }
      break;

    case ENTRY_FIRST:
      if (index == 1) finishPassage(true, now);
      break;

    case EXIT_FIRST:
      if (index == 0) finishPassage(false, now);
      break;

    case WAIT_CLEAR:
      break;
  }
}

void servicePassageTimeouts(unsigned long now) {
  if ((passageState == ENTRY_FIRST || passageState == EXIT_FIRST) &&
      (unsigned long)(now - sequenceStartedMs) >= SEQUENCE_TIMEOUT_MS) {
    passageState = WAIT_CLEAR;
    clearTimerRunning = false;
    Serial.println(F("EVENT=PASSAGE_TIMEOUT"));
  }

  if (passageState == WAIT_CLEAR) {
    if (!sensors[0].blocked && !sensors[1].blocked) {
      if (!clearTimerRunning) {
        clearTimerRunning = true;
        clearStartedMs = now;
      } else if ((unsigned long)(now - clearStartedMs) >= CLEAR_REARM_MS) {
        passageState = READY;
        clearTimerRunning = false;
      }
    } else {
      clearTimerRunning = false;
    }
  }
}

void sampleUltrasonic(uint8_t index) {
  UltrasonicSensor &sensor = sensors[index];

  digitalWrite(sensor.trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.trig, LOW);

  unsigned long duration = pulseIn(sensor.echo, HIGH, ECHO_TIMEOUT_US);
  sensor.distanceCm = duration == 0 ? -1.0f : duration * 0.0343f / 2.0f;

  bool candidateBlocked;
  if (duration == 0) {
    candidateBlocked = false;
  } else if (sensor.blocked) {
    candidateBlocked = sensor.distanceCm < RELEASE_DISTANCE_CM;
  } else {
    candidateBlocked = sensor.distanceCm <= DETECT_DISTANCE_CM;
  }

  // Require two consecutive samples to change the detection state.
  if (candidateBlocked == sensor.blocked) {
    sensor.changeSamples = 0;
  } else {
    if (++sensor.changeSamples >= 2) {
      sensor.blocked = candidateBlocked;
      sensor.changeSamples = 0;
      if (sensor.blocked) {
        unsigned long now = millis();
        servicePassageTimeouts(now);
        handleSensorRise(index, now);
      }
    }
  }
}

void serviceUltrasonics(unsigned long now) {
  // Alternate pings to avoid simultaneous ultrasonic transmissions.
  if ((unsigned long)(now - lastPingMs) >= PING_INTERVAL_MS) {
    sampleUltrasonic(nextSensor);
    nextSensor ^= 1;
    lastPingMs = millis();
  }
}

void serviceTemperature(unsigned long now) {
  if ((unsigned long)(now - lastDhtMs) < DHT_INTERVAL_MS) return;
  lastDhtMs = now;

  float value = dht.readTemperature();
  if (!isnan(value) && value >= 0.0f && value <= 50.0f) {
    temperatureC = value;
    haveTemperature = true;
    lastGoodTemperatureMs = millis();
  } else {
    // Retain the most recent valid value only until its stale timeout.
    Serial.println(F("WARNING=DHT_READ_FAILED"));
  }
  displayDirty = true;
}

void serviceButton(unsigned long now) {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != buttonRaw) {
    buttonRaw = reading;
    buttonChangedMs = now;
  }

  if (buttonStable != buttonRaw &&
      (unsigned long)(now - buttonChangedMs) >= BUTTON_DEBOUNCE_MS) {
    buttonStable = buttonRaw;
    if (buttonStable == LOW) {
      showTemperature = !showTemperature;
      displayDirty = true;
    }
  }
}

void serviceDisplay(unsigned long now) {
  if (!displayDirty &&
      (unsigned long)(now - lastDisplayMs) < DISPLAY_INTERVAL_MS) return;

  lastDisplayMs = now;
  displayDirty = false;
  display.setBrightness(occupancy == 0 ? 1 : 7, true);

  if (!showTemperature) {
    display.showNumberDec(occupancy, false);
    return;
  }

  if (!temperatureUsable(now)) {
    const uint8_t unavailable[4] = {SEG_G, SEG_G, SEG_G, SEG_G};
    display.setSegments(unavailable);
    return;
  }

  int roundedTemperature = (int)(temperatureC + 0.5f);
  uint8_t segments[4];
  segments[0] = roundedTemperature >= 10
                    ? display.encodeDigit((roundedTemperature / 10) % 10)
                    : 0;
  segments[1] = display.encodeDigit(roundedTemperature % 10);
  segments[2] = SEG_A | SEG_B | SEG_F | SEG_G;  // Degree symbol.
  segments[3] = SEG_A | SEG_D | SEG_E | SEG_F;  // C.
  display.setSegments(segments);
}

void processSerialCommand() {
  if (serialOverflow) {
    Serial.println(F("ERROR=COMMAND_TOO_LONG"));
  } else if (serialLength != 0) {
    serialBuffer[serialLength] = '\0';

    char *command = serialBuffer;
    while (*command && isspace((unsigned char)*command)) ++command;

    char *end = command + strlen(command);
    while (end > command && isspace((unsigned char)*(end - 1))) --end;
    *end = '\0';

    for (char *p = command; *p; ++p) {
      *p = (char)toupper((unsigned char)*p);
    }

    if (strcmp(command, "STATUS") == 0) {
      printStatus();
    } else if (strcmp(command, "TEMP") == 0) {
      showTemperature = true;
      displayDirty = true;
      Serial.println(F("DISPLAY=TEMP"));
    } else if (strcmp(command, "COUNT") == 0) {
      showTemperature = false;
      displayDirty = true;
      Serial.println(F("DISPLAY=COUNT"));
    } else if (strcmp(command, "HELP") == 0) {
      Serial.println(F("COMMANDS: STATUS, TEMP, COUNT, HELP"));
    } else if (*command) {
      Serial.println(F("ERROR=UNKNOWN_COMMAND; USE HELP"));
    }
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastSerialByteMs = millis();

    if (c == '\r' || c == '\n') {
      processSerialCommand();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = c;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength != 0 || serialOverflow) &&
      (unsigned long)(millis() - lastSerialByteMs) >= SERIAL_IDLE_TIMEOUT_MS) {
    processSerialCommand();
  }
}

void setup() {
  pinMode(FAN_PWM_PIN, OUTPUT);
  analogWrite(FAN_PWM_PIN, 0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  for (uint8_t i = 0; i < 2; ++i) {
    pinMode(sensors[i].trig, OUTPUT);
    digitalWrite(sensors[i].trig, LOW);
    pinMode(sensors[i].echo, INPUT);
  }

  Serial.begin(9600);
  dht.begin();

  display.setBrightness(1, true);
  display.showNumberDec(0, false);

  unsigned long now = millis();
  lastDhtMs = now;  // Allow the DHT11 to stabilize before its first read.
  lastPingMs = now;
  lastStatusMs = now;
  buttonRaw = digitalRead(BUTTON_PIN);
  buttonStable = buttonRaw;
  buttonChangedMs = now;

  Serial.println(F("SMART_CLASSROOM_READY"));
  Serial.println(F("COUNT_STARTS_AT_ZERO; COMMANDS: STATUS, TEMP, COUNT, HELP"));
}

void loop() {
  unsigned long now = millis();
  serviceButton(now);
  serviceSerial();
  servicePassageTimeouts(millis());
  serviceUltrasonics(millis());
  serviceTemperature(millis());

  now = millis();
  updateFan(now);
  serviceDisplay(now);

  if ((unsigned long)(now - lastStatusMs) >= STATUS_INTERVAL_MS) {
    lastStatusMs = now;
    printStatus();
  }
}
