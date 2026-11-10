#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Arduino Mega 2560 - LED control sequence
// Wiring preserved:
// - led1:C -> mega GND
// - led1:A -> mega pin 2

const uint8_t LED_PIN = 2;

#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 8 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_19\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off at power on/reset
}

void loop() {
  // 1) ON for 1 second
  digitalWrite(LED_PIN, HIGH);
  delay(1000);

  // 2) OFF for 2 seconds
  digitalWrite(LED_PIN, LOW);
  delay(2000);

  // 3) ON for 3 seconds
  digitalWrite(LED_PIN, HIGH);
  delay(3000);

  // 4) OFF for 3 seconds
  digitalWrite(LED_PIN, LOW);
  delay(3000);
}
