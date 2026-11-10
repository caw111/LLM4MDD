#include <Arduino.h>
#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Servo.h>

Servo servo1;

static const uint8_t SR_SHCP = A2; // gray
static const uint8_t SR_STCP = A1; // purple
static const uint8_t SR_DS   = A0; // blue

// 74HC595 outputs (Q0..Q7) mapped to 7-seg pins:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Q7 unused.
// Using common cathode 7-seg, so HIGH turns a segment ON.
static const uint8_t segMap[10] = {
  // A B C D E F G (then packed into Q0..Q6)
  // Digit 9 to 0 will use indices accordingly in countdown logic.
  // 0: A B C D E F
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// Map rgb1: Mega pins 2(R),3(G),4(B) (common cathode)
static const uint8_t RGB_R = 2;
static const uint8_t RGB_G = 3;
static const uint8_t RGB_B = 4;

#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srShiftOut(uint8_t value);
#line 59 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void set7segDigit(uint8_t d);
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGBColor(uint8_t r, uint8_t g, uint8_t b);
#line 76 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 105 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_58\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void srShiftOut(uint8_t value) {
  // shift 8 bits MSB first; outputs correspond Q0..Q7 for LSB..Q7 depending on shift order.
  // We'll shift MSB first so that the final bit at position 0 ends up at Q0.
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_SHCP, LOW);

  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_SHCP, LOW);
    digitalWrite(SR_DS, (value >> i) & 0x01);
    digitalWrite(SR_SHCP, HIGH);
  }

  digitalWrite(SR_STCP, HIGH);
  digitalWrite(SR_STCP, LOW);
}

static void set7segDigit(uint8_t d) {
  if (d > 9) d = 9;

  // segMap bit positions:
  // We'll use bits: bit0->A(Q0), bit1->B(Q1), bit2->C(Q2), bit3->D(Q3),
  // bit4->E(Q4), bit5->F(Q5), bit6->G(Q6). bit7 unused (Q7=0).
  uint8_t packed = segMap[d] & 0x7F;
  srShiftOut(packed);
}

static void setRGBColor(uint8_t r, uint8_t g, uint8_t b) {
  // Common cathode: HIGH means current flows -> ON.
  digitalWrite(RGB_R, r ? HIGH : LOW);
  digitalWrite(RGB_G, g ? HIGH : LOW);
  digitalWrite(RGB_B, b ? HIGH : LOW);
}

void setup() {
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);

  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  // Ensure shift register outputs are stable
  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  // Initialization states:
  // - RGB off
  setRGBColor(0, 0, 0);

  // - 7-seg shows "9"
  set7segDigit(9);

  // - Servo at 0 degrees
  servo1.attach(5);
  servo1.write(0);

  // Allow time for power-up settling
  delay(50);
}

void loop() {
  // Colors in order: Red, Green, Blue, Yellow, Cyan, Magenta, White
  // Each for 2 seconds. 7-seg counts down 9..0 each number for 2 seconds.
  // Servo angle: (countdown value) mapped 0 degrees for 9, 20 degrees for 8, ..., 180 degrees for 0
  // => angle = (9 - value) * 20 where value is countdown (9..0).

  const struct {
    uint8_t r, g, b;
  } colors[7] = {
    {1, 0, 0}, // Red
    {0, 1, 0}, // Green
    {0, 0, 1}, // Blue
    {1, 1, 0}, // Yellow
    {0, 1, 1}, // Cyan
    {1, 0, 1}, // Magenta
    {1, 1, 1}  // White
  };

  // We must update synchronously every 2 seconds for 10 steps (9..0).
  // Colors last 7 steps; after that, keep last color (White) while countdown continues to 0.
  for (int value = 9; value >= 0; --value) {
    uint8_t colorIndex = 9 - value; // 0..9
    if (colorIndex > 6) colorIndex = 6;

    // Update all hardware states synchronously for this step
    setRGBColor(colors[colorIndex].r, colors[colorIndex].g, colors[colorIndex].b);
    set7segDigit((uint8_t)value);

    int angle = (9 - value) * 20; // 0deg for 9, ..., 180deg for 0
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    servo1.write(angle);

    delay(2000);
  }

  // After reaching 0, system resets and repeats the sequence (loop naturally repeats).
}
