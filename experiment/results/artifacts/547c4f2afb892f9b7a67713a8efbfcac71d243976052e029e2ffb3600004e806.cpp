#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

const byte BTN1_PIN = 2;
const byte BTN2_PIN = 3;
const byte BTN3_PIN = 4;

const byte BARGRAPH_FIRST_LED_PIN = 5;   // A1..A10 are on Mega pins 5..14 (per wiring)
const byte BARGRAPH_LED_COUNT = 10;

const int ANGLE_1 = 30;
const int ANGLE_2 = 60;
const int ANGLE_3 = 90;

const unsigned long STATE_HOLD_MS = 2000;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphForAngle(int angleDeg);
#line 31 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void applyState(int angleDeg);
#line 37 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_73\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphForAngle(int angleDeg) {
  // Map 0..90 to 0..10 LEDs proportionally, with exact requirements:
  // 30 -> 3 LEDs, 60 -> 6 LEDs, 90 -> 10 LEDs.
  // Use proportional then clamp.
  int leds = (angleDeg * 10) / 90; // 30->3, 60->6, 90->10
  if (leds < 0) leds = 0;
  if (leds > 10) leds = 10;

  for (byte i = 0; i < BARGRAPH_LED_COUNT; i++) {
    digitalWrite(BARGRAPH_FIRST_LED_PIN + i, (i < leds) ? HIGH : LOW);
  }
}

static void applyState(int angleDeg) {
  servo1.write(angleDeg);
  setBargraphForAngle(angleDeg);
  delay(STATE_HOLD_MS);
}

void setup() {
  servo1.attach(44); // mega:44 -> servo1:PWM
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Bargraph LEDs: mega:5..14 to A1..A10 (all share common cathode on GND.1)
  for (byte i = 0; i < BARGRAPH_LED_COUNT; i++) {
    pinMode(BARGRAPH_FIRST_LED_PIN + i, OUTPUT);
    digitalWrite(BARGRAPH_FIRST_LED_PIN + i, LOW);
  }

  // Initialization state: servo at 0 degrees, bargraph off.
  servo1.write(0);
  setBargraphForAngle(0);
}

void loop() {
  // Buttons are active-low (wiring: button connects to GND when pressed via green/black nodes)
  if (digitalRead(BTN1_PIN) == LOW) {
    applyState(ANGLE_1);
    while (digitalRead(BTN1_PIN) == LOW) { /* wait for release */ }
  } else if (digitalRead(BTN2_PIN) == LOW) {
    applyState(ANGLE_2);
    while (digitalRead(BTN2_PIN) == LOW) { /* wait for release */ }
  } else if (digitalRead(BTN3_PIN) == LOW) {
    applyState(ANGLE_3);
    while (digitalRead(BTN3_PIN) == LOW) { /* wait for release */ }
  }
}
