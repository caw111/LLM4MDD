#include <Arduino.h>
#include <string.h>

const uint8_t LED_PIN = 13;
const unsigned long IDLE_TIMEOUT_MS = 20;

char commandBuffer[4];
uint8_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastByteTime = 0;

void processCommand() {
  if (!commandOverflow && commandLength > 0) {
    commandBuffer[commandLength] = '\0';

    if (strcmp(commandBuffer, "ON") == 0) {
      digitalWrite(LED_PIN, HIGH);
    } else if (strcmp(commandBuffer, "OFF") == 0) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  if ((commandLength > 0 || commandOverflow) &&
      (unsigned long)(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    processCommand();
  }

  while (Serial.available() > 0) {
    char received = (char)Serial.read();
    lastByteTime = millis();

    if (received == '\r' || received == '\n') {
      processCommand();
    } else if (!commandOverflow) {
      if (commandLength < sizeof(commandBuffer) - 1) {
        commandBuffer[commandLength++] = received;
      } else {
        commandOverflow = true;
      }
    }
  }
}