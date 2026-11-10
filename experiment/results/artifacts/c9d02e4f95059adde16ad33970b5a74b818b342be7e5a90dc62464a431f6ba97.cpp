#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

static const int pot1Pin = A3;
static const int pot2Pin = A4;

static const int servo1Pin = 2;
static const int servo2Pin = 3;
static const int servo3Pin = 4;

static const int ledPins[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 44};

Servo servo1;
Servo servo2;
Servo servo3;

#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readPotAngle(int pin);
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int computeBarSegments(int a1, int a2);
#line 34 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setServosToAngle(int a1, int a2, int a3);
#line 40 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setBarSegments(int segments);
#line 48 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 72 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 16 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_31\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
int readPotAngle(int pin) {
  // Map potentiometer ADC reading (0..1023) to angle (0..180)
  int raw = analogRead(pin);
  raw = constrain(raw, 0, 1023);
  return (raw * 180 + 511) / 1023; // rounded mapping
}

int computeBarSegments(int a1, int a2) {
  // Average rounded to nearest integer, then scale to 0..10 segments
  // Let avgDeg = round((a1 + a2)/2). Then segments = round(avgDeg * 10/180).
  int sum = a1 + a2;
  int avgDeg = (sum + 1) / 2; // nearest integer
  avgDeg = constrain(avgDeg, 0, 180);

  int segments = (avgDeg * 10 + 90) / 180; // rounded mapping
  return constrain(segments, 0, 10);
}

void setServosToAngle(int a1, int a2, int a3) {
  servo1.write(constrain(a1, 0, 180));
  servo2.write(constrain(a2, 0, 180));
  servo3.write(constrain(a3, 0, 180));
}

void setBarSegments(int segments) {
  // Segment order: A1 to A10 correspond to ledPins[0..9]
  // 0 = all off, 10 = all on
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPins[i], (i < segments) ? HIGH : LOW);
  }
}

void setup() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);

  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  pinMode(ledPins[3], OUTPUT);
  pinMode(ledPins[4], OUTPUT);
  pinMode(ledPins[5], OUTPUT);
  pinMode(ledPins[6], OUTPUT);
  pinMode(ledPins[7], OUTPUT);
  pinMode(ledPins[8], OUTPUT);
  pinMode(ledPins[9], OUTPUT);

  // Initialization: servos at 0 degrees, LED bar fully off.
  setServosToAngle(0, 0, 0);
  setBarSegments(0);

  // Ensure ADC is ready
  analogReference(DEFAULT);
}

void loop() {
  int a1 = readPotAngle(pot1Pin); // servo1
  int a2 = readPotAngle(pot2Pin); // servo2

  // servo3 angle = average of pot1 and pot2, rounded to nearest integer
  int avgRounded = (a1 + a2 + 1) / 2;
  avgRounded = constrain(avgRounded, 0, 180);

  setServosToAngle(a1, a2, avgRounded);

  // LED segments correspond to average potentiometer value scaled to 0..10 segments
  int segments = computeBarSegments(a1, a2);
  setBarSegments(segments);

  delay(20);
}
