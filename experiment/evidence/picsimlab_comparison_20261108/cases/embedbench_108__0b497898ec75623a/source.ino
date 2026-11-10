#include <Arduino.h>

// Frozen interface contract bindings (Mega 2560)
static const uint8_t PIN_LED1 = 2;   // io_0
static const uint8_t PIN_RGB1_R = 3;  // io_1
static const uint8_t PIN_RGB1_G = 4;  // io_2
static const uint8_t PIN_RGB1_B = 5;  // io_3

static const uint8_t PIN_RGB2_R = 6;  // io_4
static const uint8_t PIN_RGB2_G = 7;  // io_5
static const uint8_t PIN_RGB2_B = 8;  // io_6

static const uint8_t PIN_RGB3_R = 9;   // io_7
static const uint8_t PIN_RGB3_G = 10;  // io_8
static const uint8_t PIN_RGB3_B = 11;  // io_9

// Controller state (must preserve exact initial values)
static int32_t ledAccumMs = 0;
static int32_t rgbAccumMs = 0;
static bool ledOn = false;
static int32_t rgbIndex = 0;

// Scheduling granularity: control_period_ms = 10
static uint32_t lastStepMs = 0;
static constexpr uint32_t control_period_ms = 10;

static inline uint8_t toPwmCount(float v) {
  // Frozen contract uses pwm_count in [0..255] exactly; scale=1.0, offset=0.0
  if (v <= 0.0f) return 0;
  if (v >= 255.0f) return 255;
  return (uint8_t)(v + 0.5f);
}

static void applyOutputs() {
  // io_0: digital boolean, active_high
  digitalWrite(PIN_LED1, ledOn ? HIGH : LOW);

  // Outputs io_1..io_9 are pwm_count as f64 but values are exactly 0.0 or 255.0
  // Expressions from frozen model:
  // io_1: rgb1 R -> (rgbIndex == 0 or 3 or 5 or 6) ? 255 : 0
  // io_2: rgb1 G -> (rgbIndex == 1 or 3 or 4 or 6) ? 255 : 0
  // io_3: rgb1 B -> (rgbIndex == 2 or 4 or 5 or 6) ? 255 : 0
  // io_4: rgb2 R -> same as io_1
  // io_5: rgb2 G -> same as io_2
  // io_6: rgb2 B -> same as io_3
  // io_7: rgb3 R -> same as io_1
  // io_8: rgb3 G -> same as io_2
  // io_9: rgb3 B -> same as io_3

  const bool rOn = (rgbIndex == 0 || rgbIndex == 3 || rgbIndex == 5 || rgbIndex == 6);
  const bool gOn = (rgbIndex == 1 || rgbIndex == 3 || rgbIndex == 4 || rgbIndex == 6);
  const bool bOn = (rgbIndex == 2 || rgbIndex == 4 || rgbIndex == 5 || rgbIndex == 6);

  const uint8_t rVal = rOn ? 255 : 0;
  const uint8_t gVal = gOn ? 255 : 0;
  const uint8_t bVal = bOn ? 255 : 0;

  analogWrite(PIN_RGB1_R, rVal);
  analogWrite(PIN_RGB1_G, gVal);
  analogWrite(PIN_RGB1_B, bVal);

  analogWrite(PIN_RGB2_R, rVal);
  analogWrite(PIN_RGB2_G, gVal);
  analogWrite(PIN_RGB2_B, bVal);

  analogWrite(PIN_RGB3_R, rVal);
  analogWrite(PIN_RGB3_G, gVal);
  analogWrite(PIN_RGB3_B, bVal);
}

static void tick(int32_t io_10_delta_ms) {
  // perform action tick (match frozen model ordering)
  ledAccumMs = ledAccumMs + io_10_delta_ms;
  if (ledAccumMs >= 2000) {
    ledAccumMs = ledAccumMs - 2000;
    ledOn = (!ledOn);
  }

  rgbAccumMs = rgbAccumMs + io_10_delta_ms;
  if (rgbAccumMs >= 2000) {
    rgbAccumMs = rgbAccumMs - 2000;
    rgbIndex = (rgbIndex == 6) ? 0 : (rgbIndex + 1);
  }

  // Outputs depend directly on current state
  applyOutputs();
}

void setup() {
  // Pin modes
  pinMode(PIN_LED1, OUTPUT);

  // Ensure PWM pins are configured as outputs
  pinMode(PIN_RGB1_R, OUTPUT);
  pinMode(PIN_RGB1_G, OUTPUT);
  pinMode(PIN_RGB1_B, OUTPUT);

  pinMode(PIN_RGB2_R, OUTPUT);
  pinMode(PIN_RGB2_G, OUTPUT);
  pinMode(PIN_RGB2_B, OUTPUT);

  pinMode(PIN_RGB3_R, OUTPUT);
  pinMode(PIN_RGB3_G, OUTPUT);
  pinMode(PIN_RGB3_B, OUTPUT);

  // Initial state per contract:
  // ledOn=false, rgbIndex=0 => Red for all rgb LEDs, led1 off.
  ledAccumMs = 0;
  rgbAccumMs = 0;
  ledOn = false;
  rgbIndex = 0;

  applyOutputs();

  lastStepMs = millis();
}

void loop() {
  const uint32_t now = millis();
  if (now - lastStepMs >= control_period_ms) {
    // Frozen model uses io_10 = elapsed_ms delta_since_previous_step.
    // When multiple periods have elapsed, apply ticks repeatedly to preserve behavior.
    while (now - lastStepMs >= control_period_ms) {
      lastStepMs += control_period_ms;
      tick((int32_t)control_period_ms); // io_10
    }
  }
}