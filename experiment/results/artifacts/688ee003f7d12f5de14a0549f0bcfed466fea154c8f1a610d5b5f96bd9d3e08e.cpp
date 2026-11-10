#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUZZER_PIN = 6; // D6

#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 10 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 5 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_006\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Buzz ON for 200ms
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);

  // Buzz OFF for the remaining 2s interval (2s - 200ms = 1800ms)
  digitalWrite(BUZZER_PIN, LOW);
  delay(1800);
}
