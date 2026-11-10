#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const unsigned long SERIAL_BAUD = 9600;
const unsigned long IDLE_TIMEOUT_MS = 20;

String receivedValue;
unsigned long lastByteTime = 0;
bool skipNextLF = false;

#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void printReceivedValue();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 20 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_monitor\\direct\\attempt_0\\sketch\\sketch.ino"
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
