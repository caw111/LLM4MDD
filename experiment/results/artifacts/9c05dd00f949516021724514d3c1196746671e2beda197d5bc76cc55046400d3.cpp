#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_led_RGB_basic\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

constexpr uint8_t R_PIN = 4;
constexpr uint8_t G_PIN = 3;
constexpr uint8_t B_PIN = 2;

#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_led_RGB_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_led_RGB_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 7 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\direct_gpt61_full_20261021\\pceval_led_RGB_basic\\direct\\attempt_0\\sketch\\sketch.ino"
void setup() {
  // Common-anode RGB LED: HIGH is off, LOW is fully on.
  // Preload output levels to prevent red or green startup flashes.
  digitalWrite(R_PIN, HIGH);
  digitalWrite(G_PIN, HIGH);
  digitalWrite(B_PIN, LOW);

  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}

void loop() {
  // Blue remains fully on; no further actions are required.
}
