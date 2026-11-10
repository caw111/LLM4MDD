#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>
#include <Servo.h>

#include <stdint.h>

// ---------- 74HC595 pin mapping (from wiring) ----------
static const uint8_t SR1_SHCP = A2; // SR1 SHCP -> mega:A2
static const uint8_t SR1_STCP = A1; // SR1 STCP -> mega:A1
static const uint8_t SR1_DS   = A0; // SR1 DS -> mega:A0
static const uint8_t SR1_OE   = A3; // Not connected in provided mapping; OE is tied low in circuit via GND.3->OE.
                                   // We'll keep it unmanaged here.

static const uint8_t SR2_SHCP = SR1_SHCP; // Wired SHCP chained
static const uint8_t SR2_STCP = SR1_STCP; // Wired STCP chained
// DS is driven through SR1:Q7S -> SR2:DS (serial chain), so we just shift out 16 bits to cover both chips.

// ---------- Servo mapping (from wiring) ----------
static const uint8_t SERVO1_PWM = 12; // mega:12 -> servo1:PWM
static const uint8_t SERVO2_PWM = 13; // mega:13 -> servo2:PWM

Servo servo1;
Servo servo2;

// ---------- 7-seg digit bit order (from wiring) ----------
// Each 7-segment uses a 74HC595 with outputs Q0..Q7 mapped to segments A..G and decimal point not used.
// Wiring:
// sr1 Q0 -> sevseg1 A
// sr1 Q1 -> sevseg1 B
// sr1 Q2 -> sevseg1 C
// sr1 Q3 -> sevseg1 D
// sr1 Q4 -> sevseg1 E
// sr1 Q5 -> sevseg1 F
// sr1 Q6 -> sevseg1 G
// (Q7 not used for sevseg1; decimal point not mapped)
//
// sr2 Q0..Q6 -> sevseg2 A..G respectively.

// ---------- Segment encoding ----------
// We'll assume a common behavior where '1' turns a segment ON.
// If your hardware is common-anode and segments are active-low,
// you must invert the pattern. The circuit suggests direct segment drive through resistors,
// so using active-high patterns is typical in Wokwi 7segment models.
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t encode7segActiveHigh(uint8_t digit);
#line 79 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateBargraph(uint8_t number);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shift16ToBoth(uint16_t word16);
#line 119 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void update7seg(uint8_t tens, uint8_t units);
#line 133 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void updateServos(uint8_t number);
#line 145 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 174 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 43 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_41\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t encode7segActiveHigh(uint8_t digit) {
  // Bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
  // Standard digit patterns (A B C D E F G):
  // 0: 1 1 1 1 1 1 0
  // 1: 0 1 1 0 0 0 0
  // 2: 1 1 0 1 1 0 1
  // 3: 1 1 1 1 0 0 1
  // 4: 0 1 1 0 0 1 1
  // 5: 1 0 1 1 0 1 1
  // 6: 1 0 1 1 1 1 1
  // 7: 1 1 1 0 0 0 0
  // 8: 1 1 1 1 1 1 1
  // 9: 1 1 1 1 0 1 1
  switch (digit) {
    case 0: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5);                 // A,B,C,D,E,F
    case 1: return (1<<1)|(1<<2);                                         // B,C
    case 2: return (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6);                    // A,B,D,E,G
    case 3: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6);                    // A,B,C,D,G
    case 4: return (1<<1)|(1<<2)|(1<<5)|(1<<6);                           // B,C,F,G
    case 5: return (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6);                    // A,C,D,F,G
    case 6: return (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);             // A,C,D,E,F,G
    case 7: return (1<<0)|(1<<1)|(1<<2);                                  // A,B,C
    case 8: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6);       // A..G
    case 9: return (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6);             // A,B,C,D,F,G
    default: return 0;
  }
}

