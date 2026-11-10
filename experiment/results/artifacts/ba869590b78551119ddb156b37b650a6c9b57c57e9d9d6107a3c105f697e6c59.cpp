#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;

// Wiring from circuit:
static const uint8_t POT_PIN = A3;     // pot1:SIG -> mega:A3
static const uint8_t SERVO1_PIN = 2;   // mega:2 -> servo1:PWM
static const uint8_t SERVO2_PIN = 3;   // mega:3 -> servo2:PWM

// Bargraph pins: bargraph1:A1..A10 -> mega:4..13
static const uint8_t LED_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToLEDs(int pot);
#line 22 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphLEDs(int ledsOn);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setServosFromPot(int pot);
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 14 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_32\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int mapPotToLEDs(int pot) {
  // pot: 0..1023 -> LEDs: 0..10
  long leds = (long)pot * 10L / 1023L;
  if (leds < 0) leds = 0;
  if (leds > 10) leds = 10;
  return (int)leds;
}

static void setBargraphLEDs(int ledsOn) {
  // ledsOn: 0..10, light sequential from left (A1) to right (A10)
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_PINS[i], (i < ledsOn) ? HIGH : LOW);
  }
}

static void setServosFromPot(int pot) {
  // Linear mapping with opposite direction for servo2:
  // pot:0..1023 -> servo1: 0..180
  //              -> servo2: 180..0
  // Use 0..180 inclusive using integer math to avoid fractional flicker.
  long s1 = (long)pot * 180L / 1023L;
  long s2 = 180L - s1;

  if (s1 < 0) s1 = 0;
  if (s1 > 180) s1 = 180;
  if (s2 < 0) s2 = 0;
  if (s2 > 180) s2 = 180;

  servo1.write((int)s1);
  servo2.write((int)s2);
}

void setup() {
  // Ensure LEDs start OFF
  for (int i = 0; i < 10; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // Attach servos and initialize at 0 degrees as required
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Requirements:
  // - Both servos start at 0° when powered on.
  // Note: Later, pot at 0% must enforce servo1=0 and servo2=180.
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  int pot = analogRead(POT_PIN);           // 0..1023
  int ledsOn = mapPotToLEDs(pot);         // 0..10
  setBargraphLEDs(ledsOn);                // fully on/off, sequential
  setServosFromPot(pot);                 // synchronized mapping
}
