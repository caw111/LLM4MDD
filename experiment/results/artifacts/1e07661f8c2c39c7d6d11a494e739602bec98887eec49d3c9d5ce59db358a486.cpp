#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
#include <Arduino.h>

//
// Hardware bindings (per frozen contract / circuit)
// RGB (common cathode LEDs, COM to GND.1): active-high PWM on color pins
//   rgb1: R=D2, G=D3, B=D4
//   rgb2: R=D5, G=D6, B=D7
//   rgb3: R=D8, G=D9, B=D10
//
// Shift registers (74HC595), daisy-chained for two digits
//   sr1 control: SHCP=A2 (clock), STCP=A1 (latch), DS=A0 (data)
//   sr1 OE: GND.3 -> hard grounded (active enable)
//   sr1 MR: tied to VCC (sr1:MR connected to VCC); sr2 MR tied to same VCC
//   sr1 Q0..Q7S and sr2 Q0..Q6 drive segments A..G for sevseg1/sevseg2 respectively
//
// Note: countdown and color changes must occur simultaneously every 2000ms.
// The model tick period is 5ms; we implement tick at 5ms for state updates.
//

// PWM pins for RGB intensities
static constexpr uint8_t RGB1_R_PIN = 2;
static constexpr uint8_t RGB1_G_PIN = 3;
static constexpr uint8_t RGB1_B_PIN = 4;

static constexpr uint8_t RGB2_R_PIN = 5;
static constexpr uint8_t RGB2_G_PIN = 6;
static constexpr uint8_t RGB2_B_PIN = 7;

static constexpr uint8_t RGB3_R_PIN = 8;
static constexpr uint8_t RGB3_G_PIN = 9;
static constexpr uint8_t RGB3_B_PIN = 10;

// Shift register pins
static constexpr uint8_t SR_CLOCK_PIN = A2; // SHCP
static constexpr uint8_t SR_LATCH_PIN = A1; // STCP
static constexpr uint8_t SR_DATA_PIN  = A0; // DS

// Model timing
static constexpr uint32_t CONTROL_PERIOD_MS = 5;
static constexpr uint32_t STEP_MS = 2000; // color + countdown decrement period

// Controller state (exact names per model bundle)
static int32_t countdown = 99;
static int32_t colorIndex = 0;

// 74HC595 bits:
// Q0..Q6 correspond to segments A..G for each digit; active_low=true.
// Contract says segment_bits [0..6] are mapped to registers Q0..Q6,
// and active_low=true means 0 bit => segment ON, 1 => segment OFF.
//
// We must render digits for 7-seg (A..G only; no decimal point).
// We'll build the full 16-bit chain value as:
//   - First shifted: sr2 segment bits for digit sevseg2 (right digit), in order Q7S then Q6..Q0? Wait:
//     Contract: shift595_number uses registers sr1 (digit 0) and sr2 (digit 1) with digit mapping
//     segment_bits [0,1,2,3,4,5,6]. We'll implement daisy chain shifting of 8 bits for sr1 then
//     8 bits for sr2 (total 16). Data to sr2 enters after sr1, so shift order in hardware is MSB->LSB.
//     Most Arduino implementations shift MSB first for the chain. We'll implement explicit composition
//     by assuming standard daisy chain where the first 8 shifted bits land in sr2? That would break.
//
// To avoid ambiguity, we align with the contract expectation by using the common approach:
//   - Shift out 16 bits where the first 8 bits become sr1 (right after SR_DATA into Q0..Q7S),
//     and the last 8 bits become sr2. In that case, sr1 output goes to sevseg1 and sr2 output to sevseg2.
// This is the only consistent way to satisfy "sevseg1=left digit, sevseg2=right digit" in a chain where
// sr1 is physically first in the chain (sr1 Q7S -> sr2 DS).
// So: output bits for sr1 go first (higher byte), sr2 go last (lower byte).
//
// We will implement: shiftOut16(msbByteFirst) => sr1 byte first, then sr2 byte.

