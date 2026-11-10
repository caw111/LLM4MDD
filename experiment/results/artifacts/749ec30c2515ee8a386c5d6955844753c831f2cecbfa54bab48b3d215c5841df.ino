#include <Arduino.h>
#include <Servo.h>

static const uint8_t SR_SHCP = A2;   // mega:A2 -> sr1:SHCP
static const uint8_t SR_STCP = A1;   // mega:A1 -> sr1:STCP
static const uint8_t SR_DS   = A0;    // mega:A0 -> sr1:DS
static const uint8_t SR_OE   = A3;    // not wired; OE is tied to GND in circuit (always enabled)

// Push buttons (wired to pins 2,3,4)
static const uint8_t BTN1 = 2; // btn1: green -> mega:2
static const uint8_t BTN2 = 3; // btn2: green -> mega:3
static const uint8_t BTN3 = 4; // btn3: green -> mega:4

// Servos (wired to pins 5 and 6)
static const uint8_t SERVO1_PIN = 5; // mega:5 -> servo1:PWM
static const uint8_t SERVO2_PIN = 6; // mega:6 -> servo2:PWM

Servo servo1;
Servo servo2;

static uint8_t displayValue = 0;

// sevseg1 is driven by 74HC595 outputs Q0..Q6 mapped to segments:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
//
// Segment patterns for common-anode (typical in Wokwi 7-seg + 74HC595 setups):
// bit0=A ... bit6=G, 1 means segment ON.
// To be robust, we use the Wokwi-standard encoding for common-anode:
// 0: A B C D E F      (G off)
// 1: B C
// 2: A B D E G
// 3: A B C D G
// 4: B C F G
// 5: A C D F G
// 6: A C D E F G
// 7: A B C
// 8: A B C D E F G
// 9: A B C D F G
static const uint8_t segOnPattern[10] = {
  // A B C D E F G -> bits 0..6
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

static void shiftOut7Segments(uint8_t value)
{
  uint8_t pattern = segOnPattern[value];

  // 74HC595 expects serial data first. We must map pattern bit i to Q{i}.
  // We'll clock 7 bits in order Q0..Q6 (DS presented, then shift clock).
  digitalWrite(SR_STCP, LOW);

  for (int i = 0; i < 7; i++) {
    digitalWrite(SR_DS, (pattern >> i) & 0x01);
    // Shift on rising edge of SHCP
    digitalWrite(SR_SHCP, HIGH);
    digitalWrite(SR_SHCP, LOW);
  }

  // Latch to outputs on rising edge of STCP
  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void updateServos(uint8_t v)
{
  if (v <= 4) {
    servo1.write(0);
    servo2.write(90);
  } else {
    servo1.write(90);
    servo2.write(180);
  }
}

static void updateDisplayAndServos()
{
  shiftOut7Segments(displayValue);
  updateServos(displayValue);
}

static bool isButtonPressed(uint8_t pin)
{
  // Circuit likely uses pushbuttons to GND with INPUT pullup enabled:
  // If button pressed, pin reads LOW.
  return digitalRead(pin) == LOW;
}

static void handleButtons()
{
  // Simple edge-detect for immediate, correct increments on each press.
  static uint8_t lastState1 = HIGH;
  static uint8_t lastState2 = HIGH;
  static uint8_t lastState3 = HIGH;

  uint8_t s1 = digitalRead(BTN1);
  uint8_t s2 = digitalRead(BTN2);
  uint8_t s3 = digitalRead(BTN3);

  bool inc = false;
  int increment = 0;

  if (lastState1 == HIGH && s1 == LOW) { // falling edge
    increment += 1;
    inc = true;
  }
  if (lastState2 == HIGH && s2 == LOW) {
    increment += 2;
    inc = true;
  }
  if (lastState3 == HIGH && s3 == LOW) {
    increment += 3;
    inc = true;
  }

  lastState1 = s1;
  lastState2 = s2;
  lastState3 = s3;

  if (inc) {
    int next = (int)displayValue + increment;
    if (next > 9) next = 0;
    displayValue = (uint8_t)next;
    updateDisplayAndServos();
  }
}

void setup()
{
  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization: show "0" and both servos at 0-degree position.
  displayValue = 0;
  shiftOut7Segments(displayValue);

  // Both servos should be at their 0-degree position at boot.
  servo1.write(0);
  servo2.write(0);

  delay(200);

  // Then apply required servo mapping for number 0 (servo1=0, servo2=90)
  // so behavior matches the specification after initialization completes.
  updateServos(displayValue);
}

void loop()
{
  handleButtons();
}