#include <Arduino.h>

// --- Pins (Mega 2560) preserved from wiring ---
static const uint8_t SR1_SHCP = A2; // mega:A2 -> sr1:SHCP
static const uint8_t SR1_STCP = A1; // mega:A1 -> sr1:STCP
static const uint8_t SR1_DS   = A0; // mega:A0 -> sr1:DS

static const uint8_t SR2_SHCP = SR1_SHCP; // shared via wiring
static const uint8_t SR2_STCP = SR1_STCP; // shared via wiring
static const uint8_t SR2_DS   = A0; // sr2:DS is connected to sr1:Q7S, not directly from MCU; handled by chaining

static const uint8_t LED1_PWM_PIN = 2; // mega:2 -> led1:A (PWM)
static const uint8_t LED2_PWM_PIN = 3; // mega:3 -> led2:A (PWM)

static const uint8_t POT1_ADC = A3; // pot1:SIG -> mega:A3 (brightness for even/odd LED rule)
static const uint8_t POT2_ADC = A4; // pot2:SIG -> mega:A4 (brightness for odd/even rule)
static const uint8_t POT3_ADC = A5; // pot3:SIG -> mega:A5 (display number)

// --- 74HC595 chain: sr1 drives sevseg1, sr2 drives sevseg2 ---
// For a common-anode 7-seg with typical segment connections in Wokwi,
// "1" typically turns a segment OFF (depends on how the display is wired).
// The provided wiring uses sr1 Q0..Q6 -> sevseg1 A..G and preserves COM.1 to both displays.
// We'll use an invert scheme (1=segment off) for safety with common-anode.
// Segment bit mapping (A,B,C,D,E,F,G) => bits 0..6.
static uint8_t digitToSegmentMask(uint8_t digit) {
  // Segment presence for digits on 7-seg: A B C D E F G
  // We'll define which segments should be ON for a given digit, then invert
  // to match common-anode behavior with shift-register outputs.
  static const uint8_t onMask[10] = {
    // A B C D E F G
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),         // 0 (A,B,C,D,E,F)
    (1<<1)|(1<<2),                                   // 1 (B,C)
    (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),              // 2 (A,B,D,E,G)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),             // 3 (A,B,C,D,G)
    (1<<1)|(1<<2)|(1<<5)|(1<<6),                    // 4 (B,C,F,G)
    (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),             // 5 (A,C,D,F,G)
    (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),    // 6 (A,C,D,E,F,G)
    (1<<0)|(1<<1)|(1<<2),                           // 7 (A,B,C)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),// 8 (all)
    (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)      // 9 (A,B,C,D,F,G)
  };

  uint8_t on = onMask[digit % 10];
  // Common-anode: assume segment ON corresponds to output LOW -> invert bits.
  // Unused Q7S is chained; we only use Q0..Q6.
  uint8_t off = (~on) & 0x7F;
  return off;
}

static void shiftOutDigitToChain(uint8_t tens, uint8_t units) {
  // Chain order for two cascaded 74HC595:
  // First shifted bits end up in the farthest chip (sr2) depending on wiring.
  // With DS connected from sr1.Q7S to sr2.DS, typical behavior:
  // - Bits shifted first go into sr2 (farthest)
  // - Bits shifted later go into sr1 (nearest)
  //
  // We need: sr1 -> sevseg1 (tens), sr2 -> sevseg2 (units)
  // Thus, we must shift sr2 data first, then sr1 data.
  uint8_t sr2Data = digitToSegmentMask(units); // sevseg2
  uint8_t sr1Data = digitToSegmentMask(tens);  // sevseg1

  // Need 8 bits each (Q0..Q7). Q7S is used for chaining; we must set it to 0.
  // We'll place 7-seg bits in Q0..Q6, and set Q7 to 0.
  uint8_t sr2Byte = (sr2Data & 0x7F); // Q7 = bit7 remains 0
  uint8_t sr1Byte = (sr1Data & 0x7F);

  // Bit-bang to both chips via one shared SR clock and latch:
  // Data is clocked by SHCP, latched by STCP.
  uint8_t combined[2] = { sr2Byte, sr1Byte };

  for (int chip = 0; chip < 2; chip++) {
    uint8_t b = combined[chip];
    for (int bit = 7; bit >= 0; bit--) {
      uint8_t outBit = (b >> bit) & 0x01;
      digitalWrite(SR1_DS, outBit ? HIGH : LOW);
      digitalWrite(SR1_SHCP, HIGH);
      // small hold
      digitalWrite(SR1_SHCP, LOW);
    }
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

static uint8_t mapADCLinear0_255(int adc) {
  // Original: "ADC0..1023线性映射0..255，容差1计数"
  // We'll use rounding: floor(adc*255/1023).
  if (adc < 0) adc = 0;
  if (adc > 1023) adc = 1023;
  return (uint8_t)((adc * 255L) / 1023L);
}

static uint8_t mapADCTo0_99(int adc) {
  // floor(ADC*99/1023)
  if (adc < 0) adc = 0;
  if (adc > 1023) adc = 1023;
  return (uint8_t)((adc * 99L) / 1023L);
}

void setup() {
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  pinMode(LED1_PWM_PIN, OUTPUT);
  pinMode(LED2_PWM_PIN, OUTPUT);

  // Initial state: displays "00" and both LEDs off.
  // pot3 initial may be arbitrary, but requirement says on power/reset show "00".
  shiftOutDigitToChain(0, 0);
  analogWrite(LED1_PWM_PIN, 0);
  analogWrite(LED2_PWM_PIN, 0);
}

void loop() {
  // Read potentiometers continuously and update within 50ms of real sampling.
  int adc1 = analogRead(POT1_ADC); // pot1
  int adc2 = analogRead(POT2_ADC); // pot2
  int adc3 = analogRead(POT3_ADC); // pot3

  uint8_t ledBrightness1 = mapADCLinear0_255(adc1); // A3 -> 0..255
  uint8_t ledBrightness2 = mapADCLinear0_255(adc2); // A4 -> 0..255

  uint8_t value0_99 = mapADCTo0_99(adc3);            // A5 -> 0..99
  uint8_t tens = value0_99 / 10;
  uint8_t units = value0_99 % 10;

  // LED indication rule with even/odd:
  // Even: led1 on with pot1 brightness, led2 off.
  // Odd:  led2 on with pot2 brightness, led1 off.
  // Brightness 0 is allowed to fully turn off.
  if ((value0_99 & 0x01) == 0) {
    analogWrite(LED1_PWM_PIN, ledBrightness1);
    analogWrite(LED2_PWM_PIN, 0);
  } else {
    analogWrite(LED1_PWM_PIN, 0);
    analogWrite(LED2_PWM_PIN, ledBrightness2);
  }

  // Update displays: sr1 tens, sr2 units, keep leading zero (0..9 -> tens=0)
  shiftOutDigitToChain(tens, units);

  // No extra delay to keep responsiveness; but a minimal yield helps timing stability.
  // Target: continuous response; changes within 50ms.
  delay(5);
}