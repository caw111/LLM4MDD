#include <Arduino.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

const uint8_t LM35_PIN = A1;
const uint8_t FAN_PIN = 5;
const uint8_t LED_PIN = 13;

const float ADC_REFERENCE_VOLTS = 5.0f;
const float OVERHEAT_MARGIN_C = 5.0f;
const unsigned long SAMPLE_INTERVAL_MS = 100;
const unsigned long REPORT_INTERVAL_MS = 1000;
const unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;

float targetTemperatureC = 30.0f;
uint8_t minimumFanPWM = 80;
float temperatureC = 0.0f;
uint8_t fanPWM = 0;
bool overheated = false;

unsigned long lastSampleMs = 0;
unsigned long lastReportMs = 0;
unsigned long lastSerialByteMs = 0;

char serialBuffer[80];
size_t serialLength = 0;
bool serialOverflow = false;

void printHelp() {
  Serial.println(F("Commands (newline or 20 ms idle):"));
  Serial.println(F("  TEMP 30       Set threshold, 0..150 C"));
  Serial.println(F("  MIN 80        Set minimum running PWM, 0..255"));
  Serial.println(F("  SET TEMP 30   Alternative command format"));
  Serial.println(F("  STATUS"));
  Serial.println(F("  HELP"));
  Serial.println(F("Fan: off below threshold; minimum PWM at threshold;"));
  Serial.println(F("linear ramp to 255 at threshold + 5 C."));
}

void printStatus() {
  Serial.print(F("temperature="));
  Serial.print(temperatureC, 2);
  Serial.print(F(" C, threshold="));
  Serial.print(targetTemperatureC, 2);
  Serial.print(F(" C, minPWM="));
  Serial.print((unsigned int)minimumFanPWM);
  Serial.print(F(", fanPWM="));
  Serial.print((unsigned int)fanPWM);
  if (overheated) {
    Serial.print(F(", overheat"));
  }
  Serial.println();
}

void updateControl() {
  if (temperatureC < targetTemperatureC) {
    fanPWM = 0;
  } else {
    float fraction =
        (temperatureC - targetTemperatureC) / OVERHEAT_MARGIN_C;
    fraction = constrain(fraction, 0.0f, 1.0f);
    fanPWM = (uint8_t)(minimumFanPWM +
                      (255 - minimumFanPWM) * fraction + 0.5f);
  }

  analogWrite(FAN_PIN, fanPWM);

  const bool nowOverheated =
      temperatureC > targetTemperatureC + OVERHEAT_MARGIN_C;
  digitalWrite(LED_PIN, nowOverheated ? HIGH : LOW);

  if (nowOverheated && !overheated) {
    Serial.print(F("overheat: "));
    Serial.print(temperatureC, 2);
    Serial.println(F(" C"));
  }
  overheated = nowOverheated;
}

void sampleTemperature() {
  const int adcValue = analogRead(LM35_PIN);
  // LM35 output is 10 mV per degree Celsius.
  temperatureC =
      adcValue * (ADC_REFERENCE_VOLTS / 1023.0f) * 100.0f;
  updateControl();
}

bool parseNumber(const char *text, float &value) {
  if (text == NULL || *text == '\0') {
    return false;
  }

  char *end = NULL;
  const double parsed = strtod(text, &end);
  if (end == text || *end != '\0' || isnan(parsed) || isinf(parsed)) {
    return false;
  }

  value = (float)parsed;
  return !isnan(value) && !isinf(value);
}

void processCommand(char *line) {
  char *save = NULL;
  const char *delimiters = " \t=,:";
  char *command = strtok_r(line, delimiters, &save);
  if (command == NULL) {
    return;
  }

  if (strcasecmp(command, "SET") == 0) {
    command = strtok_r(NULL, delimiters, &save);
    if (command == NULL) {
      Serial.println(F("ERROR: use SET TEMP value or SET MIN value"));
      return;
    }
  }

  char *argument = strtok_r(NULL, delimiters, &save);
  char *extra = strtok_r(NULL, delimiters, &save);

  if (strcasecmp(command, "HELP") == 0 ||
      strcasecmp(command, "STATUS") == 0) {
    if (argument != NULL) {
      Serial.println(F("ERROR: unexpected argument"));
      return;
    }
    if (strcasecmp(command, "HELP") == 0) {
      printHelp();
    } else {
      printStatus();
    }
    return;
  }

  const bool isTemperature =
      strcasecmp(command, "TEMP") == 0 ||
      strcasecmp(command, "THRESHOLD") == 0 ||
      strcasecmp(command, "T") == 0;

  const bool isMinimum =
      strcasecmp(command, "MIN") == 0 ||
      strcasecmp(command, "MINPWM") == 0 ||
      strcasecmp(command, "MINSPEED") == 0;

  if (!isTemperature && !isMinimum) {
    Serial.println(F("ERROR: unknown command; send HELP"));
    return;
  }

  float value;
  if (extra != NULL || !parseNumber(argument, value)) {
    Serial.println(F("ERROR: expected exactly one numeric value"));
    return;
  }

  if (isTemperature) {
    if (value < 0.0f || value > 150.0f) {
      Serial.println(F("ERROR: temperature must be 0..150 C"));
      return;
    }
    targetTemperatureC = value;
  } else {
    if (value < 0.0f || value > 255.0f || floor(value) != value) {
      Serial.println(F("ERROR: minimum PWM must be an integer 0..255"));
      return;
    }
    minimumFanPWM = (uint8_t)value;
  }

  updateControl();
  Serial.print(F("OK: "));
  printStatus();
}

void finishSerialFrame() {
  if (serialOverflow) {
    Serial.println(F("ERROR: command too long"));
  } else if (serialLength > 0) {
    serialBuffer[serialLength] = '\0';
    processCommand(serialBuffer);
  }

  serialLength = 0;
  serialOverflow = false;
}

void serviceSerial() {
  while (Serial.available() > 0) {
    const char incoming = (char)Serial.read();
    lastSerialByteMs = millis();

    if (incoming == '\r' || incoming == '\n') {
      finishSerialFrame();
    } else if (!serialOverflow) {
      if (serialLength < sizeof(serialBuffer) - 1) {
        serialBuffer[serialLength++] = incoming;
      } else {
        serialOverflow = true;
      }
    }
  }

  if ((serialLength > 0 || serialOverflow) &&
      (unsigned long)(millis() - lastSerialByteMs) >=
          SERIAL_IDLE_TIMEOUT_MS) {
    finishSerialFrame();
  }
}

void setup() {
  pinMode(LM35_PIN, INPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  analogWrite(FAN_PIN, 0);
  digitalWrite(LED_PIN, LOW);
  analogReference(DEFAULT);

  Serial.begin(9600);
  printHelp();

  // Discard the first conversion after configuring the ADC reference.
  analogRead(LM35_PIN);
  sampleTemperature();
  printStatus();

  lastSampleMs = millis();
  lastReportMs = millis();
}

void loop() {
  serviceSerial();

  const unsigned long now = millis();

  if ((unsigned long)(now - lastSampleMs) >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;
    sampleTemperature();
  }

  if ((unsigned long)(now - lastReportMs) >= REPORT_INTERVAL_MS) {
    lastReportMs = now;
    printStatus();
  }
}