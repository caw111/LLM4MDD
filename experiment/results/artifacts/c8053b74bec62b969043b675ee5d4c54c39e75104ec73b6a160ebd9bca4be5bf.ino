#include <Arduino.h>

namespace PinMap {
  // Potentiometers
  constexpr uint8_t POT1_PIN = A3; // io_3
  constexpr uint8_t POT2_PIN = A4; // io_4

  // RGB PWM (common-cathode: COM -> GND.1, R/G/B anodes driven)
  constexpr uint8_t RGB_R_PWM = 2; // D2  io_0
  constexpr uint8_t RGB_G_PWM = 3; // D3  io_1
  constexpr uint8_t RGB_B_PWM = 4; // D4  io_2

  // 74HC595 to 7-seg
  constexpr uint8_t SR_DS   = A0; // A0  (shift register data)
  constexpr uint8_t SR_SHCP = A2; // A2  (shift clock)
  constexpr uint8_t SR_STCP = A1; // A1  (latch clock)
}

static inline uint8_t clampToU8FromReal(float v) {
  if (v < 0.0f) return 0;
  if (v > 255.0f) return 255;
  return (uint8_t)v; // values should be within [0,255] after clamp
}

static inline float adcToReal01_1023(int adc) {
  return (float)adc; // already in [0,1023]
}

static uint8_t adcRead10bitAsReal(uint8_t pin) {
  return (uint8_t)analogRead(pin); // return [0..255]?? No: keep full precision using int below.
}

// 7-seg active-low segment bits for digits 0..9
// SR bits Q0..Q6 drive segments A..G respectively; active_low=true.
// These mappings are for typical 74HC595 + common-cathode module where segments are driven active-high.
// Since the model says active_low=true, we use active-low patterns directly.
static const uint8_t SEG_ACTIVE_LOW_BY_DIGIT[10] = {
  // digit:  gfedcba (bit0=A ... bit6=G) active-low
  0b0000001, // 0 -> segments A,B,C,D,E,F on, G off  => Q0..Q5 low, Q6 high
  0b1001111, // 1 -> B,C on; others off
  0b0010010, // 2 -> A,B on? (standard)
  0b0000110, // 3
  0b1001100, // 4
  0b0100100, // 5
  0b0100000, // 6
  0b0001111, // 7
  0b0000000, // 8
  0b0000100  // 9
};

static inline uint8_t computeIo0FromPot1(float io_3) {
  // io_0 = if 0<=io_3<=1023 then io_3*255/1023 else if io_3<0 then 0 else 255
  if (io_3 >= 0.0f && io_3 <= 1023.0f) return clampToU8FromReal(io_3 * 255.0f / 1023.0f);
  return (io_3 < 0.0f) ? 0 : 255;
}

static inline uint8_t computeIo1FromPot1(float io_3) {
  // same as io_0
  return computeIo0FromPot1(io_3);
}

static inline uint8_t computeIo2FromPot2(float io_4) {
  // io_2 = if 0<=io_4<=1023 then io_4*255/1023 else if io_4<0 then 0 else 255
  if (io_4 >= 0.0f && io_4 <= 1023.0f) return clampToU8FromReal(io_4 * 255.0f / 1023.0f);
  return (io_4 < 0.0f) ? 0 : 255;
}

static inline int computeIo5DigitFromPots(float io_3, float io_4) {
  // model:
  // if 0<=io_3<=1023 and 0<=io_4<=1023 then floor((((((io_3*255/1023)+(io_3*255/1023))/3) + (io_4*255/1023/3))/28.33))
  // else if (io_3<0 or io_4<0) then 0 else 9
  if (io_3 >= 0.0f && io_3 <= 1023.0f && io_4 >= 0.0f && io_4 <= 1023.0f) {
    float r = (io_3 * 255.0f / 1023.0f);
    float g = (io_3 * 255.0f / 1023.0f);
    float b = (io_4 * 255.0f / 1023.0f);
    float avg = (r + g + b) / 3.0f;
    float scaled = avg / 28.33f;
    return (int)floorf(scaled);
  } else {
    if (io_3 < 0.0f || io_4 < 0.0f) return 0;
    return 9;
  }
}

