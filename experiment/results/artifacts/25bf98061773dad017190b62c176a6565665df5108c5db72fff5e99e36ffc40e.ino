#include <Arduino.h>

const uint8_t R_PIN = 4;
const uint8_t G_PIN = 3;
const uint8_t B_PIN = 2;

// Common pin of the RGB LED is anode connected to 5V.
// Therefore, to "fully turn on" a color pin, we drive that pin LOW.
static inline void setColor(uint8_t rOn, uint8_t gOn, uint8_t bOn) {
  digitalWrite(R_PIN, rOn ? LOW : HIGH);
  digitalWrite(G_PIN, gOn ? LOW : HIGH);
  digitalWrite(B_PIN, bOn ? LOW : HIGH);
}

// Check whether input contains "Red", "Green", or "Blue" (case-sensitive).
static inline bool containsToken(const String &s, const char *token) {
  return s.indexOf(token) != -1;
}

void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // Start with all off (since common is anode, OFF = HIGH)
  setColor(0, 0, 0);

  Serial.begin(9600);
}

void loop() {
  // Continuously check if the serial monitor is available, and read until '\n' appears.
  if (!Serial) {
    delay(1);
    return;
  }

  static String input;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      // Evaluate the received line.
      bool r = containsToken(input, "Red");
      bool g = containsToken(input, "Green");
      bool b = containsToken(input, "Blue");

      setColor(r ? 1 : 0, g ? 1 : 0, b ? 1 : 0);

      input = "";
    } else {
      // Collect characters until newline appears.
      input += c;
    }
  }

  // Optional idle behavior: if no newline arrives within a short time,
  // keep waiting (do not alter wiring/behavior beyond requirements).
  // The requirement says to continuously check and read until '\n' appears.
  // So we intentionally do not act on partial lines.
}