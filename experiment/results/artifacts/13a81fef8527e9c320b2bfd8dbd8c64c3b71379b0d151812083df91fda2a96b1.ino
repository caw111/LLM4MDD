#include <Arduino.h>

const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static bool readIntegerFromSerial(long &outValue) {
  static char buf[32];
  static uint8_t idx = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Accept newline or carriage return as end-of-number
    if (c == '\n' || c == '\r') {
      if (idx == 0) continue;

      buf[idx] = '\0';
      idx = 0;

      // Parse integer
      char *endptr = nullptr;
      long v = strtol(buf, &endptr, 10);
      if (endptr != buf) {
        outValue = v;
        return true;
      }
      // If parsing fails, keep waiting for the next input
      continue;
    }

    // Ignore leading spaces/tabs
    if (idx == 0 && (c == ' ' || c == '\t')) continue;

    // Allow digits, sign, and spaces (spaces will be safely handled by strtol only if at ends;
    // keep behavior robust by simply collecting them)
    if (idx < sizeof(buf) - 1) {
      buf[idx++] = c;
    }
  }

  return false;
}

void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {
  long threshold = 0;

  if (readIntegerFromSerial(threshold)) {
    for (uint8_t i = 0; i < 10; i++) {
      // LED "pin number" corresponds to 1..10 for A1..A10
      uint8_t ledNumber = i + 1;
      digitalWrite(LED_PINS[i], (ledNumber < threshold) ? HIGH : LOW);
    }
  } else {
    // No new serial input yet; keep the current LED states unchanged.
  }
}