#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const int SR_DATA_1 = A0; // sr1 DS -> DS on Q? (sr1:DS on A0)
static const int SR_LATCH_1 = A1; // sr1 STCP -> A1
static const int SR_CLOCK_1 = A2; // sr1 SHCP -> A2

// Note: Wokwi wiring uses these exact pins:
static const int SR1_SHCP = A2; // mega:A2 -> sr1:SHCP
static const int SR1_STCP = A1; // mega:A1 -> sr1:STCP
static const int SR1_DS   = A0; // mega:A0 -> sr1:DS

static const int SR2_SHCP = SR1_SHCP; // connected via sr1:SHCP to sr2:SHCP
static const int SR2_STCP = SR1_STCP; // connected via sr1:STCP to sr2:STCP
static const int SR2_DS   = 0;        // not used; sr1:Q7S -> sr2:DS (serial cascade)

static const int LED1_PIN = 2; // mega D2 (through wiring: A3->D2 PWM via rule)
static const int LED2_PIN = 3; // mega D3 (through wiring: A4->D3 PWM via rule)

static const int POT1_ADC = A3; // pot1 -> A3
static const int POT2_ADC = A4; // pot2 -> A4
static const int POT3_ADC = A5; // pot3 -> A5

// 7-seg segments A..G as connected to sr outputs Q0..Q6:
// sr1:Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G
// Uses common-anode displays, so segment ON = LOW. With 74HC595 default outputs,
// we must invert segment bits: set 1 for segment OFF, 0 for segment ON.
static byte segMapCommonAnode[10] = {
  // digits: a b c d e f g (ON segments)
  // Segment ON for digit:
  // 0: a b c d e f
  // 1: b c
  // 2: a b d e g
  // 3: a b c d g
  // 4: b c f g
  // 5: a c d f g
  // 6: a c d e f g
  // 7: a b c
  // 8: a b c d e f g
  // 9: a b c d f g
  //
  // For common-anode: we output LOW to turn a segment ON.
  // We'll build bits as active-high "segment OFF". Easiest:
  // For each segment: OFF bit = NOT(ON).
  //
  // We'll generate the OFF-mask directly for each digit in terms of A..G order
  // for the cascade shift registers where bit0 -> Q0 -> A, ... bit6 -> Q6 -> G.
  //
  // Return mask for OFF=1, ON=0:
  // 0 (ON a b c d e f, OFF g):
  //   a b c d e f ON => OFF bits 0, g OFF => 1
  //   bits0..6: a0 b0 c0 d0 e0 f0 g1 => 0b1000000 = 0x40
  0x40,
  // 1 (ON b c, OFF a d e f g): bits:
  //   a1 b0 c0 d1 e1 f1 g1 => b6..b0: g1 f1 e1 d1 a1? Wait order is b0=a ...:
  //   a=1 -> bit0=1
  //   b=0 -> bit1=0
  //   c=0 -> bit2=0
  //   d=1 -> bit3=1
  //   e=1 -> bit4=1
  //   f=1 -> bit5=1
  //   g=1 -> bit6=1
  //   => bits: 1 0 0 1 1 1 1 (g..a?) in bit positions:
  //   bit0=1 => 0x01
  //   bit1=0
  //   bit2=0
  //   bit3=1 => 0x08
  //   bit4=1 => 0x10
  //   bit5=1 => 0x20
  //   bit6=1 => 0x40
  //   total 0x69
  0x69,
  // 2 (ON a b d e g, OFF c f):
  //   a0 b0 c1 d0 e0 f1 g0 => bit0=0 bit1=0 bit2=1 bit3=0 bit4=0 bit5=1 bit6=0
  //   => 0x04 + 0x20 = 0x24
  0x24,
  // 3 (ON a b c d g, OFF e f):
  //   a0 b0 c0 d0 e1 f1 g0 => bit4=1 (0x10) bit5=1 (0x20)
  //   => 0x30
  0x30,
  // 4 (ON b c f g, OFF a d e):
  //   a1 b0 c0 d1 e1 f0 g0 => bit0=1 (0x01) bit3=1 (0x08) bit4=1 (0x10) => 0x19
  0x19,
  // 5 (ON a c d f g, OFF b e):
  //   a0 b1 c0 d0 e1 f0 g0 => bit1=1 (0x02) bit4=1 (0x10) => 0x12
  0x12,
  // 6 (ON a c d e f g, OFF b):
  //   a0 b1 c0 d0 e0 f0 g0 => bit1=1 => 0x02
  0x02,
  // 7 (ON a b c, OFF d e f g):
  //   a0 b0 c0 d1 e1 f1 g1 => bits3..6 set => 0x78? let's compute:
  //   bit3(d)=1 =>0x08
  //   bit4(e)=1 =>0x10
  //   bit5(f)=1 =>0x20
  //   bit6(g)=1 =>0x40
  //   total 0x08+0x10+0x20+0x40 = 0x78
  0x78,
  // 8 (ON a b c d e f g, OFF none): all ON => OFF bits all 0 => 0x00
  0x00,
  // 9 (ON a b c d f g, OFF e):
  //   a0 b0 c0 d0 e1 f0 g0 => bit4=1 =>0x10
  0x10
};

