#include <Arduino.h>

// ==== Pin mapping from provided wiring ====
static const uint8_t LED1_PIN = 2;   // mega:2 -> led1:A
static const uint8_t RGB_R_PIN = 3;   // mega:3 -> rgb1:R
static const uint8_t RGB_G_PIN = 4;   // mega:4 -> rgb1:G
static const uint8_t RGB_B_PIN = 5;   // mega:5 -> rgb1:B

// Shift registers (74HC595) shared control lines for both displays
static const uint8_t SR1_SHCP = A2;  // mega:A2 -> sr1:SHCP
static const uint8_t SR1_STCP = A1;  // mega:A1 -> sr1:STCP
static const uint8_t SR1_DS   = A0;   // mega:A0 -> sr1:DS

// OE tied low (black), MR tied to VCC (reset inactive) in circuit.
// We still set used pins properly for Arduino.

static const unsigned long PERIOD_MS = 2000UL;

// 7-seg encoding (common cathode as per circuit).
// Bits: a b c d e f g dp -> we only use a..g, dp=0
// Map: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G
static const uint8_t DIGIT_TO_Q_MASK[10] = {
  // a b c d e f g (dp=0)
  // digit 0: a,b,c,d,e,f on; g off
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5),                 // 0
  // digit 1: b,c
  (1<<1)|(1<<2),                                            // 1
  // digit 2: a,b,d,e,g
  (1<<0)|(1<<1)|(1<<3)|(1<<4)|(1<<6),                      // 2
  // digit 3: a,b,c,d,g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<6),                      // 3
  // digit 4: b,c,f,g
  (1<<1)|(1<<2)|(1<<5)|(1<<6),                               // 4
  // digit 5: a,c,d,f,g
  (1<<0)|(1<<2)|(1<<3)|(1<<5)|(1<<6),                      // 5
  // digit 6: a,c,d,e,f,g
  (1<<0)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),             // 6
  // digit 7: a,b,c
  (1<<0)|(1<<1)|(1<<2),                                      // 7
  // digit 8: a,b,c,d,e,f,g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6),       // 8
  // digit 9: a,b,c,d,f,g
  (1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<5)|(1<<6)                // 9
};

// We have two 74HC595, both share SHCP and STCP, and SR chain DS is shared from A0.
// Wiring: sr1:Q* -> sevseg1, sr2:Q* -> sevseg2. Thus shifting 16 bits total would be ideal,
// but the provided wiring ties sr1:DS to Arduino DS, and sr1:Q7S to sr2:DS.
// We'll shift 14 bits (7 for sr1, 7 for sr2) by writing 16 clock pulses with
// two leading zeros/ don't-care. Here, we explicitly shift 16 bits: top bit first,
// where the first 8 clocked bits end up in sr1 (last 7 matter), and the next 8 into sr2.
// To keep it deterministic, we shift 16 bits with layout:
//   - clocked bit15..bit8 -> sr1 (Q0..Q7S), we only use Q0..Q6
//   - clocked bit7..bit0  -> sr2 (Q0..Q6 used)
static void shiftOut16ToTwoDigits(uint8_t tensDigit, uint8_t unitsDigit) {
  uint8_t sr1Mask = DIGIT_TO_Q_MASK[tensDigit];  // Q0..Q6
  uint8_t sr2Mask = DIGIT_TO_Q_MASK[unitsDigit];

  // Convert masks to 8-bit shift register images:
  // We define Q0..Q6 correspond to bits 0..6. Q7 unused -> 0.
  uint8_t sr1Image = sr1Mask & 0x7F;
  uint8_t sr2Image = sr2Mask & 0x7F;

  // Shift top bit first for 16 clocks.
  // Bit positions for shift chain:
  //   For sr1: it samples DS at each SHCP rising edge and eventually maps to Q0..Q7.
  // With standard 74HC595, the order for "MSB first" depends on how you interpret mapping.
  // Given constraints, we will implement a bit order consistent with direct Qn mapping to bit n
  // by shifting LSB-first into each register segment using a 16-bit loop where:
  //   - first 8 clocks load sr1 bits (into Q0..Q7) in LSB-first order
  //   - next 8 clocks load sr2 similarly
  //
  // Therefore: shift LSB-first for each 8-bit image, while clocking 16 times total.
  // Bits to send (16 iterations): for i=0..7 send sr1Image bit i to sr1; for i=0..7 send sr2Image bit i to sr2.
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_DS, LOW);

  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR1_DS, (sr1Image >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SR1_DS, (sr2Image >> i) & 0x01);
    digitalWrite(SR1_SHCP, HIGH);
    delayMicroseconds(2);
    digitalWrite(SR1_SHCP, LOW);
    delayMicroseconds(2);
  }

  // Latch outputs
  digitalWrite(SR1_STCP, HIGH);
  delayMicroseconds(2);
  digitalWrite(SR1_STCP, LOW);
}