#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t sevenSegActiveLowDigitBits(uint8_t digit);
#line 109 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void writeShift595Chain(uint16_t srChainValue);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void updateDisplays(uint8_t twoDigitValue);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static void setRGBFromColorIndex(int32_t idx);
#line 181 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void setup();
#line 210 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
void loop();
#line 69 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_57\\repeat_1\\llm_backend\\final\\sketch\\sketch.ino"
static uint8_t sevenSegActiveLowDigitBits(uint8_t digit) {
  // Segment order: A,B,C,D,E,F,G => bits 0..6
  // Active-low: bit=0 means segment ON, bit=1 means OFF.
  //
  // Standard digit patterns for A..G:
  // 0: A B C D E F (G off)
  // 1: B C
  // 2: A B D E G
  // 3: A B C D G
  // 4: B C F G
  // 5: A C D F G
  // 6: A C D E F G
  // 7: A B C
  // 8: A B C D E F G
  // 9: A B C D F G
  static const uint8_t onMask[10] = {
    // bits: A B C D E F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                 // 0
    (1<<1)|(1<<2),                                             // 1
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                       // 2
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                       // 3
    (1<<1)|(1<<2)|(1<<5)|(1<<6),                             // 4
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                       // 5
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),              // 6
    (1<<0)|(1<<1)|(1<<2),                                     // 7
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),       // 8
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                // 9
  };

  uint8_t segOn = (digit < 10) ? onMask[digit] : 0;
  // Active-low for segments A..G:
  // For each segment bit k: output bit = 0 if segment is ON else 1.
  uint8_t bits = 0;
  for (uint8_t k = 0; k <= 6; k++) {
    bool on = (segOn & (1u << k)) != 0;
    if (!on) bits |= (1u << k);
  }
  return bits; // bits[0..6] only; bit 7 is unused in contract (Q7S)
}

static void writeShift595Chain(uint16_t srChainValue) {
  // srChainValue layout: [15..8] => sr1 byte, [7..0] => sr2 byte
  // Shift MSB first for the chain.
  noInterrupts();
  digitalWrite(SR_LATCH_PIN, LOW);

  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR_CLOCK_PIN, LOW);
    uint8_t bitVal = (srChainValue >> i) & 0x1;
    digitalWrite(SR_DATA_PIN, bitVal ? HIGH : LOW);
    digitalWrite(SR_CLOCK_PIN, HIGH);
  }

  digitalWrite(SR_LATCH_PIN, HIGH);
  interrupts();
}

static void updateDisplays(uint8_t twoDigitValue) {
  uint8_t leftDigit = (twoDigitValue / 10) % 10;  // sevseg1
  uint8_t rightDigit = twoDigitValue % 10;         // sevseg2

  uint8_t leftBitsAtoG = sevenSegActiveLowDigitBits(leftDigit);   // bits 0..6 => Q0..Q6 for sevseg1
  uint8_t rightBitsAtoG = sevenSegActiveLowDigitBits(rightDigit); // bits 0..6 => Q0..Q6 for sevseg2

  // Put A..G bits into Q0..Q6; Q7S unused, set to 1 (OFF) to avoid unintended segment.
  uint8_t sr1Byte = (leftBitsAtoG & 0x7Fu) | (1u << 7);
  uint8_t sr2Byte = (rightBitsAtoG & 0x7Fu) | (1u << 7);

  uint16_t chain = (uint16_t(sr1Byte) << 8) | uint16_t(sr2Byte);
  writeShift595Chain(chain);
}

