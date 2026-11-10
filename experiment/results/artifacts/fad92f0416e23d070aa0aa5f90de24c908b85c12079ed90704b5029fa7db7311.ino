#include <Servo.h>

Servo servo1;

const int btn1Pin = 2;
const int btn2Pin = 3;
const int btn3Pin = 4;

const int pot1Pin = A3;
const int pot2Pin = A4;
const int pot3Pin = A5;

volatile int activePot = 0; // 0 = none, 1 = pot1, 2 = pot2, 3 = pot3

void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);
  pinMode(btn3Pin, INPUT_PULLUP);

  // Servo wiring: mega:5 -> PWM, mega:5V -> V+, mega:GND.1 -> GND
  servo1.attach(5);

  // Default position 0 degrees on startup/reset
  servo1.write(0);
}

void loop() {
  // Last button pressed determines active potentiometer.
  // Buttons wired to GND with pull-up enabled: pressed => LOW.
  if (digitalRead(btn1Pin) == LOW) {
    activePot = 1;
  } else if (digitalRead(btn2Pin) == LOW) {
    activePot = 2;
  } else if (digitalRead(btn3Pin) == LOW) {
    activePot = 3;
  }

  int potValue = 0;
  switch (activePot) {
    case 1: potValue = analogRead(pot1Pin); break; // 0..1023
    case 2: potValue = analogRead(pot2Pin); break;
    case 3: potValue = analogRead(pot3Pin); break;
    default:
      // No button pressed yet: keep servo at its last/default position.
      // (Requirement specifies default at power-on; after that, last button pressed controls.)
      break;
  }

  if (activePot >= 1 && activePot <= 3) {
    // Map 0..1023 to 0..180 degrees
    int angle = map(potValue, 0, 1023, 0, 180);
    servo1.write(angle);
  }

  delay(15);
}