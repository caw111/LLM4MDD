#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

// ---- Frozen interface bindings (from contract) ----
// RGB outputs (PWM counts) mapped from pots: pot0..1023 -> 0..255 then written as PWM duty
static constexpr uint8_t RGB1_R_PIN = 2; // io_0
static constexpr uint8_t RGB1_G_PIN = 3; // io_1
static constexpr uint8_t RGB1_B_PIN = 4; // io_2

static constexpr uint8_t RGB2_R_PIN = 5; // io_3
static constexpr uint8_t RGB2_G_PIN = 6; // io_4
static constexpr uint8_t RGB2_B_PIN = 7; // io_5

static constexpr uint8_t POT1_PIN = A3;  // io_6
static constexpr uint8_t POT2_PIN = A4;  // io_7
static constexpr uint8_t POT3_PIN = A5;  // io_8

// Shift register interface for 7-seg number
static constexpr uint8_t SR_CLOCK_PIN = A2; // clock_pin = A2 (SHCP)
static constexpr uint8_t SR_DATA_PIN  = A0; // data_pin  = A0 (DS)
static constexpr uint8_t SR_LATCH_PIN = A1; // latch_pin = A1 (STCP)

// ---- SysML/IR/IL/C-freeze behavior: exact math used by controller model ----
// Model mapping: io_0..io_5 = io_6/io_7/io_8 * 255 / 1023
// Model io_9 is weirdly specified in contract; however 7-seg requires digit encoding 0-9
// based on red and blue mappings. We'll compute digits as implied by requirement text.

// To honor "invalid_policy: blank": for digits outside 0..9, output all segments off.
// Also note: "segment_bits": [0,1,2,3,4,5,6] => only 7 segments A..G are controlled.
// active_low=true => segment on means bit=0.

// Segment order assumption for each digit:
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G (matches wiring in circuit).
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(uint8_t digit);
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeShift595TwoDigits(uint8_t digit0, uint8_t digit1);
#line 102 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint16_t readPot10bit(uint8_t pin);
#line 107 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t map1023To255_u8(uint16_t adc);
#line 113 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 138 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 33 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_60\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t digitToSegmentsActiveLow(uint8_t digit) {
  // Patterns are for common cathode wiring through active-low shift-register:
  // Bit value = 0 => segment ON, 1 => segment OFF.
  // We'll use standard 7-seg (A,B,C,D,E,F,G) truth table for each numeral.
  if (digit > 9) {
    // blank: all off => all segments OFF => for active-low, all bits = 1
    return 0x7F; // bits 0..6 all 1
  }

  // bit mapping: b0=A, b1=B, b2=C, b3=D, b4=E, b5=F, b6=G
  // For active-low, we will set bits to 0 where segment is ON.
  // Start from all 1 (all off), clear bits for segments that should be ON.
  uint8_t b = 0x7F;

  auto on = [&](uint8_t segBit) { b &= (uint8_t)~(1u << segBit); };

  switch (digit) {
    case 0: on(0); on(1); on(2); on(3); on(4); on(5);       break; // A B C D E F
    case 1: on(1); on(2);                                   break; // B C
    case 2: on(0); on(1);       on(3); on(4);             on(6); break; // A B D E G
    case 3: on(0); on(1); on(2); on(3);                   on(6); break; // A B C D G
    case 4:       on(1); on(2);       on(5); on(6);       break; // B C F G
    case 5: on(0);       on(2); on(3);       on(5); on(6); break; // A C D F G
    case 6: on(0);       on(2); on(3); on(4); on(5); on(6); break; // A C D E F G
    case 7: on(0); on(1); on(2);                           break; // A B C
    case 8: on(0); on(1); on(2); on(3); on(4); on(5); on(6); break; // A B C D E F G
    case 9: on(0); on(1); on(2); on(3);       on(5); on(6); break; // A B C D F G
  }
  return b;
}

