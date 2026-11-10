#include <Arduino.h>

// Frozen interface contract bindings (Mega 2560)
static const uint8_t LED1_PIN   = 2;   // io_2
static const uint8_t LED2_PIN   = 3;   // io_3
static const uint8_t LED3_PIN   = 4;   // io_4

static const uint8_t RGB1_R_PIN = 5;   // io_5
static const uint8_t RGB1_G_PIN = 6;   // io_6
static const uint8_t RGB1_B_PIN = 7;   // io_7

static const uint8_t RGB2_R_PIN = 8;   // io_8
static const uint8_t RGB2_G_PIN = 9;   // io_9
static const uint8_t RGB2_B_PIN = 10;  // io_10

static const uint8_t POT1_PIN = A3;    // io_0
static const uint8_t POT2_PIN = A4;    // io_1

static inline uint8_t clampToU8(long v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return static_cast<uint8_t>(v);
}

// Map ADC readings (0..1023) into PWM count (0..255) using the exact formula:
// out = (in * 255.0 / 1023.0)
static inline uint8_t pwmFromADC(uint16_t adc) {
  // Using long arithmetic to match deterministic behavior.
  // (adc * 255) / 1023 with truncation matches typical "floor" of real->int.
  return clampToU8((static_cast<long>(adc) * 255L) / 1023L);
}

// Blue/gradient uses: ((io_0 + io_1) * 255.0 / 2046.0)
static inline uint8_t blueFromADCsum(uint16_t pot1, uint16_t pot2) {
  const uint32_t sum = static_cast<uint32_t>(pot1) + static_cast<uint32_t>(pot2); // 0..2046
  return clampToU8((static_cast<long>(sum) * 255L) / 2046L);
}

void setup() {
  // Inputs
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  // Outputs (PWM-capable pins for constant-rate updates)
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Initialize to the contract "initial": all outputs 0
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  analogWrite(LED3_PIN, 0);

  analogWrite(RGB1_R_PIN, 0);
  analogWrite(RGB1_G_PIN, 0);
  analogWrite(RGB1_B_PIN, 0);

  analogWrite(RGB2_R_PIN, 0);
  analogWrite(RGB2_G_PIN, 0);
  analogWrite(RGB2_B_PIN, 0);
}

void loop() {
  // Sample inputs (adc_count 0..1023)
  const uint16_t io_0 = static_cast<uint16_t>(analogRead(POT1_PIN)); // pot1
  const uint16_t io_1 = static_cast<uint16_t>(analogRead(POT2_PIN)); // pot2

  // io_2 = (io_0 * 255.0 / 1023.0)
  const uint8_t io_2 = pwmFromADC(io_0);

  // io_3 = (io_1 * 255.0 / 1023.0)
  const uint8_t io_3 = pwmFromADC(io_1);

  // io_4 = if (io_0==1023 && io_1==1023) then 255 else 0
  const uint8_t io_4 = (io_0 == 1023u && io_1 == 1023u) ? 255u : 0u;

  // RGB components:
  // io_5 = (io_0 * 255.0 / 1023.0)
  const uint8_t io_5 = pwmFromADC(io_0);

  // io_6 = (io_1 * 255.0 / 1023.0)
  const uint8_t io_6 = pwmFromADC(io_1);

  // io_7 = ((io_0 + io_1) * 255.0 / 2046.0)
  const uint8_t io_7 = blueFromADCsum(io_0, io_1);

  // io_8 = (io_0 * 255.0 / 1023.0)
  const uint8_t io_8 = io_5;

  // io_9 = (io_1 * 255.0 / 1023.0)
  const uint8_t io_9 = io_6;

  // io_10 = ((io_0 + io_1) * 255.0 / 2046.0)
  const uint8_t io_10 = io_7;

  // Ordered updates: apply all outputs in a single control iteration.
  analogWrite(LED1_PIN, io_2);
  analogWrite(LED2_PIN, io_3);
  analogWrite(LED3_PIN, io_4);

  analogWrite(RGB1_R_PIN, io_5);
  analogWrite(RGB1_G_PIN, io_6);
  analogWrite(RGB1_B_PIN, io_7);

  analogWrite(RGB2_R_PIN, io_8);
  analogWrite(RGB2_G_PIN, io_9);
  analogWrite(RGB2_B_PIN, io_10);

  // Control period: 5 ms (contract). Ensure loop pacing to ~5ms.
  delay(5);
}