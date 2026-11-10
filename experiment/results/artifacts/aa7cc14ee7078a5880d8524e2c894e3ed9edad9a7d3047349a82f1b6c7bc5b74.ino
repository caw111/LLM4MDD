#include <Arduino.h>

static const uint8_t LED1_PIN = 2; // PWM
static const uint8_t LED2_PIN = 3; // PWM
static const uint8_t LED3_PIN = 4; // PWM

static const uint8_t BTN1_PIN = 5;  // active_low with pullup
static const uint8_t BTN2_PIN = 6;  // active_low with pullup
static const uint8_t BTN3_PIN = 7;  // active_low with pullup

static const uint8_t POT1_PIN = A3; // 0..1023
static const uint8_t POT2_PIN = A4; // 0..1023
static const uint8_t POT3_PIN = A5; // 0..1023

// Controller states (ACCEPTED model)
static int32_t led1Brightness = 0;
static int32_t led2Brightness = 0;
static int32_t led3Brightness = 0;

static bool prevBtn1 = false;
static bool prevBtn2 = false;
static bool prevBtn3 = false;

static int32_t hold1RemainingMs = 0;
static int32_t hold2RemainingMs = 0;
static int32_t hold3RemainingMs = 0;

// Scheduler tick (io_9): delta_since_previous_step
static uint32_t lastTickMs = 0;

static inline uint8_t clampU8(int32_t v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return (uint8_t)v;
}

// Floor mapping exactly as in model:
// if (x <= 0) 0 else if (x >= 1023) 255 else floor((x*255)/1023)
static inline int32_t mapPotToBrightness(int32_t potCount /*0..1023*/) {
  // Model uses Real comparisons with <=0.0 and >=1023.0.
  if (potCount <= 0) return 0;
  if (potCount >= 1023) return 255;
  // potCount is integer; compute floor((pot*255)/1023) using integer arithmetic.
  // This matches floor((io*255)/1023) for io in (0,1023).
  return (potCount * 255) / 1023;
}

static inline int32_t readPotCounts(uint8_t pin) {
  return (int32_t)analogRead(pin); // scale=1, directly to 0..1023 counts
}

static inline bool readBtnActiveLowModelBool(uint8_t pin) {
  // Model semantics: true when LOW, false when HIGH.
  return (digitalRead(pin) == LOW);
}

void setup() {
  // Initial hardware contract: all LEDs off.
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);

  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  // Potentiometers are analog inputs (no pinMode needed, but harmless).
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  lastTickMs = millis();

  // Initial controller states already zero/false by static initialization.
}

void loop() {
  // Control period is 10ms in contract; model uses io_9 delta_since_previous_step.
  const uint32_t nowMs = millis();
  uint32_t dtMsU = nowMs - lastTickMs;
  if (dtMsU > 1000000UL) dtMsU = 0; // guard overflow anomalies (unlikely on Arduino)
  lastTickMs = nowMs;

  // io_3..io_5: digital_boolean gates (pressed => model true)
  const bool io_3 = readBtnActiveLowModelBool(BTN1_PIN);
  const bool io_4 = readBtnActiveLowModelBool(BTN2_PIN);
  const bool io_5 = readBtnActiveLowModelBool(BTN3_PIN);

  // io_6..io_8: ADC counts as Real with scale=1
  const int32_t pot1Counts = readPotCounts(POT1_PIN);
  const int32_t pot2Counts = readPotCounts(POT2_PIN);
  const int32_t pot3Counts = readPotCounts(POT3_PIN);
  // Contract types are Real; comparisons are modeled by integer mapping boundaries.
  // We'll use the exact same formula via mapPotToBrightness.

  // io_9: elapsed_ms delta since previous step
  const int32_t io_9 = (int32_t)dtMsU;

  // --- Model tick (in exact order of updates as given) ---

  // Updates: decrement hold timers, clamp to 0.
  if (hold1RemainingMs > 0) hold1RemainingMs -= io_9;
  if (hold1RemainingMs <= 0) hold1RemainingMs = 0;

  if (hold2RemainingMs > 0) hold2RemainingMs -= io_9;
  if (hold2RemainingMs <= 0) hold2RemainingMs = 0;

  if (hold3RemainingMs > 0) hold3RemainingMs -= io_9;
  if (hold3RemainingMs <= 0) hold3RemainingMs = 0;

  // Button rising-edge guarded updates:
  // if ((io_3) and ((not prevBtn1)))
  if (io_3 && (!prevBtn1)) {
    hold1RemainingMs = 2000;
    led1Brightness = mapPotToBrightness(pot1Counts);
  }

  // if ((io_4) and ((not prevBtn2)))
  if (io_4 && (!prevBtn2)) {
    hold2RemainingMs = 2000;
    led2Brightness = mapPotToBrightness(pot2Counts);
  }

  // if ((io_5) and ((not prevBtn3)))
  if (io_5 && (!prevBtn3)) {
    hold3RemainingMs = 2000;
    led3Brightness = mapPotToBrightness(pot3Counts);
  }

  // assign prevBtn*
  prevBtn1 = io_3;
  prevBtn2 = io_4;
  prevBtn3 = io_5;

  // Outputs: io_0..io_2 = floor(led*Brightness)
  // Hardware adapter: PWM outputs on D2/D3/D4.
  // When no button press happens, model retains state; we always write current outputs.
  // This matches "no port updates" semantics at the model level; writing the same value is benign.
  analogWrite(LED1_PIN, clampU8(led1Brightness));
  analogWrite(LED2_PIN, clampU8(led2Brightness));
  analogWrite(LED3_PIN, clampU8(led3Brightness));

  // Enforce approximate 10ms control period (not part of model logic, but stabilizes io_9 behavior).
  // If dtMsU already exceeds 10ms, don't delay to avoid changing semantics of delta.
  if (dtMsU < 10) delay(10 - dtMsU);
}