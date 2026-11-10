#include <Arduino.h>

//
// Frozen hardware contract mapping (common-cathode RGB LEDs, active LOW per channel via PWM):
// rgb1: R D2, G D3, B D4
// rgb2: R D5, G D6, B D7
// rgb3: R D8, G D9, B D10
//
static constexpr uint8_t RGB1_R_PIN = 2;
static constexpr uint8_t RGB1_G_PIN = 3;
static constexpr uint8_t RGB1_B_PIN = 4;

static constexpr uint8_t RGB2_R_PIN = 5;
static constexpr uint8_t RGB2_G_PIN = 6;
static constexpr uint8_t RGB2_B_PIN = 7;

static constexpr uint8_t RGB3_R_PIN = 8;
static constexpr uint8_t RGB3_G_PIN = 9;
static constexpr uint8_t RGB3_B_PIN = 10;

// Contract sample period: 10 ms (not strictly used as a barrier; behavior remains time-based).
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

// Model-required: initial outputs are 0.0 for all nine channels and must remain 0.0.
// Therefore, this sketch keeps all PWM channels off indefinitely.
static constexpr float IO_OFF = 0.0f;

struct Controller {
  float io_0 = IO_OFF; // rgb1 red
  float io_1 = IO_OFF; // rgb1 green
  float io_2 = IO_OFF; // rgb1 blue
  float io_3 = IO_OFF; // rgb2 red
  float io_4 = IO_OFF; // rgb2 green
  float io_5 = IO_OFF; // rgb2 blue
  float io_6 = IO_OFF; // rgb3 red
  float io_7 = IO_OFF; // rgb3 green
  float io_8 = IO_OFF; // rgb3 blue
};

static Controller controller;

// PWM: use analogWrite range 0..255. For active-low common-cathode LEDs,
// "off" corresponds to 0 PWM value on the channel output (as per contract initial=0).
static void applyOutputToPin(uint8_t pin, float pwm_count_f64) {
  // Contract unit: pwm_count. Scale is 1.0, offset 0.0.
  // Map pwm_count to analogWrite 0..255 with clamping.
  int pwm = (int)lroundf(pwm_count_f64);
  if (pwm < 0) pwm = 0;
  if (pwm > 255) pwm = 255;
  analogWrite(pin, pwm);
}

static void syncModelToHardware() {
  // Apply outputs io_0..io_8 to D2..D10
  applyOutputToPin(RGB1_R_PIN, controller.io_0);
  applyOutputToPin(RGB1_G_PIN, controller.io_1);
  applyOutputToPin(RGB1_B_PIN, controller.io_2);

  applyOutputToPin(RGB2_R_PIN, controller.io_3);
  applyOutputToPin(RGB2_G_PIN, controller.io_4);
  applyOutputToPin(RGB2_B_PIN, controller.io_5);

  applyOutputToPin(RGB3_R_PIN, controller.io_6);
  applyOutputToPin(RGB3_G_PIN, controller.io_7);
  applyOutputToPin(RGB3_B_PIN, controller.io_8);
}

void setup() {
  // Initialize pins for PWM output (analogWrite will configure as needed, but set explicitly).
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Contract: RGB LEDs off at reset represented by PWM initial=0 for all nine channels.
  controller = Controller{}; // all zeros
  syncModelToHardware();
}

void loop() {
  // ACCEPTED executable model bundle says outputs remain 0.0 for oracle "power_on_and_hold".
  // Keep controller outputs constant and continuously sync to hardware.
  syncModelToHardware();

  // Maintain a control-period cadence (10 ms) without affecting the constant-zero behavior.
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < CONTROL_PERIOD_MS) return;
  last = now;
}