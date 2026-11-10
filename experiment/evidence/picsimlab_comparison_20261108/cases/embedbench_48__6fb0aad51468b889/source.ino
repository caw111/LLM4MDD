#include <Arduino.h>
#include <Servo.h>

static const uint8_t RGB1_R_PIN = 2;   // io_2
static const uint8_t RGB1_G_PIN = 3;   // io_3
static const uint8_t RGB1_B_PIN = 4;   // io_4 (off)
static const uint8_t RGB2_R_PIN = 5;   // io_5
static const uint8_t RGB2_G_PIN = 6;   // io_6
static const uint8_t RGB2_B_PIN = 7;   // io_7 (off)
static const uint8_t RGB3_R_PIN = 8;   // io_8
static const uint8_t RGB3_G_PIN = 9;   // io_9
static const uint8_t RGB3_B_PIN = 10;  // io_10 (off)

static const uint8_t POT1_PIN = A3;    // io_0
static const uint8_t POT2_PIN = A4;    // io_1

static const uint8_t SERVO1_PIN = 11;  // io_11
static const uint8_t SERVO2_PIN = 12;  // io_12
static const uint8_t SERVO3_PIN = 13;  // io_13

// Control period required by frozen contract
static const uint32_t CONTROL_PERIOD_MS = 100;

// ACCEPTED executable model state
static bool servo3State = false; // initial false; tick assigns servo3State := servo3State (no change)

static Servo servo1;
static Servo servo2;
static Servo servo3;

static inline float clamp01_255_to_unit(float x) {
  // Contract uses comparisons and proportional expression on [0,255].
  // We still guard numeric issues for embedded analog reads.
  if (x < 0.0f) return 0.0f;
  if (x > 255.0f) return 255.0f;
  return x;
}

// RealFunctions::floor(io_0) mapping to pwm_count outputs (0..255)
static inline int floor_to_int(float x) {
  if (x <= 0.0f) return 0;
  if (x >= 255.0f) return 255;
  // floor for non-negative values
  return (int)x;
}

static inline float servoMap0_255_to_0_180(float adcCount) {
  // io_0 <= 0 => 0; io_0 >= 255 => 180; else io_0 * 180 / 255
  if (adcCount <= 0.0f) return 0.0f;
  if (adcCount >= 255.0f) return 180.0f;
  return adcCount * 180.0f / 255.0f;
}

void setup() {
  // PWM outputs for RGB red/green channels
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);

  // Digital outputs for blue channels (active_high, but model keeps false)
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Ensure blue outputs are off initially
  digitalWrite(RGB1_B_PIN, LOW); // io_4 = false
  digitalWrite(RGB2_B_PIN, LOW); // io_7 = false
  digitalWrite(RGB3_B_PIN, LOW); // io_10 = false

  // Analog inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Servo attachments
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  // Initialize with first control step outputs
  uint32_t startMs = millis();
  (void)startMs;
}

void loop() {
  static uint32_t nextTickMs = 0;
  uint32_t now = millis();
  if ((int32_t)(now - nextTickMs) < 0) return;
  nextTickMs = now + CONTROL_PERIOD_MS;

  // Sample inputs (io_0, io_1) as adc_count in contract semantics
  // analogRead returns 0..1023 on Mega; scale to 0..255 to match contract expressions.
  int raw1 = analogRead(POT1_PIN);
  int raw2 = analogRead(POT2_PIN);

  float io_0 = (raw1 * 255.0f) / 1023.0f;
  float io_1 = (raw2 * 255.0f) / 1023.0f;

  io_0 = clamp01_255_to_unit(io_0);
  io_1 = clamp01_255_to_unit(io_1);

  // Controller tick: assign servo3State := servo3State (no change)
  servo3State = servo3State;

  // Outputs per frozen model

  // io_2 = floor(io_0)
  int io_2 = floor_to_int(io_0);
  // io_3 = 0.0
  float io_3 = 0.0f;
  // io_4 = false
  bool io_4 = false;

  // io_5 = floor(io_0)
  int io_5 = floor_to_int(io_0);
  // io_6 = 0.0
  float io_6 = 0.0f;
  // io_7 = false
  bool io_7 = false;

  // io_8 = floor(io_0)
  int io_8 = floor_to_int(io_0);
  // io_9 = 0.0
  float io_9 = 0.0f;
  // io_10 = false
  bool io_10 = false;

  // io_11 = map io_0 0..255 -> 0..180 with boundary checks
  float io_11 = servoMap0_255_to_0_180(io_0);
  // io_12 = map io_1 0..255 -> 0..180 with boundary checks
  float io_12 = servoMap0_255_to_0_180(io_1);
  // io_13 = servo3State ? 180 : 0
  float io_13 = servo3State ? 180.0f : 0.0f;

  // Drive hardware according to exact pin bindings
  analogWrite(RGB1_R_PIN, io_2);
  analogWrite(RGB1_G_PIN, (int)io_3); // forced 0
  digitalWrite(RGB1_B_PIN, io_4 ? HIGH : LOW);

  analogWrite(RGB2_R_PIN, io_5);
  analogWrite(RGB2_G_PIN, (int)io_6); // forced 0
  digitalWrite(RGB2_B_PIN, io_7 ? HIGH : LOW);

  analogWrite(RGB3_R_PIN, io_8);
  analogWrite(RGB3_G_PIN, (int)io_9); // forced 0
  digitalWrite(RGB3_B_PIN, io_10 ? HIGH : LOW);

  // Servo outputs
  servo1.write((int)io_11);
  servo2.write((int)io_12);
  servo3.write((int)io_13);
}