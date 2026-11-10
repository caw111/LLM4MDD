#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

static const uint8_t PIN_POT1 = A3; // pot1 -> A3
static const uint8_t PIN_POT2 = A4; // pot2 -> A4

static const uint8_t PIN_SERVO1 = 2; // D2
static const uint8_t PIN_SERVO2 = 3; // D3
static const uint8_t PIN_SERVO3 = 4; // D4

// Control period (planning notes): 5 ms
static const uint32_t CONTROL_PERIOD_US = 5000UL;

static Servo servo1;
static Servo servo2;
static Servo servo3;

#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static float mapCountsToDeg(uint16_t adcCounts);
#line 23 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint16_t readPotCounts(uint8_t pin);
#line 29 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 54 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 18 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_25\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static inline float mapCountsToDeg(uint16_t adcCounts) {
  // Exact mapping per model: (io * 180.0) / 1023.0
  return (static_cast<float>(adcCounts) * 180.0f) / 1023.0f;
}

static inline uint16_t readPotCounts(uint8_t pin) {
  // Analog input range is 0..1023 on AVR (10-bit ADC).
  // Electrical sampling left as default (as required by contract notes).
  return static_cast<uint16_t>(analogRead(pin));
}

void setup() {
  // Use default Arduino Mega 2560 configuration.
  // Attach servos to required PWM pins.
  servo1.attach(PIN_SERVO1);
  servo2.attach(PIN_SERVO2);
  servo3.attach(PIN_SERVO3);

  // Ensure ADC is ready and analog subsystem initialized by Arduino core.
  analogRead(PIN_POT1);
  analogRead(PIN_POT2);

  // Initialize to initial mapped targets (ordered updates not observable via serial;
  // but outputs must start consistent with model).
  uint16_t pot1 = readPotCounts(PIN_POT1); // io_0
  uint16_t pot2 = readPotCounts(PIN_POT2); // io_1

  float deg1 = mapCountsToDeg(pot1); // io_2
  float deg2 = mapCountsToDeg(pot2); // io_3
  float deg3 = deg2;                 // io_4

  servo1.write(static_cast<int>(deg1 + 0.5f));
  servo2.write(static_cast<int>(deg2 + 0.5f));
  servo3.write(static_cast<int>(deg3 + 0.5f));
}

void loop() {
  static uint32_t nextTick = micros();

  // Keep strict 5 ms control-period behavior (respond <= 50ms budget).
  uint32_t now = micros();
  if ((int32_t)(now - nextTick) < 0) return;
  nextTick += CONTROL_PERIOD_US;

  // Read inputs: io_0 (pot1) and io_1 (pot2)
  uint16_t io0 = readPotCounts(PIN_POT1);
  uint16_t io1 = readPotCounts(PIN_POT2);

  // Model outputs (degrees) per Accepted executable model:
  // io_2 = (io_0 * 180.0) / 1023.0
  // io_3 = (io_1 * 180.0) / 1023.0
  // io_4 = (io_1 * 180.0) / 1023.0
  float io2 = mapCountsToDeg(io0);
  float io3 = mapCountsToDeg(io1);
  float io4 = io3;

  // Servo_angle invalid_policy: hold_last_valid.
  // analogRead is always valid; still, we always update with current mapping.
  // Servo library accepts degrees; quantization depends on library/hardware.
  servo1.write(static_cast<int>(io2 + 0.5f));
  servo2.write(static_cast<int>(io3 + 0.5f));
  servo3.write(static_cast<int>(io4 + 0.5f));
}
