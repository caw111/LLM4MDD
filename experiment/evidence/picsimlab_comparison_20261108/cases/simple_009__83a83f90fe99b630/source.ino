#include <Arduino.h>

static const uint8_t LED_PIN = 13;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  static String line = "";
  static unsigned long lastCharTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    lastCharTime = millis();

    // Accumulate until newline (or other delimiters), since "line_or_idle"
    if (c == '\n' || c == '\r') {
      if (line.length() > 0) {
        line.trim();

        if (line.equalsIgnoreCase("ON")) {
          digitalWrite(LED_PIN, HIGH);
        } else if (line.equalsIgnoreCase("OFF")) {
          digitalWrite(LED_PIN, LOW);
        }
        line = "";
      }
    } else {
      line += c;
    }
  }

  // Idle framing: if no new characters for idle_timeout_ms, treat current buffer as a line
  if (line.length() > 0 && (millis() - lastCharTime) > 20UL) {
    line.trim();
    if (line.equalsIgnoreCase("ON")) {
      digitalWrite(LED_PIN, HIGH);
    } else if (line.equalsIgnoreCase("OFF")) {
      digitalWrite(LED_PIN, LOW);
    }
    line = "";
  }
}