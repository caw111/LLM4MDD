#include <Arduino.h>

static const uint8_t SR_DATA  = A0;   // DS (blue)
static const uint8_t SR_LATCH = A1;   // STCP (purple)
static const uint8_t SR_CLOCK = A2;   // SHCP (gray)

// 74HC595 segment bits (active HIGH for common-anode COM.1 to LED via series resistor style wiring)
// Q0->A, Q1->B, Q2->C, Q3->D, Q4->E, Q5->F, Q6->G, Q7S unused
// Decimal point unused.

static const uint8_t DIGIT_TO_SEG[10] = {
  // A B C D E F G  (bit0..bit6)
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111  // 9
};

static void shiftOut595(uint8_t data, bool toSr2)
{
  // sr1: receives segment bits for sevseg1, sr2: receives segment bits for sevseg2.
  // Wiring shows SHCP and STCP are shared; DS is common only for both? Actually DS is sr1:DS to mega:A0,
  // and sr2:DS is connected to sr1:Q7S (serial out) -> cascaded shift registers.
  // So we must shift 16 bits total: first for sr1 (MSB), then for sr2 (LSB) in the cascade order:
  // With sr1 feeding sr2 via Q7S, the first shifted 8 bits will go into sr1, then next 8 bits into sr2.

  // This helper shifts exactly 8 bits and latches.
  // However, for cascaded operation we need to load both devices every update.
  // We'll not use this function; keep for clarity if needed.
  (void)data;
  (void)toSr2;
}

static void writeDisplays(uint8_t sevseg1_digit, uint8_t sevseg2_digit, bool sevseg1_extinguished)
{
  // The requirements:
  // - sevseg1 starts extinguished (no segments lit).
  // - sevseg2 shows "0" on power-up/reset.
  // - timer then updates every second; both digits show the updated number rules.
  //
  // For general operation: show sevseg1 digit and sevseg2 digit.
  // If sevseg1_extinguished is true, make sevseg1 output all 0 segment bits.

  uint8_t sr1_val = 0;
  if (!sevseg1_extinguished) {
    sr1_val = DIGIT_TO_SEG[sevseg1_digit % 10];
  } else {
    sr1_val = 0; // all segments off
  }

  uint8_t sr2_val = DIGIT_TO_SEG[sevseg2_digit % 10];

  // Cascaded shift: send sr1_val first, then sr2_val.
  // DS is driven by A0, clock by A2, latch by A1.
  // Shift 8 bits for sr1, then 8 bits for sr2.
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DATA, (sr1_val & (1 << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    digitalWrite(SR_CLOCK, LOW);
  }
  for (int i = 7; i >= 0; --i) {
    digitalWrite(SR_DATA, (sr2_val & (1 << i)) ? HIGH : LOW);
    digitalWrite(SR_CLOCK, HIGH);
    digitalWrite(SR_CLOCK, LOW);
  }

  // Latch both outputs to display
  digitalWrite(SR_LATCH, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_LATCH, LOW);
}

void setup() {
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);

  // Initialize pins to known states
  digitalWrite(SR_DATA, LOW);
  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_LATCH, LOW);

  // Upon powering on or resetting:
  // - sevseg1 extinguished
  // - sevseg2 shows "0"
  writeDisplays(0, 0, true);
}

void loop() {
  static unsigned long lastMs = 0;
  static uint32_t timerSeconds = 0; // real-world seconds passed * 6
  // The displayed value is timerSeconds (in seconds, increments by 6 per real second).

  unsigned long now = millis();
  if (lastMs == 0) {
    lastMs = now;
  }

  // Increment once per real second
  if (now - lastMs >= 1000UL) {
    // handle possible drift: increment for each elapsed full second
    unsigned long elapsed = (now - lastMs) / 1000UL;
    lastMs += elapsed * 1000UL;

    // For each real-world second, timer increments by 6 seconds.
    // (Requirements explicitly say "Every real-world second, the timer should increment by 6 seconds.")
    // So after elapsed real seconds: add elapsed*6.
    timerSeconds += (elapsed * 6UL);

    // Display rules:
    // - If timer exceeds 99 seconds, only the last two digits should be displayed.
    //   Example: 102 -> show "2" (i.e., last two digits "02" -> display '0' and '2'
    //   or, per the example text, show just '2' on sevseg2 with sevseg1 extinguished).
    //
    // We'll implement:
    // - For values 0..99: show tens on sevseg1 and ones on sevseg2.
    // - For values >99: extinguish sevseg1 and show ones digit on sevseg2.
    //
    // This matches the example "102 -> displays should show '2'".
    uint8_t sevseg2_digit = timerSeconds % 10;
    if (timerSeconds <= 99UL) {
      uint8_t sevseg1_digit = (timerSeconds / 10UL) % 10;
      writeDisplays(sevseg1_digit, sevseg2_digit, false);
    } else {
      // Only last two digits: show the last digit on sevseg2 and extinguish sevseg1,
      // preserving the explicit example. (sevseg1 is in an extinguished state when only
      // the ones digit is shown.)
      writeDisplays(0, sevseg2_digit, true);
    }
  }
}