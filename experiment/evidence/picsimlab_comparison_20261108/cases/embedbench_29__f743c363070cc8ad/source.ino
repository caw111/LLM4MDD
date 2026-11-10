#include <Arduino.h>

// Bar graph anodes A1-A10 connect to Mega digital pins 2-11.
// All bar graph cathodes connect to GND (active HIGH).
const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t POT_PINS[3] = {A3, A4, A5};
const uint8_t SECTION_START[3] = {0, 3, 6};
const uint8_t SECTION_SIZE[3] = {3, 3, 4};

void setup() {
  // Set output latches LOW before enabling outputs to avoid startup flashes.
  for (uint8_t i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], LOW);
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(POT_PINS[i], INPUT);
  }
}

void loop() {
  for (uint8_t section = 0; section < 3; ++section) {
    const int value = analogRead(POT_PINS[section]);
    const uint8_t litCount =
        static_cast<uint8_t>(map(value, 0, 1023, 0, SECTION_SIZE[section]));

    for (uint8_t i = 0; i < SECTION_SIZE[section]; ++i) {
      digitalWrite(LED_PINS[SECTION_START[section] + i],
                   i < litCount ? HIGH : LOW);
    }
  }
}