static void setRGB(bool rOn, bool gOn, bool bOn) {
  // rgb1 is common cathode per circuit. Pins are cathode? Actually common cathode means
  // COM is cathode common to GND; anodes go high to light. Wiring shows COM -> GND, and
  // COM is connected to mega:GND. Thus drive R/G/B pins HIGH to turn ON that color.
  digitalWrite(RGB_R_PIN, rOn ? HIGH : LOW);
  digitalWrite(RGB_G_PIN, gOn ? HIGH : LOW);
  digitalWrite(RGB_B_PIN, bOn ? HIGH : LOW);
}

int main() {
  init();

  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW); // initially off

  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
  setRGB(false, false, false);

  pinMode(SR1_DS, OUTPUT);
  pinMode(SR1_SHCP, OUTPUT);
  pinMode(SR1_STCP, OUTPUT);
  digitalWrite(SR1_DS, LOW);
  digitalWrite(SR1_SHCP, LOW);
  digitalWrite(SR1_STCP, LOW);

  // Initialize 7-seg to 00
  shiftOut16ToTwoDigits(0, 0);

  unsigned long lastMonoToggle = millis();
  unsigned long lastRGBChange = millis();
  unsigned long lastSegUpdate = millis();

  bool led1On = false;

  // RGB sequence: Red, Green, Blue, Yellow, Cyan, Magenta, White
  // For common cathode, HIGH=ON.
  const uint8_t RGB_SEQ[7][3] = {
    {1,0,0}, // Red
    {0,1,0}, // Green
    {0,0,1}, // Blue
    {1,1,0}, // Yellow
    {0,1,1}, // Cyan
    {1,0,1}, // Magenta
    {1,1,1}  // White
  };
  uint8_t rgbIndex = 0;
  setRGB(RGB_SEQ[rgbIndex][0], RGB_SEQ[rgbIndex][1], RGB_SEQ[rgbIndex][2]);

  uint8_t count = 0; // 00..99

  while (true) {
    const unsigned long now = millis();

    // 1) Single-color LED: toggle every 2 seconds (on 2s, off 2s)
    if (now - lastMonoToggle >= PERIOD_MS) {
      lastMonoToggle += PERIOD_MS; // maintain precise 2s cadence
      led1On = !led1On;
      digitalWrite(LED1_PIN, led1On ? HIGH : LOW);
    }

    // 2) RGB: change color every 2 seconds
    if (now - lastRGBChange >= PERIOD_MS) {
      lastRGBChange += PERIOD_MS; // maintain precise 2s cadence
      rgbIndex = (rgbIndex + 1) % 7;
      setRGB(RGB_SEQ[rgbIndex][0], RGB_SEQ[rgbIndex][1], RGB_SEQ[rgbIndex][2]);
    }

    // 3) 7-seg: update count every 2 seconds, 00..99 repeating
    if (now - lastSegUpdate >= PERIOD_MS) {
      lastSegUpdate += PERIOD_MS; // maintain precise 2s cadence
      uint8_t tens = count / 10;
      uint8_t units = count % 10;
      shiftOut16ToTwoDigits(tens, units);

      count++;
      if (count >= 100) count = 0;
    }

    // Small idle to reduce busy-loop jitter; does not affect 2s cadence due to millis scheduling.
    delay(1);
  }
}