// ---------- Bargraph mapping ----------
// bargraph1:A1..A10 correspond to mega:2..11
// bargraph1:C1..C10 all tied to mega:GND.1 in circuit.
// In Wokwi, bargraph has two pins per segment column; driving A pin HIGH lights that bar.
// We'll drive A1..A10 as active-high.
static const uint8_t BAR_A_PINS[10] = {2,3,4,5,6,7,8,9,10,11};
static const uint8_t DIGITS_COUNT = 2;

static void updateBargraph(uint8_t number) {
  // Requirement: number as percentage 0-100% with example: 50 -> 5 LEDs lit (i.e., number/10)
  // Here number is 0..99; treat it as percentage and light up number/10 LEDs, limited to 10.
  uint8_t ledsToLight = number / 10;
  if (ledsToLight > 10) ledsToLight = 10;

  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_A_PINS[i], (i < ledsToLight) ? HIGH : LOW);
  }
}

// ---------- 74HC595 shifting for both 7-seg displays ----------
// We must update both chips:
// - SR1 outputs control sevseg1 (tens digit)
// - SR2 outputs control sevseg2 (units digit)
//
// Serial chain: SR1 DS -> SR1, SR1 Q7S -> SR2 DS. Therefore when shifting N bits,
// the last bits end up in SR2 outputs. We'll send 16 bits total: first for SR1, then for SR2.
// We'll align so that after shifting, SR1 holds lower 8 bits and SR2 holds upper 8 bits.
// We'll implement by shifting MSB-first across 16 bits with:
static void shift16ToBoth(uint16_t word16) {
  // word16 bit mapping: bit0..bit7 -> SR1 Q0..Q7
  //                      bit8..bit15 -> SR2 Q0..Q7
  // Shift out from MSB (bit15) to LSB (bit0) so that SR2 gets upper byte.
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  for (int8_t i = 15; i >= 0; i--) {
    digitalWrite(SR1_DS, (word16 & (1UL << i)) ? HIGH : LOW);
    // Pulse shift clock
    digitalWrite(SR1_SHCP, HIGH);
    // slight timing not necessary in Wokwi
    digitalWrite(SR1_SHCP, LOW);
  }

  // Latch
  digitalWrite(SR1_STCP, HIGH);
  digitalWrite(SR1_STCP, LOW);
}

static void update7seg(uint8_t tens, uint8_t units) {
  uint8_t segT = encode7segActiveHigh(tens);
  uint8_t segU = encode7segActiveHigh(units);

  // Q0..Q6 used. Q7 unused (set 0).
  uint8_t sr1Byte = segT & 0x7F; // A..G bits
  uint8_t sr2Byte = segU & 0x7F;

  uint16_t combined = ((uint16_t)sr2Byte << 8) | sr1Byte;

  shift16ToBoth(combined);
}

// ---------- Servo update ----------
static void updateServos(uint8_t number) {
  // Requirement: angle = (number * 1.8) degrees
  float angle = number * 1.8f; // 0..178.2 for 0..99
  servo1.write(angle);
  servo2.write(angle);
}

// ---------- Timing ----------
static const unsigned long INCREMENT_INTERVAL_MS = 2000;
static uint8_t currentNumber = 0;
static unsigned long lastUpdateMs = 0;

void setup() {
  // SR control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);

  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Bargraph pins
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW);
  }

  // Servo attach
  servo1.attach(SERVO1_PWM);
  servo2.attach(SERVO2_PWM);

  // Initialize components to number 0
  currentNumber = 0;
  lastUpdateMs = millis();

  update7seg(0, 0);
  updateBargraph(currentNumber);
  updateServos(currentNumber);
}

void loop() {
  unsigned long now = millis();
  if (now - lastUpdateMs >= INCREMENT_INTERVAL_MS) {
    lastUpdateMs = now;

    // Increment every 2 seconds, reset after reaching 99 -> back to 0
    currentNumber++;
    if (currentNumber > 99) currentNumber = 0;

    uint8_t tens = currentNumber / 10;
    uint8_t units = currentNumber % 10;

    update7seg(tens, units);
    updateBargraph(currentNumber);
    updateServos(currentNumber);
  }
}
