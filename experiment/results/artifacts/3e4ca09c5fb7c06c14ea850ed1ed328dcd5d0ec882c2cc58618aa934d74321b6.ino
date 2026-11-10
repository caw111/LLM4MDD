#include <Servo.h>

Servo servo1;
Servo servo2;

// Servo angle tracking
int angle1 = 0;
int angle2 = 0;

// RGB LED pins (from wiring)
static const uint8_t rgb1_R = 2;
static const uint8_t rgb1_G = 3;
static const uint8_t rgb1_B = 4;

static const uint8_t rgb2_R = 5;
static const uint8_t rgb2_G = 6;
static const uint8_t rgb2_B = 7;

// Bargraph pins (from wiring)
static const uint8_t bar_A[6] = {10, 11, 12, 13, 44, 45};
static const uint8_t bar_C[6] = {0, 0, 0, 0, 0, 0}; // C pins all tied to GND in wiring

// Timing
static const unsigned long STEP_INTERVAL_MS = 2000;

unsigned long lastStepMs = 0;

void setRGB_off_all() {
  // Common cathode: HIGH turns channel ON.
  digitalWrite(rgb1_R, LOW);
  digitalWrite(rgb1_G, LOW);
  digitalWrite(rgb1_B, LOW);

  digitalWrite(rgb2_R, LOW);
  digitalWrite(rgb2_G, LOW);
  digitalWrite(rgb2_B, LOW);
}

void setRGB_fixedColors() {
  // rgb1 red, rgb2 blue (common cathode).
  digitalWrite(rgb1_R, HIGH);
  digitalWrite(rgb1_G, LOW);
  digitalWrite(rgb1_B, LOW);

  digitalWrite(rgb2_R, LOW);
  digitalWrite(rgb2_G, LOW);
  digitalWrite(rgb2_B, HIGH);
}

void setBargraphLitCount(int litCount) {
  // litCount in [0..6]
  if (litCount < 0) litCount = 0;
  if (litCount > 6) litCount = 6;

  for (int i = 0; i < 6; i++) {
    // Wiring uses A1..A6 to drive each segment; C pins are on GND.
    // For bargraph module in this circuit, setting A pin HIGH lights that segment.
    digitalWrite(bar_A[i], (i < litCount) ? HIGH : LOW);
  }
}

int averageAngleToLitCount(int a1, int a2) {
  // Requirement: number of lit LEDs corresponds to the average angle of the servos.
  // Given mapping:
  // - at [0°, 90°] => first RGB LED is lit (and all bar LEDs reset when servos return to 0°)
  // - when at (90°, 180°] => both RGB LEDs are lit
  //
  // For the 6-segment bargraph:
  // Use average angle in degrees:
  // 0..90 => 1..(something). Requirement does not define exact count mapping besides
  // "first RGB LED is lit" for [0,90] and "both RGB LEDs" for (90,180] and reset at 0°.
  //
  // To preserve behavior consistency with stated rule:
  // - Compute litCount based on average angle proportionally over 6 LEDs:
  //   litCount = floor((avgAngle / 180) * 6) + (avgAngle > 0 ? 1 : 0)
  // - Then ensure at avgAngle == 0 -> 0 lit.
  //
  // This yields 1 lit for avgAngle in [0,30] and increases thereafter. However, the
  // requirement only explicitly constrains reset at 0° and average-angle threshold for
  // RGB LEDs. We'll apply proportional mapping for the bargraph.
  int avg = (a1 + a2) / 2; // integer degrees
  if (a1 == 0 && a2 == 0) return 0;

  // Map avg in (0..180] to 1..6
  // avg==180 -> 6
  // avg==1..30 -> 1, etc.
  int lit = (avg * 6) / 180;
  if (lit < 1) lit = 1;
  if (lit > 6) lit = 6;
  return lit;
}

void updateOutputs() {
  // RGB behavior:
  // rgb1 red always.
  // rgb2 blue only when servos average angle is in (90, 180].
  // But detail rule says "rgb1 show red color" and "rgb2 show blue color".
  // Also it says: both LEDs are reset when servos return to 0°.
  //
  // Therefore:
  // - At [0°, 90°] (including 0°): rgb1 is red; rgb2 is off.
  // - At (90°, 180°]: rgb1 red and rgb2 blue.
  // - When at [0°,0°]: both RGB LEDs off.
  if (angle1 == 0 && angle2 == 0) {
    setRGB_off_all();
  } else {
    // rgb1 red always once leaving 0°
    digitalWrite(rgb1_R, HIGH);
    digitalWrite(rgb1_G, LOW);
    digitalWrite(rgb1_B, LOW);

    int avg = (angle1 + angle2) / 2;
    if (avg > 90 && avg <= 180) {
      // rgb2 blue
      digitalWrite(rgb2_R, LOW);
      digitalWrite(rgb2_G, LOW);
      digitalWrite(rgb2_B, HIGH);
    } else {
      // avg in 1..90 => rgb2 off
      digitalWrite(rgb2_R, LOW);
      digitalWrite(rgb2_G, LOW);
      digitalWrite(rgb2_B, LOW);
    }
  }

  // Bargraph display:
  // "number of lit LEDs corresponds to average angle of the servos."
  // "All LEDs reset when the servos return to 0°."
  // We'll light segments proportionally, and ensure zero at [0,0].
  int litCount = averageAngleToLitCount(angle1, angle2);
  setBargraphLitCount(litCount);
}

void stepServos() {
  angle1 += 30;
  angle2 += 30;

  if (angle1 > 180) angle1 = 0;
  if (angle2 > 180) angle2 = 0;

  // If we want cycle precisely at 180:
  // After reaching 180°, reset to 0°.
  if (angle1 == 180) {
    // next step will bump to 210 then get reset by >180 rule.
    // However, the requirement states reset after reaching 180°.
    // We'll implement that by resetting immediately after setting 180? No,
    // requirement says "After reaching 180°, both servos reset to 0° and repeat the cycle."
    // That implies 180° is a state; next 2s cycle resets. Our logic resets when >180,
    // so it will reach 180° for one step interval then reset next interval.
  }

  servo1.write(angle1);
  servo2.write(angle2);

  updateOutputs();
}

void setup() {
  // Attach servos to PWM pins from wiring.
  servo1.attach(8);   // mega:8 -> servo1:PWM
  servo2.attach(9);   // mega:9 -> servo2:PWM

  // Initialize both servos to 0°.
  angle1 = 0;
  angle2 = 0;
  servo1.write(angle1);
  servo2.write(angle2);

  // RGB LED pins (common cathode).
  pinMode(rgb1_R, OUTPUT);
  pinMode(rgb1_G, OUTPUT);
  pinMode(rgb1_B, OUTPUT);

  pinMode(rgb2_R, OUTPUT);
  pinMode(rgb2_G, OUTPUT);
  pinMode(rgb2_B, OUTPUT);

  // Start with both RGB LEDs off.
  setRGB_off_all();

  // Bargraph pins.
  for (int i = 0; i < 6; i++) {
    pinMode(bar_A[i], OUTPUT);
  }
  // Start with no lit LEDs.
  setBargraphLitCount(0);

  lastStepMs = millis();
}

void loop() {
  unsigned long now = millis();
  if (now - lastStepMs >= STEP_INTERVAL_MS) {
    lastStepMs = now;

    // Increment both servos by 30° simultaneously; reset after reaching 180° and repeat.
    // Our stepping reaches 180, stays for one interval, then resets next interval.
    stepServos();
  }
}