// Writes a 2-digit value using two cascaded 74HC595 registers.
// digit 0 (sevseg1/leftmost) -> sr1 bits (segments for one digit).
// digit 1 (sevseg2/rightmost) -> sr2 bits (segments for one digit).
// Active-low segment outputs, invalid digits => blank.
static void writeShift595TwoDigits(uint8_t digit0, uint8_t digit1) {
  uint8_t seg0 = digitToSegmentsActiveLow(digit0); // sevseg1 on sr1 (Q0..Q6)
  uint8_t seg1 = digitToSegmentsActiveLow(digit1); // sevseg2 on sr2 (Q0..Q6)

  // 74HC595 chain: shifting N*8 bits.
  // Contract indicates: sr1 and sr2 are chained, sr2 DS is driven by sr1 Q7S (Q7S connected to sr2 DS).
  // In practice with cascaded registers, the first 8 shifted bits end up in the last register.
  // We'll send bits so that:
  //   - sr1 receives seg0 in its low 7 bits,
  //   - sr2 receives seg1 in its low 7 bits.
  //
  // We'll shift MSB-first within each byte using standard approach:
  // bit7 first then bit0. Since only bits 0..6 are used and bit7 is don't care,
  // we'll set bit7=1 (inactive) to avoid accidental segment.
  uint8_t byte_sr1 = (uint8_t)((seg0 & 0x7F) | 0x80); // bit7=1
  uint8_t byte_sr2 = (uint8_t)((seg1 & 0x7F) | 0x80); // bit7=1

  // Send sr2 first so it ends up closest to output after full shift.
  // With typical cascades: first shifted byte goes to farthest device (sr2 here).
  uint8_t outBytes[2] = { byte_sr2, byte_sr1 };

  digitalWrite(SR_LATCH_PIN, LOW);
  for (int i = 0; i < 2; i++) {
    uint8_t v = outBytes[i];
    for (int bit = 7; bit >= 0; bit--) {
      digitalWrite(SR_DATA_PIN, (v & (1u << bit)) ? HIGH : LOW);
      digitalWrite(SR_CLOCK_PIN, HIGH);
      // keep minimal timing
      digitalWrite(SR_CLOCK_PIN, LOW);
    }
  }
  digitalWrite(SR_LATCH_PIN, HIGH);
}

static uint16_t readPot10bit(uint8_t pin) {
  // Arduino analogRead returns 0..1023.
  return (uint16_t)analogRead(pin);
}

static uint8_t map1023To255_u8(uint16_t adc) {
  // Exact integer mapping equivalent to RealFunctions::floor(adc * 255 / 1023) for 0..1023.
  // Since adc is integer, do integer math with floor.
  return (uint8_t)((uint32_t)adc * 255u / 1023u);
}

void setup() {
  // RGB outputs
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);
  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  // Shift register pins
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);

  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);

  // Ensure initial blank display
  writeShift595TwoDigits(10, 10);

  // Allow analog ref default
  analogReference(DEFAULT);
}

void loop() {
  // Control period is specified as 5 ms; enforce via millis.
  static uint32_t nextTick = 0;
  uint32_t now = millis();
  if ((int32_t)(now - nextTick) < 0) return;
  nextTick = now + 5;

  // Read potentiometers (io_6, io_7, io_8)
  uint16_t io_6_adc = readPot10bit(POT1_PIN); // pot1
  uint16_t io_7_adc = readPot10bit(POT2_PIN); // pot2
  uint16_t io_8_adc = readPot10bit(POT3_PIN); // pot3

  // Model outputs io_0..io_5 (Real): io_X = io_Y * 255 / 1023
  // We'll implement the same using integer floor for PWM duty.
  uint8_t io_0_u8 = map1023To255_u8(io_6_adc); // rgb1 red
  uint8_t io_1_u8 = map1023To255_u8(io_7_adc); // rgb1 green
  uint8_t io_2_u8 = map1023To255_u8(io_8_adc); // rgb1 blue

  uint8_t io_3_u8 = io_0_u8; // rgb2 red
  uint8_t io_4_u8 = io_1_u8; // rgb2 green
  uint8_t io_5_u8 = io_2_u8; // rgb2 blue

  // Apply PWM to RGB LEDs (continuous update)
  analogWrite(RGB1_R_PIN, io_0_u8);
  analogWrite(RGB1_G_PIN, io_1_u8);
  analogWrite(RGB1_B_PIN, io_2_u8);

  analogWrite(RGB2_R_PIN, io_3_u8);
  analogWrite(RGB2_G_PIN, io_4_u8);
  analogWrite(RGB2_B_PIN, io_5_u8);

  // 7-seg displays:
  // sevseg1 shows red value map 0-255 -> 0-9
  // sevseg2 shows blue value map 0-255 -> 0-9
  // Use integer floor mapping: floor(val * 9 / 255)
  uint8_t red_digit  = (uint8_t)((uint32_t)io_0_u8 * 9u / 255u);
  uint8_t blue_digit = (uint8_t)((uint32_t)io_5_u8 * 9u / 255u);

  // Update shift registers
  writeShift595TwoDigits(red_digit, blue_digit);
}
