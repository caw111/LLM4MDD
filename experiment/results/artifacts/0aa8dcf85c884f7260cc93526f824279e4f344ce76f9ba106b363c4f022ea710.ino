#include <Arduino.h>

// Pins from frozen interface contract
static constexpr uint8_t PIN_RED_PWM   = 2; // io_0
static constexpr uint8_t PIN_GRN_PWM   = 3; // io_1
static constexpr uint8_t PIN_BLU_PWM   = 4; // io_2
static constexpr uint8_t PIN_K1        = 5; // io_3 active-low (pressed => model true)
static constexpr uint8_t PIN_K2        = 6; // io_4 active-low
static constexpr uint8_t PIN_K3        = 7; // io_5 active-low

// Model time base
static constexpr uint32_t CONTROL_PERIOD_MS = 5; // frozen contract

// Business timing
static constexpr int32_t DWELL_MS = 2000;

// Model state (preserve exact variables and semantics)
static bool modeCycling = true;
static bool modeOff = false;
static int32_t currentColorIndex = 0; // 0=red, 1=green, 2=blue
static bool previousK1 = false;
static int32_t accumulatedMs = 0;

static uint32_t lastTickMs = 0;

static inline bool readBtnModel(uint8_t pin) {
  // model_semantics: active_high=false, pullup=true => pressed => LOW => model true
  return (digitalRead(pin) == LOW);
}

static inline void applyOutputs() {
  // outputs io_0/io_1/io_2 are pwm_count in [0..255] with scale=1.0
  // Exact output expressions:
  // io_0 (red):
  // (if modeOff ? 0.0
  //  else (if modeCycling && currentColorIndex==0 ? 255.0
  //        else if !modeCycling && currentColorIndex==0 ? 255.0 else 0.0))
  // Simplifies to: modeOff ? 0 : (currentColorIndex==0 ? 255 : 0) since !modeOff implies either cycling or hold
  // But we keep behavior exact by using the same logic structure.
  float io0 = 0.0f, io1 = 0.0f, io2 = 0.0f;

  if (modeOff) {
    io0 = 0.0f; io1 = 0.0f; io2 = 0.0f;
  } else {
    // io_0
    if (modeCycling && (currentColorIndex == 0)) io0 = 255.0f;
    else if ((!modeCycling) && (currentColorIndex == 0)) io0 = 255.0f;
    else io0 = 0.0f;

    // io_1
    if (modeCycling && (currentColorIndex == 1)) io1 = 255.0f;
    else if ((!modeCycling) && (currentColorIndex == 1)) io1 = 255.0f;
    else io1 = 0.0f;

    // io_2
    if (modeCycling && (currentColorIndex == 2)) io2 = 255.0f;
    else if ((!modeCycling) && (currentColorIndex == 2)) io2 = 255.0f;
    else io2 = 0.0f;
  }

  // Drive PWM channels
  analogWrite(PIN_RED_PWM,   (uint8_t)io0);
  analogWrite(PIN_GRN_PWM,   (uint8_t)io1);
  analogWrite(PIN_BLU_PWM,   (uint8_t)io2);
}

void setup() {
  // Button inputs with pullups
  pinMode(PIN_K1, INPUT_PULLUP);
  pinMode(PIN_K2, INPUT_PULLUP);
  pinMode(PIN_K3, INPUT_PULLUP);

  // PWM outputs
  pinMode(PIN_RED_PWM, OUTPUT);
  pinMode(PIN_GRN_PWM, OUTPUT);
  pinMode(PIN_BLU_PWM, OUTPUT);

  // Initialize model as in frozen bundle (already set via globals)
  // Also ensure timing reference
  lastTickMs = millis();

  // On startup: modeCycling=true, modeOff=false, currentColorIndex=0
  // Apply outputs immediately (red on)
  applyOutputs();
}

void loop() {
  // Enforce controller period ~5ms, and use elapsed_ms delta_since_previous_step semantics
  uint32_t now = millis();
  uint32_t elapsed = now - lastTickMs;
  if (elapsed < CONTROL_PERIOD_MS) return;

  // Cap/normalize to integer ms delta semantics expected by model
  lastTickMs = now;

  // Read inputs (model booleans)
  bool io_3 = readBtnModel(PIN_K1); // k1
  bool io_4 = readBtnModel(PIN_K2); // k2
  bool io_5 = readBtnModel(PIN_K3); // k3
  int32_t io_6 = (int32_t)elapsed; // i32 unit ms delta

  // ==== perform action tick ====
  // Replicate SysML2 "perform action tick" update ordering and nested conditions.
  // (1) first start; then if io_5 { ... }
  if (io_5) {
    modeOff = true;
    modeCycling = false;
  }

  // (2) then if (((not io_5) and (io_4))) and ((not modeOff)) { ... }
  if (((!io_5) && io_4) && (!modeOff)) {
    modeCycling = true;
    modeOff = false;
  }

  // (3) then if (io_3 and (not previousK1)) { modeCycling := false; }
  if (io_3 && (!previousK1)) {
    modeCycling = false;
  }

  // (4) then if (modeCycling and (not modeOff)) { accumulatedMs += io_6; if accumulatedMs>=2000 { ... } }
  if (modeCycling && (!modeOff)) {
    accumulatedMs = accumulatedMs + io_6;
    if (accumulatedMs >= DWELL_MS) {
      accumulatedMs = accumulatedMs - DWELL_MS;
      if (currentColorIndex == 2) currentColorIndex = 0;
      else currentColorIndex = currentColorIndex + 1;
    }
  }

  // (5) then assign previousK1 := io_3
  previousK1 = io_3;

  // ==== outputs ====
  applyOutputs();
}