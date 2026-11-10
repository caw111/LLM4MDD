#include <Arduino.h>

// LED1 (pin 2) is the LSB; LED5 (pin 6) is the MSB.
// Each LED anode connects through a 220 ohm resistor; cathodes go to GND.
const uint8_t LED_PINS[] = {2, 3, 4, 5, 6};
const uint8_t LED_COUNT = 5;
const unsigned long IDLE_TIMEOUT_MS = 20;

uint8_t inputValue = 0;
bool receiving = false;
bool hasDigits = false;
bool invalidInput = false;
unsigned long lastByteTime = 0;

void displayBinary(uint8_t value) {
  for (uint8_t bit = 0; bit < LED_COUNT; ++bit) {
    digitalWrite(LED_PINS[bit], (value & (1U << bit)) ? HIGH : LOW);
  }
}

void finishInput() {
  if (hasDigits && !invalidInput) {
    displayBinary(inputValue);
  }

  inputValue = 0;
  receiving = false;
  hasDigits = false;
  invalidInput = false;
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  // Finish an unterminated line after 20 ms without another byte.
  if (receiving &&
      (unsigned long)(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    finishInput();
  }

  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());
    lastByteTime = millis();

    if (c == '\r' || c == '\n') {
      finishInput();
      continue;
    }

    receiving = true;

    if (c >= '0' && c <= '9') {
      hasDigits = true;

      if (!invalidInput) {
        const uint16_t nextValue =
            static_cast<uint16_t>(inputValue) * 10U + (c - '0');

        if (nextValue <= 31) {
          inputValue = static_cast<uint8_t>(nextValue);
        } else {
          invalidInput = true;
        }
      }
    } else {
      // Reject malformed input, preserving the previous LED display.
      invalidInput = true;
    }
  }
}