#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t LED_PINS[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const uint8_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
const unsigned long IDLE_TIMEOUT_MS = 20;

bool receiving = false;
bool started = false;
bool hasDigits = false;
bool negative = false;
bool trailingSpace = false;
bool invalidInput = false;
uint8_t magnitude = 0;
unsigned long lastByteTime = 0;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void applyThreshold(int threshold);
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void finishInput();
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void processByte(char c);
#line 77 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 86 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_serial_bar_led\\direct\\attempt_0\\sketch\\sketch.ino"
void applyThreshold(int threshold) {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    // Anodes are numbered 1 through 10; only numbers below the threshold light.
    digitalWrite(LED_PINS[i], (i + 1 < threshold) ? HIGH : LOW);
  }
}

void finishInput() {
  if (hasDigits && !invalidInput) {
    applyThreshold(negative ? -static_cast<int>(magnitude) : magnitude);
  }

  receiving = false;
  started = false;
  hasDigits = false;
  negative = false;
  trailingSpace = false;
  invalidInput = false;
  magnitude = 0;
}

void processByte(char c) {
  if (c == '\r' || c == '\n') {
    finishInput();
    return;
  }

  receiving = true;
  lastByteTime = millis();

  if (c == ' ' || c == '\t') {
    if (started) {
      trailingSpace = true;
    }
    return;
  }

  if (trailingSpace) {
    invalidInput = true;
    return;
  }

  if (!started && (c == '+' || c == '-')) {
    started = true;
    negative = (c == '-');
    return;
  }

  if (c >= '0' && c <= '9') {
    started = true;
    hasDigits = true;

    // Values above 10 have identical LED behavior; saturate to avoid overflow.
    unsigned int next = magnitude * 10U + static_cast<uint8_t>(c - '0');
    magnitude = (next > 11U) ? 11U : next;
    return;
  }

  invalidInput = true;
}

void setup() {
  for (uint8_t i = 0; i < LED_COUNT; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  if (receiving &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    finishInput();
  }

  while (Serial.available() > 0) {
    processByte(static_cast<char>(Serial.read()));
  }

  if (receiving &&
      static_cast<unsigned long>(millis() - lastByteTime) >= IDLE_TIMEOUT_MS) {
    finishInput();
  }
}
