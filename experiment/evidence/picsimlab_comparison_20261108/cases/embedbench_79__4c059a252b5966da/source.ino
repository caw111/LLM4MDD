#include <Arduino.h>

static const uint8_t SR_LATCH_PIN = A1;  // STCP (purple)
static const uint8_t SR_CLOCK_PIN = A2;  // SHCP (gray)
static const uint8_t SR_DATA_PIN  = A0;  // DS   (blue)
static const uint8_t SR_OE_PIN     = A3;  // Not wired to A3 in provided circuit; will not use (OE tied to GND via sr1:GND->OE)
static const uint8_t SR_MR_PIN     = 5;   // Not used; tied to VCC/ MR through circuit

// Outputs for shift registers are implied through pins above (A0/A1/A2).
// OE is tied low to GND in the circuit (always enabled), so we do not control it.

static const uint8_t BTN1_PIN = 2;  // D2 +1
static const uint8_t BTN2_PIN = 3;  // D3 -1
static const uint8_t BTN3_PIN = 4;  // D4 clear

static const uint8_t POT_PIN = A3;  // ADC input for speed control

// Shift register outputs: sr1 drives sevseg1 (tens), sr2 drives sevseg2 (ones)
static const uint16_t FULL_SCALE_MS = 1000UL;

// Segment map for common-anode/common-cathode depends on wiring.
// Using Q->segment pins directly: we assume typical Wokwi 7segment uses active-high segments.
// Define bits: Q0=A, Q1=B, Q2=C, Q3=D, Q4=E, Q5=F, Q6=G, Q7S=DP unused.
static const uint8_t segMap[10] = {
  // A B C D E F G (no DP)
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

static uint8_t sr1Value = 0; // tens display segments
static uint8_t sr2Value = 0; // ones display segments

static bool readBtn(uint8_t pin) {
  // INPUT_PULLUP, low when pressed.
  return digitalRead(pin) == LOW;
}

static void pulseClock() {
  digitalWrite(SR_CLOCK_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_CLOCK_PIN, LOW);
}

static void latchOutputs() {
  digitalWrite(SR_LATCH_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SR_LATCH_PIN, LOW);
}

static void shiftOutByte(uint8_t value) {
  // Shift MSB first so that Q7S is the last shifted.
  // Mapping from value bits 0..7 to Q0..Q7:
  // We'll place segment bits into Q0..Q6 and leave Q7S=DP as 0.
  // If shifting MSB-first with standard 74HC595, the bit order matters.
  // We need sr Q0..Q6 correspond to our bit positions.
  // Most common approach: shift bit7..bit0, so that the last shifted bit0 ends at Q0.
  for (int8_t b = 7; b >= 0; --b) {
    digitalWrite(SR_DATA_PIN, (value >> b) & 0x01 ? HIGH : LOW);
    pulseClock();
  }
}

static void showNumber(uint8_t n) {
  uint8_t tens = (n / 10) % 10;
  uint8_t ones = n % 10;

  // Build 8-bit for each 74HC595: Q0..Q6 are A..G.
  // Our segMap uses bits 0..6 for A..G. DP (Q7) kept 0.
  sr1Value = segMap[tens] & 0x7F; // Q0..Q6 only
  sr2Value = segMap[ones] & 0x7F;

  // We have two cascaded 74HC595s sharing SHCP and STCP.
  // Circuit: sr1 outputs to sevseg1, sr2 outputs to sevseg2.
  // sr1 SHCP/STCP tied to sr2, and sr1 Q7S tied to sr2 DS.
  // Thus shifting two bytes: first goes into sr2? In cascades, typically the first byte shifted
  // ends up in the last register (sr2). Since sr1 feeds sr2's DS, shift order is:
  // - First shifted byte goes to sr2 (because sr1 passes its overflow to sr2).
  // - Second shifted byte goes to sr1.
  // We'll shift sr2Value first, then sr1Value, then latch.
  digitalWrite(SR_LATCH_PIN, LOW);

  shiftOutByte(sr2Value);
  shiftOutByte(sr1Value);

  latchOutputs();
}

// Timing:
// - Initial step on press immediately.
// - For long press, repeat with interval computed from ADC:
//   interval_ms = 100 + floor(ADC * 900 / 1023)  => [100..999]
static inline uint16_t getIntervalMsFromPot() {
  int adc = analogRead(POT_PIN);
  // Ensure 0..1023
  if (adc < 0) adc = 0;
  if (adc > 1023) adc = 1023;
  uint32_t interval = 100UL + (uint32_t)(adc * 900UL) / 1023UL;
  if (interval < 100) interval = 100;
  if (interval > 999) interval = 999;
  return (uint16_t)interval;
}

static inline uint8_t wrapAddSub(int16_t v) {
  // v can be -1..100. Wrap to 0..99 with specified behavior.
  while (v < 0) v = 99;
  while (v > 99) v = 0;
  return (uint8_t)v;
}

void setup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);

  pinMode(SR_CLOCK_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_DATA_PIN, OUTPUT);

  digitalWrite(SR_CLOCK_PIN, LOW);
  digitalWrite(SR_LATCH_PIN, LOW);
  digitalWrite(SR_DATA_PIN, LOW);

  // Initialize displays to "00"
  showNumber(0);

  // Debounce/edge detection with response tolerance ~20ms:
  // We'll sample every loop; use simple stable-state change with 20ms confirmation.
}

