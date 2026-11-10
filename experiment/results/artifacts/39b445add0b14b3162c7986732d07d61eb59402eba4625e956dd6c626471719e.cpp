#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
#include <Arduino.h>

#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPot10bit(uint8_t pin);
#line 24 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t digitToSegments7bit(uint8_t digit);
#line 42 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 68 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void shiftOut5957(uint8_t segBitsAtoG);
#line 99 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static void setBargraphByDigit(uint8_t digit, uint8_t brightness8);
#line 123 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 3 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\configured_audit_v1_20261015\\embedbench_43\\repeat_1\\direct\\attempt_0\\sketch\\sketch.ino"
static uint8_t readPot10bit(uint8_t pin) {
  // Default analogRead resolution on Mega 2560 is 10-bit.
  return (uint8_t)(analogRead(pin) & 0x03FF);
}

static uint8_t map10bitTo8bit(uint16_t v10, uint16_t outMax = 255) {
  // v10: 0..1023 -> 0..outMax
  return (uint8_t)((v10 * outMax + 511) / 1023);
}

// 7-seg digit mapping via 74HC595 outputs Q0..Q6 -> A..G
// Connections indicate:
// Q0 -> A
// Q1 -> B
// Q2 -> C
// Q3 -> D
// Q4 -> E
// Q5 -> F
// Q6 -> G
//
// We'll compute a 7-bit pattern for segments A..G and then output to Q0..Q6.
static uint8_t digitToSegments7bit(uint8_t digit) {
  // Segment bits: [A,B,C,D,E,F,G] = bit0..bit6
  // Common segment encoding for digits 0..9 (active HIGH).
  switch (digit) {
    case 0: return 0b0111111; // A B C D E F
    case 1: return 0b0000110; // B C
    case 2: return 0b1011011; // A B D E G
    case 3: return 0b1001111; // A B C D G
    case 4: return 0b1100110; // B C F G
    case 5: return 0b1101101; // A C D F G
    case 6: return 0b1111101; // A C D E F G
    case 7: return 0b0000111; // A B C
    case 8: return 0b1111111; // A B C D E F G
    case 9: return 0b1101111; // A B C D F G
    default: return 0b0000000;
  }
}

void setup() {
  // 74HC595 pins (from wiring)
  // mega:A2 -> SHCP
  // mega:A1 -> STCP
  // mega:A0 -> DS
  pinMode(A2, OUTPUT); // SHCP
  pinMode(A1, OUTPUT); // STCP
  pinMode(A0, OUTPUT); // DS

  // MR is tied to VCC through sr1:VCC->MR (see circuit) and OE tied to GND through sr1:GND->OE,
  // so we don't need additional pins for those.

  // Slide potentiometers
  // pot1: SIG -> A3
  // pot2: SIG -> A4
  // pot3: SIG -> A5
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // Initialize display off (digit 0 not yet set; but we must still drive something)
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
}

static void shiftOut5957(uint8_t segBitsAtoG) {
  // We need to clock out bits corresponding to Q0..Q6:
  // DS -> Q? shift order depends on how the hardware is wired; typical 74HC595 expects
  // MSB-first or LSb-first according to the shifting. Wokwi 74hc595 generally uses
  // DS as serial input sampled on SHCP rising edges and shifts towards Q0.
  //
  // To preserve expected mapping from Q0..Q6 to A..G (as given), we send bit0 first
  // so that Q0 receives A, Q1 receives B, ..., Q6 receives G.
  //
  // Thus: DS sequence for 7 segments is: A(B?) -> Q0 etc:
  // - send bit for A to Q0 => bit0 (segment A) first
  // - send segment B => bit1 next ... segment G => bit6 last
  //
  // We'll still do 8 bits because SR is 8-bit, with Q7 unused; send 0 for it.

  uint8_t data = segBitsAtoG & 0x7F;

  // Send bit0..bit6, then bit7=0
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t bitVal = (i < 7) ? ((data >> i) & 0x01) : 0;
    digitalWrite(A0, bitVal ? HIGH : LOW); // DS
    // Pulse SHCP (on rising edge)
    digitalWrite(A2, HIGH);
    digitalWrite(A2, LOW);
  }

  // Latch to outputs with STCP pulse
  digitalWrite(A1, HIGH);
  digitalWrite(A1, LOW);
}

static void setBargraphByDigit(uint8_t digit, uint8_t brightness8) {
  // Circuit: bargraph1 is connected with:
  // mega:2..11 -> A1..A10 (green)
  // all C1..C10 -> mega:GND.1 (common cathode)
  //
  // That indicates each segment/LED is driven from Mega digital pins (A-side) to anode,
  // with cathode common to GND. For PWM brightness, we use PWM on each A1..A10 pin.
  //
  // PWM pins on Mega 2560:
  // 2,3,4,5,6,7 have PWM? Actually on Mega: PWM on 2-13, 44-46.
  // But in this wiring we use pins 2..11, which are PWM-capable for 2..13.
  //
  // We'll analogWrite those pins with brightness. Need to turn on only first 'digit' bars.
  // Example requirement: digit 3 -> 3 segments lit.
  for (uint8_t i = 1; i <= 10; i++) {
    uint8_t pin = 1 + i; // maps A1->2, A10->11
    if (i <= digit) {
      analogWrite(pin, brightness8);
    } else {
      analogWrite(pin, 0);
    }
  }
}

void loop() {
  // Read potentiometers continuously
  uint16_t pot1_raw = analogRead(A3); // brightness (0..255)
  uint16_t pot2_raw = analogRead(A4); // speed part 1
  uint16_t pot3_raw = analogRead(A5); // speed part 2

  uint8_t brightness = map10bitTo8bit(pot1_raw, 255);

  // speed rule:
  // - pot2 controls speed range from 1 second to 2 seconds
  // - pot3 controls speed range from 1 second to 2 seconds
  // - final speed = sum of the values from pot2 and pot3
  //
  // pot2_speed and pot3_speed each map 0..1023 -> 1000..2000 ms.
  uint32_t pot2_ms = 1000UL + (uint32_t)pot2_raw * 1000UL / 1023UL; // 1..2 seconds
  uint32_t pot3_ms = 1000UL + (uint32_t)pot3_raw * 1000UL / 1023UL; // 1..2 seconds
  uint32_t intervalMs = pot2_ms + pot3_ms; // 2..4 seconds

  static uint8_t digit = 0;
  static uint32_t lastUpdate = 0;

  uint32_t now = millis();
  if (now - lastUpdate >= intervalMs) {
    digit = (digit + 1) % 10;
    lastUpdate = now;

    // Update 7-seg
    uint8_t segBits = digitToSegments7bit(digit);
    shiftOut5957(segBits);

    // Update bargraph brightness and lit count
    setBargraphByDigit(digit, brightness);
  } else {
    // Even between digit changes, continuously update bargraph brightness
    // and keep lit segments corresponding to current digit.
    setBargraphByDigit(digit, brightness);
  }
}
