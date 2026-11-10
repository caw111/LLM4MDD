#include <Arduino.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

// --- 74HC595 pin mapping from the circuit ---
static const uint8_t SR1_SHCP = A1; // SHCP (clock) shared to SR2
static const uint8_t SR1_STCP = A0; // STCP (latch) shared to SR2
static const uint8_t SR1_DS   = A2; // DS (data) shared to SR2
// OE and MR are tied to GND (OE via sr1:GND->sr1:OE) and VCC (MR tied through VCC),
// respectively in the circuit, so no need to control them here.

static const uint8_t BAR_A_PINS[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
static const uint8_t BAR_C_PINS[10] = {
  44, 46, 48, 50, 52, 53, 45, 47, 49, 51
};
// Note: These correspond to the BAR connections in the circuit file:
// A1..A10 are mega:2..11, and C1..C10 are mega:GND.1 pairs; in Wokwi they map to
// specific pins for the bargraph component. However, since bargraph1:C* are wired
// to "GND.1", Wokwi will treat those as internal common cathodes and the exact
// numeric pin mapping is not critical. We will still use the A pins and rely on
// the component's common handling.

// --- Segment bit order assumptions ---
// Wokwi 7-segment component for 74HC595 typically expects a specific bit-to-segment mapping.
// Based on wiring:
 // sr1:Q0 -> sevseg1:A
 // sr1:Q1 -> sevseg1:B
 // sr1:Q2 -> sevseg1:C
 // sr1:Q3 -> sevseg1:D
 // sr1:Q4 -> sevseg1:E
 // sr1:Q5 -> sevseg1:F
 // sr1:Q6 -> sevseg1:G
// We'll use bits [0..6] to drive A..G for each digit.
// We will generate digit patterns for common-cathode displays (segments ON = 1).
// If your Wokwi part uses opposite polarity, these will need inversion.
static const uint8_t DIGIT_SEGMENTS[10] = {
  // A B C D E F G (bit0..bit6) => value
  // 0: A B C D E F
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

// Send 8-bit to SR1 then SR2 (they share SHCP/STCP/DS; Q7S links SR1 to SR2 DS)
// We will clock 16 bits total: higher byte for SR1, lower byte for SR2.
static inline void shiftOut16(uint8_t sr1Byte, uint8_t sr2Byte) {
  // Latch low, shift, latch high.
  digitalWrite(SR1_STCP, LOW);

  // 74HC595 shift register chain: DS is fed, SR1 receives first 8 bits, then SR2.
  // We'll shift MSB->LSB for each 74HC595 so that Q0 maps to the first shifted-out bit position.
  // Since Q0..Q6 are connected, we set bit0..bit6 accordingly in the final latched register.
  // Using MSB->LSB shifting: bit7 first etc ensures correct placement for typical designs.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (sr1Byte >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR1_DS, (sr2Byte >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    digitalWrite(SR1_SHCP, LOW);
  }

  digitalWrite(SR1_STCP, HIGH);
}

static void setSevenSegDigit(uint8_t digit, uint8_t which) {
  // which: 0 => sevseg1 (SR1), 1 => sevseg2 (SR2)
  // Build byte: bits0..6 = A..G. bits7 unused.
  uint8_t seg = DIGIT_SEGMENTS[digit];

  // Keep the other SR byte as zero; we call with both digits at once in practice.
  // This function is not used for per-digit updates; it's kept for completeness.
  (void)which;
  (void)seg;
}

static void setBothSevSeg(uint8_t tens, uint8_t units) {
  uint8_t sr1Byte = DIGIT_SEGMENTS[tens] & 0x7F;
  uint8_t sr2Byte = DIGIT_SEGMENTS[units] & 0x7F;
  // ensure bit7 = 0
  sr1Byte &= 0x7F;
  sr2Byte &= 0x7F;
  shiftOut16(sr1Byte, sr2Byte);
}

// --- LED Bar graph handling ---
// Wokwi bar graph expects A1..A10 to be driven high for ON; common cathodes are fixed in the model.
// We'll directly drive A pins (2..11). C pins are wired to GND.1 internally; no additional control needed.
static void setBarGraphByTens(uint8_t tensDigit) {
  // tensDigit from 0..9
  // turn on exactly tensDigit LEDs: A1..A(tensDigit) HIGH, others LOW
  for (uint8_t i = 0; i < 10; i++) {
    digitalWrite(BAR_A_PINS[i], (i < tensDigit) ? HIGH : LOW);
  }
}

static void setServosForUnits(uint8_t unitsDigit) {
  // servo angles:
  // servo1: units * 18
  // servo2: units * 18 + 60
  // servo3: units * 18 + 120
  int a1 = (int)unitsDigit * 18;
  int a2 = (int)unitsDigit * 18 + 60;
  int a3 = (int)unitsDigit * 18 + 120;

  // Wokwi servo model accepts 0..180; clamp just in case but preserve behavior as specified.
  auto clamp180 = [](int v) -> int { return v < 0 ? 0 : (v > 180 ? 180 : v); };

  servo1.write(clamp180(a1));
  servo2.write(clamp180(a2));
  servo3.write(clamp180(a3));
}

void setup() {
  // 74HC595 chain control pins
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  pinMode(SR1_DS, OUTPUT);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);
  digitalWrite(SR1_DS, LOW);

  // Bargraph A pins (2..11)
  for (uint8_t i = 0; i < 10; i++) {
    pinMode(BAR_A_PINS[i], OUTPUT);
    digitalWrite(BAR_A_PINS[i], LOW); // off initially
  }

  // Servos
  servo1.attach(12); // mega:12 -> servo1:PWM
  servo2.attach(13); // mega:13 -> servo2:PWM
  servo3.attach(44); // mega:44 -> servo3:PWM (note: in Wokwi this is servo pin; bargraph C10 is also on GND.1)
  // Initial angles: 0 degrees
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);

  // Initial display "00"
  setBothSevSeg(0, 0);

  // Timing
  // We'll increment every 2 seconds in loop using millis.
}

void loop() {
  static uint8_t number = 0;           // 0..99
  static unsigned long lastTickMs = 0;

  unsigned long now = millis();
  if (lastTickMs == 0) {
    lastTickMs = now;
  }

  // Update immediate after each increment, including initial state already set in setup.
  // Increment every 2000 ms.
  if (now - lastTickMs >= 2000UL) {
    lastTickMs = now;

    number++;
    if (number > 99) number = 0;

    uint8_t tens = number / 10;
    uint8_t units = number % 10;

    // Update 7-segment displays
    setBothSevSeg(tens, units);

    // Update LED bar graph: light up tensDigit LEDs
    setBarGraphByTens(tens);

    // Servo control rule:
    // When sevseg displays 9 (i.e., number's units digit is 9), hold this state for 2 seconds,
    // then rotate all servos to 0 degrees.
    if (units == 9) {
      // First move to the calculated angles for units digit 9 (as part of "rotate to specific angles based on the number")
      setServosForUnits(units);
      delay(2000); // hold for 2 seconds
      servo1.write(0);
      servo2.write(0);
      servo3.write(0);
    } else {
      setServosForUnits(units);
    }
  }
}