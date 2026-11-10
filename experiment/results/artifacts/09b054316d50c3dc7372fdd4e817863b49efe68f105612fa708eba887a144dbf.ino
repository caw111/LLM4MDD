#include <Arduino.h>

// Frozen interface contract bindings
// Inputs (adc_count):
//   io_0 -> A0 (Photoresistor 1)
//   io_1 -> A1 (Photoresistor 2)
//   io_2 -> A2 (Photoresistor 3)
//   io_3 -> A3 (Photoresistor 4)
//   io_4 -> A4 (Photoresistor 5)
// Outputs (bool, active_high):
//   io_5 -> D2  (LED 1)
//   io_6 -> D3  (LED 2)
//   io_7 -> D4  (LED 3)
//   io_8 -> D5  (LED 4)
//   io_9 -> D6  (LED 5)

static constexpr uint8_t PIN_LED1 = 2;  // io_5
static constexpr uint8_t PIN_LED2 = 3;  // io_6
static constexpr uint8_t PIN_LED3 = 4;  // io_7
static constexpr uint8_t PIN_LED4 = 5;  // io_8
static constexpr uint8_t PIN_LED5 = 6;  // io_9

static constexpr uint8_t PIN_PH1 = A0;  // io_0
static constexpr uint8_t PIN_PH2 = A1;  // io_1
static constexpr uint8_t PIN_PH3 = A2;  // io_2
static constexpr uint8_t PIN_PH4 = A3;  // io_3
static constexpr uint8_t PIN_PH5 = A4;  // io_4

// Threshold per contract/model
static constexpr float THRESHOLD = 500.0f;

struct ControllerIO {
  // Inputs
  float io_0; // A0
  float io_1; // A1
  float io_2; // A2
  float io_3; // A3
  float io_4; // A4

  // Outputs
  bool io_5;
  bool io_6;
  bool io_7;
  bool io_8;
  bool io_9;
};

static ControllerIO ctrl;

static inline float readAdcCount(uint8_t analogPin) {
  // Raw ADC count (0..1023). Model scale=1, offset=0.
  return static_cast<float>(analogRead(analogPin));
}

static inline void evaluateModel() {
  // Accepted executable model:
  // io_5 = io_0 < 500.0
  // io_6 = io_1 < 500.0
  // io_7 = io_2 < 500.0
  // io_8 = io_3 < 500.0
  // io_9 = io_4 < 500.0
  ctrl.io_5 = (ctrl.io_0 < THRESHOLD);
  ctrl.io_6 = (ctrl.io_1 < THRESHOLD);
  ctrl.io_7 = (ctrl.io_2 < THRESHOLD);
  ctrl.io_8 = (ctrl.io_3 < THRESHOLD);
  ctrl.io_9 = (ctrl.io_4 < THRESHOLD);
}

static inline void writeOutputs() {
  // Initial output state contract: all false. Thereafter, follow model.
  digitalWrite(PIN_LED1, ctrl.io_5 ? HIGH : LOW);
  digitalWrite(PIN_LED2, ctrl.io_6 ? HIGH : LOW);
  digitalWrite(PIN_LED3, ctrl.io_7 ? HIGH : LOW);
  digitalWrite(PIN_LED4, ctrl.io_8 ? HIGH : LOW);
  digitalWrite(PIN_LED5, ctrl.io_9 ? HIGH : LOW);
}

void setup() {
  // Set pins: LEDs D2..D6 as OUTPUT, analog A0..A4 as INPUT
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_LED4, OUTPUT);
  pinMode(PIN_LED5, OUTPUT);

  pinMode(PIN_PH1, INPUT);
  pinMode(PIN_PH2, INPUT);
  pinMode(PIN_PH3, INPUT);
  pinMode(PIN_PH4, INPUT);
  pinMode(PIN_PH5, INPUT);

  // All GPIO outputs start OFF (initial=false)
  ctrl.io_5 = false;
  ctrl.io_6 = false;
  ctrl.io_7 = false;
  ctrl.io_8 = false;
  ctrl.io_9 = false;

  writeOutputs();
}

void loop() {
  // Control period is specified as 5 ms. This loop enforces timing.
  static uint32_t nextTick = 0;
  const uint32_t now = millis();
  if (nextTick == 0) nextTick = now;

  if (now < nextTick) return;
  nextTick += 5;

  // Ordered updates: read io_0..io_4, then evaluate outputs io_5..io_9, then write.
  ctrl.io_0 = readAdcCount(PIN_PH1);
  ctrl.io_1 = readAdcCount(PIN_PH2);
  ctrl.io_2 = readAdcCount(PIN_PH3);
  ctrl.io_3 = readAdcCount(PIN_PH4);
  ctrl.io_4 = readAdcCount(PIN_PH5);

  evaluateModel();
  writeOutputs();
}