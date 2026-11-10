#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BUZZ_PIN = 6;      // D6

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  static String line;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // line_or_idle: accept commands terminated by newline or at least process
    // accumulated tokens; also handle idle timeout via periodic checks below.
    if (c == '\r' || c == '\n') {
      if (line.length() > 0) {
        // Process complete line
        if (line == "BUZZ") {
          // BUZZ command: blink/drive buzzer for 500ms
          digitalWrite(BUZZ_PIN, HIGH);
          delay(500);
          digitalWrite(BUZZ_PIN, LOW);
        }
        line = "";
      }
    } else {
      // Accumulate characters up to a reasonable limit
      if (line.length() < 32) line += c;
    }
  }

  // Handle idle_timeout_ms=20ms for cases where sender does not include newline
  static unsigned long lastRx = 0;
  if (Serial.available() > 0) {
    lastRx = millis();
  } else {
    // If we have partial data and we've been idle long enough, attempt parse
    if (line.length() > 0 && (millis() - lastRx) >= 20UL) {
      if (line == "BUZZ") {
        digitalWrite(BUZZ_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZ_PIN, LOW);
      }
      line = "";
    }
  }
}
