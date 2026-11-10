#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\simple_009\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(13, OUTPUT);          // D13 LED
  digitalWrite(13, LOW);

  Serial.begin(9600);          // 9600 baud
}

void loop() {
  static String line;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    // Collect line-like input: stop on newline/carriage return or on idle timeout.
    if (c == '\r' || c == '\n') {
      // Process complete line
      if (line.length() > 0) {
        if (line == "ON") {
          digitalWrite(13, HIGH);
        } else if (line == "OFF") {
          digitalWrite(13, LOW);
        }
      }
      line = "";
    } else {
      // Append character
      line += c;
    }
  }

  // idle_timeout_ms = 20: if no new data arrives for 20ms, process any buffered token
  static unsigned long lastRx = 0;
  if (Serial.available() > 0) {
    lastRx = millis();
  } else {
    // If buffer has content and we've been idle for 20ms, process it as a token
    if (line.length() > 0 && (millis() - lastRx) >= 20) {
      // Trim potential whitespace
      line.trim();
      if (line == "ON") {
        digitalWrite(13, HIGH);
      } else if (line == "OFF") {
        digitalWrite(13, LOW);
      }
      line = "";
    }
  }
}
