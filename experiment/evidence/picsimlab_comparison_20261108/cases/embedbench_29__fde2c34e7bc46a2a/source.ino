#include <Arduino.h>

// Frozen interface contract bindings
// Inputs (analog)
// io_0 (pot1) -> A3
// io_1 (pot2) -> A4
// io_2 (pot3) -> A5

static const uint8_t PIN_POT1 = A3;
static const uint8_t PIN_POT2 = A4;
static const uint8_t PIN_POT3 = A5;

// Outputs (active-high digital)
// io_3_segment_0..9 -> D2..D11 respectively
static const uint8_t LED_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static volatile bool io_3_segment[10] = {false, false, false, false, false, false, false, false, false, false};

static int ledCountSec1 = 0;
static int ledCountSec2 = 0;
static int ledCountSec3 = 0;

// Match the accepted model mapping exactly (with boundary handling):
// Section 1 & 2: thresholds 0, 256, 512, 768, 1023 with special cases: <=0 =>0, >=1023 => max(3)
static int mapPotToCount0to3(double v) {
  if (v <= 0.0) return 0;
  if (v >= 1023.0) return 3;
  if (v < 256.0) return 0;
  if (v < 512.0) return 1;
  if (v < 768.0) return 2;
  return 3;
}

// Section 3: thresholds with special cases: <=0 =>0, >=1023 => max(4)
static int mapPotToCount0to4(double v) {
  if (v <= 0.0) return 0;
  if (v >= 1023.0) return 4;
  if (v < 256.0) return 0;
  if (v < 512.0) return 1;
  if (v < 768.0) return 2;
  return 3;
}

static void applyOutputsFromModel() {
  // outputs are direct comparisons (sequential segments within each section)
  io_3_segment[0] = (ledCountSec1 >= 1);
  io_3_segment[1] = (ledCountSec1 >= 2);
  io_3_segment[2] = (ledCountSec1 >= 3);

  io_3_segment[3] = (ledCountSec2 >= 1);
  io_3_segment[4] = (ledCountSec2 >= 2);
  io_3_segment[5] = (ledCountSec2 >= 3);

  io_3_segment[6] = (ledCountSec3 >= 1);
  io_3_segment[7] = (ledCountSec3 >= 2);
  io_3_segment[8] = (ledCountSec3 >= 3);
  io_3_segment[9] = (ledCountSec3 >= 4);

  // Drive the hardware pins (active_high=true => HIGH means on)
  for (int i = 0; i < 10; ++i) {
    digitalWrite(LED_PINS[i], io_3_segment[i] ? HIGH : LOW);
  }
}

// model tick order preserved by sequential assignments in one function
static void tick(double io_0, double io_1, double io_2) {
  ledCountSec1 = mapPotToCount0to3(io_0);
  ledCountSec2 = mapPotToCount0to3(io_1);
  ledCountSec3 = mapPotToCount0to4(io_2);

  applyOutputsFromModel();
}

void setup() {
  // Per contract: initial state all LEDs off at startup/reset.
  for (int i = 0; i < 10; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  // ADC inputs
  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_POT3, INPUT);
}

void loop() {
  // Control period: 5 ms
  // (Continuous updates; this implements a fixed-rate loop.)
  static unsigned long nextTickMs = 0;
  unsigned long now = millis();
  if (now < nextTickMs) return;
  nextTickMs = now + 5;

  // Read analog inputs (0..1023), register as f64 (double)
  int raw0 = analogRead(PIN_POT1);
  int raw1 = analogRead(PIN_POT2);
  int raw2 = analogRead(PIN_POT3);

  double io_0 = (double)raw0;
  double io_1 = (double)raw1;
  double io_2 = (double)raw2;

  // Update using the accepted model logic (ordered updates)
  tick(io_0, io_1, io_2);
}