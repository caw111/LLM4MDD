#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

const int btn1Pin = 2;
const int btn2Pin = 3;

const int pot1Pin = A3;
const int pot2Pin = A4;

const int servo1PwmPin = 4; // mega:4 -> servo1:PWM
const int servo2PwmPin = 5; // mega:5 -> servo2:PWM

Servo servo1;
Servo servo2;

#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readDebouncedButton(int pin, bool &stateChanged, bool &newState);
#line 47 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int mapPotToAngle(int raw);
#line 55 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
bool updateDebounced(DebouncedButton &b);
#line 73 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void handleButtonEdge(DebouncedButton &b, Servo &s, int potPin);
#line 85 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 15 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_71\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int readDebouncedButton(int pin, bool &stateChanged, bool &newState) {
  // Simple edge-detect with debounce
  static uint8_t stableStateBtn1 = 0;
  static unsigned long lastChangeTime = 0;
  static int lastReading = 0;
  (void)stableStateBtn1;
  (void)lastChangeTime;
  (void)lastReading;

  // Not used; kept to satisfy function signature if expanded.
  // We'll implement per-button debounce inline in loop().
  stateChanged = false;
  newState = false;
  return 0;
}

struct DebouncedButton {
  int pin;
  bool stableState;          // last stable state (pressed/released)
  bool lastReportedState;    // last state we reported to application
  int lastRawReading;
  unsigned long lastRawChangeTime;

  DebouncedButton(int p) : pin(p), stableState(false), lastReportedState(false),
                           lastRawReading(LOW), lastRawChangeTime(0) {}
};

static const unsigned long debounceMs = 50;

DebouncedButton btn1(btn1Pin);
DebouncedButton btn2(btn2Pin);

int mapPotToAngle(int raw) {
  // raw: 0..1023 (Mega ADC). Map to 0..180.
  long angle = map(raw, 0, 1023, 0, 180);
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  return (int)angle;
}

bool updateDebounced(DebouncedButton &b) {
  // Returns true if a stable state change occurred since last call.
  int raw = digitalRead(b.pin);

  if (raw != b.lastRawReading) {
    b.lastRawChangeTime = millis();
    b.lastRawReading = raw;
  }

  if ((millis() - b.lastRawChangeTime) >= debounceMs) {
    if (b.stableState != (raw == HIGH)) {
      b.stableState = (raw == HIGH);
      return true;
    }
  }
  return false;
}

void handleButtonEdge(DebouncedButton &b, Servo &s, int potPin) {
  // We act only on a transition to pressed (HIGH).
  if (updateDebounced(b)) {
    if (b.stableState && !b.lastReportedState) {
      int potRaw = analogRead(potPin);
      int angle = mapPotToAngle(potRaw);
      s.write(angle);
    }
    b.lastReportedState = b.stableState;
  }
}

void setup() {
  pinMode(btn1Pin, INPUT); // wiring uses external pull (not present in description), assume given circuit handles level
  pinMode(btn2Pin, INPUT);

  servo1.attach(servo1PwmPin);
  servo2.attach(servo2PwmPin);

  servo1.write(0);
  servo2.write(0);

  // Initialize button debounce states with current readings
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
