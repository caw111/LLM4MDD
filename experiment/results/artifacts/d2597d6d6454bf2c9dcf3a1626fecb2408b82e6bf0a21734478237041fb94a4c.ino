#include <Arduino.h>

static constexpr uint8_t BTN1_PIN = 2;  // io_0
static constexpr uint8_t BTN2_PIN = 3;  // io_1
static constexpr uint8_t BTN3_PIN = 4;  // io_2

static constexpr uint8_t POT1_PIN = A3; // io_3
static constexpr uint8_t POT2_PIN = A4; // io_4
static constexpr uint8_t POT3_PIN = A5; // io_5

static constexpr uint8_t PWM_R_PIN = 5;  // io_6
static constexpr uint8_t PWM_G_PIN = 6;  // io_7
static constexpr uint8_t PWM_B_PIN = 7;  // io_8

// Model/control contract
static constexpr uint32_t CONTROL_PERIOD_MS = 5; // control_period_ms=5
static constexpr uint32_t DEBOUNCE_HOLD_MS  = 150; // 0.15s press duration
static constexpr uint16_t ADC_MAX = 1023;

// ---------- Debounced "true when pressed" (active-low input with pullup) ----------
// Preserve semantics: model io_k is raw pressed/not-pressed, with debouncing handled by
// requiring 0.15s of stable pressed state to become true.
struct DebouncedActiveLowButton {
  uint8_t pin;

  // Raw samples
  bool lastRawPressed = false;
  bool stablePressed = false;

  // Debounce timing
  bool timing = false;
  uint32_t changeStartMs = 0;

  // Output semantics: "model value true when pressed"
  // => output is stablePressed.
  bool update(uint32_t nowMs) {
    // raw pressed is true when pin reads LOW (active-low with pullup)
    bool rawPressed = (digitalRead(pin) == LOW);

    if (rawPressed != lastRawPressed) {
      lastRawPressed = rawPressed;
      timing = true;
      changeStartMs = nowMs;
    }

    if (timing) {
      // If raw remains equal to current lastRawPressed, after debounce duration accept.
      // If it changed, it will reset timing on next iteration.
      if (rawPressed == lastRawPressed && (nowMs - changeStartMs) >= DEBOUNCE_HOLD_MS) {
        stablePressed = rawPressed;
        timing = false;
      }
    } else {
      // When not timing, keep stablePressed unless raw changes.
      if (rawPressed != stablePressed && rawPressed == lastRawPressed) {
        timing = true;
        changeStartMs = nowMs;
      }
    }

    return stablePressed;
  }
};

// ---------- ADC to 0..255 mapping exactly as floor(io * 255.0 / 1023.0) ----------
static inline int32_t mapAdcTo255(uint16_t adc) {
  // adc is integer 0..1023, but model uses RealFunctions::floor.
  // Using floating multiplication/division then floor.
  float v = (static_cast<float>(adc) * 255.0f) / 1023.0f;
  int32_t out = static_cast<int32_t>(floorf(v));
  if (out < 0) out = 0;
  if (out > 255) out = 255;
  return out;
}

DebouncedActiveLowButton btn1{BTN1_PIN};
DebouncedActiveLowButton btn2{BTN2_PIN};
DebouncedActiveLowButton btn3{BTN3_PIN};

static int32_t redIntensity = 0;   // Integer
static int32_t greenIntensity = 0; // Integer
static int32_t blueIntensity = 0;  // Integer

// Initialize PWM outputs to off (all channels 0)
static inline void applyPwmOutputs() {
  // common cathode: PWM duty directly corresponds to model intensity
  analogWrite(PWM_R_PIN, static_cast<uint8_t>(redIntensity));
  analogWrite(PWM_G_PIN, static_cast<uint8_t>(greenIntensity));
  analogWrite(PWM_B_PIN, static_cast<uint8_t>(blueIntensity));
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Configure PWM pins
  pinMode(PWM_R_PIN, OUTPUT);
  pinMode(PWM_G_PIN, OUTPUT);
  pinMode(PWM_B_PIN, OUTPUT);

  // Off state
  redIntensity = 0;
  greenIntensity = 0;
  blueIntensity = 0;
  applyPwmOutputs();
}

void loop() {
  static uint32_t lastTickMs = 0;

  uint32_t nowMs = millis();
  // Enforce control period cadence (5ms)
  if (nowMs - lastTickMs < CONTROL_PERIOD_MS) return;
  lastTickMs += CONTROL_PERIOD_MS;

  // ---- Read inputs (debounced button pressed booleans) ----
  bool io_0 = btn1.update(nowMs); // btn1 pressed -> enable red channel
  bool io_1 = btn2.update(nowMs); // btn2 pressed -> enable green channel
  bool io_2 = btn3.update(nowMs); // btn3 pressed -> enable blue channel

  // Pots read continuously as model inputs (io_3..io_5 in adc_count)
  // Model expressions use floor(io * 255.0 / 1023.0) and retains last intensity when button not pressed.
  uint16_t adc3 = analogRead(POT1_PIN);
  uint16_t adc4 = analogRead(POT2_PIN);
  uint16_t adc5 = analogRead(POT3_PIN);

  // ---- Controller tick with ordered updates exactly as given ----
  // if io_0 then assign redIntensity := floor(io_3 * 255 / 1023) else 0 (within if io_0 guard)
  if (io_0) {
    redIntensity = mapAdcTo255(adc3);
  }
  if (io_1) {
    greenIntensity = mapAdcTo255(adc4);
  }
  if (io_2) {
    blueIntensity = mapAdcTo255(adc5);
  }

  // ---- Outputs ----
  applyPwmOutputs();
}