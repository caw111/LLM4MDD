#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const uint8_t LED1_PIN = 2;
const uint8_t LED2_PIN = 3;
const uint8_t LED3_PIN = 4;

const uint8_t RGB_R_PIN = 5; // rgb1:R
const uint8_t RGB_G_PIN = 6; // rgb1:G
const uint8_t RGB_B_PIN = 7; // rgb1:B

const uint8_t SERVO1_PWM_PIN = 8; // servo1:PWM
const uint8_t SERVO2_PWM_PIN = 9; // servo2:PWM

Servo servo1;
Servo servo2;

#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBRed();
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBGreen();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBBlue();
#line 36 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 17 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_109\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBRed() {
  // Common cathode: HIGH turns LED ON
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);
}

static void setRGBGreen() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, HIGH);
  digitalWrite(RGB_B_PIN, LOW);
}

static void setRGBBlue() {
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, HIGH);
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initial state: all LEDs off, RGB red, servos at 0 degrees
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  setRGBRed();

  servo1.attach(SERVO1_PWM_PIN);
  servo2.attach(SERVO2_PWM_PIN);

  servo1.write(0);
  servo2.write(0);

  // Allow time for servos to move to 0 degrees
  delay(1000);
}

// 2s sweep + 2s pause at each end => 8s full cycle:
// 0->180 (2s), pause at 180 (2s), 180->0 (2s), pause at 0 (2s)
const unsigned long SWEEP_MS = 2000UL;
const unsigned long PAUSE_MS = 2000UL;

void loop() {
  // Infinite repetition of synchronized sequence:
  // At each servo end stop (0 or 180), blink next LED and flicker RGB.
  // Cycle LED: led1 -> led2 -> led3 -> repeat
  // Cycle RGB: red -> green -> blue -> repeat
  static uint8_t ledIndex = 0;   // 0..2
  static uint8_t rgbIndex = 0;   // 0..2

  // Ensure initial 0-degree placement at start of a full cycle
  // (The behavior is defined by the movement loop below.)
  // Start: 0 degrees
  servo1.write(0);
  servo2.write(0);

  // ---- Pause at 0 degrees (flicker occurs here) ----
  // Blink sequence: "each flicker occurs when the servo motor reaches 0 degrees or 180 degrees"
  // We'll treat "flicker" as: turn on the next LED and set RGB for that color for a brief moment.
  {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

    if (ledIndex == 0) digitalWrite(LED1_PIN, HIGH);
    else if (ledIndex == 1) digitalWrite(LED2_PIN, HIGH);
    else digitalWrite(LED3_PIN, HIGH);

    // RGB flicker
    if (rgbIndex == 0) setRGBRed();
    else if (rgbIndex == 1) setRGBGreen();
    else setRGBBlue();

    delay(300); // brief flicker moment; synchronized with the end-stop event
  }

  // Advance indices for next flicker event
  ledIndex = (ledIndex + 1) % 3;
  rgbIndex = (rgbIndex + 1) % 3;

  unsigned long t0 = millis();
  while (millis() - t0 < PAUSE_MS) {
    // Keep servos at 0 during this pause
    servo1.write(0);
    servo2.write(0);
  }

  // ---- Sweep 0 -> 180 ----
  t0 = millis();
  while (millis() - t0 < SWEEP_MS) {
    // Move to 180 over SWEEP_MS
    // (Smoothness not required by spec; writing continuously still keeps timing aligned.)
    unsigned long elapsed = millis() - t0;
    int angle = (int)((180UL * elapsed) / SWEEP_MS);
    servo1.write(angle);
    servo2.write(angle);
  }

  // Ensure at 180 exactly
  servo1.write(180);
  servo2.write(180);

  // ---- Pause at 180 degrees (flicker occurs here) ----
  {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

    if (ledIndex == 0) digitalWrite(LED1_PIN, HIGH);
    else if (ledIndex == 1) digitalWrite(LED2_PIN, HIGH);
    else digitalWrite(LED3_PIN, HIGH);

    if (rgbIndex == 0) setRGBRed();
    else if (rgbIndex == 1) setRGBGreen();
    else setRGBBlue();

    delay(300);
  }

  ledIndex = (ledIndex + 1) % 3;
  rgbIndex = (rgbIndex + 1) % 3;

  t0 = millis();
  while (millis() - t0 < PAUSE_MS) {
    servo1.write(180);
    servo2.write(180);
  }

  // ---- Sweep 180 -> 0 ----
  t0 = millis();
  while (millis() - t0 < SWEEP_MS) {
    unsigned long elapsed = millis() - t0;
    int angle = 180 - (int)((180UL * elapsed) / SWEEP_MS);
    servo1.write(angle);
    servo2.write(angle);
  }

  // Ensure at 0 exactly
  servo1.write(0);
  servo2.write(0);

  // ---- Pause at 0 degrees (flicker occurs here) ----
  // Note: This is the third end-stop event within the full cycle (0 again).
  {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

    if (ledIndex == 0) digitalWrite(LED1_PIN, HIGH);
    else if (ledIndex == 1) digitalWrite(LED2_PIN, HIGH);
    else digitalWrite(LED3_PIN, HIGH);

    if (rgbIndex == 0) setRGBRed();
    else if (rgbIndex == 1) setRGBGreen();
    else setRGBBlue();

    delay(300);
  }

  ledIndex = (ledIndex + 1) % 3;
  rgbIndex = (rgbIndex + 1) % 3;

  t0 = millis();
  while (millis() - t0 < PAUSE_MS) {
    servo1.write(0);
    servo2.write(0);
  }

  // Loop repeats: next iteration begins at 0 degrees and continues flicker cadence.
}
