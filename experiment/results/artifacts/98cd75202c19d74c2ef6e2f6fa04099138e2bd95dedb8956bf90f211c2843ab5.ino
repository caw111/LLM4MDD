#include <Arduino.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <math.h>
#include <string.h>

const uint8_t BUTTON_PIN = 2;
const uint8_t TM1637_CLK_PIN = 4;
const uint8_t TM1637_DIO_PIN = 5;
const uint8_t DHT_PIN = 7;

const unsigned long SERIAL_IDLE_MS = 20;
const unsigned long DEBOUNCE_MS = 35;
const unsigned long SENSOR_INTERVAL_MS = 2000;
const unsigned long DISPLAY_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT11);
TM1637Display display(TM1637_CLK_PIN, TM1637_DIO_PIN);

enum DisplayMode : uint8_t {
  MODE_BOTH = 1,
  MODE_HUMI = 2,
  MODE_TEMP = 3
};

DisplayMode displayMode = MODE_BOTH;
bool serialOutputEnabled = true;
bool showingTemperature = true;
bool displayDirty = true;

float temperatureC = NAN;
float humidityPercent = NAN;

unsigned long lastSensorReadMs = 0;
unsigned long lastDisplaySwitchMs = 0;

bool buttonRawState = HIGH;
bool buttonStableState = HIGH;
unsigned long buttonChangedMs = 0;

char commandBuffer[24];
uint8_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastSerialByteMs = 0;

void printMenu() {
  Serial.println(F("=== DHT11 / TM1637 Menu ==="));
  Serial.println(F("TEMP : display temperature (C)"));
  Serial.println(F("HUMI : display humidity (%)"));
  Serial.println(F("BOTH : alternate temperature/humidity every 2 s"));
  Serial.println(F("Send a newline, or pause for 20 ms, to submit."));
  Serial.println(F("D2 button: toggle periodic serial sensor output."));
  Serial.println(F("Display prefix: t = temperature, H = humidity"));
  Serial.println(F("Mode: BOTH; serial sensor output: ON"));
}

void updateDisplay() {
  uint8_t segments[4];
  const float value = showingTemperature ? temperatureC : humidityPercent;

  // Lowercase 't' and uppercase 'H' distinguish the displayed quantity.
  segments[0] = showingTemperature ? 0x78 : 0x76;

  if (isnan(value) || isinf(value) || value < -99.0f || value > 999.0f) {
    segments[1] = 0x40;
    segments[2] = 0x40;
    segments[3] = 0x40;
  } else {
    const int number = (int)roundf(value);
    const unsigned int magnitude =
        number < 0 ? (unsigned int)(-number) : (unsigned int)number;

    segments[1] = 0;
    segments[2] = 0;
    segments[3] = display.encodeDigit(magnitude % 10);

    if (magnitude >= 10) {
      segments[2] = display.encodeDigit((magnitude / 10) % 10);
    }
    if (magnitude >= 100) {
      segments[1] = display.encodeDigit((magnitude / 100) % 10);
    }
    if (number < 0) {
      if (magnitude >= 10) {
        segments[1] = 0x40;
      } else {
        segments[2] = 0x40;
      }
    }
  }

  display.setSegments(segments);
  displayDirty = false;
}

void printSensorValues() {
  if (!serialOutputEnabled) {
    return;
  }

  Serial.print(F("Temperature: "));
  if (isnan(temperatureC)) {
    Serial.print(F("ERROR"));
  } else {
    Serial.print(temperatureC, 1);
    Serial.print(F(" C"));
  }

  Serial.print(F(" | Humidity: "));
  if (isnan(humidityPercent)) {
    Serial.println(F("ERROR"));
  } else {
    Serial.print(humidityPercent, 1);
    Serial.println(F(" %"));
  }
}

void executeCommand() {
  if (commandOverflow) {
    Serial.println(F("ERROR: command too long. Use TEMP, HUMI, or BOTH."));
  } else if (commandLength > 0) {
    commandBuffer[commandLength] = '\0';

    char *command = commandBuffer;
    while (*command == ' ' || *command == '\t') {
      ++command;
    }

    char *end = command + strlen(command);
    while (end > command && (end[-1] == ' ' || end[-1] == '\t')) {
      --end;
    }
    *end = '\0';

    for (char *p = command; *p; ++p) {
      if (*p >= 'a' && *p <= 'z') {
        *p = (char)(*p - 'a' + 'A');
      }
    }

    bool valid = true;
    if (strcmp(command, "TEMP") == 0) {
      displayMode = MODE_TEMP;
      showingTemperature = true;
    } else if (strcmp(command, "HUMI") == 0) {
      displayMode = MODE_HUMI;
      showingTemperature = false;
    } else if (strcmp(command, "BOTH") == 0) {
      displayMode = MODE_BOTH;
      showingTemperature = true;
    } else {
      valid = false;
      if (*command != '\0') {
        Serial.println(F("ERROR: use TEMP, HUMI, or BOTH."));
      }
    }

    if (valid) {
      lastDisplaySwitchMs = millis();
      displayDirty = true;
      Serial.print(F("Display mode: "));
      Serial.println(command);
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void serviceSerial() {
  unsigned long now = millis();

  // Honor an idle gap before accepting a newly arrived command.
  if ((commandLength > 0 || commandOverflow) &&
      (unsigned long)(now - lastSerialByteMs) >= SERIAL_IDLE_MS) {
    executeCommand();
  }

  uint8_t processed = 0;
  while (Serial.available() > 0 && processed < 32) {
    const char c = (char)Serial.read();
    ++processed;
    lastSerialByteMs = millis();

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

  now = millis();
  if ((commandLength > 0 || commandOverflow) &&
      (unsigned long)(now - lastSerialByteMs) >= SERIAL_IDLE_MS) {
    executeCommand();
  }
}

void serviceButton(unsigned long now) {
  const bool reading = digitalRead(BUTTON_PIN);

  if (reading != buttonRawState) {
    buttonRawState = reading;
    buttonChangedMs = now;
  }

  if (buttonRawState != buttonStableState &&
      (unsigned long)(now - buttonChangedMs) >= DEBOUNCE_MS) {
    buttonStableState = buttonRawState;

    if (buttonStableState == LOW) {
      serialOutputEnabled = !serialOutputEnabled;

      // Control acknowledgments remain available even when telemetry is off.
      Serial.print(F("Serial sensor output: "));
      Serial.println(serialOutputEnabled ? F("ON") : F("OFF"));
    }
  }
}

void setup() {
  // Wire the button between D2 and GND.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600, SERIAL_8N1);
  dht.begin();

  display.setBrightness(5, true);
  display.clear();
  updateDisplay();

  const unsigned long now = millis();
  lastSensorReadMs = now;  // Allow the DHT11 to stabilize before its first read.
  lastDisplaySwitchMs = now;
  buttonChangedMs = now;

  printMenu();
}

void loop() {
  serviceSerial();

  unsigned long now = millis();
  serviceButton(now);

  if ((unsigned long)(now - lastSensorReadMs) >= SENSOR_INTERVAL_MS) {
    lastSensorReadMs = now;

    humidityPercent = dht.readHumidity();
    temperatureC = dht.readTemperature();

    displayDirty = true;
    printSensorValues();
  }

  now = millis();
  if (displayMode == MODE_BOTH &&
      (unsigned long)(now - lastDisplaySwitchMs) >= DISPLAY_INTERVAL_MS) {
    lastDisplaySwitchMs = now;
    showingTemperature = !showingTemperature;
    displayDirty = true;
  }

  if (displayDirty) {
    updateDisplay();
  }
}