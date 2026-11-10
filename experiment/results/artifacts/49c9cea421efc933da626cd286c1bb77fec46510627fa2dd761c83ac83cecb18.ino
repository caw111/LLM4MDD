#include <Servo.h>

const int btn1Pin = 2;
const int btn2Pin = 3;

const int pot1Pin = A3;
const int pot2Pin = A4;

const int servo1PwmPin = 4;
const int servo2PwmPin = 5;

static const unsigned long debounceDelayMs = 50;

Servo servo1;
Servo servo2;

int lastBtn1Reading = HIGH;
int lastBtn2Reading = HIGH;

int btn1StableState = HIGH;
int btn2StableState = HIGH;

unsigned long btn1LastChangeMs = 0;
unsigned long btn2LastChangeMs = 0;

void updateButton(int pin, int currentReading, int &stableState, int &lastReading, unsigned long &lastChangeMs) {
  if (currentReading != lastReading) {
    lastReading = currentReading;
    lastChangeMs = millis();
  }

  if ((millis() - lastChangeMs) >= debounceDelayMs) {
    stableState = currentReading;
  }
}

int potToAngle(int potReading) {
  // potReading expected 0..1023 => angle 0..180
  long angle = map(potReading, 0, 1023, 0, 180);
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return (int)angle;
}

void setup() {
  pinMode(btn1Pin, INPUT_PULLUP);
  pinMode(btn2Pin, INPUT_PULLUP);

  servo1.attach(servo1PwmPin);
  servo2.attach(servo2PwmPin);

  servo1.write(0);
  servo2.write(0);

  // Initialize debounce trackers with current readings
  lastBtn1Reading = digitalRead(btn1Pin);
  lastBtn2Reading = digitalRead(btn2Pin);
  btn1StableState = lastBtn1Reading;
  btn2StableState = lastBtn2Reading;
  btn1LastChangeMs = millis();
  btn2LastChangeMs = millis();
}

void loop() {
  // Read raw states
  int btn1Reading = digitalRead(btn1Pin);
  int btn2Reading = digitalRead(btn2Pin);

  // Debounce
  updateButton(btn1Pin, btn1Reading, btn1StableState, lastBtn1Reading, btn1LastChangeMs);
  updateButton(btn2Pin, btn2Reading, btn2StableState, lastBtn2Reading, btn2LastChangeMs);

  // Detect press edges (stable HIGH->LOW because INPUT_PULLUP)
  static int prevBtn1Stable = HIGH;
  static int prevBtn2Stable = HIGH;

  bool btn1Pressed = (prevBtn1Stable == HIGH && btn1StableState == LOW);
  bool btn2Pressed = (prevBtn2Stable == HIGH && btn2StableState == LOW);

  if (btn1Pressed) {
    int pot1Val = analogRead(pot1Pin);
    servo1.write(potToAngle(pot1Val));
  }

  if (btn2Pressed) {
    int pot2Val = analogRead(pot2Pin);
    servo2.write(potToAngle(pot2Val));
  }

  prevBtn1Stable = btn1StableState;
  prevBtn2Stable = btn2StableState;
}