#include <Servo.h>

Servo servo1;

// Buttons (pushbuttons)
static const uint8_t BTN1_PIN = 2;  // mega:2 -> btn1:1.l
static const uint8_t BTN2_PIN = 3;  // mega:3 -> btn2:1.l
static const uint8_t BTN3_PIN = 4;  // mega:4 -> btn3:1.l

// Pots (slide potentiometers)
static const uint8_t POT1_PIN = A3; // pot1:SIG -> mega:A3
static const uint8_t POT2_PIN = A4; // pot2:SIG -> mega:A4
static const uint8_t POT3_PIN = A5; // pot3:SIG -> mega:A5

enum ActivePot : uint8_t { NONE = 0, POT1 = 1, POT2 = 2, POT3 = 3 };
volatile ActivePot activePot = NONE;

static ActivePot readActivePotFromButtons() {
  // Buttons are wired to ground on one side and to digital pins on the other.
  // Using INPUT_PULLUP makes the released state HIGH and pressed state LOW.
  const bool b1 = (digitalRead(BTN1_PIN) == LOW);
  const bool b2 = (digitalRead(BTN2_PIN) == LOW);
  const bool b3 = (digitalRead(BTN3_PIN) == LOW);

  // "last button pressed determines which potentiometer is active"
  // We determine this using edge detection with "last pressed" latch in loop().
  // This function just reports which are currently pressed.
  if (b1) return POT1;
  if (b2) return POT2;
  if (b3) return POT3;
  return NONE;
}

static uint16_t readPotADC(uint8_t pin) {
  return (uint16_t)analogRead(pin); // 0..1023
}

void setup() {
  // Buttons with internal pullups
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Servo to default 0 degrees
  servo1.attach(5); // mega:5 -> servo1:PWM
  servo1.write(0);

  // Initialize latch state
  activePot = NONE;

  // Small settle delay
  delay(50);
}

void loop() {
  static uint8_t lastB1 = HIGH;
  static uint8_t lastB2 = HIGH;
  static uint8_t lastB3 = HIGH;

  // Read current button states (released=HIGH, pressed=LOW)
  uint8_t b1 = digitalRead(BTN1_PIN);
  uint8_t b2 = digitalRead(BTN2_PIN);
  uint8_t b3 = digitalRead(BTN3_PIN);

  // Detect falling edge => transition from released(HIGH) to pressed(LOW)
  // The "last button pressed" is implemented by updating activePot immediately
  // on each detected press edge. If multiple edges occur close together,
  // the one processed last in this loop iteration wins.
  if (lastB1 == HIGH && b1 == LOW) activePot = POT1;
  if (lastB2 == HIGH && b2 == LOW) activePot = POT2;
  if (lastB3 == HIGH && b3 == LOW) activePot = POT3;

  lastB1 = b1;
  lastB2 = b2;
  lastB3 = b3;

  // Drive servo based on active potentiometer
  uint16_t potVal = 0;
  switch (activePot) {
    case POT1:
      potVal = readPotADC(POT1_PIN);
      break;
    case POT2:
      potVal = readPotADC(POT2_PIN);
      break;
    case POT3:
      potVal = readPotADC(POT3_PIN);
      break;
    case NONE:
    default:
      // If no button has ever been pressed, keep default position at 0 degrees.
      potVal = 0;
      break;
  }

  // Map 0..1023 to 0..180 degrees
  // Use long for safe multiplication.
  uint8_t angle = (uint8_t)((long)potVal * 180L / 1023L);
  servo1.write(angle);

  delay(10);
}