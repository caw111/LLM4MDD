#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t BUTTON_PIN = 2; // pushbutton connected to D2
const uint8_t LED_PIN = 13;    // LED connected via resistor to D13

#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 12 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 6 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(BUTTON_PIN, INPUT);   // configured as INPUT (no internal pull-up implied)
  pinMode(LED_PIN, OUTPUT);    // LED output
  digitalWrite(LED_PIN, LOW); // ensure LED is off on power-up
}

void loop() {
  // Expected behavior:
  // - LED ON when button is pressed (reads LOW)
  // - LED OFF when button is released (reads HIGH)
  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}