static void setRGBFromColorIndex(int32_t idx) {
  // Apply outputs io_0..io_8 exactly as model expressions:
  // io_0: rgb1 red   = (colorIndex==0 ? 255 : 0)
  // io_1: rgb1 green = (colorIndex==1 ? 255 : (colorIndex==4 ? 255 : (colorIndex==5 ? 0 : 0)))
  //                  => simplifies to 255 when colorIndex==1 or 4; otherwise 0 (including when 5 gives 0)
  // io_2: rgb1 blue  = (colorIndex==2 ? 255 : (colorIndex==3 ? 255 : 0))
  // io_3: rgb2 red   = (colorIndex==1 ? 0 : (colorIndex==3 ? 0 : (colorIndex==4 ? 255 : (colorIndex==1 ? 255 : 0))))
  //                  => effectively 255 only when colorIndex==4; otherwise 0
  // io_4: rgb2 green = (colorIndex==1 ? 255 : (colorIndex==4 ? 255 : (colorIndex==0 ? 0 : 0)))
  //                  => 255 when colorIndex==1 or 4; otherwise 0
  // io_5: rgb2 blue  = (colorIndex==2 ? 0 : (colorIndex==5 ? 0 : 0)) => always 0
  // io_6: rgb3 red   = (colorIndex==2 ? 0 : (colorIndex==0 ? 0 : 0)) => always 0
  // io_7: rgb3 green = (colorIndex==5 ? 255 : (colorIndex==1 ? 0 : 0)) => 255 only when colorIndex==5
  // io_8: rgb3 blue  = (colorIndex==2 ? 255 : (colorIndex==5 ? 255 : 0))
  uint8_t v0 = (idx == 0) ? 255 : 0;
  uint8_t v1 = (idx == 1 || idx == 4) ? 255 : 0;
  uint8_t v2 = (idx == 2 || idx == 3) ? 255 : 0;

  uint8_t v3 = (idx == 4) ? 255 : 0;
  uint8_t v4 = (idx == 1 || idx == 4) ? 255 : 0;
  uint8_t v5 = 0;

  uint8_t v6 = 0;
  uint8_t v7 = (idx == 5) ? 255 : 0;
  uint8_t v8 = (idx == 2 || idx == 5) ? 255 : 0;

  // Contract indicates intensities are PWM_count real values; use analogWrite with 0..255.
  analogWrite(RGB1_R_PIN, v0);
  analogWrite(RGB1_G_PIN, v1);
  analogWrite(RGB1_B_PIN, v2);

  analogWrite(RGB2_R_PIN, v3);
  analogWrite(RGB2_G_PIN, v4);
  analogWrite(RGB2_B_PIN, v5);

  analogWrite(RGB3_R_PIN, v6);
  analogWrite(RGB3_G_PIN, v7);
  analogWrite(RGB3_B_PIN, v8);
}

void setup() {
  // RGB pins
  pinMode(RGB1_R_PIN, OUTPUT);
  pinMode(RGB1_G_PIN, OUTPUT);
  pinMode(RGB1_B_PIN, OUTPUT);

  pinMode(RGB2_R_PIN, OUTPUT);
  pinMode(RGB2_G_PIN, OUTPUT);
  pinMode(RGB2_B_PIN, OUTPUT);

  pinMode(RGB3_R_PIN, OUTPUT);
  pinMode(RGB3_G_PIN, OUTPUT);
  pinMode(RGB3_B_PIN, OUTPUT);

  // Shift register pins
  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);

  // Initial state per contract: countdown=99, colorIndex=0.
  // Startup outputs must match model at t=0.
  setRGBFromColorIndex(colorIndex);
  updateDisplays((uint8_t)countdown);
}

void loop() {
  static uint32_t lastControlMs = 0;
  static uint32_t lastStepMs = 0;

  uint32_t now = millis();

  // Tick every 5ms (control_period_ms). Model updates only occur at step boundary (2000ms).
  if (now - lastControlMs >= CONTROL_PERIOD_MS) {
    lastControlMs += CONTROL_PERIOD_MS;

    // Update every 2000ms: decrement countdown and increment colorIndex simultaneously.
    // Keep within tick structure: model "tick" action corresponds to each 2000ms boundary.
    // We'll trigger when elapsed step >= STEP_MS (and update exactly once per boundary).
    while (now - lastStepMs >= STEP_MS) {
      lastStepMs += STEP_MS;

      // Controller tick action as per SysML model:
      // first start;
      // then if countdown == 0 { countdown=99; colorIndex=0; }
      // then if countdown != 0 { countdown=countdown-1; colorIndex=colorIndex+1; }
      // then if colorIndex == 6 { colorIndex=0; }
      if (countdown == 0) {
        countdown = 99;
        colorIndex = 0;
      }
      if (countdown != 0) {
        countdown = countdown - 1;
        colorIndex = colorIndex + 1;
      }
      if (colorIndex == 6) {
        colorIndex = 0;
      }

      // Apply outputs in the same cycle after state update
      setRGBFromColorIndex(colorIndex);
      updateDisplays((uint8_t)countdown);
    }
  }
}
