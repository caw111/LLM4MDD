#include <Arduino.h>
#include <Servo.h>

static const uint8_t POT_PIN = A3;   // pot1 SIG -> mega:A3
static const uint8_t SERVO1_PIN = 2; // contract: io_1 -> D2
static const uint8_t SERVO2_PIN = 3; // contract: io_2 -> D3

// Bargraph segments A1..A10 -> D4..D13 respectively
static const uint8_t LED_PINS[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

static Servo servo1;
static Servo servo2;

static float readPotADCtoF64() {
  // io_0 is f64 adc_count. Read 0..1023, keep as float.
  int v = analogRead(POT_PIN);
  if (v < 0) v = 0;
  if (v > 1023) v = 1023;
  return (float)v;
}

// Integer floor(x) for non-negative x; safe for our domain.
static int iFloorNonNeg(float x) {
  int i = (int)x;
  // x is expected non-negative; guard against any float edge cases.
  if (i < 0) i = 0;
  // If x is exactly representable as integer, (int)x == x.
  // Otherwise (int) is truncation toward 0 which equals floor for non-negative values.
  return i;
}

static void setServoDegrees(float deg1, float deg2) {
  servo1.write((int)deg1);
  servo2.write((int)deg2);
}

static void setBargraphCount(int ledCount /* 0..10 */) {
  if (ledCount < 0) ledCount = 0;
  if (ledCount > 10) ledCount = 10;

  // Outputs io_3..io_12 correspond to ledCount >= 1..10
  // Light sequentially left (A1) to right (A10).
  for (int i = 0; i < 10; ++i) {
    bool on = (i + 1) <= ledCount;
    digitalWrite(LED_PINS[i], on ? HIGH : LOW);
  }
}

int main() {
  init();
  setup();
  while (true) {
    loop();
  }
  return 0;
}

void setup() {
  // Pin modes for bargraph LEDs
  for (int i = 0; i < 10; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW); // all LEDs off initially
  }

  // Attach servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Initialization requirement: Both servos must start at 0° when powered on.
  // Contract states initial servo angles are 0, but functional mapping at first tick
  // will correct based on pot position. Here we satisfy "start at 0°".
  setServoDegrees(0.0f, 0.0f);
}

void loop() {
  // Controller tick period target is 5ms. Keep simple fixed delay for exact control loop.
  unsigned long startMs = millis();

  float io0 = readPotADCtoF64(); // io_0 in [0..1023] as f64

  // ACCEPTED model updates:
  // servo1Angle:
  // if io_0 <= 0.0 => 0.0
  // else if io_0 >= 1023.0 => 180.0
  // else if io_0 == 1023.0 => 180.0
  // else => io_0 * 180.0 / 1023.0
  float servo1Angle;
  if (io0 <= 0.0f) {
    servo1Angle = 0.0f;
  } else if (io0 >= 1023.0f) {
    servo1Angle = 180.0f;
  } else if (io0 == 1023.0f) {
    servo1Angle = 180.0f;
  } else {
    servo1Angle = io0 * 180.0f / 1023.0f;
  }

  // servo2Angle:
  // if io_0 <= 0.0 => 180.0
  // else if io_0 >= 1023.0 => 0.0
  // else if io_0 == 1023.0 => 0.0
  // else => 180.0 - (io_0 * 180.0 / 1023.0)
  float servo2Angle;
  if (io0 <= 0.0f) {
    servo2Angle = 180.0f;
  } else if (io0 >= 1023.0f) {
    servo2Angle = 0.0f;
  } else if (io0 == 1023.0f) {
    servo2Angle = 0.0f;
  } else {
    servo2Angle = 180.0f - (io0 * 180.0f / 1023.0f);
  }

  // ledCount := floor(io_0 * 10.0 / 1023.0)
  float tmp = io0 * 10.0f / 1023.0f;
  int ledCount = iFloorNonNeg(tmp);

  // Drive outputs
  setServoDegrees(servo1Angle, servo2Angle);
  setBargraphCount(ledCount);

  // Enforce control period ~5ms
  unsigned long elapsed = millis() - startMs;
  if (elapsed < 5UL) {
    delay(5UL - elapsed);
  }
}