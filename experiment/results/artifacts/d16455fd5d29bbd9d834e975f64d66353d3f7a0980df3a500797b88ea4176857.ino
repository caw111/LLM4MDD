#include <Servo.h>

const int led1Pin = 2;
const int led2Pin = 3;

const int servo1PwmPin = 4;
const int servo2PwmPin = 5;
const int servo3PwmPin = 6;

Servo servo1;
Servo servo2;
Servo servo3;

const unsigned long ledBlinkIntervalMs = 2000;

void setup() {
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // Initial LED state: both OFF
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);

  // Attach servos
  servo1.attach(servo1PwmPin);
  servo2.attach(servo2PwmPin);
  servo3.attach(servo3PwmPin);

  // Initial servo positions: 0° for all servos
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Give servos time to reach initial positions
  delay(50);
}

void loop() {
  // Synchronize LEDs with continuous servo sequence.
  // LED behavior: alternate every 2000 ms (led1 on, led2 off; then vice versa).
  static unsigned long lastLedToggleMs = 0;
  static bool led1On = false; // both off at start until first toggle

  // Update LEDs
  unsigned long now = millis();
  if (now - lastLedToggleMs >= ledBlinkIntervalMs) {
    lastLedToggleMs = now;

    // Toggle LEDs alternately
    led1On = !led1On;
    if (led1On) {
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, LOW);
    } else {
      digitalWrite(led1Pin, LOW);
      digitalWrite(led2Pin, HIGH);
    }
  }

  // Servo behavior: continuous repeating sequence, each state maintained for 2 seconds.
  // State 1: servo1=0°, servo2=90°, servo3=180° for 2 seconds
  servo1.write(0);
  servo2.write(90);
  servo3.write(180);

  unsigned long stateStart = millis();
  while (millis() - stateStart < 2000) {
    now = millis();
    if (now - lastLedToggleMs >= ledBlinkIntervalMs) {
      lastLedToggleMs = now;
      led1On = !led1On;
      if (led1On) {
        digitalWrite(led1Pin, HIGH);
        digitalWrite(led2Pin, LOW);
      } else {
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2Pin, HIGH);
      }
    }
  }

  // State 2: servo1=180°, servo2=0°, servo3=90° for 2 seconds
  servo1.write(180);
  servo2.write(0);
  servo3.write(90);

  stateStart = millis();
  while (millis() - stateStart < 2000) {
    now = millis();
    if (now - lastLedToggleMs >= ledBlinkIntervalMs) {
      lastLedToggleMs = now;
      led1On = !led1On;
      if (led1On) {
        digitalWrite(led1Pin, HIGH);
        digitalWrite(led2Pin, LOW);
      } else {
        digitalWrite(led1Pin, LOW);
        digitalWrite(led2Pin, HIGH);
      }
    }
  }
}