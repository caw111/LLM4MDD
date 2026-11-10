#include <Servo.h>

static const uint8_t SERVO1_PIN = 2; // mega:2
static const uint8_t SERVO2_PIN = 3; // mega:3
static const uint8_t SERVO3_PIN = 4; // mega:4

static const uint8_t POT1_PIN = A3;  // mega:A3
static const uint8_t POT2_PIN = A4;  // mega:A4

static const uint8_t MOVE_INTERVAL_MS = 20; // keep updates frequent for smooth motion
static const uint16_t ADC_MAX = 1023;

Servo servo1, servo2, servo3;

static inline int mapPotToAngle(int adc) {
  // Linear map 0..1023 -> 0..180 with rounding
  // angle = round(adc * 180 / 1023)
  long a = (long)adc * 180L;
  long ang = (a + (ADC_MAX / 2)) / ADC_MAX;
  if (ang < 0) ang = 0;
  if (ang > 180) ang = 180;
  return (int)ang;
}

static inline int clampInt(int v, int lo, int hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static int currentAngle1 = 0;
static int currentAngle2 = 0;
static int currentAngle3 = 0;

static int targetAngle1 = 0;
static int targetAngle2 = 0; // shared for servo2 & servo3
static int lastSampleP1 = -1;
static int lastSampleP2 = -1;

unsigned long lastMoveMs = 0;

void setup() {
  // Use internal pullups off; slide pots in the circuit have VCC/GND already wired.
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialize angles based on current potentiometer positions
  int adc1 = analogRead(POT1_PIN);
  int adc2 = analogRead(POT2_PIN);

  targetAngle1 = mapPotToAngle(adc1);
  targetAngle2 = mapPotToAngle(adc2);

  currentAngle1 = targetAngle1;
  currentAngle2 = targetAngle2;
  currentAngle3 = targetAngle2;

  servo1.write(currentAngle1);
  servo2.write(currentAngle2);
  servo3.write(currentAngle3);

  lastSampleP1 = adc1;
  lastSampleP2 = adc2;
  lastMoveMs = millis();
}

void loop() {
  // Sample potentiometers frequently to keep command response within 50 ms.
  // (At 20 ms interval below plus analogRead time, this stays well within the bound.)
  unsigned long now = millis();

  // Update targets whenever input changes (quantization tolerance not tightened beyond required).
  int adc1 = analogRead(POT1_PIN);
  int adc2 = analogRead(POT2_PIN);

  // Update targets (also ensures continuity of mapping).
  targetAngle1 = mapPotToAngle(adc1);
  int newTargetAngle2 = mapPotToAngle(adc2);

  if (adc1 != lastSampleP1 || adc2 != lastSampleP2) {
    lastSampleP1 = adc1;
    lastSampleP2 = adc2;
    targetAngle2 = newTargetAngle2;
  } else {
    // keep targetAngle2 consistent
    targetAngle2 = newTargetAngle2;
  }

  // Smooth motion: move a small step toward each target at a fixed interval.
  if (now - lastMoveMs >= MOVE_INTERVAL_MS) {
    lastMoveMs = now;

    // Step size selection:
    // With 20 ms updates, moving up to 1 degree per tick yields <= ~3.6 s for full-scale,
    // and still stays responsive while being smooth. Command update for mapping is immediate
    // via target computation above.
    const int stepMax = 1;

    // Servo1
    if (currentAngle1 < targetAngle1) currentAngle1 = min(targetAngle1, currentAngle1 + stepMax);
    else if (currentAngle1 > targetAngle1) currentAngle1 = max(targetAngle1, currentAngle1 - stepMax);

    // Servo2 & Servo3 in unison (shared targetAngle2)
    if (currentAngle2 < targetAngle2) currentAngle2 = min(targetAngle2, currentAngle2 + stepMax);
    else if (currentAngle2 > targetAngle2) currentAngle2 = max(targetAngle2, currentAngle2 - stepMax);

    currentAngle3 = currentAngle2;

    servo1.write(clampInt(currentAngle1, 0, 180));
    servo2.write(clampInt(currentAngle2, 0, 180));
    servo3.write(clampInt(currentAngle3, 0, 180));
  }

  // No blocking delays to ensure target command changes follow input within 50 ms.
}