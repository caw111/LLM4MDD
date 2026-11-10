#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RGB_led\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t R_LED_PIN = 11;
constexpr uint8_t G_LED_PIN = 10;
constexpr uint8_t B_LED_PIN = 9;

constexpr uint8_t R_BUTTON_PIN = 4;
constexpr uint8_t G_BUTTON_PIN = 3;
constexpr uint8_t B_BUTTON_PIN = 2;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RGB_led\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RGB_led\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_button_RGB_led\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);

  analogWrite(R_LED_PIN, 0);
  analogWrite(G_LED_PIN, 0);
  analogWrite(B_LED_PIN, 0);

  // External 10k ohm resistors pull the button inputs up to 5V.
  pinMode(R_BUTTON_PIN, INPUT);
  pinMode(G_BUTTON_PIN, INPUT);
  pinMode(B_BUTTON_PIN, INPUT);
}

void loop() {
  const bool redPressed = digitalRead(R_BUTTON_PIN) == LOW;
  const bool greenPressed = digitalRead(G_BUTTON_PIN) == LOW;
  const bool bluePressed = digitalRead(B_BUTTON_PIN) == LOW;

  // Common-cathode RGB LED: 255 is fully on, 0 is off.
  analogWrite(R_LED_PIN, redPressed ? 255 : 0);
  analogWrite(G_LED_PIN, greenPressed ? 255 : 0);
  analogWrite(B_LED_PIN, bluePressed ? 255 : 0);
}
