#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

// Shift register (74HC595) to drive 7-seg segments
// Wiring preserved from circuit:
// mega:A2 -> sr1:SHCP  (SER clock)
// mega:A1 -> sr1:STCP  (latch clock)
// mega:A0 -> sr1:DS    (data)

// Additionally wired:
// mega:2 -> led_red:A
// mega:3 -> led_yellow:A
// mega:4 -> led_green:A
// All three LEDs cathodes at mega:GND.1 (active HIGH)

// 7-seg segments:
// sr1:Q0 -> sevseg1:A
// sr1:Q5 -> sevseg1:F
// sr1:Q6 -> sevseg1:G
// sr1:Q1 -> sevseg1:B
// sr1:Q2 -> sevseg1:C
// sr1:Q3 -> sevseg1:D
// sr1:Q4 -> sevseg1:E
// sevseg1:COM.1 connected to sr1:MR (tied to VCC via sr1:MR), so we assume segments are active HIGH
// (i.e., setting segment line HIGH lights it).

// Pin assignments
static const uint8_t PIN_LED_RED    = 2;
static const uint8_t PIN_LED_YELLOW = 3;
static const uint8_t PIN_LED_GREEN  = 4;

static const uint8_t PIN_SR_DS   = A0; // DS
static const uint8_t PIN_SR_STCP = A1; // STCP (latch)
static const uint8_t PIN_SR_SHCP = A2; // SHCP (shift)

static uint8_t currentLitCount = 0;

// Segment mapping for digits 0-9 (active HIGH) using standard 7-seg:
// Segments: A B C D E F G
// Digit to segments:
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
//
// Need to translate into Q bits:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
static const uint8_t digitSegmentsQMask[10] = {
  // Each mask bit corresponds to Q0..Q6.
  // bit0=A, bit1=B, bit2=C, bit3=D, bit4=E, bit5=F, bit6=G
  // 0: A B C D E F = 0b00111111 = 0x3F
  0x3F,
  // 1: B C = 0b00000110 = 0x06
  0x06,
  // 2: A B D E G = A(1)+B(2)+D(8)+E(16)+G(64)=1+2+8+16+64=91 => 0b01011011=0x5B
  0x5B,
  // 3: A B C D G = 1+2+4+8+64=79 => 0b01001111=0x4F
  0x4F,
  // 4: B C F G = 2+4+32+64=102 => 0b01100110=0x66
  0x66,
  // 5: A C D F G = 1+4+8+32+64=109 => 0b01101101=0x6D
  0x6D,
  // 6: A C D E F G = 1+4+8+16+32+64=125 => 0b01111101=0x7D
  0x7D,
  // 7: A B C = 1+2+4=7 => 0b00000111=0x07
  0x07,
  // 8: A B C D E F G = 0b01111111=0x7F
  0x7F,
  // 9: A B C D F G = 1+2+4+8+32+64=111 => 0b01101111=0x6F
  0x6F
};

#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srShiftOut7Segments(uint8_t qMask7);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayDigit(uint8_t d);
#line 108 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setAllLedsOff();
#line 115 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setLedsByCountAndRandomSelection(uint8_t count);
#line 147 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 176 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 78 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_13\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srShiftOut7Segments(uint8_t qMask7)
{
  // We must write Q0..Q6 according to qMask7; Q7 is don't care.
  // We'll shift out 8 bits: bit7 first (Q7) then bit6..bit0.
  // Many implementations are MSB-first with DS sampled on rising SHCP.
  // We'll set Q7 = 0 to avoid unintended behavior.
  uint8_t data = (uint8_t)(qMask7 & 0x7F);
  uint8_t byteToSend = data; // bit0..bit6 used, bit7=0

  // Shift out 8 bits MSB-first
  digitalWrite(PIN_SR_STCP, LOW); // keep latch low during shifting

  for (int i = 7; i >= 0; --i) {
    digitalWrite(PIN_SR_DS, (byteToSend >> i) & 0x01 ? HIGH : LOW);
    // pulse SHCP
    digitalWrite(PIN_SR_SHCP, HIGH);
    digitalWrite(PIN_SR_SHCP, LOW);
  }

  // latch
  digitalWrite(PIN_SR_STCP, HIGH);
  digitalWrite(PIN_SR_STCP, LOW);
}

static void displayDigit(uint8_t d)
{
  if (d > 9) d = 0; // should not happen
  srShiftOut7Segments(digitSegmentsQMask[d]);
}

static void setAllLedsOff()
{
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
}

static void setLedsByCountAndRandomSelection(uint8_t count)
{
  // Turn all off first
  setAllLedsOff();

  // Randomly select which of the 3 LEDs to light up, for the given count.
  // For count 0: none on.
  if (count == 0) return;

  // Map LEDs to bits
  // bit0=red, bit1=yellow, bit2=green
  uint8_t mask = 0;

  // Choose a random subset with exactly 'count' bits set.
  // There are 8 subsets total (excluding order); brute force is simplest and deterministic.
  uint8_t candidates[8];
  uint8_t n = 0;
  for (uint8_t m = 0; m < 8; ++m) {
    // popcount for 3-bit number
    uint8_t pc = ((m >> 0) & 1) + ((m >> 1) & 1) + ((m >> 2) & 1);
    if (pc == count) {
      candidates[n++] = m;
    }
  }

  uint8_t chosen = candidates[random(0, n)];

  if (chosen & 0x01) digitalWrite(PIN_LED_RED, HIGH);
  if (chosen & 0x02) digitalWrite(PIN_LED_YELLOW, HIGH);
  if (chosen & 0x04) digitalWrite(PIN_LED_GREEN, HIGH);
}

void setup()
{
  // LEDs
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  setAllLedsOff();

  // Shift register pins
  pinMode(PIN_SR_DS, OUTPUT);
  pinMode(PIN_SR_STCP, OUTPUT);
  pinMode(PIN_SR_SHCP, OUTPUT);

  // Initialize SR control lines to known states
  digitalWrite(PIN_SR_DS, LOW);
  digitalWrite(PIN_SR_STCP, LOW);
  digitalWrite(PIN_SR_SHCP, LOW);

  // Display '0' on reset
  displayDigit(0);

  // Random seed
  // Use analogRead from a floating analog pin as a seed source.
  // (A3 exists on Mega; if unused, likely floating.)
  randomSeed((uint32_t)analogRead(A3) ^ (uint32_t)micros());

  currentLitCount = 0;
}

void loop()
{
  static uint32_t lastUpdateMs = 0;

  uint32_t now = millis();
  if (now - lastUpdateMs >= 2000UL) {
    lastUpdateMs = now;

    // Randomly select 0 to 3 LEDs
    currentLitCount = (uint8_t)random(0, 4);

    // Randomly select which LEDs to light up (for that count)
    setLedsByCountAndRandomSelection(currentLitCount);

    // Immediately update display to show the number of lit LEDs
    displayDigit(currentLitCount);
  }
}
