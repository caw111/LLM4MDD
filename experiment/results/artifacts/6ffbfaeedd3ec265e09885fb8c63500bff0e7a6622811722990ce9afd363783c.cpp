#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_serial_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t ledPins[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // bar graph anodes A1..A10

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_serial_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_serial_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_serial_bar_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  static uint32_t lastCharMs = 0;
  static bool haveNumber = false;

  // We continuously read serial input, which is the threshold (integer).
  // Wiring/behavior: Turn on LEDs only up to the threshold.
  // From success criteria: pins whose number is smaller than the threshold are HIGH.
  // Here: pin number is 1..10 corresponding to anodes A1..A10.
  static long threshold = 0;
  static long value = 0;
  static bool negative = false;
  static bool inNumber = false;

  while (Serial.available() > 0) {
    lastCharMs = millis();
    int c = Serial.read();

    if (c == '\r' || c == '\n') {
      // End of line: apply parsed value (if any)
      if (inNumber) {
        value = negative ? -value : value;
        threshold = value;
      }
      // Reset parsing state for next number
      value = 0;
      negative = false;
      inNumber = false;
      haveNumber = true;
      continue;
    }

    if (c == '-' && !inNumber && value == 0) {
      negative = true;
      inNumber = true;
      value = 0;
      continue;
    }

    if (c >= '0' && c <= '9') {
      inNumber = true;
      value = value * 10 + (c - '0');
      continue;
    }

    // Ignore other characters
  }

  // Idle timeout: treat end-of-input burst as end of number.
  if (inNumber && (millis() - lastCharMs > 20)) {
    value = negative ? -value : value;
    threshold = value;
    value = 0;
    negative = false;
    inNumber = false;
    haveNumber = true;
  }

  // Apply LED outputs based on current threshold.
  // LED index i corresponds to pin number (i+1). Condition: (i+1) < threshold => HIGH
  for (uint8_t i = 0; i < 10; i++) {
    uint8_t ledNumber = i + 1; // 1..10
    digitalWrite(ledPins[i], (ledNumber < threshold) ? HIGH : LOW);
  }

  (void)haveNumber; // retained to preserve behavior without requiring serial input to start loop
}
