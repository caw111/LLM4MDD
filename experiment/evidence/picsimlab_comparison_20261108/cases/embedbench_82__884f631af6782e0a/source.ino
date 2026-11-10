#include <Arduino.h>
#include <Servo.h>

static const uint8_t BTN1_PIN = 2;  // io_0
static const uint8_t BTN2_PIN = 3;  // io_1
static const uint8_t BTN3_PIN = 4;  // io_2

static const uint8_t RGB_R_PIN = 5; // io_3
static const uint8_t RGB_G_PIN = 6; // io_4
static const uint8_t RGB_B_PIN = 7; // io_5

static const uint8_t SERVO1_PIN = 8; // io_6
static const uint8_t SERVO2_PIN = 9; // io_7

// Controller state (from ACCEPTED model)
static bool previousPressed1 = false;
static bool previousPressed2 = false;
static bool previousPressed3 = false;
static double holdPressedSeconds = 0.0;
static int activeState = 0; // 0=idle/none; 1=btn1; 2=btn2; 3=btn3

// Outputs (derived from activeState) mapped directly each tick
static float io_3 = 0.0f; // red channel pwm_count (0..255-like)
static float io_4 = 0.0f; // green channel
static float io_5 = 0.0f; // blue channel
static float io_6 = 0.0f; // servo1 target angle deg
static float io_7 = 0.0f; // servo2 target angle deg

// Servo objects
static Servo servo1;
static Servo servo2;

// Scheduling granularity: control_period_ms = 10
static const uint32_t CONTROL_PERIOD_MS = 10;

static void applyOutputsFromModel() {
  io_3 = (activeState == 1) ? 1.0f : 0.0f;
  io_4 = (activeState == 2) ? 1.0f : 0.0f;
  io_5 = (activeState == 3) ? 1.0f : 0.0f;

  io_6 = ((activeState == 1) || (activeState == 2)) ? 90.0f : 0.0f;
  io_7 = (activeState == 2) ? 90.0f : 0.0f;

  // RGB LED common cathode:
  // Model uses pwm_count where 0 means off. Set exactly one channel on.
  // Using digitalWrite to guarantee exact on/off behavior; PWM not required.
  digitalWrite(RGB_R_PIN, (io_3 > 0.5f) ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, (io_4 > 0.5f) ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, (io_5 > 0.5f) ? HIGH : LOW);

  // Servo angle outputs
  servo1.write((int)io_6);
  servo2.write((int)io_7);
}

static void tickController(bool io_0, bool io_1, bool io_2) {
  // Model update logic (trace-accurate)
  if (io_2 && !previousPressed3) {
    activeState = 3;
    holdPressedSeconds = 0.0;
  }
  if (io_0 && !previousPressed1) {
    activeState = 1;
    holdPressedSeconds = 0.0;
  }
  if (io_1 && !previousPressed2) {
    activeState = 2;
    holdPressedSeconds = 0.0;
  }
  if (!(io_2 || io_0 || io_1)) {
    holdPressedSeconds = 2.0;
  }
  if (io_2 || io_0 || io_1) {
    holdPressedSeconds = 2.0;
  }

  previousPressed1 = io_0;
  previousPressed2 = io_1;
  previousPressed3 = io_2;
}

void setup() {
  // Inputs with pullups, active_high=false => pressed when pin reads LOW
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // RGB pins as outputs (common cathode)
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Initialize controller outputs per model: servo angles = 0, rgb channels = 0 (off)
  activeState = 0;
  previousPressed1 = false;
  previousPressed2 = false;
  previousPressed3 = false;
  holdPressedSeconds = 0.0;

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Ensure initial positions are at 0 degrees
  servo1.write(0);
  servo2.write(0);

  // Ensure RGB is off (all channels initial=0)
  digitalWrite(RGB_R_PIN, LOW);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);

  // Apply outputs from model (consistent with above)
  applyOutputsFromModel();
}

void loop() {
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if (now < nextTick) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // Read model inputs with polarity: active_high=false means pressed corresponds to LOW -> model true
  bool io_0 = (digitalRead(BTN1_PIN) == LOW);
  bool io_1 = (digitalRead(BTN2_PIN) == LOW);
  bool io_2 = (digitalRead(BTN3_PIN) == LOW);

  tickController(io_0, io_1, io_2);
  applyOutputsFromModel();
}