static void srWrite7SegNumber0to9(int digit) {
  // invalid policy blank: model already returns 0..9. We implement digits 0..9 only.
  // Q0..Q6 drive segments A..G. active-low; latch after shifting.
  if (digit < 0 || digit > 9) {
    // blank: all segments off (active-low => all high)
    digit = -1;
  }

  uint8_t seg = 0x7F; // all high for blank (A..G off)
  if (digit >= 0 && digit <= 9) {
    seg = SEG_ACTIVE_LOW_BY_DIGIT[digit] & 0x7F;
  }

  // Shift out bits Q0..Q6 => segment bits A..G.
  // DS on A0, SHCP on A2, STCP on A1.
  // We'll clock 8 bits for convenience; Q7 unused but keep it 0.
  uint8_t outByte = (uint8_t)(seg); // bit0..6 used; bit7 ignored
  digitalWrite(PinMap::SR_STCP, LOW);
  for (int i = 7; i >= 0; --i) {
    digitalWrite(PinMap::SR_SHCP, LOW);
    digitalWrite(PinMap::SR_DS, (outByte & (1u << i)) ? HIGH : LOW);
    digitalWrite(PinMap::SR_SHCP, HIGH);
  }
  digitalWrite(PinMap::SR_STCP, HIGH);
}

void setup() {
  // Initial state: RGB off, sevseg shows "0"
  pinMode(PinMap::RGB_R_PWM, OUTPUT);
  pinMode(PinMap::RGB_G_PWM, OUTPUT);
  pinMode(PinMap::RGB_B_PWM, OUTPUT);

  // PWM outputs start at 0
  analogWrite(PinMap::RGB_R_PWM, 0);
  analogWrite(PinMap::RGB_G_PWM, 0);
  analogWrite(PinMap::RGB_B_PWM, 0);

  // Shift register pins
  pinMode(PinMap::SR_DS, OUTPUT);
  pinMode(PinMap::SR_SHCP, OUTPUT);
  pinMode(PinMap::SR_STCP, OUTPUT);

  digitalWrite(PinMap::SR_DS, LOW);
  digitalWrite(PinMap::SR_SHCP, LOW);
  digitalWrite(PinMap::SR_STCP, LOW);

  // Set initial digit to 0
  srWrite7SegNumber0to9(0);

  analogReference(DEFAULT); // default 5V in Arduino Mega environment
}

void loop() {
  // Control period: 10ms
  static uint32_t last = 0;
  uint32_t now = millis();
  if (now - last < 10) return;
  last = now;

  // Read potentiometers as io_3/io_4 in [0..1023]
  int adc3 = analogRead(PinMap::POT1_PIN);
  int adc4 = analogRead(PinMap::POT2_PIN);

  float io_3 = adcToReal01_1023(adc3);
  float io_4 = adcToReal01_1023(adc4);

  // Outputs io_0..io_2: PWM counts 0..255
  uint8_t io_0 = computeIo0FromPot1(io_3); // red
  uint8_t io_1 = computeIo1FromPot1(io_3); // green
  uint8_t io_2 = computeIo2FromPot2(io_4); // blue

  // Output io_5: integer 0..9 (or 0/9 in out-of-range per model)
  int io_5 = computeIo5DigitFromPots(io_3, io_4);

  // Apply PWM
  analogWrite(PinMap::RGB_R_PWM, io_0);
  analogWrite(PinMap::RGB_G_PWM, io_1);
  analogWrite(PinMap::RGB_B_PWM, io_2);

  // Update 7-segment display with io_5
  srWrite7SegNumber0to9(io_5);
}