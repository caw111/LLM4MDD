#include <Arduino.h>

// Frozen interface contract bindings
static constexpr uint8_t LED1_PWM_PIN = 2;   // D2 (PWM)
static constexpr uint8_t LED2_PWM_PIN = 3;   // D3 (PWM)

static constexpr uint8_t BAR_A1_PIN  = 4;   // D4
static constexpr uint8_t BAR_A2_PIN  = 5;   // D5
static constexpr uint8_t BAR_A3_PIN  = 6;   // D6
static constexpr uint8_t BAR_A4_PIN  = 7;   // D7
static constexpr uint8_t BAR_A5_PIN  = 8;   // D8
static constexpr uint8_t BAR_A6_PIN  = 9;   // D9
static constexpr uint8_t BAR_A7_PIN  = 10;  // D10
static constexpr uint8_t BAR_A8_PIN  = 11;  // D11
static constexpr uint8_t BAR_A9_PIN  = 12;  // D12
static constexpr uint8_t BAR_A10_PIN = 13;  // D13

static constexpr uint8_t POT1_ADC_PIN = A3;  // io_12
static constexpr uint8_t POT2_ADC_PIN = A4;  // io_13
static constexpr uint8_t POT3_ADC_PIN = A5;  // io_14

// Control period per contract (used as pacing; model is memoryless)
static constexpr uint32_t CONTROL_PERIOD_MS = 10;

static inline uint16_t adcRead10bit(uint8_t pin) {
  return analogRead(pin); // 0..1023
}

// Uniform mapping 0..255 from ADC 0..1023, as per "mapped uniformly"
static inline uint8_t mapAdcTo0_255(uint16_t adcVal) {
  // result = floor(adc * 255 / 1023)
  return (uint8_t)((uint32_t)adcVal * 255u / 1023u);
}

// Map ADC to Real for io_14 semantics. Contract says pot3 is mapped uniformly to 0..10.
// We'll compute real-valued 0..10 linearly; then compare io_14 >= k.
static inline float mapAdcTo0_10_real(uint16_t adcVal) {
  // real = adc * 10 / 1023
  return (float)adcVal * (10.0f / 1023.0f);
}

static void writeBarSegment(uint8_t pin, bool on) {
  digitalWrite(pin, on ? HIGH : LOW); // active_high contract
}

static void allOff() {
  analogWrite(LED1_PWM_PIN, 0);
  analogWrite(LED2_PWM_PIN, 0);

  writeBarSegment(BAR_A1_PIN, false);
  writeBarSegment(BAR_A2_PIN, false);
  writeBarSegment(BAR_A3_PIN, false);
  writeBarSegment(BAR_A4_PIN, false);
  writeBarSegment(BAR_A5_PIN, false);
  writeBarSegment(BAR_A6_PIN, false);
  writeBarSegment(BAR_A7_PIN, false);
  writeBarSegment(BAR_A8_PIN, false);
  writeBarSegment(BAR_A9_PIN, false);
  writeBarSegment(BAR_A10_PIN, false);
}

void setup() {
  // Pin modes
  pinMode(LED1_PWM_PIN, OUTPUT);
  pinMode(LED2_PWM_PIN, OUTPUT);

  pinMode(BAR_A1_PIN, OUTPUT);
  pinMode(BAR_A2_PIN, OUTPUT);
  pinMode(BAR_A3_PIN, OUTPUT);
  pinMode(BAR_A4_PIN, OUTPUT);
  pinMode(BAR_A5_PIN, OUTPUT);
  pinMode(BAR_A6_PIN, OUTPUT);
  pinMode(BAR_A7_PIN, OUTPUT);
  pinMode(BAR_A8_PIN, OUTPUT);
  pinMode(BAR_A9_PIN, OUTPUT);
  pinMode(BAR_A10_PIN, OUTPUT);

  // Inputs for potentiometers
  // analogRead works regardless of pinMode, but set to default input for clarity.
  pinMode(POT1_ADC_PIN, INPUT);
  pinMode(POT2_ADC_PIN, INPUT);
  pinMode(POT3_ADC_PIN, INPUT);

  // Initialization: all outputs off
  allOff();
}

void loop() {
  static uint32_t nextTick = millis();

  // Ensure control period pacing (~10 ms). Model itself is memoryless,
  // but this preserves timing structure from contract.
  uint32_t now = millis();
  if ((int32_t)(now - nextTick) < 0) return;
  nextTick = now + CONTROL_PERIOD_MS;

  // Inputs (io_12, io_13, io_14)
  uint16_t pot1_adc = adcRead10bit(POT1_ADC_PIN); // 0..1023
  uint16_t pot2_adc = adcRead10bit(POT2_ADC_PIN);
  uint16_t pot3_adc = adcRead10bit(POT3_ADC_PIN);

  // Model mapping:
  // io_0 = io_12, io_1 = io_13 (outputs are PWM counts mapped from pot1/pot2 0..255)
  uint8_t io_0 = mapAdcTo0_255(pot1_adc);
  uint8_t io_1 = mapAdcTo0_255(pot2_adc);

  // io_14 is mapped uniformly from pot3 to 0..10
  float io_14 = mapAdcTo0_10_real(pot3_adc);

  // Write PWM outputs (io_0->D2, io_1->D3), continuous
  analogWrite(LED1_PWM_PIN, io_0);
  analogWrite(LED2_PWM_PIN, io_1);

  // Model outputs:
  // io_2 : io_14 >= 1
  // io_3 : io_14 >= 2
  // ...
  // io_11: io_14 >= 10
  writeBarSegment(BAR_A1_PIN,  (io_14 >= 1.0f));
  writeBarSegment(BAR_A2_PIN,  (io_14 >= 2.0f));
  writeBarSegment(BAR_A3_PIN,  (io_14 >= 3.0f));
  writeBarSegment(BAR_A4_PIN,  (io_14 >= 4.0f));
  writeBarSegment(BAR_A5_PIN,  (io_14 >= 5.0f));
  writeBarSegment(BAR_A6_PIN,  (io_14 >= 6.0f));
  writeBarSegment(BAR_A7_PIN,  (io_14 >= 7.0f));
  writeBarSegment(BAR_A8_PIN,  (io_14 >= 8.0f));
  writeBarSegment(BAR_A9_PIN,  (io_14 >= 9.0f));
  writeBarSegment(BAR_A10_PIN, (io_14 >= 10.0f));
}