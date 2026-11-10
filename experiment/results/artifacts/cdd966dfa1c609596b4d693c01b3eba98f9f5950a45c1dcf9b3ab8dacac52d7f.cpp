#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r, uint8_t g, uint8_t b);
#line 70 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite7Segments(uint8_t segMask);
#line 89 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraph(uint8_t n);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 137 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_63\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setRGB(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(2, r);
  analogWrite(3, g);
  analogWrite(4, b);
}

/*
  Wiring (per provided circuit):
  - RGB common cathode:
      COM -> GND
      R -> pin 2
      G -> pin 3
      B -> pin 4
  - Shift register (74HC595) sr1:
      VCC -> 5V
      MR  -> sr1:MR -> pin A? (wired from sr1:MR to sr1:MR red; actual connection line: sr1:MR to sevseg1:COM.1 is confusing)
      SHCP -> pin A2
      STCP -> pin A1
      DS   -> pin A0
      OE   -> GND
      GND  -> GND
  - 7-seg display (sevseg1) with pins:
      A -> sr1:Q0
      B -> sr1:Q1
      C -> sr1:Q2
      D -> sr1:Q3
      E -> sr1:Q4
      F -> sr1:Q5
      G -> sr1:Q6
      COM.1 -> sr1:MR (per connection: sr1:MR -> sevseg1:COM.1)
*/
static const uint8_t SR_SHCP_PIN = A2; // shift clock
static const uint8_t SR_STCP_PIN = A1; // latch clock
static const uint8_t SR_DS_PIN   = A0; // data

// Map digit 0-9 to segments A..G (common-cathode assumed by "common": cathode).
// We'll drive the shift-register outputs Q0..Q6 to segments A..G.
// Bit0 -> Q0 -> segment A
// Bit1 -> Q1 -> segment B
// Bit2 -> Q2 -> segment C
// Bit3 -> Q3 -> segment D
// Bit4 -> Q4 -> segment E
// Bit5 -> Q5 -> segment F
// Bit6 -> Q6 -> segment G
static const uint8_t digitSegments[10] = {
  // 0: A B C D E F
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5),
  // 1: B C
  (1 << 1) | (1 << 2),
  // 2: A B D E G
  (1 << 0) | (1 << 1) | (1 << 3) | (1 << 4) | (1 << 6),
  // 3: A B C D G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 6),
  // 4: B C F G
  (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6),
  // 5: A C D F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6),
  // 6: A C D E F G
  (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 7: A B C
  (1 << 0) | (1 << 1) | (1 << 2),
  // 8: A B C D E F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6),
  // 9: A B C D F G
  (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) | (1 << 6)
};

static void srWrite7Segments(uint8_t segMask) {
  // Shift out 8 bits: send bit0..bit6 to Q0..Q6, bit7 as 0.
  // For 74HC595, the order depends on wiring; in Wokwi typically DS on rising SHCP clocks
  // shift towards Q0. We'll send LSB first to match Q0..Q6 mapping.
  uint8_t out = segMask & 0x7F;

  digitalWrite(SR_STCP_PIN, LOW);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR_DS_PIN, (out >> i) & 0x01 ? HIGH : LOW);
    digitalWrite(SR_SHCP_PIN, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR_SHCP_PIN, LOW);
    delayMicroseconds(2);
  }
  digitalWrite(SR_STCP_PIN, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR_STCP_PIN, LOW);
}

static void setBargraph(uint8_t n) {
  // bargraph1: A1..A10 to pins 5..13 and 44. Bargraph cathodes C1..C10 to GND (already wired).
  // Rule: light exactly n segments for number n (0..9).
  // Pin mapping per circuit:
  // A1->5, A2->6, A3->7, A4->8, A5->9, A6->10, A7->11, A8->12, A9->13, A10->44
  const uint8_t pins[10] = {5,6,7,8,9,10,11,12,13,44};

  for (uint8_t i = 0; i < 10; i++) {
    if (i < n) digitalWrite(pins[i], HIGH);
    else        digitalWrite(pins[i], LOW);
  }
}

void setup() {
  // RGB pins (common cathode, so HIGH turns LED off; LOW turns on for cathode? In Wokwi rgb-led cathode:
  // likely HIGH drives current. We'll use PWM values where higher means brighter (common cathode modeled internally).
  pinMode(2, OUTPUT); // R
  pinMode(3, OUTPUT); // G
  pinMode(4, OUTPUT); // B

  // Shift register pins
  pinMode(SR_SHCP_PIN, OUTPUT);
  pinMode(SR_STCP_PIN, OUTPUT);
  pinMode(SR_DS_PIN, OUTPUT);

  digitalWrite(SR_SHCP_PIN, LOW);
  digitalWrite(SR_STCP_PIN, LOW);
  digitalWrite(SR_DS_PIN, LOW);

  // Bargraph pins
  const uint8_t pins[10] = {5,6,7,8,9,10,11,12,13,44};
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  }

  // Initialization requirements:
  // - 7-seg display shows "0"
  srWrite7Segments(digitSegments[0]);

  // - LED bar graph all segments off
  setBargraph(0);

  // - RGB LED red
  // Use full intensity for red, off others.
  setRGB(255, 0, 0);
}

void loop() {
  static uint8_t num = 0;
  static unsigned long lastUpdate = 0;

  unsigned long now = millis();
  if (now - lastUpdate >= 2000) {
    lastUpdate = now;

    // Output current num before increment? Requirement: "cycle ... updating ... every 2 seconds"
    // Start already initialized to 0; now advance to next number each 2 seconds.
    num++;
    if (num > 9) num = 0;

    // 7-seg: show current number
    srWrite7Segments(digitSegments[num]);

    // Bargraph: light up number of segments equal to current number
    setBargraph(num);

    // RGB color scheme
    if (num <= 3) {
      setRGB(255, 0, 0);   // Red
    } else if (num <= 6) {
      setRGB(0, 255, 0);   // Green
    } else {
      setRGB(0, 0, 255);   // Blue
    }
  }
}