#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOutCascade(uint8_t sr1bits, uint8_t sr2bits);
#line 126 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateDisplays(int number00_99);
#line 141 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 164 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 104 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_104\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static inline void shiftOutCascade(uint8_t sr1bits, uint8_t sr2bits) {
  // Cascade: sr1 shift register feeds sr2 serially.
  // With both shift registers using same SHCP/STCP.
  // We must shift 16 bits MSB-first as typical: First clocked bit ends up in Q7S of last stage?:
  // In Wokwi and 74HC595 cascade with Q7S->DS, the typical approach:
  // - Shift sr2bits first, then sr1bits, with MSB-first order.
  // We'll do: send 8 bits for sr2 then 8 bits for sr1, MSB-first each.
  //
  // We use active bits for OFF=1. Bit0->Q0(A), ... Bit6->Q6(G). Q7 is unused; set to 0 (OFF).
  uint16_t combined = ((uint16_t)sr2bits & 0x7F) << 8 | ((uint16_t)sr1bits & 0x7F);

  // combined layout is [sr2bits in high 8] [sr1bits low 8] before ordering.
  // We'll clock MSB-first from bit15 to bit0.
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_DS, (combined >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

static inline void updateDisplays(int number00_99) {
  int n = number00_99;
  if (n < 0) n = 0;
  if (n > 99) n = 99;

  int tens = n / 10;
  int ones = n % 10;

  // sr1 controls sevseg1 (tens), sr2 controls sevseg2 (ones)
  uint8_t sr1bits = segMapCommonAnode[tens] & 0x7F;
  uint8_t sr2bits = segMapCommonAnode[ones] & 0x7F;

  shiftOutCascade(sr1bits, sr2bits);
}

void setup() {
  // Shift register control pins
  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);

  digitalWrite(SR1_DS, LOW);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // LED PWM pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Initial state: show "00" and both LEDs off
  analogWrite(LED1_PIN, 0);
  analogWrite(LED2_PIN, 0);
  updateDisplays(0);

  // Small settle time
  delay(10);
}

void loop() {
  // Read potentiometers
  int adc1 = analogRead(POT1_ADC); // 0..1023
  int adc2 = analogRead(POT2_ADC);
  int adc3 = analogRead(POT3_ADC);

  // Map pot1, pot2 linearly to 0..255 (tolerance in acceptance: 1 count)
  // Using direct scaling: floor(adc*255/1023)
  uint8_t led1Pwm = (uint8_t)((long)adc1 * 255L / 1023L);
  uint8_t led2Pwm = (uint8_t)((long)adc2 * 255L / 1023L);

  // pot3 -> 0..99 using floor(adc*99/1023)
  int number = (int)((long)adc3 * 99L / 1023L);
  if (number < 0) number = 0;
  if (number > 99) number = 99;

  // Update displays (keep leading zero)
  updateDisplays(number);

  // LED indication: even->LED1 on, LED2 off; odd->LED2 on, LED1 off
  // Brightness set from respective potentiometer.
  // When brightness is 0, it is allowed to be off.
  if ((number % 2) == 0) {
    analogWrite(LED1_PIN, led1Pwm);
    analogWrite(LED2_PIN, 0);
  } else {
    analogWrite(LED1_PIN, 0);
    analogWrite(LED2_PIN, led2Pwm);
  }

  delay(1); // keep continuous response; within 50ms acceptance
}
