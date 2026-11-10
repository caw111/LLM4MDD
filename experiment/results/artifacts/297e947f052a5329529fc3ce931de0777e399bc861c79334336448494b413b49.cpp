#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
// Button Controlled LED

// Given wiring:
// - LED anode -> resistor -> Arduino pin 13
// - LED cathode -> GND
// - Pushbutton input connected to Arduino pin 2 and uses pull-up behavior:
//   Button reads LOW when pressed, HIGH when released.

const int BUTTON_PIN = 2;
const int LED_PIN = 13;

#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT);   // INPUT as required
  pinMode(LED_PIN, OUTPUT);    // Configure LED pin as OUTPUT
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN); // reads LOW when pressed, HIGH when released

  if (buttonState == LOW) {
    digitalWrite(LED_PIN, HIGH);  // LED ON when pressed
  } else {
    digitalWrite(LED_PIN, LOW);   // LED OFF when released
  }
}
