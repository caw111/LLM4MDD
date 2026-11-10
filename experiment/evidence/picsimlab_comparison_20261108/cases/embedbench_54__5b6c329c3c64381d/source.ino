#include <Arduino.h>

// Pin bindings (from frozen contract / preserved wiring)
static constexpr uint8_t PIN_RGB_R = 2;   // io_0 -> D2
static constexpr uint8_t PIN_RGB_G = 3;   // io_1 -> D3
static constexpr uint8_t PIN_RGB_B = 4;   // io_2 -> D4

static constexpr uint8_t PIN_BAR_0 = 5;    // io_4_segment_0 -> D5
static constexpr uint8_t PIN_BAR_1 = 6;    // io_4_segment_1 -> D6
static constexpr uint8_t PIN_BAR_2 = 7;    // io_4_segment_2 -> D7
static constexpr uint8_t PIN_BAR_3 = 8;    // io_4_segment_3 -> D8
static constexpr uint8_t PIN_BAR_4 = 9;    // io_4_segment_4 -> D9
static constexpr uint8_t PIN_BAR_5 = 10;   // io_4_segment_5 -> D10
static constexpr uint8_t PIN_BAR_6 = 11;   // io_4_segment_6 -> D11
static constexpr uint8_t PIN_BAR_7 = 12;   // io_4_segment_7 -> D12
static constexpr uint8_t PIN_BAR_8 = 13;   // io_4_segment_8 -> D13
static constexpr uint8_t PIN_BAR_9 = 44;   // io_4_segment_9 -> D44

static constexpr uint8_t PIN_POT = A3;     // io_3 -> A3 (analog input)

static inline int32_t floor_i32_from_real(float x) {
  // Matches RealFunctions::floor(x) for values expected in [0,1023]
  if (x >= 0.0f) return (int32_t)x;
  // Not expected, but keep mathematically safe:
  return (int32_t)x - ((float)((int32_t)x) == x ? 0 : 1);
}

static inline int32_t map_to_floor_0_10(float io3) {
  // Not used directly; segment computation below follows the model formula.
  (void)io3;
  return 0;
}

int main_logic_tick(int32_t& elapsedModeMs,
                     int32_t& modeIndex,
                     int32_t& currentRed,
                     int32_t& currentGreen,
                     int32_t& currentBlue) {
  // i/o_3 sampled as float from ADC; model assumes Real in [0..1023]
  float io_3 = (float)analogRead(PIN_POT);

  // ===== Model "tick" (ACCEPTED executable model semantics as given) =====
  // perform action tick { first start; then assign elapsedModeMs := 0;
  // then assign modeIndex := 0;
  // then assign currentRed := (if modeIndex==0 then floor(io_3) else currentRed); etc. }
  // Note: Because assignments force modeIndex to 0 every tick, only currentRed updates.
  // We implement exactly as specified.
  elapsedModeMs = 0;
  modeIndex = 0;

  if (modeIndex == 0) currentRed = floor_i32_from_real(io_3);
  if (modeIndex == 1) currentGreen = floor_i32_from_real(io_3);
  if (modeIndex == 2) currentBlue = floor_i32_from_real(io_3);

  // Outputs io_0..io_2 are current* * 1.0; used for PWM duty count
  // Outputs io_4_segment_i are based on floor(io_3 * 10 / 1023) >= (i+1)
  int32_t threshold = floor_i32_from_real(io_3 * 10.0f / 1023.0f);

  // io_4_segment_0 .. io_4_segment_9
  const bool s0 = (threshold >= 1);
  const bool s1 = (threshold >= 2);
  const bool s2 = (threshold >= 3);
  const bool s3 = (threshold >= 4);
  const bool s4 = (threshold >= 5);
  const bool s5 = (threshold >= 6);
  const bool s6 = (threshold >= 7);
  const bool s7 = (threshold >= 8);
  const bool s8 = (threshold >= 9);
  const bool s9 = (threshold >= 10);

  // ===== Hardware adaptation (pwm_output and digital bool active-high) =====
  // PWM scaling: contract says scale=1.0 offset=0.0 with unit "pwm_count".
  // In Arduino, analogWrite range is 0..255, but the contract does not provide scaling.
  // Use direct mapping from model count (0..1023) to Arduino PWM (0..255) proportionally.
  // This preserves monotonicity while keeping off at 0 and max at 1023 -> 255.
  auto pwmFromModel = [](int32_t modelVal) -> uint8_t {
    if (modelVal <= 0) return 0;
    if (modelVal >= 1023) return 255;
    // floor-like scaling
    return (uint8_t)((modelVal * 255L) / 1023L);
  };

  analogWrite(PIN_RGB_R, pwmFromModel(currentRed));
  analogWrite(PIN_RGB_G, pwmFromModel(currentGreen));
  analogWrite(PIN_RGB_B, pwmFromModel(currentBlue));

  digitalWrite(PIN_BAR_0, s0 ? HIGH : LOW);
  digitalWrite(PIN_BAR_1, s1 ? HIGH : LOW);
  digitalWrite(PIN_BAR_2, s2 ? HIGH : LOW);
  digitalWrite(PIN_BAR_3, s3 ? HIGH : LOW);
  digitalWrite(PIN_BAR_4, s4 ? HIGH : LOW);
  digitalWrite(PIN_BAR_5, s5 ? HIGH : LOW);
  digitalWrite(PIN_BAR_6, s6 ? HIGH : LOW);
  digitalWrite(PIN_BAR_7, s7 ? HIGH : LOW);
  digitalWrite(PIN_BAR_8, s8 ? HIGH : LOW);
  digitalWrite(PIN_BAR_9, s9 ? HIGH : LOW);

  return 0;
}

