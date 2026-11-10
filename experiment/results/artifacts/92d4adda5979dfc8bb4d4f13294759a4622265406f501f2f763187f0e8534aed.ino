#include <Arduino.h>

static const uint8_t RGB1_R_PIN = 2; // io_2
static const uint8_t RGB1_G_PIN = 3; // io_3
static const uint8_t RGB1_B_PIN = 4; // io_4
static const uint8_t RGB2_R_PIN = 5; // io_5
static const uint8_t RGB2_G_PIN = 6; // io_6
static const uint8_t RGB2_B_PIN = 7; // io_7

static const uint8_t POT1_PIN = A3;  // io_0
static const uint8_t POT2_PIN = A4;  // io_1

// Controller state (ACCEPTED executable model)
static double rgb1R = 0.0;
static double rgb1G = 0.0;
static double rgb1B = 0.0;
static double rgb2R = 0.0;
static double rgb2G = 0.0;
static double rgb2B = 0.0;

static unsigned long lastTickMs = 0;

static inline void applyPwmOutputsToPins(double v, uint8_t pin) {
  // Outputs are "pwm_count" units with initial value 0.0.
  // Use Arduino analogWrite range 0..255.
  // Clamp to avoid any undefined behavior.
  if (v < 0.0) v = 0.0;
  if (v > 255.0) v = 255.0;
  analogWrite(pin, (uint8_t)lround(v));
}

void setup() {
  // Initialize RGB outputs to 0 (both LEDs off)
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);
  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);

  // Inputs (analog)
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  lastTickMs = millis();
}

void loop() {
  // control_period_ms = 5
  unsigned long now = millis();
  if (now - lastTickMs < 5) return;
  lastTickMs += 5;

  // Read inputs (io_0, io_1) to preserve behavior, even though controller model ignores them
  (void)analogRead(POT1_PIN);
  (void)analogRead(POT2_PIN);

  // ACCEPTED executable model tick:
  // first start; then assign rgb1R..rgb2B all to 0.0 in the specified ordered updates.
  rgb1R = 0.0;
  rgb1G = 0.0;
  rgb1B = 0.0;
  rgb2R = 0.0;
  rgb2G = 0.0;
  rgb2B = 0.0;

  // Ordered outputs mapping:
  // io_2..io_7 carry rgb1R..rgb2B
  applyPwmOutputsToPins(rgb1R, RGB1_R_PIN);
  applyPwmOutputsToPins(rgb1G, RGB1_G_PIN);
  applyPwmOutputsToPins(rgb1B, RGB1_B_PIN);
  applyPwmOutputsToPins(rgb2R, RGB2_R_PIN);
  applyPwmOutputsToPins(rgb2G, RGB2_G_PIN);
  applyPwmOutputsToPins(rgb2B, RGB2_B_PIN);
}