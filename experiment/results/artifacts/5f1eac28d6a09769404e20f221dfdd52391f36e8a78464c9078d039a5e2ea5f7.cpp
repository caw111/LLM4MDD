#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

static const uint8_t LED1_PIN = 2;  // mega:2 -> led1:A
static const uint8_t RGB_R_PIN = 3;  // mega:3 -> rgb1:R
static const uint8_t RGB_G_PIN = 4;  // mega:4 -> rgb1:G
static const uint8_t RGB_B_PIN = 5;  // mega:5 -> rgb1:B

// Shift register control (74HC595)
static const uint8_t SR1_SHCP = A2;  // mega:A2 -> sr1:SHCP
static const uint8_t SR1_STCP = A1;  // mega:A1 -> sr1:STCP
static const uint8_t SR1_DS   = A0;  // mega:A0 -> sr1:DS
static const uint8_t SR_OE    = 0;   // sr1:OE tied to GND via wiring; no Arduino control

// Cascaded shift registers: SR1 outputs for sevseg1 (Q0..Q7), SR2 outputs for sevseg2
static const uint8_t SR1_MR  = 0;   // sr1:MR tied to VCC; no Arduino control
static const uint8_t SR2_MR  = 0;   // sr2:MR tied to VCC; no Arduino control

// 7-segment wiring assumed standard for Q outputs:
// sr? mapping to segments: A->Q0, B->Q1, C->Q2, D->Q3, E->Q4, F->Q5, G->Q6, (Q7S unused)
static const uint8_t SEG_A = 1u << 0;
static const uint8_t SEG_B = 1u << 1;
static const uint8_t SEG_C = 1u << 2;
static const uint8_t SEG_D = 1u << 3;
static const uint8_t SEG_E = 1u << 4;
static const uint8_t SEG_F = 1u << 5;
static const uint8_t SEG_G = 1u << 6;

// For common cathode 7-seg, segment ON = HIGH.
// Bits correspond to Q0..Q6 as: A,B,C,D,E,F,G.
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments(uint8_t d);
#line 63 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void srWrite16(uint16_t data16);
#line 81 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void displayTwoDigits(uint8_t tens, uint8_t ones);
#line 97 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 125 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 30 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_116\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments(uint8_t d) {
  // Digits 0-9:
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
  switch (d) {
    case 0: return SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F;
    case 1: return SEG_B|SEG_C;
    case 2: return SEG_A|SEG_B|SEG_D|SEG_E|SEG_G;
    case 3: return SEG_A|SEG_B|SEG_C|SEG_D|SEG_G;
    case 4: return SEG_B|SEG_C|SEG_F|SEG_G;
    case 5: return SEG_A|SEG_C|SEG_D|SEG_F|SEG_G;
    case 6: return SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G;
    case 7: return SEG_A|SEG_B|SEG_C;
    case 8: return SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G;
    case 9: return SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G;
    default: return 0;
  }
}

// Shift out 16 bits to cascaded 74HC595s.
// Since sr1 DS feeds both (Q7S of sr1 -> DS of sr2) via sr1:Q7S, and DS is driven by Arduino,
// the usual order for cascaded devices is: shift MSB for SR1 first (or LSB depending on wiring).
// We'll implement a robust mapping by shifting bits from MSB to LSB for SR1 then SR2,
// then latch. Here we assume SR1 data appears in its outputs after shifting 8 bits,
// and SR2 after 16 bits; using MSB-first consistent with typical hardware.
static void srWrite16(uint16_t data16) {
  // Latch low during shifting
  digitalWrite(SR1_STCP, LOW);

  // Shift 16 bits MSB first: bit15 corresponds to first shifted into SR1's DS.
  for (int i = 15; i >= 0; --i) {
    digitalWrite(SR1_SHCP, LOW);
    digitalWrite(SR1_DS, (data16 & (1u << i)) ? HIGH : LOW);
    digitalWrite(SR1_SHCP, HIGH);
  }

  // Latch high
  digitalWrite(SR1_STCP, HIGH);
  // Small pulse width; keep it stable
  delayMicroseconds(5);
  digitalWrite(SR1_STCP, LOW);
}

static void displayTwoDigits(uint8_t tens, uint8_t ones) {
  uint8_t seg1 = digitToSegments(tens);
  uint8_t seg2 = digitToSegments(ones);

  // Compose 16-bit word: lower 8 bits -> SR1 outputs, upper 8 bits -> SR2 outputs
  // because in cascaded shift registers, the last 8 shifted bits end up in the first device (SR1)
  // if shifting order is MSB-first. With our MSB-first shift implementation, mapping is:
  // - Bits [7:0] -> SR2 (later shifted bits)
  // - Bits [15:8] -> SR1
  //
  // To match wiring requirement (tens on sevseg1 driven by sr1, ones on sevseg2 driven by sr2),
  // we select mapping: SR1 content must be in upper byte, SR2 in lower byte.
  uint16_t word = (uint16_t(seg1) << 8) | uint16_t(seg2);
  srWrite16(word);
}

void setup() {
  // GPIO
  pinMode(LED1_PIN, OUTPUT);
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);

  // Shift registers control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Initial states per requirements
  digitalWrite(LED1_PIN, LOW); // led1 initially off

  // Start RGB with Red on (first color in sequence)
  digitalWrite(RGB_R_PIN, HIGH);
  digitalWrite(RGB_G_PIN, LOW);
  digitalWrite(RGB_B_PIN, LOW);

  // Initial displays: 00
  displayTwoDigits(0, 0);
}

void loop() {
  static bool ledOn = false;
  static uint8_t rgbIndex = 0; // 0..6 sequence: R,G,B,Y,C,M,W
  static uint8_t count = 0;   // 0..99
  static unsigned long lastUpdateMs = 0;

  const unsigned long intervalMs = 2000;

  unsigned long now = millis();
  if (now - lastUpdateMs < intervalMs) return;
  lastUpdateMs += intervalMs;

  // 1) Single-color LED flash: 2s on, 2s off
  ledOn = !ledOn;
  digitalWrite(LED1_PIN, ledOn ? HIGH : LOW);

  // 2) RGB cycle every 2 seconds in order:
  // Red, Green, Blue, Yellow, Cyan, Magenta, White (common cathode -> ON = HIGH)
  rgbIndex = (rgbIndex + 1) % 7;
  switch (rgbIndex) {
    case 0: // Red
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case 1: // Green
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case 2: // Blue
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case 3: // Yellow (R+G)
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, LOW);
      break;
    case 4: // Cyan (G+B)
      digitalWrite(RGB_R_PIN, LOW);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case 5: // Magenta (R+B)
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, LOW);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
    case 6: // White (R+G+B)
    default:
      digitalWrite(RGB_R_PIN, HIGH);
      digitalWrite(RGB_G_PIN, HIGH);
      digitalWrite(RGB_B_PIN, HIGH);
      break;
  }

  // 3) 7-segment count from 00 to 99, update every 2 seconds, reset to 00 after 99
  count = (count + 1) % 100;
  uint8_t tens = count / 10;
  uint8_t ones = count % 10;
  displayTwoDigits(tens, ones);
}
