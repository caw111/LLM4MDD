#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

// Pins from the provided wiring
const uint8_t LED1_PIN = 2;  // mega:2 -> led1:A
const uint8_t LED2_PIN = 3;  // mega:3 -> led2:A

// Bargraph segment pins (10 segments)
const uint8_t BAR_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; // mega:4..13 -> bargraph1:A1..A10

#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllOutputsOff();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void updateLEDsAndBargraph(int servo1Deg, int servo2Deg);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 13 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_98\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setAllOutputsOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  for (uint8_t i = 0; i < 10; i++) digitalWrite(BAR_PINS[i], LOW);
}

void updateLEDsAndBargraph(int servo1Deg, int servo2Deg) {
  // LED1: ON when Servo1 between 0..90 inclusive; OFF when 91..180
  if (servo1Deg >= 0 && servo1Deg <= 90) digitalWrite(LED1_PIN, HIGH);
  else digitalWrite(LED1_PIN, LOW);

  // LED2: ON when Servo2 between 90..180 inclusive; OFF when 0..89
  if (servo2Deg >= 90 && servo2Deg <= 180) digitalWrite(LED2_PIN, HIGH);
  else digitalWrite(LED2_PIN, LOW);

  // Bargraph based on Servo1 position:
  // 0 -> 0 segments, 90 -> 5 segments, 180 -> 10 segments
  // For increments of 10, mapping is linear: segments = servo1Deg / 18
  // But we should ensure exact behavior for specified example points.
  uint8_t segments = (uint8_t)(servo1Deg / 18.0); // 0..180 -> 0..10 (integer trunc)
  if (segments > 10) segments = 10;

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_PINS[i], (i < segments) ? HIGH : LOW);
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  for (uint8_t i = 0; i < 10; i++) pinMode(BAR_PINS[i], OUTPUT);

  setAllOutputsOff();

  // Servos wiring: mega:44 -> servo1:PWM, mega:45 -> servo2:PWM
  servo1.attach(44);
  servo2.attach(45);

  // Initialization: start both servos at 0 degrees
  servo1.write(0);
  servo2.write(0);

  // Ensure LEDs/bargraph are off after powering on/reset
  setAllOutputsOff();
}

void loop() {
  // Servo movement repeat pattern:
  // Servo1: 0 -> 180 by 10, each step pause 2000ms
  // Servo2: 180 -> 0 by 10, each step pause 2000ms
  for (int s1 = 0, s2 = 180; s1 <= 180 && s2 >= 0; s1 += 10, s2 -= 10) {
    servo1.write(s1);
    servo2.write(s2);

    updateLEDsAndBargraph(s1, s2);

    delay(2000);
  }

  // Continue repeating
  // (Next iteration naturally starts again due to loop() repeating,
  // but ensure servos start at the required positions for the sequence.)
  for (int s1 = 0, s2 = 180; s1 <= 180 && s2 >= 0; s1 += 10, s2 -= 10) {
    servo1.write(s1);
    servo2.write(s2);

    updateLEDsAndBargraph(s1, s2);

    delay(2000);
  }
}
