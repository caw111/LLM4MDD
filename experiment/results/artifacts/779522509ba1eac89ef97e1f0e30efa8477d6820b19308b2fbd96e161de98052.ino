#include <Arduino.h>
#include <string.h>

constexpr uint8_t LED_PIN = 13;
constexpr uint8_t PWM_LED_PIN = 9;
constexpr uint8_t BUZZER_PIN = 6;
constexpr uint8_t BUTTON_PIN = 2;

constexpr unsigned long SERIAL_IDLE_MS = 20;
constexpr unsigned long DEBOUNCE_MS = 30;
constexpr unsigned long BLINK_INTERVAL_MS = 500;
constexpr unsigned int BUZZ_FREQUENCY_HZ = 1000;
constexpr unsigned long BUZZ_DURATION_MS = 200;

constexpr size_t COMMAND_BUFFER_SIZE = 48;
char commandBuffer[COMMAND_BUFFER_SIZE];
size_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastSerialByteMs = 0;

bool ledState = false;
bool blinkEnabled = false;
uint8_t pwmValue = 0;
unsigned long lastBlinkMs = 0;

uint8_t buttonRawState = HIGH;
uint8_t buttonStableState = HIGH;
unsigned long buttonChangedMs = 0;

void setLed(bool on) {
  ledState = on;
  digitalWrite(LED_PIN, on ? HIGH : LOW);
}

void printStatus() {
  Serial.print(F("LED="));
  Serial.print(ledState ? F("ON") : F("OFF"));
  Serial.print(F(" PWM="));
  Serial.print(pwmValue);
  Serial.print(F(" MODE="));
  Serial.println(blinkEnabled ? F("BLINK") : F("STEADY"));
}

void executeCommand(char *command) {
  // Ignore leading/trailing spaces and tabs.
  while (*command == ' ' || *command == '\t') {
    ++command;
  }

  size_t length = strlen(command);
  while (length > 0 &&
         (command[length - 1] == ' ' || command[length - 1] == '\t')) {
    command[--length] = '\0';
  }

  if (length == 0) {
    return;
  }

  if (strcmp(command, "ON") == 0) {
    blinkEnabled = false;
    setLed(true);
    Serial.println(F("OK ON"));
  } else if (strcmp(command, "OFF") == 0) {
    blinkEnabled = false;
    setLed(false);
    Serial.println(F("OK OFF"));
  } else if (strcmp(command, "BLINK") == 0) {
    blinkEnabled = true;
    setLed(true);
    lastBlinkMs = millis();
    Serial.println(F("OK BLINK"));
  } else if (strncmp(command, "PWM:", 4) == 0) {
    const char *valueText = command + 4;
    unsigned int value = 0;
    bool valid = (*valueText != '\0');

    for (const char *p = valueText; valid && *p != '\0'; ++p) {
      if (*p < '0' || *p > '9') {
        valid = false;
      } else {
        value = value * 10U + static_cast<unsigned int>(*p - '0');
        if (value > 255U) {
          valid = false;
        }
      }
    }

    if (!valid) {
      Serial.println(F("ERROR PWM requires an integer from 0 to 255"));
      return;
    }

    pwmValue = static_cast<uint8_t>(value);
    analogWrite(PWM_LED_PIN, pwmValue);
    Serial.print(F("OK PWM="));
    Serial.println(pwmValue);
  } else if (strcmp(command, "BUZZ") == 0) {
    // Timed tone is nonblocking; serial, blinking and button remain active.
    tone(BUZZER_PIN, BUZZ_FREQUENCY_HZ, BUZZ_DURATION_MS);
    Serial.println(F("OK BUZZ"));
  } else if (strcmp(command, "STATUS") == 0) {
    printStatus();
  } else {
    Serial.println(F("ERROR Unknown command"));
  }
}

void finishCommand() {
  if (commandOverflow) {
    Serial.println(F("ERROR Command too long"));
  } else if (commandLength > 0) {
    commandBuffer[commandLength] = '\0';
    executeCommand(commandBuffer);
  }

  commandLength = 0;
  commandOverflow = false;
}

void serviceSerial() {
  while (Serial.available() > 0) {
    const char received = static_cast<char>(Serial.read());
    lastSerialByteMs = millis();

    if (received == '\r' || received == '\n') {
      finishCommand();
    } else if (!commandOverflow) {
      if (received == '\0' ||
          commandLength >= COMMAND_BUFFER_SIZE - 1) {
        // Discard the entire malformed/oversized frame, not just its suffix.
        commandOverflow = true;
      } else {
        commandBuffer[commandLength++] = received;
      }
    }
  }

  if ((commandLength > 0 || commandOverflow) &&
      millis() - lastSerialByteMs >= SERIAL_IDLE_MS) {
    finishCommand();
  }
}

void serviceBlink(unsigned long now) {
  if (blinkEnabled && now - lastBlinkMs >= BLINK_INTERVAL_MS) {
    const unsigned long intervals =
        (now - lastBlinkMs) / BLINK_INTERVAL_MS;
    lastBlinkMs += intervals * BLINK_INTERVAL_MS;

    if (intervals & 1UL) {
      setLed(!ledState);
    }
  }
}

void serviceButton(unsigned long now) {
  // Wire the button between D2 and GND; the internal pull-up makes it active-low.
  const uint8_t reading = digitalRead(BUTTON_PIN);

  if (reading != buttonRawState) {
    buttonRawState = reading;
    buttonChangedMs = now;
  }

  if (buttonRawState != buttonStableState &&
      now - buttonChangedMs >= DEBOUNCE_MS) {
    buttonStableState = buttonRawState;
    if (buttonStableState == LOW) {
      printStatus();
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setLed(false);
  analogWrite(PWM_LED_PIN, pwmValue);
  digitalWrite(BUZZER_PIN, LOW);

  buttonRawState = digitalRead(BUTTON_PIN);
  buttonStableState = HIGH;
  buttonChangedMs = millis();

  Serial.begin(9600);
}

void loop() {
  serviceSerial();

  const unsigned long now = millis();
  serviceBlink(now);
  serviceButton(now);
}