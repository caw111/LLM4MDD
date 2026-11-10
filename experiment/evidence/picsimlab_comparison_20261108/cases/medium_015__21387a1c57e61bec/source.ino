#include <Arduino.h>
#include <string.h>

constexpr uint8_t TRIGGER_PIN = 8;
constexpr uint8_t ECHO_PIN = 9;
constexpr unsigned long SERIAL_IDLE_TIMEOUT_MS = 20;
constexpr unsigned long ECHO_TIMEOUT_US = 30000;

char commandBuffer[32];
size_t commandLength = 0;
bool commandOverflow = false;
unsigned long lastByteTime = 0;

void readDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  const unsigned long duration =
      pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    Serial.println(F("Distance: timeout"));
    return;
  }

  const float distanceCm = duration * 0.0343f / 2.0f;
  Serial.print(F("Distance: "));
  Serial.print(distanceCm, 2);
  Serial.println(F(" cm"));
}

void processCommand() {
  if (!commandOverflow && commandLength > 0) {
    commandBuffer[commandLength] = '\0';

    char *start = commandBuffer;
    while (*start == ' ' || *start == '\t') {
      ++start;
    }

    char *end = commandBuffer + commandLength;
    while (end > start && (end[-1] == ' ' || end[-1] == '\t')) {
      --end;
    }
    *end = '\0';

    if (strcmp(start, "READ") == 0) {
      readDistance();
    }
  }

  commandLength = 0;
  commandOverflow = false;
}

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    const char received = static_cast<char>(Serial.read());
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

  if ((commandLength > 0 || commandOverflow) &&
      static_cast<unsigned long>(millis() - lastByteTime) >=
          SERIAL_IDLE_TIMEOUT_MS) {
    processCommand();
  }
}