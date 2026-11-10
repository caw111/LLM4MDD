#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_SR_SHCP = A2;   // shift register shift clock
static const uint8_t PIN_SR_STCP = A1;   // shift register latch clock
static const uint8_t PIN_SR_DS   = A0;   // shift register data
static const uint8_t PIN_POT1    = A3;   // pot1 -> 0..9
static const uint8_t PIN_POT2    = A4;   // pot2 -> servo angles

// SevSeg mapping to 74HC595 outputs Q0..Q6 (A..G)
static const uint8_t QA = 0; // Q0 -> A
static const uint8_t QB = 1; // Q1 -> B
static const uint8_t QC = 2; // Q2 -> C
static const uint8_t QD = 3; // Q3 -> D
static const uint8_t QE = 4; // Q4 -> E
static const uint8_t QF = 5; // Q5 -> F
static const uint8_t QG = 6; // Q6 -> G

// Common cathode assumption: '1' means segment ON.
// If your display is common anode, invert segments.
static const uint8_t segDigit[10] = {
  // GFEDCBA (bit0=A ... bit6=G)
  // digits: 0..9 with segments A,B,C,D,E,F,G
  // 0: A B C D E F (no G)
  (1u<<QA)|(1u<<QB)|(1u<<QC)|(1u<<QD)|(1u<<QE)|(1u<<QF),
  // 1: B C
  (1u<<QB)|(1u<<QC),
  // 2: A B D E G
  (1u<<QA)|(1u<<QB)|(1u<<QD)|(1u<<QE)|(1u<<QG),
  // 3: A B C D G
  (1u<<QA)|(1u<<QB)|(1u<<QC)|(1u<<QD)|(1u<<QG),
  // 4: B C F G
  (1u<<QB)|(1u<<QC)|(1u<<QF)|(1u<<QG),
  // 5: A C D F G
  (1u<<QA)|(1u<<QC)|(1u<<QD)|(1u<<QF)|(1u<<QG),
  // 6: A C D E F G
  (1u<<QA)|(1u<<QC)|(1u<<QD)|(1u<<QE)|(1u<<QF)|(1u<<QG),
  // 7: A B C
  (1u<<QA)|(1u<<QB)|(1u<<QC),
  // 8: A B C D E F G
  (1u<<QA)|(1u<<QB)|(1u<<QC)|(1u<<QD)|(1u<<QE)|(1u<<QF)|(1u<<QG),
  // 9: A B C D F G
  (1u<<QA)|(1u<<QB)|(1u<<QC)|(1u<<QD)|(1u<<QF)|(1u<<QG)
};

#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t srWriteByte(uint8_t value);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(uint8_t d);
#line 75 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int clampAngle(int a, int lo, int hi);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static int smoothStep(int current, int target, int maxStep);
#line 87 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint16_t readPot(uint8_t pin);
#line 100 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 135 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 46 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_37\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t srWriteByte(uint8_t value) {
  // Write 8 bits to 74HC595 using manual bit-banging (DS -> Q0..Q7)
  // Arduino Mega 2560: A0/A1/A2 used as digital pins here.
  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);

  for (int8_t i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_DS, (value >> i) & 0x01);
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
  return value;
}

static void displayDigit(uint8_t d) {
  if (d > 9) d = 0;

  // sr outputs Q0..Q6 drive A..G.
  // Byte bits: bit0 -> Q0, ..., bit7 -> Q7 (unused).
  uint8_t segMask = segDigit[d];
  srWriteByte(segMask); // bit7 assumed 0
}

Servo servo1, servo2, servo3;

// Servo constraints per requirements.
static int clampAngle(int a, int lo, int hi) {
  if (a < lo) return lo;
  if (a > hi) return hi;
  return a;
}

static int smoothStep(int current, int target, int maxStep) {
  if (current == target) return current;
  if (current < target) return min(current + maxStep, target);
  return max(current - maxStep, target);
}

static uint16_t readPot(uint8_t pin) {
  return analogRead(pin);
}

int currentDigit = -1;
int lastDigit = -1;

int curS1 = 0, curS2 = 90, curS3 = 180;
int tgtS1 = 0, tgtS2 = 90, tgtS3 = 180;

unsigned long digitHoldStart = 0;
unsigned long servoHoldStart = 0;

