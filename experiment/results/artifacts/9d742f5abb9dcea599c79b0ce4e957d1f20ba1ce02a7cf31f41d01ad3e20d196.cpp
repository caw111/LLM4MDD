#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t BUZZ_PIN = 6;   // D6

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\medium_014\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);

  Serial.begin(9600);
}

void loop() {
  // Read line-or-idle: since we only need BUZZ, trigger immediately when a complete token is detected.
  // We'll treat any of these as valid:
  //  - "BUZZ" followed by newline/CR
  //  - "BUZZ" followed by any non-alphanumeric delimiter
  //  - "BUZZ" arriving and then an idle gap of ~20ms (handled naturally by token assembly below)
  static String token;
  static unsigned long lastCharTime = 0;

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    unsigned long now = millis();
    if (token.length() > 0 && (now - lastCharTime) > 20) {
      token = "";
    }
    lastCharTime = now;

    // If delimiter/newline, evaluate token
    if (c == '\n' || c == '\r' || c == ',' || c == ' ' || c == '\t') {
      if (token.length() > 0) {
        token.trim();
        if (token == "BUZZ") {
          // 串口输入 BUZZ：蜂鸣器（D6）鸣叫 500ms
          tone(BUZZ_PIN, 1000);      // audible frequency (kept internal to preserve "buzzer鸣叫")
          delay(500);
          noTone(BUZZ_PIN);
          digitalWrite(BUZZ_PIN, LOW);
        }
        token = "";
      }
      continue;
    }

    // Build token for alphanumeric characters
    if (isalnum((unsigned char)c)) {
      token += c;
    } else {
      // Other punctuation ends token
      if (token.length() > 0) {
        token.trim();
        if (token == "BUZZ") {
          tone(BUZZ_PIN, 1000);
          delay(500);
          noTone(BUZZ_PIN);
          digitalWrite(BUZZ_PIN, LOW);
        }
        token = "";
      }
    }
  }

  // Handle idle timeout even if no delimiter arrived.
  static unsigned long idleStart = 0;
  if (token.length() > 0) {
    if (idleStart == 0) idleStart = millis();
    if ((millis() - idleStart) > 20) {
      token = "";
      idleStart = 0;
    }
  } else {
    idleStart = 0;
  }
}