void setup() {
  // Initialization: all components off
  pinMode(PIN_RGB_R, OUTPUT);
  pinMode(PIN_RGB_G, OUTPUT);
  pinMode(PIN_RGB_B, OUTPUT);

  analogWrite(PIN_RGB_R, 0);
  analogWrite(PIN_RGB_G, 0);
  analogWrite(PIN_RGB_B, 0);

  pinMode(PIN_BAR_0, OUTPUT);
  pinMode(PIN_BAR_1, OUTPUT);
  pinMode(PIN_BAR_2, OUTPUT);
  pinMode(PIN_BAR_3, OUTPUT);
  pinMode(PIN_BAR_4, OUTPUT);
  pinMode(PIN_BAR_5, OUTPUT);
  pinMode(PIN_BAR_6, OUTPUT);
  pinMode(PIN_BAR_7, OUTPUT);
  pinMode(PIN_BAR_8, OUTPUT);
  pinMode(PIN_BAR_9, OUTPUT);

  digitalWrite(PIN_BAR_0, LOW);
  digitalWrite(PIN_BAR_1, LOW);
  digitalWrite(PIN_BAR_2, LOW);
  digitalWrite(PIN_BAR_3, LOW);
  digitalWrite(PIN_BAR_4, LOW);
  digitalWrite(PIN_BAR_5, LOW);
  digitalWrite(PIN_BAR_6, LOW);
  digitalWrite(PIN_BAR_7, LOW);
  digitalWrite(PIN_BAR_8, LOW);
  digitalWrite(PIN_BAR_9, LOW);

  pinMode(PIN_POT, INPUT);

  // Controller state per contract initial values
  // elapsedModeMs=0, modeIndex=0, currentRed=0, currentGreen=0, currentBlue=0
}

void loop() {
  // Model control period: 10 ms
  static uint32_t lastTickMs = 0;

  static int32_t elapsedModeMs = 0;
  static int32_t modeIndex = 0;
  static int32_t currentRed = 0;
  static int32_t currentGreen = 0;
  static int32_t currentBlue = 0;

  uint32_t now = millis();
  if ((uint32_t)(now - lastTickMs) >= 10u) {
    // Tick as per model (includes internal mode assignments)
    lastTickMs = now;

    main_logic_tick(elapsedModeMs, modeIndex, currentRed, currentGreen, currentBlue);
  }
}