void setup() {
  // Shift register pins
  pinMode(PIN_SR_SHCP, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_DS, OUTPUT);

  digitalWrite(PIN_SR_SHCP, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_DS, LOW);

  // Pots
  analogReference(DEFAULT);

  // Servos (wiring: mega:2,3,4 to PWM)
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);

  // Initialize to clamped start positions
  curS1 = 0;
  curS2 = 90;
  curS3 = 180;

  servo1.write(curS1);
  servo2.write(curS2);
  servo3.write(curS3);

  // Initialize display
  displayDigit(0);

  // Start hold timers
  digitHoldStart = millis();
  servoHoldStart = millis();
}

void loop() {
  // Read potentiometers continuously and update behavior.
  uint16_t p1 = readPot(PIN_POT1); // 0..1023
  uint16_t p2 = readPot(PIN_POT2); // 0..1023

  // Map pot1 to 0..9
  int mappedDigit = (int)map((long)p1, 0, 1023, 0, 9);
  if (mappedDigit < 0) mappedDigit = 0;
  if (mappedDigit > 9) mappedDigit = 9;

  // Compute servo target angles from pot2
  int angle1 = (int)map((long)p2, 0, 1023, 0, 90);
  int angle2 = (int)map((long)p2, 0, 1023, 90, 180);
  int angle3 = (int)map((long)p2, 0, 1023, 180, 270);

  // If servo doesn't support 270, clamp to 180 as per requirements.
  angle3 = clampAngle(angle3, 180, 180);

  tgtS1 = clampAngle(angle1, 0, 90);
  tgtS2 = clampAngle(angle2, 90, 180);
  tgtS3 = clampAngle(angle3, 180, 180);

  // 7-seg update continuously as pot1 adjusted, but keep state at least 2 seconds
  if (mappedDigit != lastDigit) {
    // Start/refresh 2s hold timer on state change
    digitHoldStart = millis();
    lastDigit = mappedDigit;
  }

  // Update display immediately on change, then ensure it stays at least 2 seconds.
  displayDigit(mappedDigit);
  currentDigit = mappedDigit;

  // Servo smooth movement with 2s minimum state maintenance:
  // - Update target continuously from pot2.
  // - Move servos smoothly each loop toward latest target.
  // - Also ensure that once a target is changed, we allow at least 2 seconds
  //   where movement continues toward that target (no abrupt changes/teleport).
  // Since we always smooth-step toward current targets, this naturally preserves
  // the state. Additionally, we can enforce a minimum hold by not updating
  // targets within the 2s window after they changed.
  static int heldTgtS1 = 0, heldTgtS2 = 90, heldTgtS3 = 180;
  static int prevTgtS1 = -999, prevTgtS2 = -999, prevTgtS3 = -999;

  bool targetChanged = (tgtS1 != prevTgtS1) || (tgtS2 != prevTgtS2) || (tgtS3 != prevTgtS3);

  unsigned long now = millis();
  if (targetChanged) {
    servoHoldStart = now;
    heldTgtS1 = tgtS1;
    heldTgtS2 = tgtS2;
    heldTgtS3 = tgtS3;
    prevTgtS1 = tgtS1;
    prevTgtS2 = tgtS2;
    prevTgtS3 = tgtS3;
  } else {
    // If 2 seconds have elapsed since last change, allow new targets to be held.
    if ((now - servoHoldStart) >= 2000UL) {
      heldTgtS1 = tgtS1;
      heldTgtS2 = tgtS2;
      heldTgtS3 = tgtS3;
      prevTgtS1 = tgtS1;
      prevTgtS2 = tgtS2;
      prevTgtS3 = tgtS3;
    }
  }

  // Smoothly move toward held targets.
  // Choose small step for smoother movement.
  const int maxStepPerLoop = 1;
  curS1 = smoothStep(curS1, heldTgtS1, maxStepPerLoop);
  curS2 = smoothStep(curS2, heldTgtS2, maxStepPerLoop);
  curS3 = smoothStep(curS3, heldTgtS3, maxStepPerLoop);

  servo1.write(curS1);
  servo2.write(curS2);
  servo3.write(curS3);

  // Ensure digit stays at least 2 seconds per state:
  // If mappedDigit changes again quickly, this requirement would be violated
  // unless we "hold" the displayed digit for 2 seconds.
  static int heldDigit = 0;
  static int prevDigit = -1;
  if (mappedDigit != prevDigit) {
    digitHoldStart = millis();
    heldDigit = mappedDigit;
    prevDigit = mappedDigit;
  } else {
    if ((millis() - digitHoldStart) >= 2000UL) {
      heldDigit = mappedDigit;
      digitHoldStart = millis(); // keep consistent window for next update
    }
  }
  displayDigit(heldDigit);

  delay(10);
}
