#include <Arduino.h>

const unsigned long SERIAL_BAUD = 9600;
const unsigned long IDLE_TIMEOUT_MS = 20;

String receivedValue;
unsigned long lastByteTime = 0;
bool skipNextLF = false;

void printReceivedValue() {
  Serial.print(F("Received: "));
  Serial.println(receivedValue);
  receivedValue = "";
}

void setup() {
  Serial.begin(SERIAL_BAUD);
}

void loop() {
  while (Serial.available() > 0) {
    char incoming = static_cast<char>(Serial.read());
    lastByteTime = millis();

    // Treat CRLF as a single line ending.
    if (incoming == '\n' && skipNextLF) {
      skipNextLF = false;
      continue;
    }

    skipNextLF = false;

    if (incoming == '\r') {
      printReceivedValue();
      skipNextLF = true;
    } else if (incoming == '\n') {
      printReceivedValue();
    } else {
      receivedValue += incoming;
    }
  }

  // Also accept input sent without a line ending.
  if (receivedValue.length() > 0 &&
      millis() - lastByteTime >= IDLE_TIMEOUT_MS) {
    printReceivedValue();
  }
}