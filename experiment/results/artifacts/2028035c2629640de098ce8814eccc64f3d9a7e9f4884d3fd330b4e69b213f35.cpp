#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

const uint8_t R_LED_PIN = 11;
const uint8_t G_LED_PIN = 10;
const uint8_t B_LED_PIN = 9;

const uint8_t R_BUTTON_PIN = 4;
const uint8_t G_BUTTON_PIN = 3;
const uint8_t B_BUTTON_PIN = 2;

#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setColor(bool rPressed, bool gPressed, bool bPressed);
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 11 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\pceval_button_RGB_led\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void setColor(bool rPressed, bool gPressed, bool bPressed) {
  // Common cathode: PWM 0 = off, PWM 255 = fully on
  analogWrite(R_LED_PIN, rPressed ? 255 : 0);
  analogWrite(G_LED_PIN, gPressed ? 255 : 0);
  analogWrite(B_LED_PIN, bPressed ? 255 : 0);
}

void setup() {
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);

  pinMode(R_BUTTON_PIN, INPUT);
  pinMode(G_BUTTON_PIN, INPUT);
  pinMode(B_BUTTON_PIN, INPUT);

  // Ensure LED off on startup
  setColor(false, false, false);
}

void loop() {
  int rVal = digitalRead(R_BUTTON_PIN); // LOW when pressed (pull-up)
  int gVal = digitalRead(G_BUTTON_PIN);
  int bVal = digitalRead(B_BUTTON_PIN);

  bool rPressed = (rVal == LOW);
  bool gPressed = (gVal == LOW);
  bool bPressed = (bVal == LOW);

  setColor(rPressed, gPressed, bPressed);
}
