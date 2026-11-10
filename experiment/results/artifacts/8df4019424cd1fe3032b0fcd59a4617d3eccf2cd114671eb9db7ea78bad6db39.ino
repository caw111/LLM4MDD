#include <Arduino.h>

const uint8_t LED_PIN = 9;
const unsigned long IDLE_TIMEOUT_MS = 20;
const size_t INPUT_CAPACITY = 32;

char inputBuffer[INPUT_CAPACITY];
size_t inputLength = 0;
bool inputOverflow = false;
unsigned long lastByteTime = 0;

void processInput() {
  if (inputOverflow) {
    Serial.println(F("Error: enter an integer from 0 to 255"));
  } else {
    size_t start = 0;
    size_t end = inputLength;

    while (start < end &&
           (inputBuffer[start] == ' ' || inputBuffer[start] == '\t')) {
      ++start;
    }
    while (end > start &&
           (inputBuffer[end - 1] == ' ' || inputBuffer[end - 1] == '\t')) {
      --end;
    }

    if (start < end) {
      uint16_t value = 0;
      bool valid = true;

      for (size_t i = start; i < end; ++i) {
        char c = inputBuffer[i];
        if (c < '0' || c > '9') {
          valid = false;
          break;
        }

        value = value * 10 + (c - '0');
        if (value > 255) {
          valid = false;
          break;
        }
      }

      if (valid) {
        analogWrite(LED_PIN, static_cast<uint8_t>(value));
        Serial.println(value);
      } else {
        Serial.println(F("Error: enter an integer from 0 to 255"));
      }
    }
  }

  inputLength = 0;
  inputOverflow = false;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
  Serial.begin(9600);
}

void loop() {
  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastByteTime >= IDLE_TIMEOUT_MS) {
    processInput();
  }

  while (Serial.available() > 0) {
    char c = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (c == '\r' || c == '\n') {
      if (inputLength > 0 || inputOverflow) {
        processInput();
      }
    } else if (!inputOverflow) {
      if (inputLength < INPUT_CAPACITY) {
        inputBuffer[inputLength++] = c;
      } else {
        inputOverflow = true;
      }
    }
  }

  if ((inputLength > 0 || inputOverflow) &&
      millis() - lastByteTime >= IDLE_TIMEOUT_MS) {
    processInput();
  }
}