#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Pins (Mega 2560)
static const uint8_t POT1_PIN = A3; // pot1 SIG
static const uint8_t POT2_PIN = A4; // pot2 SIG

// 74HC595 sr1 control pins
static const uint8_t SR_SHCP = A2; // SHCP (gray)
static const uint8_t SR_STCP = A1; // STCP (purple)
static const uint8_t SR_DS   = A0;   // DS   (blue)

// Potentiometer range
static const int POT_MIN = 0;
static const int POT_MAX = 1023;

// Display / RGB behavior constants from requirements
static const float SCALE_FOR_0_TO_9 = 28.33f;

#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t clampU8(int v);
#line 26 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut595(uint8_t value);
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t segPatternForDigit(uint8_t digit);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 19 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_61\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t clampU8(int v) {
  if (v < 0) return 0;
  if (v > 255) return 255;
  return (uint8_t)v;
}

// Send one byte MSB-first to 74HC595
static void shiftOut595(uint8_t value) {
  // DS is SR_DS; SHCP toggles clock; STCP latches
  digitalWrite(SR_DS, LOW);

  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DS, (value & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR_SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SR_SHCP, LOW);
    delayMicroseconds(1);
  }
  // Latch into outputs
  digitalWrite(SR_STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_STCP, LOW);
}

static uint8_t segPatternForDigit(uint8_t digit) {
  // Common cathode 7-seg.
  // The circuit wires:
  // sr1:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
  // With this common-cathode display, setting the corresponding output HIGH
  // turns the segment ON.
  // We map digits 0-9 to segments {A,B,C,D,E,F,G}.
  // Bit mapping in shift register output bits:
  // Q0 = bit0, ... Q6 = bit6.
  static const uint8_t patterns[10] = {
    // 0: A B C D E F (no G)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),
    // 1: B C
    (1<<1)|(1<<2),
    // 2: A B D E G
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),
    // 3: A B C D G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),
    // 4: B C F G
    (1<<1)|(1<<2)|(1<<5)|(1<<6),
    // 5: A C D F G
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),
    // 6: A C D E F G
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
    // 7: A B C
    (1<<0)|(1<<1)|(1<<2),
    // 8: A B C D E F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),
    // 9: A B C D F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)
  };

  if (digit > 9) digit = 0;
  return patterns[digit];
}

void setup() {
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);

  pinMode(SR_SHCP, OUTPUT);
  pinMode(SR_STCP, OUTPUT);
  pinMode(SR_DS, OUTPUT);

  digitalWrite(SR_SHCP, LOW);
  digitalWrite(SR_STCP, LOW);
  digitalWrite(SR_DS, LOW);

  // Initialization requirement:
  // RGB LED off, sevseg display shows "0".
  // - RGB is driven by shift register outputs Q0..Q6 as wired, plus MR/OE fixed LOW
  //   via circuit; to ensure RGB off, output all zeros.
  // - But we also need sevseg show "0". Q0..Q6 control segments; RGB LED shares Q lines
  //   only via the circuit wiring: rgb COM is on mega GND, rgb1 R/G/B are:
  //     rgb1 R -> sr1:MR (red) (wired to sr1:MR) (note: MR is connected to sr1:VCC in circuit)
  //     rgb1 G -> Q1..Q4
  //     rgb1 B -> Q0 and Q5 and Q6 through the shown connections:
  //       Q0->A(green), Q5->F(green), Q6->G(green) (these are segments, not RGB)
  //     rgb1 B is not wired to Q lines; rgb1:B is directly blue pin from mega:4 in circuit,
  //     however in this provided wiring, rgb1 B is on mega:4 (not connected to sr1 outputs).
  //
  // In the provided connections, sr1 Q lines go only to sevseg segments (A..G) and mr ties
  // to sevseg COM. There is no sr output directly to rgb R/G/B pins except:
  //   sr1:MR -> sevseg1:COM.1
  //   sr1 Q0..Q6 -> sevseg1 segments
  // So setting the shift register outputs will only affect the 7-seg display.
  //
  // Therefore: shift in pattern for digit "0". RGB stays off by virtue of its direct pins
  // (mega:2/3/4) not being written by this sketch.
  shiftOut595(segPatternForDigit(0));
}

void loop() {
  int p1 = analogRead(POT1_PIN);
  int p2 = analogRead(POT2_PIN);

  // Map pot1 to red and green (0..255)
  int red   = map(p1, POT_MIN, POT_MAX, 0, 255);
  int green = map(p1, POT_MIN, POT_MAX, 0, 255);
  // Map pot2 to blue (0..255)
  int blue  = map(p2, POT_MIN, POT_MAX, 0, 255);

  red = clampU8(red);
  green = clampU8(green);
  blue = clampU8(blue);

  // Requirement states RGB LED uses potentiometers to display color.
  // RGB pins in circuit: mega:2->R, mega:3->G, mega:4->B.
  // Drive them with PWM.
  analogWrite(2, red);
  analogWrite(3, green);
  analogWrite(4, blue);

  // Compute average brightness and map to 0..9:
  // average = (red + green + blue) / 3
  // displayDigit = (average / 28.33)
  float average = (red + green + blue) / 3.0f;
  float scaled = average / SCALE_FOR_0_TO_9;
  int digit = (int)(scaled + 0.000001f); // floor

  if (digit < 0) digit = 0;
  if (digit > 9) digit = 9;

  // Update 7-seg through shift register
  uint8_t pattern = segPatternForDigit((uint8_t)digit);
  shiftOut595(pattern);

  // Small delay for stable reads/updates
  delay(20);
}
