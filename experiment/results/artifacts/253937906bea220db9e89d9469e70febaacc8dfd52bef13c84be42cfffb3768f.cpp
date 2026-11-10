#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t BTN_PIN = 2;

// 74HC595 control pins (per circuit wiring)
static const uint8_t SR_DS   = A0; // DS -> mega:A0
static const uint8_t SR_STCP = A1; // STCP -> mega:A1
static const uint8_t SR_SHCP = A2; // SHCP -> mega:A2

// Sevseg1 segment mapping:
// sr1:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 unused; we will keep it low.
static const uint8_t DIGIT_TO_SEGQ[10] = {
  // bits: Q6 Q5 Q4 Q3 Q2 Q1 Q0  (G F E D C B A)
  // We assume active-HIGH segments as typical in Wokwi sevseg.
  // 0: A B C D E F on, G off
  (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),                 // 0
  // 1: B C
  (1 << 2) | (1 << 1),                                                           // 1
  // 2: A B D E G
  (1 << 6) | (1 << 3) | (1 << 4) | (1 << 1) | (1 << 0),                         // 2
  // 3: A B C D G
  (1 << 6) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0),                         // 3
  // 4: B C F G
  (1 << 6) | (1 << 5) | (1 << 2) | (1 << 1),                                   // 4
  // 5: A C D F G
  (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2) | (1 << 0),                         // 5
  // 6: A C D E F G
  (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 0),             // 6
  // 7: A B C
  (1 << 2) | (1 << 1) | (1 << 0),                                                 // 7
  // 8: A B C D E F G
  (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0), // 8
  // 9: A B C D F G
  (1 << 6) | (1 << 5) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)              // 9
};

Servo servo1;
Servo servo2;

static uint8_t number = 0;
static bool lastBtnState = LOW;

#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t n);
#line 61 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateServos(uint8_t n);
#line 67 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 45 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_77\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7Seg(uint8_t n) {
  // Shift out 8 bits MSB->LSB so that Q0 corresponds to the last shifted bit (LSB).
  // We'll shift bits for Q7..Q0. Q7 is kept LOW.
  uint8_t pattern = DIGIT_TO_SEGQ[n] & 0x7F; // only Q0..Q6 used

  digitalWrite(SR_STCP, LOW);
  for (int i = 7; i >= 0; --i) {
    uint8_t bitVal = (i == 7) ? 0 : ((pattern >> i) & 0x01);
    digitalWrite(SR_DS, bitVal ? HIGH : LOW);
    // Toggle SHCP for each bit
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }
  digitalWrite(SR_STCP, HIGH);
}

static void updateServos(uint8_t n) {
  uint8_t angle = (n % 2 == 0) ? 0 : 90;
  servo1.write(angle);
  servo2.write(angle);
}

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  pinMode(SR_DS, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_SHCP, OUTPUT);

  digitalWrite(SR_DS, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  servo1.attach(3); // mega:3 -> servo1:PWM
  servo2.attach(5); // mega:5 -> servo2:PWM

  number = 0;
  // Initial: display "0" and servos at 0°
  srWrite7Seg(number);
  updateServos(number);

  lastBtnState = digitalRead(BTN_PIN);
}

void loop() {
  // Button handling with debounce
  bool btnState = digitalRead(BTN_PIN);

  // Detect press (active LOW due to INPUT_PULLUP)
  if (lastBtnState == HIGH && btnState == LOW) {
    delay(25);
    // Confirm still pressed
    if (digitalRead(BTN_PIN) == LOW) {
      number++;
      if (number > 9) number = 0;

      // Immediate updates after each press
      srWrite7Seg(number);
      updateServos(number);

      // Wait for release to avoid multiple increments per press
      while (digitalRead(BTN_PIN) == LOW) {
        delay(1);
      }
      btnState = digitalRead(BTN_PIN);
    }
  }

  lastBtnState = btnState;
}
