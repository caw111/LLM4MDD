#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

static const uint8_t LED1_PIN = 2;   // mega:2 -> led1:A
static const uint8_t LED2_PIN = 3;   // mega:3 -> led2:A
static const uint8_t LED3_PIN = 4;   // mega:4 -> led3:A

static const uint8_t SERVO1_PIN = 5; // mega:5 -> servo1:PWM
static const uint8_t SERVO2_PIN = 6; // mega:6 -> servo2:PWM
static const uint8_t SERVO3_PIN = 7; // mega:7 -> servo3:PWM

// Bargraph1 segments:
// A1-A8 are the anode/control pins; C1-C8 are common cathode to GND already.
static const uint8_t BAR_A1 = 8;   // mega:8  -> A1
static const uint8_t BAR_A2 = 9;   // mega:9  -> A2
static const uint8_t BAR_A3 = 10;  // mega:10 -> A3
static const uint8_t BAR_A4 = 11;  // mega:11 -> A4
static const uint8_t BAR_A5 = 12;  // mega:12 -> A5
static const uint8_t BAR_A6 = 13;  // mega:13 -> A6
static const uint8_t BAR_A7 = 44;  // mega:44 -> A7
static const uint8_t BAR_A8 = 45;  // mega:45 -> A8

static const uint8_t BAR_PINS[8] = { BAR_A1, BAR_A2, BAR_A3, BAR_A4, BAR_A5, BAR_A6, BAR_A7, BAR_A8 };

#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setServosTo0();
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphOff();
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphSegmentsOn(uint8_t startIndexInclusive, uint8_t endIndexInclusive);
#line 52 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllLEDsOff();
#line 58 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLED1On();
#line 64 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLED2On();
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLED3On();
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 28 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_97\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setServosTo0() {
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

static void setBargraphOff() {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(BAR_PINS[i], LOW);
  }
}

static void setBargraphSegmentsOn(uint8_t startIndexInclusive, uint8_t endIndexInclusive) {
  // Indices: 0..7 correspond to A1..A8
  setBargraphOff();
  if (startIndexInclusive > endIndexInclusive) return;
  if (endIndexInclusive > 7) endIndexInclusive = 7;
  if (startIndexInclusive > 7) return;

  for (uint8_t i = startIndexInclusive; i <= endIndexInclusive; i++) {
    digitalWrite(BAR_PINS[i], HIGH);
  }
}

static void setAllLEDsOff() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

static void setLED1On() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
}

static void setLED2On() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, LOW);
}

static void setLED3On() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, HIGH);
}

void setup() {
  // LEDs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Bargraph segments (A1-A8). Common cathode already on GND.
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(BAR_PINS[i], OUTPUT);
  }

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialization state:
  // All servos at 0°, all LEDs off, all bar graph segments off.
  setAllLEDsOff();
  setBargraphOff();
  setServosTo0();

  // Persist this state for 2 seconds.
  delay(2000);
}

void loop() {
  // State 1 (2 seconds):
  // Servo1 -> 60°, LED1 on, first two bargraph segments on (A1-A2)
  servo1.write(60);
  // Ensure other servos remain at 0 in this state (no overlap with next state's movements)
  servo2.write(0);
  servo3.write(0);

  setLED1On();
  setBargraphSegmentsOn(0, 1); // A1..A2
  delay(2000);

  // State 2 (2 seconds):
  // Servo2 -> 120°, LED2 on, next three segments on (A3-A5)
  // Keep Servo1/Servo3 at their reset position for non-overlapping transitions.
  servo1.write(0);
  servo2.write(120);
  servo3.write(0);

  setLED2On();
  setBargraphSegmentsOn(2, 4); // A3..A5
  delay(2000);

  // State 3 (2 seconds):
  // Servo3 -> 180°, LED3 on, final three segments on (A6-A8)
  servo1.write(0);
  servo2.write(0);
  servo3.write(180);

  setLED3On();
  setBargraphSegmentsOn(5, 7); // A6..A8
  delay(2000);

  // Reset (2 seconds):
  // All servos -> 0°, all LEDs off, all bargraph segments off (A1-A8)
  setServosTo0();
  setAllLEDsOff();
  setBargraphOff();
  delay(2000);
}
