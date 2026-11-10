#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\simple_027\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  static bool state = false;

  state = !state;

  // Switch LED states every 1 second
  digitalWrite(12, state ? HIGH : LOW);
  digitalWrite(13, state ? LOW : HIGH);

  delay(1000);
}
