#include <Arduino.h>

static constexpr uint8_t PWM_PIN = 9;

// Controller state
static float brightness = 0.0f;

// Contract: control_period_ms = 10
static unsigned long lastStepMs = 0;
static bool firstRun = true;

// Reads elapsed delta since previous control step as i32 "ms".
static int32_t readDeltaMsSincePrevStep() {
  unsigned long now = millis();
  if (firstRun) {
    firstRun = false;
    lastStepMs = now;
    return 0;
  }

  unsigned long delta = now - lastStepMs;
  lastStepMs = now;

  if (delta > 0x7FFFFFFFUL) return 0x7FFFFFFF;
  return (int32_t)delta;
}

static void controllerTick(int32_t io_1_ms_delta) {
  // Update 1:
  // brightness := (if (brightness >= 255.0) then 0.0 else brightness + (io_1 * 255.0 / 1000.0))
  float next1;
  if (brightness >= 255.0f) {
    next1 = 0.0f;
  } else {
    next1 = brightness + (static_cast<float>(io_1_ms_delta) * 255.0f / 1000.0f);
  }

  // Update 2:
  // brightness := (if (brightness >= 255.0) then 255.0 else brightness)
  if (next1 >= 255.0f) {
    brightness = 255.0f;
  } else {
    brightness = next1;
  }
}

// Output mapping: hardware contract maps scalar brightness (0..255) directly to PWM.
static void writePwm(float value0to255) {
  if (value0to255 <= 0.0f) {
    analogWrite(PWM_PIN, 0);
    return;
  }
  if (value0to255 >= 255.0f) {
    analogWrite(PWM_PIN, 255);
    return;
  }
  analogWrite(PWM_PIN, (int)value0to255); // truncate toward zero
}

void setup() {
  pinMode(PWM_PIN, OUTPUT);

  analogWrite(PWM_PIN, 0);
  brightness = 0.0f;

  lastStepMs = millis();
  firstRun = true;
}

void loop() {
  unsigned long now = millis();

  // Enforce control period of 10ms
  // Use a fixed-period scheduler (may catch up if delayed).
  static unsigned long scheduledNext = 0;
  if (scheduledNext == 0) {
    scheduledNext = now;
  }

  while ((long)(now - scheduledNext) >= 0) {
    scheduledNext += 10;

    int32_t io_1 = readDeltaMsSincePrevStep();
    controllerTick(io_1);
    writePwm(brightness);
  }
}