#include <Arduino.h>
#include <string.h>

constexpr uint8_t BUZZER_PIN = 6;
constexpr unsigned int BUZZER_FREQUENCY_HZ = 2000;
constexpr unsigned long BUZZ_DURATION_MS = 500;
constexpr unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;

char commandBuffer[32];
size_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastByteTime = 0;

void processCommand() {
  if (!commandOverflow && commandLength > 0) {
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

    if (strcmp(command, "BUZZ") == 0) {
      tone(BUZZER_PIN, BUZZER_FREQUENCY_HZ, BUZZ_DURATION_MS);
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  if ((commandLength > 0 || commandOverflow) &&
      millis() - lastByteTime >= SERIAL_IDLE_TIMEOUT_MS) {
    processCommand();
  }

  while (Serial.available() > 0) {
    const char incoming = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (incoming == '\r' || incoming == '\n') {
      processCommand();
    } else if (!commandOverflow) {
      if (commandLength < sizeof(commandBuffer) - 1) {
        commandBuffer[commandLength++] = incoming;
      } else {
        commandOverflow = true;
      }
    }
  }
}