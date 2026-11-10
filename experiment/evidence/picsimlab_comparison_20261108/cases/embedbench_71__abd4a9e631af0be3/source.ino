#include <Servo.h>

const int btn1Pin = 2;
const int btn2Pin = 3;

const int pot1Pin = A3;
const int pot2Pin = A4;

const int servo1PwmPin = 4; // mega:4 -> servo1:PWM
const int servo2PwmPin = 5; // mega:5 -> servo2:PWM

Servo servo1;
Servo servo2;

struct DebouncedButton {
  int pin;
  bool stableState;           // last stable pressed/released state
  bool lastReportedState;    // last stable state we acted on
  int lastRawReading;
  unsigned long lastRawChangeTime;

  DebouncedButton(int p)
      : pin(p),
        stableState(false),
        lastReportedState(false),
        lastRawReading(LOW),
        lastRawChangeTime(0) {}
};

static const unsigned long debounceMs = 50;

static int mapPotToAngle(int raw) {
  long angle = map(raw, 0, 1023, 0, 180);
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return (int)angle;
}

static bool updateDebounced(DebouncedButton &b) {
  int raw = digitalRead(b.pin);

  if (raw != b.lastRawReading) {
    b.lastRawChangeTime = millis();
    b.lastRawReading = raw;
  }

  if ((millis() - b.lastRawChangeTime) >= debounceMs) {
    bool pressed = (raw == HIGH);
    if (b.stableState != pressed) {
      b.stableState = pressed;
      return true; // stable state change occurred
    }
  }
  return false;
}

static void handleButtonEdge(DebouncedButton &b, Servo &s, int potPin) {
  // Act only when button becomes pressed (debounced rising edge).
  if (updateDebounced(b)) {
    if (b.stableState && !b.lastReportedState) {
      int potRaw = analogRead(potPin);
      int angle = mapPotToAngle(potRaw);
      s.write(angle);
    }
    b.lastReportedState = b.stableState;
  }
}

DebouncedButton btn1(btn1Pin);
DebouncedButton btn2(btn2Pin);

void setup() {
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);

  servo1.attach(servo1PwmPin);
  servo2.attach(servo2PwmPin);

  servo1.write(0);
  servo2.write(0);

  btn1.lastRawReading = digitalRead(btn1.pin);
  btn1.stableState = (btn1.lastRawReading == HIGH);
  btn1.lastReportedState = btn1.stableState;
  btn1.lastRawChangeTime = millis();

  btn2.lastRawReading = digitalRead(btn2.pin);
  btn2.stableState = (btn2.lastRawReading == HIGH);
  btn2.lastReportedState = btn2.stableState;
  btn2.lastRawChangeTime = millis();
}

void loop() {
  handleButtonEdge(btn1, servo1, pot1Pin);
  handleButtonEdge(btn2, servo2, pot2Pin);
}