void loop() {
  static uint8_t number = 0;

  // Debounce state
  static bool b1Stable = false, b2Stable = false, b3Stable = false;
  static bool b1LastRaw = false, b2LastRaw = false, b3LastRaw = false;
  static uint32_t b1ChangeAt = 0, b2ChangeAt = 0, b3ChangeAt = 0;
  const uint32_t DEBOUNCE_MS = 20; // response/repeat tolerance 20ms

  bool b1Raw = readBtn(BTN1_PIN);
  bool b2Raw = readBtn(BTN2_PIN);
  bool b3Raw = readBtn(BTN3_PIN);

  uint32_t now = millis();

  if (b1Raw != b1LastRaw) { b1LastRaw = b1Raw; b1ChangeAt = now; }
  if (b2Raw != b2LastRaw) { b2LastRaw = b2Raw; b2ChangeAt = now; }
  if (b3Raw != b3LastRaw) { b3LastRaw = b3Raw; b3ChangeAt = now; }

  if ((now - b1ChangeAt) >= DEBOUNCE_MS) b1Stable = b1Raw;
  if ((now - b2ChangeAt) >= DEBOUNCE_MS) b2Stable = b2Raw;
  if ((now - b3ChangeAt) >= DEBOUNCE_MS) b3Stable = b3Raw;

  // Track press edges for immediate step
  static bool b1Prev = false, b2Prev = false, b3Prev = false;

  bool b1PressedEdge = (!b1Prev && b1Stable);
  bool b2PressedEdge = (!b2Prev && b2Stable);
  bool b3PressedEdge = (!b3Prev && b3Stable);

  b1Prev = b1Stable;
  b2Prev = b2Stable;
  b3Prev = b3Stable;

  // Replication timers (from last step)
  // Requirements:
  // - long press repeats with interval based on current potentiometer and starts from last step time
  // - when released, stop repeating; new press restarts timing
  // - if clear pressed (btn3), priority: immediate clear + reset repeat timers
  static uint32_t nextStepAtB1 = 0;
  static uint32_t nextStepAtB2 = 0;

  // Clear priority
  if (b3PressedEdge || b3Stable) {
    // "presses" should clear; if held, remain at 00 and no stepping from others.
    // Also reset repeated timers for add/sub.
    number = 0;
    showNumber(number);
    nextStepAtB1 = 0;
    nextStepAtB2 = 0;

    // Update prev states already handled; just ensure others don't act.
    // Also prevent their immediate edge processing by skipping below.
    return;
  }

  // If neither clear is active:
  // Step immediately on edges for btn1/btn2; if both held down, no stepping.
  bool bothAddSub = b1Stable && b2Stable;

  if (!bothAddSub) {
    if (b1PressedEdge) {
      number = wrapAddSub((int16_t)number + 1);
      showNumber(number);
      nextStepAtB1 = now + getIntervalMsFromPot();
    }
    if (b2PressedEdge) {
      number = wrapAddSub((int16_t)number - 1);
      showNumber(number);
      nextStepAtB2 = now + getIntervalMsFromPot();
    }
  } else {
    // If both buttons pressed, do not step immediately or repeat.
    // Reset repeat timers to satisfy "加减同时按下时不步进".
    nextStepAtB1 = 0;
    nextStepAtB2 = 0;
  }

  // Long press repeats:
  // - computed interval should be based on current pot value at each scheduling (tied to requirement:
  //   knob changes immediately affect current interval).
  // - "间隔从上一次步进算起": so we use nextStepAt derived from the time of last step.
  // We'll check due and then perform a step, then reschedule using current pot.
  if (!bothAddSub) {
    if (b1Stable && nextStepAtB1 != 0 && (int32_t)(now - nextStepAtB1) >= 0) {
      number = wrapAddSub((int16_t)number + 1);
      showNumber(number);
      nextStepAtB1 = now + getIntervalMsFromPot();
    }
    if (b2Stable && nextStepAtB2 != 0 && (int32_t)(now - nextStepAtB2) >= 0) {
      number = wrapAddSub((int16_t)number - 1);
      showNumber(number);
      nextStepAtB2 = now + getIntervalMsFromPot();
    }

    // If button is released, stop repeating by clearing its nextStepAt.
    if (!b1Stable) nextStepAtB1 = 0;
    if (!b2Stable) nextStepAtB2 = 0;
  } else {
    // Both pressed: no stepping; keep timers cleared.
    nextStepAtB1 = 0;
    nextStepAtB2 = 0;
  }
}